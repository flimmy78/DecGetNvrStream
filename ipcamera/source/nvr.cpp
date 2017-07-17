#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


#include "nvr.h"
#include "mod_encode.h"

#include "ctrlprotocol.h"

#include "circlebuf.h"

extern "C" u32 GetLocalIp();
extern "C" u32 GetNetMask();
extern "C" u32 GetDefaultGateway();
extern "C" int ConnectWithTimeout(u32 dwHostIp, u16 wHostPort, u32 dwTimeOut, u16 *pwErrorCode);
extern "C" void SetKeepAlive(int Sock);
extern int DoStreamStateCallBack(int chn, real_stream_state_e msg, u32 nvrIP, u32 alarm_key);



/**********************************************************************
NVR_info: ����ÿ�������ϵ�NVR��Ϣ
CHN_info: ����ÿ��ͨ������Ϣ���߼��ϴ���NVR_client_info�¼�
eg: 
	���������һ��16·NVR����ô�ڵ�ǰģ�����NVR
����Ӧһ��NVR_client_info�ṹ��16��chn_info�ṹ��
**********************************************************************/
#define NVR_DEBUG
//#undef NVR_DEBUG

#define INVALID_PID	(-1)
#define INVALID_VAL	(-1)
#define NvrLinkMax	(8)	//��� NVR������
#define DestroyNvrDelayTime	(10) //��λ: ��

struct chn_info;

typedef struct
{
	int valid;//��ʾ�ýṹ�Ƿ���Ч
	int cmd_fd;//����ͨ��SOCKET
	struct chn_info **ppchn_info;//�����NVR������ͨ����Ϣ��ָ��
	//���ü�����������ɾ����NVR������ͨ�������ٸýṹ
	//�ɹ������NVR��һ������+1���Ͽ�һ������-1
	int ref_cnt;
	//NVRͨ��0��ͼ����ʾ��GUI������display_chnͨ��
	//���ھ����Ƿ�DestroyNvrInfo
	int display_chn;
	//��ref_cnt == 0ʱtm��¼��ǰʱ�䣬��ʱDestroyNvrDelayTime������
	time_t tm;
	ifly_DeviceInfo_t dev_info;
	ifly_loginpara_t login;
	
	pthread_mutex_t lock;
} NVR_info;

typedef struct chn_info
{
	//private 
	u8 nvr_chn;//��ʾ���Ѿ�������NVR�ĵڼ���ͨ������������/������
	u8 sys_chn;//��ʾ��ϵͳ�ĵڼ���ͨ����������/������

	u8 eventLoopWatchVariable;//���������߳��˳�����
	pthread_t pid;//���������߳�ID
	int cap_fd;//�������ݽ���SOCKET
	u32 link_id;//���ڹر�����
	RealStreamCB pStreamCB;//���ݻص�
	unsigned int dwContext;//�ص�����
	int video_width;//������
	int video_height;//������
	//
	NVR_info *pnvr_info;
	
	pthread_mutex_t lock;
	pthread_cond_t CondThreadOver;//�ȴ������߳̽���
} CHN_info;

typedef struct
{
	u8 nvr_index;//��ӦNVR_info�ṹ��������±�
	u8 type;//0: will be destroy
} MSG_TYPE;

#define MaxLinstenFds	(20)
typedef struct
{
	int linsten_fds[MaxLinstenFds];
	pthread_mutex_t lock;
} SLinsten_info;
static SLinsten_info g_linsten_info;

static NVR_info *g_nvr_info = NULL;
static CHN_info *g_chn_info = NULL;
static unsigned int g_client_max = 0;
static unsigned char g_init_flag = 0;
static pthread_t g_ack_thread_id = 0;
static pthread_t g_msgrcv_thread_id = 0;
static pthread_t g_destroy_thread_id = 0;

//������Ϣͬ��
#define BufSize	(1024)
static char *g_pbuf_snd = NULL;
static char *g_pbuf_rcv = NULL;
static volatile int g_dwSyncAckLen = 0;
static volatile u16	g_wSyncNumber = 0;
static pthread_mutex_t g_hSyncMutex;//ͬһʱ��ֻ����һ������Ի�
static sem_t g_hSyncSem;//�߳̽���ACK���ѷ��ͷ�

//�����й�
static pthread_mutex_t g_search_lock = PTHREAD_MUTEX_INITIALIZER;
static unsigned char g_search_start = 0;
static ipc_node *g_search_head = NULL;
static ipc_node *g_search_tail = NULL;

/********************************************************************
�ڲ�����
********************************************************************/
//Search
void SearchDevice_CB(ifly_DeviceInfo_t dev, void* pContext)
{
	pthread_mutex_lock(&g_search_lock);

	if (g_search_start == 0)//δ��������
	{
		pthread_mutex_unlock(&g_search_lock);
		printf("%s Has not yet started\n", __func__);
		return;
	}

#if 0
	struct in_addr in;
	in.s_addr = dev.deviceIP;
	printf("%s deviceIP: %s, nNVROrDecoder: %d\n", __func__, inet_ntoa(in), dev.nNVROrDecoder);
#endif

	//����NVR���ų�DEC
	if (dev.nNVROrDecoder != 1)
	{
		pthread_mutex_unlock(&g_search_lock);
		//printf("%s search device is DEC, return\n", __func__);
		return;
	}
	//

	//�õ�����IP���ų�����
	u32 localIP = GetLocalIp();
	if (localIP == 0)
	{
		pthread_mutex_unlock(&g_search_lock);
		printf("Error ****** %s, GetLocalIp failed\n", __func__);
		return;
	}

	if (localIP == dev.deviceIP)
	{
		pthread_mutex_unlock(&g_search_lock);
		printf("%s search device is itself, return\n", __func__);
		return;
	}
	//
	
	ipc_node *pNode = (ipc_node *)malloc(sizeof(ipc_node));
	if(pNode == NULL)
	{
		printf("%s Not enough space to save new ipc info.\n", __func__);
		pthread_mutex_unlock(&g_search_lock);
		return;
	}
	memset(pNode, 0, sizeof(*pNode));
	pNode->next = NULL;
	
	//strncpy(pNode->ipcam.address, pstruDevInfo->aszMAC, sizeof(pNode->ipcam.address)-1);
	strcpy(pNode->ipcam.user, "admin");
	//strcpy(pNode->ipcam.pwd, "admin");//��¼����Ϊ��
	pNode->ipcam.channel_no = 0;
	pNode->ipcam.enable = 0;
	pNode->ipcam.ipc_type = IPC_TYPE_720P;
	pNode->ipcam.protocol_type = PRO_TYPE_NVR;
	pNode->ipcam.stream_type = STREAM_TYPE_MAIN;
	pNode->ipcam.trans_type = TRANS_TYPE_TCP;
	pNode->ipcam.force_fps = 0;
	pNode->ipcam.frame_rate = 30;
	pNode->ipcam.dwIp = dev.deviceIP;
	pNode->ipcam.wPort = dev.devicePort;
	pNode->ipcam.main_audio_sw = dev.maxChnNum;//nvr���ͨ������AddIPCҪ�õ�
	//strcpy(pNode->ipcam.uuid, pstruDevInfo->aszDevID);	
	//strcpy(pNode->ipcam.name, pstruDevInfo->aszDevName);
	//pNode->ipcam.net_mask = inet_addr(pstruDevInfo->aszMASK);
	//pNode->ipcam.net_gateway = inet_addr(pstruDevInfo->aszGTW);
	
	//��䱾�������������
	pNode->ipcam.net_mask = GetNetMask();
	pNode->ipcam.net_gateway = GetDefaultGateway();
	pNode->ipcam.dns1 = inet_addr("8.8.8.8");
	pNode->ipcam.dns2 = inet_addr("4.4.4.4");
	
	if(g_search_head == NULL)
	{
		g_search_head = pNode;
		g_search_tail = pNode;
	}
	else
	{
		g_search_tail->next = pNode;
		g_search_tail = pNode;
	}
		
	pthread_mutex_unlock(&g_search_lock);
}


/************************************
ret:
	[0, NvrLinkMax) : 	��������g_nvr_info �У��豸IP==����IP��
					��������g_nvr_info �У����ص�һ��δʹ�õĳ�Ա
caution: 	1���������ػ������
		2���ض���putNvrIno �ɶ�ʹ��
************************************/
static NVR_info *getNvrInfoByIP_lock(u32 dwIp)
{
	int i = 0;
	int first_valid = -1;
	NVR_info *pNVR_info = NULL;

	for (i=0; i<NvrLinkMax; i++)
	{
		pthread_mutex_lock(&g_nvr_info[i].lock);

		if (0 == g_nvr_info[i].valid)
		{			
			if (first_valid == -1)
			{
				first_valid = i;
				continue;//ע��δ�ͷ���
			}
		}
		else
		{
			if (dwIp == g_nvr_info[i].dev_info.deviceIP)
			{
				break;
			}
		}
		
		pthread_mutex_unlock(&g_nvr_info[i].lock);
	}

	if (i < NvrLinkMax)
	{
	#ifdef NVR_DEBUG
		printf("%s dev exist, pos: %d\n", __func__, i);
	#endif
	
		pNVR_info = &g_nvr_info[i];

		//����ҵ��ˣ���ô�ͷ�first_valid ��
		if (first_valid != -1)
		{
			pthread_mutex_unlock(&g_nvr_info[first_valid].lock);
		}
	}
	else
	{
		if (first_valid != -1)
		{
			printf("%s dev not exist, pos: %d\n", __func__, first_valid);
			pNVR_info = &g_nvr_info[first_valid];
		}
		else
		{
			printf("%s ***Error*** g_nvr_info all dev in used\n", __func__);
			pNVR_info = NULL;
		}
	}
	
	return pNVR_info;
}

static void putNvrInfo(NVR_info *pNVR_info)
{
	if (NULL != pNVR_info)
	{
		pthread_mutex_unlock(&pNVR_info->lock);
	}
}

static inline u16 getNo()
{
	static u16 no = 0;
	return no++;
}

//�ײ����ݽ���
static int NetDialogue(int sock, u16 event, const void *content, int length, void* ackbuf, int ackbuflen, int *realacklen)
{
	//u8 abyBuf[1024];
	int ret = SOCKET_SUCCESS;
	int msglen = 0;
	int rcvlen = 0;
	u16 number = 0;
	ifly_cp_header_t cp_head_snd;
	ifly_cp_header_t cp_head_rcv;
	struct timeval tv1;
								
	
	if(sock < 0)
	{
		printf("%s ***Error*** sock invalid\n", __func__);
		return CTRL_FAILED_PARAM;
	}
	
	if(length < 0)
	{
		printf("%s ***Error*** length: %d invalid\n", __func__, length);
		return CTRL_FAILED_PARAM;
	}
	if(NULL == ackbuf)
	{
		printf("%s ***Error*** ackbuf invalid\n", __func__);
		return CTRL_FAILED_PARAM;
	}
	if(NULL == realacklen)
	{
		printf("%s ***Error*** realacklen invalid\n", __func__);
		return CTRL_FAILED_PARAM;
	}
	//ȫ�ֻػ���
	pthread_mutex_lock(&g_hSyncMutex);

	//����
	msglen = sizeof(cp_head_snd)+length;
	memset(&cp_head_snd, 0, sizeof(cp_head_snd));
	cp_head_snd.length = htonl(msglen);
	cp_head_snd.type = htons(CTRL_COMMAND);
	cp_head_snd.event = htons(event);
	
	number = getNo();

	gettimeofday(&tv1, NULL);
	printf("%s snd event: %d, number: %d, time: %u.%u\n", __func__, event, number, tv1.tv_sec, tv1.tv_usec);
	
	cp_head_snd.number = htons(number);
	cp_head_snd.version = htons(CTRL_VERSION);
	if(msglen > BufSize)
	{
		printf("%s ***Error*** cp_head_snd.length(%d) > BufSize\n", __func__, msglen);
		
		pthread_mutex_unlock(&g_hSyncMutex);
		return CTRL_FAILED_PARAM;
	}
	
	memcpy(g_pbuf_snd, &cp_head_snd, sizeof(cp_head_snd));

	if (content && length)
	{
		memcpy(g_pbuf_snd+sizeof(cp_head_snd), content, length);
	}

	g_wSyncNumber = ntohs(cp_head_snd.number);
	ret = sem_trywait(&g_hSyncSem);//��ֹ��һ����Ϣ��Ӧ��
	if (ret)
	{
		if (EAGAIN == errno)
		{
		#ifdef NVR_DEBUG
			//printf("%s sem_trywait EAGAIN\n", __func__);
		#endif
		}
		else
		{
			printf("%s ***Error*** sem_trywait failed, error:[%d,%s]!\n", 
				__func__, errno, strerror(errno));
		}
	}

	ret = loopsend(sock, (char *)g_pbuf_snd, msglen);
	if(ret <= 0)
	{
		printf("%s ***Error*** loopsend failed\n", __func__);

		pthread_mutex_unlock(&g_hSyncMutex);
		return CTRL_FAILED_NETSND;
	}
	//printf("%s snd number2: %d\n", __func__, number);
	
	//����
	struct timespec tm;
	time_t cur_t;
	u8 cnt = 0;
#if 1
	do {
		memset(&tm, 0, sizeof(tm));
		time(&cur_t);
		tm.tv_sec = cur_t+5;//δ��ʱ�䣬��ʱʱ��3��

		ret = sem_timedwait(&g_hSyncSem, &tm);
		if (ret == 0)//success
		{
			#ifdef NVR_DEBUG
				printf("%s sem_timedwait success\n", __func__);
			#endif
			break;
		}
		else
		{
			if (ETIMEDOUT == errno)
			{
			#ifdef NVR_DEBUG
				printf("%s sem_timedwait timeout cnt: %d\n", __func__, cnt);
			#endif
			}
			else
			{
				printf("%s ***Error*** sem_timedwait failed, error:[%d,%s]!\n", 
					__func__, errno, strerror(errno));
				break;
			}
		}
		
		cnt++;
	} while (0);	//(cnt > 1);
#else
	ret = sem_wait(&g_hSyncSem);
#endif
	gettimeofday(&tv1, NULL);
	printf("%s sem_timedwait ret: %d, event: %d, number: %d, time: %u.%u\n", __func__, ret, event, number, tv1.tv_sec, tv1.tv_usec);
	
	if (ret)//timeout or failed
	{
		printf("%s ***Error*** sem_timedwait failed, error:[%d,%s]!\n", 
					__func__, errno, strerror(errno));
		
		pthread_mutex_unlock(&g_hSyncMutex);
		return CTRL_FAILED_NETRCV;
	}

	if (g_dwSyncAckLen < (int)sizeof(ifly_cp_header_t))
	{
		printf("%s ***Error*** g_dwSyncAckLen < sizeof(ifly_cp_header_t)\n", __func__);
		
		pthread_mutex_unlock(&g_hSyncMutex);
		return CTRL_FAILED_NETRCV;
	}

	memcpy(&cp_head_rcv, g_pbuf_rcv, sizeof(ifly_cp_header_t));
	if (cp_head_rcv.event != CTRL_SUCCESS)
	{
		printf("%s ***Error*** cp_head_rcv.event: %d\n", __func__, cp_head_rcv.event);
		
		pthread_mutex_unlock(&g_hSyncMutex);
		return CTRL_FAILED_NETRCV;
	}

	rcvlen = g_dwSyncAckLen - sizeof(ifly_cp_header_t);
	
	if(realacklen != NULL) 
	{
		*realacklen = rcvlen;
	}
	
	if(rcvlen > 0)
	{
		if (ackbuf == NULL)
		{
			printf("%s ***Error*** ackbuf == NULL\n", __func__);
		
			pthread_mutex_unlock(&g_hSyncMutex);
			return CTRL_FAILED_NETRCV;
		}
		
		if(ackbuflen < rcvlen)
		{
			printf("%s ***Error*** ackbuflen(%d) < rcvlen(%d)\n", 
				__func__, ackbuflen, rcvlen);
			
			pthread_mutex_unlock(&g_hSyncMutex);
			return CTRL_FAILED_OUTOFMEMORY;
		}
		
		memcpy(ackbuf, g_pbuf_rcv+sizeof(ifly_cp_header_t), rcvlen);
	}

	pthread_mutex_unlock(&g_hSyncMutex);
	return SOCKET_SUCCESS;
}


//ifly_DeviceInfo_t DeviceInfo;
static int GetDeviceInfo(int sock, ifly_DeviceInfo_t *pDeviceInfo)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;

    if (INVALID_SOCKET == sock || NULL == pDeviceInfo)
    {
        printf("%s param invalid\n", __func__);
        return SOCKET_ERROR;
    }
     
	ret = NetDialogue(sock, CTRL_CMD_GETDEVICEINFO, NULL, 0, pDeviceInfo, sizeof(ifly_DeviceInfo_t), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s failed\n", __func__);
		return SOCKET_ERROR;
	}

	//���ݸı��ֽ���
	pDeviceInfo->devicePort = ntohs(pDeviceInfo->devicePort);//ֻ��һ��

#ifdef NVR_DEBUG
	struct in_addr in;
	in.s_addr = pDeviceInfo->deviceIP;
	
	printf("%s get device info deviceIP:%s, devicePort:%d, device_name:%s, device_mode:%s, MaxChn=%d, nNVROrDecoder:%d\n", \
		__func__,
		inet_ntoa(in),
		pDeviceInfo->devicePort,
		pDeviceInfo->device_name,
		pDeviceInfo->device_mode,
		pDeviceInfo->maxChnNum,
		pDeviceInfo->nNVROrDecoder);
#endif

	return ret;
}

static int NvrLogin(int sock, ifly_loginpara_t *plogin)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;
	u8 buf[128];
    
	ret = NetDialogue(sock, CTRL_CMD_LOGIN, plogin, sizeof(ifly_loginpara_t), buf, sizeof(buf), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s failed ret = %d\n", __func__, ret);
		ret = SOCKET_ERROR;
	}

	return ret;
}

static int NvrLogout(int sock, ifly_loginpara_t *plogin)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;
	u8 buf[128];
    
	ret = NetDialogue(sock, CTRL_CMD_LOGOFF, plogin, sizeof(ifly_loginpara_t), buf, sizeof(buf), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s failed ret = %d\n", __func__, ret);
		ret = SOCKET_ERROR;
	}

	return ret;
}

//upload_enable [0-1]
static int NvrSetAlarmUpload(int sock, unsigned char upload_enable)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;
	u8 buf[128];

    if (INVALID_SOCKET == sock)
    {
        printf("%s param invalid\n", __func__);
        return SOCKET_ERROR;
    }
    if (0 != upload_enable && 1 != upload_enable)
    {
        printf("%s param invalid\n", __func__);
        return SOCKET_ERROR;
    }
    
	ret = NetDialogue(sock, CTRL_CMD_ALARMUPLOADCENTER, &upload_enable,sizeof(upload_enable), buf, sizeof(buf), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s failed ret = %d\n", __func__, ret);
		ret = SOCKET_ERROR;
	}

	return ret;
}

//��ѯNVR��Ӧͨ��������������Ƿ��������
static int checkNvrChnLinkStatus(NVR_info *pnvr, u8 req_nvr_chn , u8 bmain)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;
	ifly_ipc_chn_status_t ipc_chn_status;
    
	ret = NetDialogue(pnvr->cmd_fd, CTRL_CMD_GET_IPCCHN_LINKSTATUS, NULL, 0, &ipc_chn_status, sizeof(ipc_chn_status), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s ***Error*** ret = %d\n", __func__, ret);
		return -1;
	}

	u8 nvr_stream_chn = bmain ? req_nvr_chn : (req_nvr_chn + pnvr->dev_info.maxChnNum);
	#ifdef NVR_DEBUG
		printf("%s real_req_chn%d, NVR max_chn_num = %d, 0x%x, 0x%x, 0x%x, 0x%x\n", \
			__func__, nvr_stream_chn, ipc_chn_status.max_chn_num, \
			ipc_chn_status.chn_status[0], \
			ipc_chn_status.chn_status[1], \
			ipc_chn_status.chn_status[2], \
			ipc_chn_status.chn_status[3]);
	#endif
	
	if (pnvr->dev_info.maxChnNum != ipc_chn_status.max_chn_num/2)
	{
		printf("%s ***Error*** pnvr->dev_info.maxChnNum(%d) != ipc_chn_status.max_chn_num/2(%d)\n", \
			__func__, pnvr->dev_info.maxChnNum, ipc_chn_status.max_chn_num/2);
		return -1;
	}

	//u8 nvr_stream_chn = bmain ? req_nvr_chn : (req_nvr_chn + pnvr->dev_info.maxChnNum);

	if (nvr_stream_chn >= ipc_chn_status.max_chn_num)
	{
		printf("%s ***Error*** nvr_stream_chn%d >= ipc_chn_status.max_chn_num%d\n", \
			__func__, nvr_stream_chn, ipc_chn_status.max_chn_num);
		return -1;
	}

	ret = 0;
	if (ipc_chn_status.chn_status[nvr_stream_chn/8] & (1<<(nvr_stream_chn%8)))
	{
		ret = 1;
	}
	
	return ret;
}

static int NVRStartStream(u32 dwIp, u16 wPort, u8 req_nvr_chn , u8 bmain, int *pstream_sock, u32 *plink_id)
{
	int ret = 0;
	int sock = INVALID_SOCKET;
	
	ifly_TCP_Stream_Req req;
	memset(&req, 0, sizeof(ifly_TCP_Stream_Req));
	req.command = 0;
	req.Monitor_t.chn = req_nvr_chn;
	req.Monitor_t.type = bmain ? 0:2;
	/*
	req.reserved[0] = 0;
	req.reserved[1] = 0;
	req.reserved[6] = 0;
	*/

	struct in_addr in;
	in.s_addr = dwIp;

	//����NVR
	u16 wErrorCode = 0;
	
	sock = ConnectWithTimeout(dwIp, wPort, 5000, &wErrorCode);
	if(sock == INVALID_SOCKET)
	{
		printf("%s ***Error*** connect failed, NVR IP: %s, port: %d, wErrorCode:%d\n", \
			__func__, inet_ntoa(in), wPort, wErrorCode);
		
		return -1;
	}
#ifdef NVR_DEBUG
	printf("%s ConnectWithTimeout success\n", __func__);
#endif

	//����socket
	struct linger m_sLinger;
	m_sLinger.l_onoff = 1;  //(��closesocket()����,���ǻ�������û������ϵ�ʱ��������)
	m_sLinger.l_linger = 0; //(��������ʱ��Ϊ0��)
	ret = setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&m_sLinger, sizeof(struct linger));
	if(ret)
	{
		printf("%s setsockopt failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
		close(sock);
		
		return -1;
	}

	//�������ͣ�0x1���������0x2���������䣻0x3���㲥������0x4  ��Ѳͬ��(ֻ���Ѿ�ʹ����Ѳ���豸��Ч)
	ret = SendTcpConnHead(sock, 0x2);
	if (ret <= 0)
	{
		printf("%s SendTcpConnHead failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
		close(sock);
		
		return -1;
	}
#ifdef NVR_DEBUG
	printf("%s SendTcpConnHead success\n", __func__);
#endif

	SetKeepAlive(sock);

	pthread_mutex_lock(&g_hSyncMutex);

	ret = loopsend(sock, (char *)&req, sizeof(ifly_TCP_Stream_Req));
	if(ret <= 0)
	{
		printf("%s loopsend failed\n", __func__);
		close(sock);

		pthread_mutex_unlock(&g_hSyncMutex);
		return -1;
	}
#ifdef NVR_DEBUG
		printf("%s Send Stream_Req success\n", __func__);
#endif

	ifly_TCP_Stream_Ack ack;
	memset(&ack, 0, sizeof(ifly_TCP_Stream_Ack));

#if 1
	ret = loopRcvTimeoutNS(sock, (char *)&ack, sizeof(ifly_TCP_Stream_Ack), 5);//timeout 5s
	if (SOCKET_SUCCESS != ret)
	{
		if (SOCKET_TIMEOUT == ret)
		{
			printf("%s ***Error*** cp_head_rcv timeout\n", __func__);			
		}
		else
		{
			printf("%s loopRcvTimeoutNS cp_head_rcv failed\n", __func__);
		}
		
		close(sock);
		pthread_mutex_unlock(&g_hSyncMutex);
		return -1;
	}
#else
	ret = looprecv(sock, (char *)&ack, sizeof(ifly_TCP_Stream_Ack));//timeout 5s
	if (ret <= 0)
	{
		printf("%s looprecv ifly_TCP_Stream_Ack failed\n", __func__);
		
		close(sock);
		pthread_mutex_unlock(&g_hSyncMutex);
		return -1;
	}
#endif

	ack.ackid = ntohl(ack.ackid);
	ack.errcode = ntohl(ack.errcode);

	if (ack.errcode != 0)
	{
		printf("%s Stream_Ack errcode: %d\n", __func__, ack.errcode);
		close(sock);
		pthread_mutex_unlock(&g_hSyncMutex);
		return -1;
	}
#ifdef NVR_DEBUG
		printf("%s Rcv Stream_Ack success\n", __func__);
#endif

	*pstream_sock = sock;
	*plink_id = ack.ackid;
	pthread_mutex_unlock(&g_hSyncMutex);
	return 0;	
}

//nvr_cmd_sock : ��¼NVR ��sockfd��
static int NVRStopStream(int nvr_cmd_sock, u32 link_id)
{
	int realacklen = 0;
	int ret = SOCKET_SUCCESS;
	u8 buf[128];

	if (INVALID_SOCKET == nvr_cmd_sock)
	{
		printf("%s INVALID_SOCKET == nvr_cmd_sock:%d\n", __func__, nvr_cmd_sock);
		return -1;
	}

#ifdef NVR_DEBUG
	printf("%s link_id: %d\n", __func__, link_id);
#endif

	link_id = htonl(link_id); 
	ret = NetDialogue(nvr_cmd_sock, CTRL_CMD_STOPVIDEOMONITOR, &link_id, sizeof(u32), buf, sizeof(buf), &realacklen);
	if (SOCKET_SUCCESS != ret)
	{
		printf("%s failed ret = %d\n", __func__, ret);
		ret = SOCKET_ERROR;
	}

	return ret;
}

static int AttachLinsten(int sock)
{
	int i = 0;
	
	pthread_mutex_lock(&g_linsten_info.lock);
	
	for(i = 0; i < MaxLinstenFds; i++)
	{
		if (INVALID_SOCKET == g_linsten_info.linsten_fds[i]) //��Ч�ı�־		
		{
			g_linsten_info.linsten_fds[i] = sock;
			break;
		}
	}

	pthread_mutex_unlock(&g_linsten_info.lock);
	
	if (i < MaxLinstenFds)
	{
		
	#ifdef NVR_DEBUG
		printf("%s success, pos: %d\n", __func__, i);
	#endif
		return 0;
	}	
	
	printf("%s ***Error*** no unused sock\n", __func__);
	return -1;	
}

static int DetachLinsten(int sock)
{
	int i = 0;
	
	pthread_mutex_lock(&g_linsten_info.lock);
	
	for(i = 0; i < MaxLinstenFds; i++)
	{
		if (sock == g_linsten_info.linsten_fds[i])		
		{
			g_linsten_info.linsten_fds[i] = INVALID_SOCKET;
			break;
		}
	}

	pthread_mutex_unlock(&g_linsten_info.lock);
	
	if (i < MaxLinstenFds)
	{
		
	#ifdef NVR_DEBUG
		printf("%s success, pos: %d\n", __func__, i);
	#endif
		return 0;
	}	
	
	printf("%s ***Error*** not found sock\n", __func__);
	return -1;
}


static int CreateNvrInfo(NVR_info *pnvr, unsigned int dwIp, unsigned short wPort, char *user, char *pwd)
{
	int ret = 0;
	int cmd_sock = INVALID_SOCKET;
	CHN_info **ppchn_info = NULL;
	u16 wErrorCode = 0;
	ifly_DeviceInfo_t DeviceInfo;
	ifly_loginpara_t login;
	struct in_addr in;
	in.s_addr = dwIp;	

	if (NULL == pnvr \
		|| NULL == user \
		|| NULL == pwd)
	{
		printf("%s ***Error*** param valid\n", __func__);
		
		goto exit1;
	}
	
	//����NVR	
	cmd_sock = ConnectWithTimeout(dwIp, wPort, 5000, &wErrorCode);
	if(cmd_sock == INVALID_SOCKET)
	{
		printf("%s ***Error*** cmd_sock connect NVR IP: %s, port: %d, wErrorCode:%d\n", \
			__func__, inet_ntoa(in), wPort, wErrorCode);

		goto exit1;
	}
#ifdef NVR_DEBUG
	printf("%s cmd_sock ConnectWithTimeout success\n", __func__);
#endif

	//
	ret = SetSockLinkOpt(cmd_sock);
	if (CTRL_SUCCESS != ret)
	{
		printf("%s ***Error*** cmd_sock SetSockLinkOpt failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);

		goto exit2;
	}
#ifdef NVR_DEBUG
	printf("%s cmd_sock SetSockLinkOpt success\n", __func__);
#endif

	//�������ͣ�0x1���������0x2���������䣻0x3���㲥������0x4  ��Ѳͬ��(ֻ���Ѿ�ʹ����Ѳ���豸��Ч)
	ret = SendTcpConnHead(cmd_sock, 0x1);
	if (ret <= 0)
	{
		printf("%s ***Error*** cmd_sock SendTcpConnHead failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
		
		goto exit2;
	}
#ifdef NVR_DEBUG
	printf("%s cmd_sock SendTcpConnHead success\n", __func__);
#endif

	SetKeepAlive(cmd_sock);

	ret = AttachLinsten(cmd_sock);
	if (ret < 0)
	{
		printf("%s ***Error*** AttachLinsten sock: %d failed\n", \
			__func__, cmd_sock);

		goto exit2;
	}

	//�õ��豸��Ϣ
	memset(&DeviceInfo, 0, sizeof(ifly_DeviceInfo_t));
	ret = GetDeviceInfo(cmd_sock, &DeviceInfo);
	if (ret < 0)
	{
		printf("%s ***Error*** cmd_sock GetDeviceInfo failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);

		goto exit3;
	}
#ifdef NVR_DEBUG
	printf("%s cmd_sock GetDeviceInfo success\n", __func__);
#endif

	//��¼�豸	
	memset(&login, 0, sizeof(ifly_loginpara_t));
	
	login.ipAddr = GetLocalIp();
	if (login.ipAddr == 0)
	{
		printf("%s GetLocalIp failed\n", __func__);
		goto exit3;
	}
	
	strcpy(login.username, user);
	strcpy(login.loginpass, pwd);
	strcpy(login.macAddr, "00:00:00:00:00:00");

	ret = NvrLogin(cmd_sock, &login);
	if (ret < 0)
	{
		printf("%s ***Error*** cmd_sock NvrLogin failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);

		goto exit3;
	}
#ifdef NVR_DEBUG
	printf("%s cmd_sock NvrLogin success\n", __func__);
#endif
#if 0
	//���ý��ձ�����Ϣ
	ret = NvrSetAlarmUpload(cmd_sock, 1);
	if (ret < 0)
	{
		printf("%s ***Error*** alarm_sock NvrSetAlarmUpload failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
		
		goto exit4;
	}
#endif	
#ifdef NVR_DEBUG
	printf("%s NvrSetAlarmUpload success\n", __func__);
#endif

	//��������������Ӧһ��CHN_info �ṹ
	ppchn_info = (CHN_info **)malloc(DeviceInfo.maxChnNum * 2 * sizeof(CHN_info *));
	if (NULL == ppchn_info)
	{
		printf("%s ppchn_info malloc failed\n", \
			__func__);

		goto exit6;
	}
	memset(ppchn_info, 0, DeviceInfo.maxChnNum * 2 * sizeof(CHN_info *));

	//���NVR_info �ṹ
	pnvr->valid = 1;
	pnvr->cmd_fd = cmd_sock;
	pnvr->ppchn_info = ppchn_info;
	pnvr->ref_cnt = 0;
	pnvr->dev_info = DeviceInfo;
	pnvr->login = login;
	pnvr->tm = INVALID_VAL;
	
	/*
	ret = SetNvrInfo(pnvr, 1, cmd_sock, sock, ppchn_info, 0, &DeviceInfo, &login);
	if (ret < 0)
	{
		printf("%s SetNvrInfo failed\n", __func__);
		
		goto exit4;
	}
	*/
#ifdef NVR_DEBUG
	printf("%s success\n", __func__);
#endif
	
	return 0;

exit6:
#if 0
	//���ò����ձ�����Ϣ
	ret = NvrSetAlarmUpload(cmd_sock, 0);
	if (ret < 0)
	{
		printf("%s alarm_sock NvrSetAlarmUpload failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
	}
#endif	
exit4:
	//�˳��豸
	ret = NvrLogout(cmd_sock, &login);
	if (ret < 0)
	{
		printf("%s cmd_sock NvrLogout failed, NVR IP: %s, port: %d\n", \
			__func__, inet_ntoa(in), wPort);
	}
exit3:
	ret = DetachLinsten(cmd_sock);
	if (ret < 0)
	{
		printf("%s ***Error*** DetachLinsten sock: %d failed\n", \
			__func__, cmd_sock);
	}
exit2:
	close(cmd_sock);
	
exit1:
	return -1;		
}

static int DestroyNvrInfo(NVR_info *pnvr)
{
	int ret = 0;
	
	if (NULL == pnvr \
		|| 0 == pnvr->valid)
	{
		printf("%s param invalid\n", __func__);
		return -1;
	}

	struct in_addr in;
	in.s_addr = pnvr->dev_info.deviceIP;
	
	//������ü���==  0��������������
	if (pnvr->ref_cnt < 0)
	{
		printf("%s ***Error*** nvr index: %d, ref_cnt: %d\n", __func__, pnvr-g_nvr_info, pnvr->ref_cnt);
	}
	else if (pnvr->ref_cnt)
	{
		printf("%s nvr index: %d, ref_cnt != 0\n", __func__, pnvr-g_nvr_info);
		return 0;
	}

#if 0
	//���ò����ձ�����Ϣ
	ret = NvrSetAlarmUpload(pnvr->cmd_fd, 0);
	if (ret < 0)
	{
		printf("%s ***Error*** cmd_fd NvrSetAlarmUpload failed, nvr index: %d\n", \
			__func__, pnvr-g_nvr_info);
	}
#endif
	//cmd_fd �˳��豸
	ret = NvrLogout(pnvr->cmd_fd, &pnvr->login);
	if (ret < 0)
	{
		printf("%s ***Error*** cmd_fd NvrLogout failed, nvr index: %d\n", \
			__func__, pnvr-g_nvr_info);
	}

	ret = DetachLinsten(pnvr->cmd_fd);
	if (ret < 0)
	{
		printf("%s ***Error*** DetachLinsten sock: %d failed, nvr index: %d\n", \
			__func__, pnvr->cmd_fd, pnvr-g_nvr_info);
	}
	
	close(pnvr->cmd_fd);
	pnvr->cmd_fd = INVALID_SOCKET;
	
	free(pnvr->ppchn_info);
	pnvr->ppchn_info = NULL;

	memset(&pnvr->dev_info, 0, sizeof(ifly_DeviceInfo_t));
	memset(&pnvr->login, 0, sizeof(ifly_loginpara_t));
	pnvr->ref_cnt = 0;
	pnvr->display_chn= INVALID_VAL;//��Чֵ
	pnvr->tm = INVALID_VAL;
	pnvr->valid = 0;

	printf("%s nvr index: %d success\n", __func__, pnvr-g_nvr_info);
	return 0;
}

static int ChnAttachNvr(CHN_info *pchn_info, NVR_info *pnvr_info)
{
	if (NULL == pchn_info \
		|| NULL == pnvr_info)
	{
		printf("%s param invalid\n", __func__);
		return -1;
	}

	int nvr_stream_chn = -1;
	if (pchn_info->sys_chn < (int)(g_client_max/2))	//������
	{
		nvr_stream_chn = pchn_info->nvr_chn;
	}
	else	//������
	{
		nvr_stream_chn = pchn_info->nvr_chn + pnvr_info->dev_info.maxChnNum;
	}

	if (NULL != pnvr_info->ppchn_info[nvr_stream_chn])
	{
		//����-- ǰһ������δ�ͷ�
		printf("%s ***Error*** ppchn_info[%d] != NULL\n", __func__, nvr_stream_chn);
	}
	pnvr_info->ppchn_info[nvr_stream_chn] = pchn_info;
	pnvr_info->ref_cnt++;
	pnvr_info->tm = INVALID_VAL;
	
	if (pchn_info->nvr_chn == 0)
	{
		pnvr_info->display_chn = pchn_info->sys_chn;
		if (pnvr_info->display_chn >= g_client_max/2)
		{
			pnvr_info->display_chn -= g_client_max/2;
		}
	}
	//printf("%s display_chn: %d\n", __func__, pnvr_info->display_chn);
	
	pchn_info->pnvr_info = pnvr_info;	
	
#ifdef NVR_DEBUG
	printf("%s nvr index:%d, ref_cnt: %d, sys_chn%d, nvr_chn%d\n", \
			__func__, pnvr_info-g_nvr_info, pnvr_info->ref_cnt, pchn_info->sys_chn, pchn_info->nvr_chn);
#endif	
	return 0;
}

static int ChnDetachNvr(CHN_info *pchn_info, NVR_info *pnvr_info)
{
	if (NULL == pchn_info \
		|| NULL == pnvr_info \
		|| pchn_info->pnvr_info != pnvr_info)
	{
		printf("%s ***Error*** chn%d param invalid\n", __func__, pchn_info->sys_chn);
		return -1;
	}

	int nvr_stream_chn = -1;
	if (pchn_info->sys_chn < (int)(g_client_max/2))	//������
	{
		nvr_stream_chn = pchn_info->nvr_chn;
	}
	else	//������
	{
		nvr_stream_chn = pchn_info->nvr_chn + pnvr_info->dev_info.maxChnNum;
	}

	if (NULL == pnvr_info->ppchn_info[nvr_stream_chn])
	{
		//����-- ǰһ������δ���
		printf("%s ***Error*** ppchn_info[%d] == NULL\n", __func__, nvr_stream_chn);
	}
	pnvr_info->ppchn_info[nvr_stream_chn] = NULL;

	pnvr_info->ref_cnt--;
	if (pnvr_info->ref_cnt < 0)
	{
		printf("%s ***Error*** ref_cnt: %d, sys_chn%d, nvr_chn%d\n", \
			__func__, pnvr_info->ref_cnt, pchn_info->sys_chn, pchn_info->nvr_chn);
	}
	else if (pnvr_info->ref_cnt == 0)
	{
		pnvr_info->tm = time(NULL);//��¼	ref_cnt==0��ʱ��
	}

#ifdef NVR_DEBUG
	printf("%s chn%d nvr index:%d, ref_cnt: %d\n", \
		__func__, pchn_info->sys_chn, pnvr_info-g_nvr_info, pnvr_info->ref_cnt);
#endif

	pchn_info->pnvr_info = NULL;

	return 0;
}

static int DestroyChnInfo(CHN_info *pchn_info, NVR_info *pnvr_info)
{
	int ret = 0;
	
	if (NULL == pchn_info \
		|| NULL == pnvr_info \
		|| pchn_info->pnvr_info != pnvr_info)
	{
		printf("%s ***Error*** param invalid\n", __func__);
		return -1;
	}

	ret = NVRStopStream(pnvr_info->cmd_fd, pchn_info->link_id);
	if (ret)
	{
		printf("%s ***Error*** chn%d NVRStopStream failed\n", __func__, pchn_info->sys_chn);
	}
	ChnDetachNvr(pchn_info, pnvr_info);
	
	//���CHN_info
	pchn_info->nvr_chn = 0;
	pchn_info->sys_chn = 0;
	pchn_info->eventLoopWatchVariable = 0;
	pchn_info->pid = INVALID_PID;
	close(pchn_info->cap_fd);
	pchn_info->cap_fd = INVALID_SOCKET;
	pchn_info->link_id = 0;
	pchn_info->pStreamCB = NULL;
	pchn_info->dwContext = 0;
	pchn_info->video_width = 0;
	pchn_info->video_height = 0;
	pchn_info->pnvr_info = NULL;
	
	return 0;
}

typedef struct 
{
 u8 type;      //����
 			//9-IPCEXT
 u8 state;       //  1���� 2�ָ�
 u8 id;        //ͨ��,Ӳ�̺�,���������,ȡ����type 
 u16 reserved1;      //Ԥ��
 u32  reserved2;      //Ԥ��
}ifly_alarmstate_t;


void* ThreadDestroyNvrFxn(void* pParam)
{
	int ret = 0;
	int i = 0;
	ipc_unit ipcam;
	NVR_info *pnvr_info = NULL;
	time_t cur_tm = INVALID_VAL;

	ret = pthread_detach(pthread_self());
	if(ret < 0)
	{
		printf("%s pthread_detach failed\n", __func__);
		pthread_exit((void *)0);
	}

	printf("%s running\n", __func__);

	while (1)
	{
		for (i=0; i<NvrLinkMax; i++)
		{
			pnvr_info = &g_nvr_info[i];
			pthread_mutex_lock(&pnvr_info->lock);

			cur_tm = time(NULL);
			
			if (pnvr_info->valid)
			{
				if (pnvr_info->ref_cnt < 0)
				{
					printf("%s ***Error*** nvr index: %d, ref_cnt: %d\n", \
						__func__, pnvr_info-g_nvr_info, pnvr_info->ref_cnt);
					
					DestroyNvrInfo(pnvr_info);
				}
				else if (0 == pnvr_info->ref_cnt)	//��ʱ����pnvr_info
				{
					if (INVALID_VAL == pnvr_info->tm)
					{
						printf("%s ***Error*** nvr index: %d, INVALID_VAL == pnvr_info->tm\n", \
							__func__, pnvr_info-g_nvr_info);

						pnvr_info->tm = cur_tm;
					}
					else
					{
						if (cur_tm < pnvr_info->tm || \
							cur_tm - 2*DestroyNvrDelayTime > pnvr_info->tm)
						{
							printf("%s ***Error*** nvr index: %d, pnvr_info->tm: %d, cur_tm: %d, something wrong\n", \
								__func__, pnvr_info-g_nvr_info, (int)pnvr_info->tm, (int)cur_tm);

							pnvr_info->tm = cur_tm;
						}
						else
						{
							if (cur_tm - DestroyNvrDelayTime > pnvr_info->tm)
							{
								if (INVALID_VAL != pnvr_info->display_chn \
									&& IPC_Get(pnvr_info->display_chn, &ipcam) == 0 \
									&& ipcam.enable == 0)//�û��ֶ�ɾ��
								{
									printf("%s DestroyNvrInfo, nvr index: %d\n", __func__, pnvr_info-g_nvr_info);
						
									DestroyNvrInfo(pnvr_info);
								}
							}
						}
					}					
				}
			}
				
			pthread_mutex_unlock(&pnvr_info->lock);
		}
		
		sleep(1);
	}
	
	pthread_exit((void *)0);
}

void* ThreadMsgRcvFxn(void* pParam)
{
	int ret = 0;
	int max_fd = INVALID_SOCKET;
	int i = 0;
	int sock = INVALID_SOCKET;
	struct timeval timeout;
	fd_set set;
	ifly_cp_header_t *pheader = NULL;
	ifly_alarmstate_t alarm_state;
	int msglen = 0;
	char *pbuf = NULL;
	u32 alarm_key = 0;
	struct timeval tv1;
	
	pbuf = (char *)malloc(1024);
	if (NULL == pbuf)
	{
		printf("%s malloc pbuf failed\n", __func__);
		pthread_exit((void *)0);
	}
	
	ret = pthread_detach(pthread_self());
	if(ret < 0)
	{
		printf("%s pthread_detach failed\n", __func__);

		free(pbuf);
		pbuf = NULL;
		pthread_exit((void *)0);
	}

	printf("%s running\n", __func__);
	
	while(1)
	{
		//linuxƽ̨��timeout�ᱻ�޸��Ա�ʾʣ��ʱ��,��ÿ�ζ�Ҫ���¸�ֵ
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&set);
		max_fd = INVALID_SOCKET;		

		pthread_mutex_lock(&g_linsten_info.lock);
		
		for (i=0; i<MaxLinstenFds; i++)
		{
			sock = g_linsten_info.linsten_fds[i];
			
			if (INVALID_SOCKET != sock)
			{	
				FD_SET(sock, &set);

				if (max_fd < sock)
				{
					max_fd = sock;
				}
			}
		}

		pthread_mutex_unlock(&g_linsten_info.lock);

		if (max_fd > 0)
		{
			ret = select(max_fd+1, &set, NULL, NULL, &timeout);
			if (ret < 0)//����
			{
				printf("%s ***Error*** select failed, error:[%d,%s]!\n", 
					__func__, errno, strerror(errno));
			}
			else if(ret > 0) // == 0 ��ʱ������
			{
				for (i=0; i<MaxLinstenFds; i++)
				{	
					sock = g_linsten_info.linsten_fds[i];
					//�������
					if(INVALID_SOCKET != sock && FD_ISSET(sock, &set))
					{
						//����
						pheader = (ifly_cp_header_t *)pbuf;
						memset(pheader, 0, sizeof(ifly_cp_header_t));
						//����Э��ͷ
						ret = looprecv(sock, (char *)pheader, sizeof(ifly_cp_header_t));
						if (ret <= 0)
						{
							printf("%s ***Error*** looprecv cp_head_rcv failed, error:[%d,%s]!\n", \
								__func__, errno, strerror(errno));
							continue;
						}
						
						pheader->length	= ntohl(pheader->length);
						pheader->type	= ntohs(pheader->type);
						pheader->version	= ntohs(pheader->version);
						pheader->number	= ntohs(pheader->number);
						pheader->event	= ntohs(pheader->event);

					#ifdef NVR_DEBUG
					
						gettimeofday(&tv1, NULL);
								//printf("%s IPC_Start channel:%d, time: %u.%u\n", 
								//	__func__, channel, tv1.tv_sec, tv1.tv_usec);
						printf("%s time: %u.%u, type: %d, number: %d, event: %d\n", 
							__func__, tv1.tv_sec, tv1.tv_usec, pheader->type, pheader->number, pheader->event);
					#endif

						if (pheader->length < sizeof(ifly_cp_header_t))
						{
							printf("%s ***Error*** pheader->length(%d) < sizeof(ifly_cp_header_t)(%d)\n", \
									__func__, pheader->length, sizeof(ifly_cp_header_t));
							continue;
						}
						
						msglen = pheader->length - sizeof(ifly_cp_header_t);
						//printf("%s msglen%d!\n", __func__, msglen);
						
						//���ݳ��ȳ������ջ����С
						if(msglen > BufSize-(int)sizeof(ifly_cp_header_t))
						{
							printf("%s ***Error*** OUTOFMEMORY msglen: %d > BufSize: %d\n", \
								__func__, msglen, BufSize);

							int data_size = msglen;
							int rcv_len = BufSize-(int)sizeof(ifly_cp_header_t);
							while (data_size > 0)
							{
								ret = looprecv(sock, pbuf+sizeof(ifly_cp_header_t), rcv_len);
								if (ret <= 0)
								{
									printf("%s ***Error*** OUTOFMEMORY looprecv error:[%d,%s]!\n", \
										__func__, errno, strerror(errno));
									break;
								}
								
								data_size -= rcv_len;

								rcv_len = data_size>(BufSize-(int)sizeof(ifly_cp_header_t)) ? (BufSize-(int)sizeof(ifly_cp_header_t)) : data_size;					
							}

							if (data_size == 0)//�������
							{
							#ifdef NVR_DEBUG
								printf("%s OUTOFMEMORY looprecv finish!\n", __func__);
							#endif
							}
							else	//���ճ���
							{
								//printf("%s ***Error*** OUTOFMEMORY looprecv failed\n", __func__);
							}
							
							continue;
			
						}
						
						//��������
						if (msglen > 0)
						{
							ret = looprecv(sock, pbuf+sizeof(ifly_cp_header_t), msglen);
							if (ret <= 0)
							{
								printf("%s ***Error*** looprecv msg, ret: %d, error:[%d,%s]!\n", \
									__func__, ret, errno, strerror(errno));
								continue;
							}
						}
							
						//���汾��
						if(pheader->version != CTRL_VERSION)
						{
							printf("%s ***Error*** pheader->version(%d) != CTRL_VERSION(%d)\n", \
								__func__, pheader->version,  CTRL_VERSION);
							continue;
						}
						
						//��������Ϣ
						if(pheader->type == CTRL_NOTIFY \
							&& pheader->event == CTRL_NOTIFY_ALARMINFO)
						{
							if (sizeof(ifly_alarmstate_t) != msglen)
							{
								printf("%s ***Error*** sizeof(ifly_alarmstate_t)%d != msglen%d!\n", \
									__func__, sizeof(ifly_alarmstate_t), msglen);
								continue;
							}

							memcpy(&alarm_state, pbuf+sizeof(ifly_cp_header_t), sizeof(ifly_alarmstate_t));
							
						//#ifdef NVR_DEBUG
						#if 1
							printf("%s type = %d, id = %d, state = %d\n", \
								__func__, alarm_state.type, alarm_state.id, alarm_state.state);
						#endif
							//ֻ����NVR Ӳ�̶�ʧ��Ӳ�̴��󱨾�
							if (alarm_state.type == 1 \
								|| alarm_state.type == 2)
								//|| alarm_state.type == 6)
							{
								int j, nvr_cmd_sock;
								u32 nvr_IP;
								
								for(j = 0; j < NvrLinkMax; j++)
								{
									pthread_mutex_lock(&g_nvr_info[j].lock);
									
									nvr_cmd_sock = g_nvr_info[j].cmd_fd;
									nvr_IP = g_nvr_info[j].dev_info.deviceIP;
									
									pthread_mutex_unlock(&g_nvr_info[j].lock);
									
									if (nvr_cmd_sock == sock)
									{
										//�任ǰ: 1 ������2 �ָ�
										//�任��: 1 ������0 �ָ�
										#if 1
											printf("%s enable type = %d, id = %d, state = %d\n", \
												__func__, alarm_state.type, alarm_state.id, alarm_state.state);
										#endif
										alarm_state.state = 2 - alarm_state.state;
										alarm_key = (alarm_state.type << 16) | (alarm_state.id << 8) | alarm_state.state;

										//Ӳ�̶�ʧ��Ӳ�̴����ָܻ�
										//if (alarm_state.type != 6) 
										{
											if (alarm_state.state == 0)
											{
												continue;
											}
										}
										
										DoStreamStateCallBack(0, REAL_STREAM_STATE_ALARM, nvr_IP, alarm_key);

										break;
									}
								}
							}							
						}

						//���������Ӧ��Ϣ
						if(pheader->type == CTRL_ACK)
						{
							if(pheader->number != g_wSyncNumber)
							{
								printf("%s ***Error*** pheader->number(%d) != g_wSyncNumber(%d)\n", \
									__func__, pheader->number,  g_wSyncNumber);
								continue;
							}
							
							g_dwSyncAckLen = pheader->length;
							memcpy(g_pbuf_rcv, pbuf, g_dwSyncAckLen);

							ret = sem_post(&g_hSyncSem);
							if (ret)
							{
								printf("%s ***Error*** sem_post failed, error:[%d,%s]!\n", 
									__func__, errno, strerror(errno));
							}
							else
							{
							#ifdef NVR_DEBUG
								printf("%s sem_post\n", __func__);
							#endif
							}
						}											
					}
				}
			}
		}
		else
		{
			sleep(1);
		}
	}

	free(pbuf);
	pbuf = NULL;
	pthread_exit((void *)0);
}

void* ThreadProcStream(void* pParam)
{
	u32 time_out_count = 0;
	struct timeval timeout;
	real_stream_s stream;
	u8 *pbuf = NULL;
	u32 buf_len = 0;
	int sock = INVALID_SOCKET;
	int ret = 0;
	int chn = (int)pParam;
	int real_chn = 0;
	u8 bexit = 0;
	fd_set set;
	ifly_MediaFRAMEHDR_t hdr;
	NVR_info *pnvr = NULL;

	if(!g_init_flag)
	{
		printf("%s ***Error*** chn%d, g_init_flag: %d\n", __func__, chn, g_init_flag);
		goto ProcQuit;
	}

	ret = pthread_detach(pthread_self());
	if(ret < 0)
	{
		printf("%s ***Error*** pthread_detach failed\n", __func__);
		goto ProcQuit;
	}
	
	if(chn < 0 || chn >= (int)g_client_max)
	{
		printf("%s ***Error*** chn%d invalid\n", __func__, chn);
		goto ProcQuit;
	}

	real_chn = (chn < (int)(g_client_max/2)) ? chn : (chn - (int)(g_client_max/2));
	
	if(chn == real_chn)//������
	{
		buf_len = 500*1024;
	}
	else	//������
	{
		buf_len = 200*1024;
	}
	
	pbuf = (u8 *)malloc(buf_len);
	if(NULL == pbuf)
	{
		printf("%s ***Error*** chn%d, pbuf malloc failed\n", __func__, chn);
		goto ProcQuit;
	}

#ifdef NVR_DEBUG
	printf("%s chn%d run, buf_len: %d\n", __func__, chn, buf_len);
#endif

	while (1)
	{
		bexit = 0;
		
		pthread_mutex_lock(&g_chn_info[chn].lock);

		sock = g_chn_info[chn].cap_fd;
		bexit = g_chn_info[chn].eventLoopWatchVariable;
		
		pthread_mutex_unlock(&g_chn_info[chn].lock);

		if (INVALID_SOCKET == sock)
		{
			printf("%s ***Error*** chn%d stream socket invalid!!!\n", __func__, chn);
			break;
		}
		
		if (bexit)
		{
		#ifdef NVR_DEBUG
			printf("%s chn%d bexit True!!!\n", __func__, chn);
		#endif
		
			break;
		}

		FD_ZERO(&set);
		FD_SET(sock, &set);
		//linuxƽ̨��timeout�ᱻ�޸��Ա�ʾʣ��ʱ��,��ÿ�ζ�Ҫ���¸�ֵ
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		ret = select(sock+1, &set, NULL, NULL, &timeout);
		if (ret == 0)
		{
			if(++time_out_count > 5)
			{
			#ifdef NVR_DEBUG
				printf("%s chn%d select sock timeout & quit!!!\n", __func__, chn);
			#endif
				time_out_count = 0;
				break;
			}
			else
			{
				//printf("ThreadPROCAnni:chn%d select sock timeout & continue!\n",chn);
				continue;
			}
		}
		else if (ret < 0)
		{
			printf("%s ***Error*** chn%d select sock error:[%s]!\n", __func__, chn, strerror(errno));
			break;
		}

		if (FD_ISSET(sock, &set))
		{
			time_out_count = 0;

			memset(&hdr, 0, sizeof(ifly_MediaFRAMEHDR_t));
			//����Э��ͷ
			ret = looprecv(sock, (char *)&hdr, sizeof(ifly_MediaFRAMEHDR_t));
			if (ret <= 0)
			{
				printf("%s ***Error*** chn%d looprecv hdr error:[%s]!\n", __func__, chn, strerror(errno));
				break;
			}

			hdr.m_dwDataSize = 	ntohl(hdr.m_dwDataSize);
			hdr.m_dwFrameID = 	ntohl(hdr.m_dwFrameID);
			hdr.m_dwTimeStamp = ntohl(hdr.m_dwTimeStamp);
			hdr.m_nVideoHeight = ntohl(hdr.m_nVideoHeight);
			hdr.m_nVideoWidth = ntohl(hdr.m_nVideoWidth);

			if (hdr.m_dwDataSize > buf_len)//���ݳ��ȳ���buf�����۽����Ƿ���ɶ��˳�
			{
				printf("%s ***Error*** chn%d m_dwDataSize:%d > buf_len:%d\n", \
					__func__, chn, hdr.m_dwDataSize, buf_len);
				
				int data_size = hdr.m_dwDataSize;
				int rcv_len = buf_len;
				while (data_size > 0)
				{
					ret = looprecv(sock, (char *)pbuf, rcv_len);
					if (ret <= 0)
					{
						printf("%s ***Error*** chn%d looprecv stream 1 error:[%s]!\n", __func__, chn, strerror(errno));
						break;
					}
					
					data_size -= rcv_len;

					rcv_len = data_size>(int)buf_len ? buf_len : data_size;					
				}

				if (data_size == 0)//�������
				{
				#ifdef NVR_DEBUG
					printf("%s chn%d looprecv stream 1 finish!\n", __func__, chn);
				#endif
				}
				else	//���ճ���
				{
				}
				
				break;
			}
			else	//������������ݳ���δ����buf
			{
				ret = looprecv(sock, (char *)pbuf, hdr.m_dwDataSize);
				if (ret <= 0)
				{
					printf("%s ***Error*** chn%d looprecv stream 2 error:[%s]!\n", __func__, chn, strerror(errno));
					break;
				}

				if (hdr.m_byMediaType == EM_ENCODE_VIDEO_H264)
				{
					memset(&stream, 0, sizeof(stream));
					stream.chn = chn;
					stream.media_type = MEDIA_PT_H264;
					if(hdr.m_bKeyFrame)
					{
						stream.frame_type = REAL_FRAME_TYPE_I;
					}
					else
					{
						stream.frame_type = REAL_FRAME_TYPE_P;
					}
					stream.pts = hdr.m_dwTimeStamp;
					stream.pts *= 1000;
					stream.len = hdr.m_dwDataSize;
					stream.data = (unsigned char *)pbuf;
					stream.width = hdr.m_nVideoWidth;
					stream.height = hdr.m_nVideoHeight;
					
				#ifdef NVR_DEBUG
					//printf("%s chn: %d, frame_type: %d, pts: %llu, len: %d, width: %d, height: %d\n", 
					//	__func__, stream.chn, stream.frame_type, stream.pts, stream.len, stream.width, stream.height);
				#endif
				
					g_chn_info[chn].pStreamCB(&stream, g_chn_info[chn].dwContext);
					
				}
			}
		}		
	}

	free(pbuf);
	pbuf = NULL;

	//����g_chn_info[chn]
#ifdef NVR_DEBUG
	printf("%s chn%d exit 1\n", __func__, chn);
#endif

	pthread_mutex_lock(&g_chn_info[chn].lock);
	pnvr = g_chn_info[chn].pnvr_info;
	
	if (NULL != pnvr)
	{
		if (!bexit) //������˳�ѭ��
		{
			u32 alarm_key = 0;
			u32 alarm_chn = g_chn_info[chn].nvr_chn;
			//			type			chn			status
			alarm_key = (6 << 16) | (alarm_chn << 8) | 1;

			//��Ƶ��ʧ	
			DoStreamStateCallBack(0, REAL_STREAM_STATE_ALARM, pnvr->dev_info.deviceIP, alarm_key);
			//printf("%s chn%d video lost\n", __func__, alarm_chn);
		}
		
		pthread_mutex_lock(&pnvr->lock);

	#ifdef NVR_DEBUG
		printf("%s chn%d exit 2\n", __func__, chn);
	#endif

		DestroyChnInfo(&g_chn_info[chn], pnvr);
		
		pthread_mutex_unlock(&pnvr->lock);
	}
	
	ret = pthread_cond_broadcast(&g_chn_info[chn].CondThreadOver);
	if (ret)
	{
		printf("%s ***Error*** pthread_cond_broadcast chn%d failed, ret: %d, error[%d, %s]\n", \
			__func__, chn, ret, errno, strerror(errno));
	}
	else
	{
		#ifdef NVR_DEBUG
			printf("%s chn%d pthread_cond_broadcast success\n", __func__, chn);
		#endif
	}
	
	pthread_mutex_unlock(&g_chn_info[chn].lock);

#ifdef NVR_DEBUG
	printf("%s chn%d exit 3\n", __func__, chn);
#endif

	DoStreamStateCallBack(chn, REAL_STREAM_STATE_LOST, 0, 0);

ProcQuit:

	pthread_exit((void *)0);

}


int NVR_Init(unsigned int max_client_num)
{
	int i = 0;
	int ret = 0;
	
	printf("%s start, max_client_num: %u\n", __func__, max_client_num);
	
	g_client_max = max_client_num;

	//chn_info
	pthread_condattr_t condattr;
	pthread_condattr_init(&condattr);
	pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
	
	g_chn_info = (CHN_info *)malloc(g_client_max * sizeof(CHN_info));
	if(g_chn_info == NULL)
	{
		printf("%s ***Error*** malloc g_chn_info\n", __func__);
		goto exit1;
	}
	memset(g_chn_info, 0, g_client_max*sizeof(CHN_info));
	
	for(i = 0; i < (int)g_client_max; i++)
	{
		g_chn_info[i].cap_fd = INVALID_SOCKET;//��Ч�ı�־
		g_chn_info[i].pid = INVALID_PID;
		pthread_mutex_init(&g_chn_info[i].lock, NULL);
		pthread_cond_init(&g_chn_info[i].CondThreadOver, &condattr);//��ʼ����������
	}

	//NVR_info
	g_nvr_info = (NVR_info *)malloc(NvrLinkMax * sizeof(NVR_info));
	if(g_nvr_info == NULL)
	{
		printf("%s ***Error*** malloc g_nvr_info\n", __func__);
		goto exit2;
	}
	memset(g_nvr_info, 0, NvrLinkMax*sizeof(NVR_info));
	
	for(i = 0; i < (int)NvrLinkMax; i++)
	{
		g_nvr_info[i].valid = 0;
		g_nvr_info[i].cmd_fd = INVALID_SOCKET;//��Ч�ı�־
		g_nvr_info[i].display_chn= INVALID_VAL;//��Чֵ
		g_nvr_info[i].tm = INVALID_VAL;//��Чֵ
		
		pthread_mutex_init(&g_nvr_info[i].lock, NULL);		
	}

	//��ʼ��������Ϣͬ������
	g_pbuf_snd = (char *)malloc(BufSize);
	if(g_pbuf_snd == NULL)
	{
		printf("%s ***Error*** malloc g_pbuf_snd\n", __func__);
		goto exit3;
	}

	g_pbuf_rcv = (char *)malloc(BufSize);
	if(g_pbuf_rcv == NULL)
	{
		printf("%s ***Error*** malloc g_pbuf_rcv\n", __func__);
		goto exit4;
	}

	ret = sem_init(&g_hSyncSem, 0, 0);
	if (ret)
	{
		printf("%s ***Error*** sem_init, error[%d, %s]\n", __func__, errno, strerror(errno));
		goto exit5;
	}
	pthread_mutex_init(&g_hSyncMutex, NULL);
	g_dwSyncAckLen = 0;
	g_wSyncNumber = 0;

	for(i = 0; i < (int)MaxLinstenFds; i++)
	{
		g_linsten_info.linsten_fds[i] = INVALID_SOCKET;//��Ч�ı�־		
	}
	pthread_mutex_init(&g_linsten_info.lock, NULL);

	//ע�������ص�����
	SetFindDeviceCB(SearchDevice_CB, NULL);

	//�������������߳�	
	ret = pthread_create(&g_msgrcv_thread_id, NULL, ThreadMsgRcvFxn, NULL);
	if(ret)
	{
		printf("%s create msg rcv thread failure, ret: %d, error[%d, %s]\n", \
			__func__, ret, errno, strerror(errno));
		goto exit5;
	}

	ret = pthread_create(&g_destroy_thread_id, NULL, ThreadDestroyNvrFxn, NULL);
	if(ret)
	{
		printf("%s create destroy nvr thread failure, ret: %d, error[%d, %s]\n", \
			__func__, ret, errno, strerror(errno));
		goto exit5;
	}
	
	g_init_flag = 1;
	
	printf("%s end\n", __func__);
	return 0;

exit5:
	if (g_pbuf_rcv)
	{
		free(g_pbuf_rcv);		
	}
	g_pbuf_rcv = NULL;
		
exit4:
	if (g_pbuf_snd)
	{
		free(g_pbuf_snd);
	}
	g_pbuf_snd = NULL;

exit3:
	if (g_nvr_info)
	{
		free(g_nvr_info);		
	}
	g_nvr_info = NULL;
		
exit2:
	if (g_chn_info)
	{
		free(g_chn_info);
	}
	g_chn_info = NULL;
	
exit1:
	return -1;
}

int NVR_DeInit()
{
	return 0;
}

int NVR_Start(int chn, RealStreamCB pCB, unsigned int dwContext, char* streamInfo, unsigned int dwIp, unsigned short wPort, char *user, char *pwd, unsigned char req_nvr_chn)
{
	int ret = 0;
	NVR_info *pnvr = NULL;
	u8 bcreate_nvr = 0; //�ڱ��������Ƿ�������NVR
	int real_chn = 0;
	u8 bmain = 0;//��/������
	int stream_sock = INVALID_SOCKET;
	u32 link_id = 0;
	pthread_t thread_id = 0;
	u32 alarm_key = 0;
	u32 alarm_chn = 0;
	
	if(!g_init_flag)
	{
		printf("%s ***Error*** g_init_flag: %d\n", __func__, g_init_flag);
		return -1;
	}
	
	if(chn < 0 || chn >= (int)g_client_max)
	{
		printf("%s ***Error*** chn: %d\n", __func__, chn);
		return -1;
	}

	real_chn = (chn < (int)(g_client_max/2)) ? chn : (chn - (int)(g_client_max/2));

#ifdef NVR_DEBUG	
	printf("%s chn=%d, real_chn=%d, req_nvr_chn=%d\n", \
		__func__, chn, real_chn, req_nvr_chn);
#endif

	ret = NVR_GetLinkStatus(chn);
	if(ret)
	{
		printf("%s ***Error*** NVR_GetLinkStatus chn%d, ret: %d\n", __func__, chn, ret);
		return -1;
	}

	pthread_mutex_lock(&g_chn_info[chn].lock);

#ifdef NVR_DEBUG
	printf("%s chn%d lock\n", __func__, chn);
#endif

	pnvr = getNvrInfoByIP_lock(dwIp);//�ɹ�����ʱ������
	if (NULL == pnvr)
	{
		printf("%s ***Error*** chn%d, getNvrInfoByIP_lock failed\n", __func__, chn);

		pthread_mutex_unlock(&g_chn_info[chn].lock);
		return -1;
	}

	//bcreate_nvr = 0;
	if (0 == pnvr->valid)	//nvr δ����
	{
		ret = CreateNvrInfo(pnvr, dwIp, wPort, user, pwd);
		if (ret)
		{
			printf("%s ***Error*** chn%d CreateNvrInfo failed\n", __func__, chn);

			goto exit1;
		}
			
		//bcreate_nvr = 1;
	}
	
	//���
	if(req_nvr_chn >= pnvr->dev_info.maxChnNum)
	{
		printf("%s ***Error*** chn%d, req_nvr_chn%d >= nvr_max_chn_num%d\n", \
			__func__, chn, req_nvr_chn, pnvr->dev_info.maxChnNum);
		
		goto exit2;
	}

	//������Ƶ��
	bmain = chn==real_chn ? 1:0;//��/������

	//��ѯNVR��Ӧͨ��������������Ƿ��������
	ret = checkNvrChnLinkStatus(pnvr, req_nvr_chn , bmain);
	if (ret < 0)//����
	{
		printf("%s ***Error*** checkNvrChnLinkStatus failed, req_nvr_chn: %d , bmain: %d\n", \
			__func__, req_nvr_chn , bmain);
		goto exit2;
	}
	else if (ret == 0)//NVR��ǰͨ��������
	{
	#if 1	//#ifdef NVR_DEBUG
		printf("%s ***Warning*** checkNvrChnLinkStatus = 0, req_nvr_chn: %d , bmain: %d\n", \
			__func__, req_nvr_chn , bmain);
	#endif
		goto exit2;
	}
	
	stream_sock = INVALID_SOCKET;
	link_id = 0;
	ret = NVRStartStream(dwIp, wPort, req_nvr_chn , bmain, &stream_sock, &link_id);
	if (ret)
	{
		printf("%s ***Error*** chn%d NVR_StartStream failed\n", \
			__func__, chn);

		goto exit2;
	}
#ifdef NVR_DEBUG
	printf("%s NVR_StartStream success, link_id: %d\n", __func__, link_id);
#endif

	ret = pthread_create(&thread_id, NULL, ThreadProcStream, (void *)chn);
	if (ret)
	{
		printf("%s ***Error*** chn%d pthread_create failed, ret :%d, error[%d, %s]\n", __func__, chn, ret, errno, strerror(errno));
		goto exit3;
	}
	
	//���CHN_info
	g_chn_info[chn].nvr_chn = req_nvr_chn;
	g_chn_info[chn].sys_chn = chn;
	g_chn_info[chn].eventLoopWatchVariable = 0;
	g_chn_info[chn].pid = thread_id;
	g_chn_info[chn].cap_fd = stream_sock;
	g_chn_info[chn].link_id = link_id;
	g_chn_info[chn].pStreamCB = pCB;
	g_chn_info[chn].dwContext = dwContext;
	
	ChnAttachNvr(&g_chn_info[chn], pnvr);

	alarm_key = 0;
	alarm_chn = g_chn_info[chn].nvr_chn;
	//			type			chn			status
	alarm_key = (6 << 16) | (alarm_chn << 8) | 0;

	//��Ƶ�ָ�
	DoStreamStateCallBack(0, REAL_STREAM_STATE_ALARM, pnvr->dev_info.deviceIP, alarm_key);
	//printf("%s chn%d video resume\n", __func__, alarm_chn);
	
	putNvrInfo(pnvr);
	pthread_mutex_unlock(&g_chn_info[chn].lock);

#ifdef NVR_DEBUG
	printf("%s chn:%d, release lock\n", __func__, chn);
#endif
	
	return 0;

exit3:
	ret = NVRStopStream(pnvr->cmd_fd, link_id);
	if (ret)
	{
		printf("%s ***Error*** chn%d NVRStopStream failed\n", \
			__func__, chn);
	}
	close(stream_sock);

exit2:
	//if (bcreate_nvr)
	if (pnvr->valid)
	{
		ret = DestroyNvrInfo(pnvr);//ֻ��ref_cnt ==  0��������������
		if (ret)
		{
			printf("%s ***Error*** chn%d DestroyNvrInfo failed\n", __func__, chn);
		}
	}
	
exit1:
	putNvrInfo(pnvr);
	pthread_mutex_unlock(&g_chn_info[chn].lock);

	return -1;
}

//caution: g_chn_info[chn] �ṹ���������߳��˳�ǰ����
int NVR_Stop(int chn)
{
	int ret = 0;
	//u8 bstop = 0;
	//pthread_t pid = INVALID_PID;
	
	if(!g_init_flag)
	{
		return -1;
	}
	
	if(chn < 0 || chn >= (int)g_client_max)
	{
		return -1;
	}
#ifdef NVR_DEBUG
	printf("%s chn%d start\n", __func__, chn);
#endif

#if 0	
	bstop = 0;
	
	pthread_mutex_lock(&g_chn_info[chn].lock);

	printf("%s chn%d lock\n", __func__, chn);
	
	if(INVALID_PID != g_chn_info[chn].pid)
	{		
		//g_chn_info[chn] �ṹ�����߳��˳�ǰ���٣������ڴ˱����Ժ���
		pid = g_chn_info[chn].pid;
		g_chn_info[chn].pid = INVALID_PID;
		g_chn_info[chn].eventLoopWatchVariable = 1;
		
		bstop = 1;
	}
	
	pthread_mutex_unlock(&g_chn_info[chn].lock);
	printf("%s chn%d, release lock, bstop: %d, pid: %d\n", __func__, chn, bstop, pid);

	if (bstop)
	{
		ret = pthread_join(pid, NULL);
		if (ret)
		{
			printf("%s pthread_join failed, error[%d, %s]\n", __func__, errno, strerror(errno));
		}
	}
#else

	pthread_mutex_lock(&g_chn_info[chn].lock);

#ifdef NVR_DEBUG
	printf("%s chn%d lock\n", __func__, chn);
#endif	

	if (INVALID_SOCKET != g_chn_info[chn].cap_fd)
	{
		
	#ifdef NVR_DEBUG
		printf("%s chn%d pthread_cond_timedwait\n", __func__, chn);
	#endif
	
		g_chn_info[chn].eventLoopWatchVariable = 1;
	
		struct timespec tv;
		clock_gettime(CLOCK_MONOTONIC, &tv);
		tv.tv_sec += 5;//��ʱ5s
	
		ret = pthread_cond_timedwait(&g_chn_info[chn].CondThreadOver, &g_chn_info[chn].lock, &tv);
		if (ret)
		{
			if (ETIMEDOUT == ret)
			{
				printf("%s chn%d pthread_cond_timedwait timeout, cap_fd: %d\n", \
					__func__, chn, g_chn_info[chn].cap_fd);
			}
			else
			{
				printf("%s ***Error*** chn%d pthread_cond_timedwait failed, cap_fd: %d, ret: %d, error[%d, %s]\n", \
					__func__, chn, g_chn_info[chn].cap_fd, ret, errno, strerror(errno));
			}

			NVR_info *pnvr = g_chn_info[chn].pnvr_info;
			if (NULL != pnvr)
			{
				pthread_mutex_lock(&pnvr->lock);

				DestroyChnInfo(&g_chn_info[chn], pnvr);
				
				pthread_mutex_unlock(&pnvr->lock);
			}
		}
		else
		{
		#ifdef NVR_DEBUG
			printf("%s pthread_cond_timedwait chn%d success\n", __func__, chn);
		#endif
		}
	}

	pthread_mutex_unlock(&g_chn_info[chn].lock);
#endif

#ifdef NVR_DEBUG
	printf("%s chn%d success\n", __func__, chn);
#endif
	return 0;
}


//return value : 1 - Link; 0 - Lost
int NVR_GetLinkStatus(int chn)
{
	if(!g_init_flag)
	{
		return 0;
	}
	
	if(chn < 0 || chn >= (int)g_client_max)
	{
		return -1;
	}
	
	int status = 0;
	
	pthread_mutex_lock(&g_chn_info[chn].lock);
	
	status = (g_chn_info[chn].cap_fd != INVALID_SOCKET);
	
	pthread_mutex_unlock(&g_chn_info[chn].lock);
	
	return status;
}


int NVR_Search(ipc_node** head, unsigned char check_conflict)
{
	int ret = 0;

	//������Ӧ������Ӧ�߳�
	if (g_ack_thread_id)
	{
		//ȡ����Ӧ�߳�
		ret = pthread_cancel(g_ack_thread_id);
		if (ret)
		{
			printf("%s pthread_cancel failed\n", __func__);
			return 0;
		}
		
		ret = pthread_join(g_ack_thread_id, NULL);
		if (ret)
		{
			printf("%s pthread_join failed\n", __func__);
			return 0;
		}

		g_ack_thread_id = 0;
	}
	
	ret = CreateAckSearchProc(&g_ack_thread_id);
	if (ret)
	{
		printf("%s CreateAckSearchProc failed\n", __func__);
		g_ack_thread_id = 0;
		return 0;
	}
	
	pthread_mutex_lock(&g_search_lock);

	if (g_search_start)//�Ѿ���ʼ����
	{
		pthread_mutex_unlock(&g_search_lock);
		printf("%s has already started\n", __func__);
		return 0;
	}
	
	g_search_start = 1;
	g_search_head = g_search_tail = NULL;
	
	pthread_mutex_unlock(&g_search_lock);
	
	SearchDevice();
	sleep(2);

	//ȡ����Ӧ�߳�
	ret = pthread_cancel(g_ack_thread_id);
	if (ret)
	{
		printf("%s pthread_cancel failed\n", __func__);
		return 0;
	}
	
	ret = pthread_join(g_ack_thread_id, NULL);
	if (ret)
	{
		printf("%s pthread_join failed\n", __func__);
		return 0;
	}
	
	g_ack_thread_id = 0;
	
	//�����������
	pthread_mutex_lock(&g_search_lock);
	
	*head = g_search_head;
	
	g_search_start = 0;
	g_search_head = g_search_tail = NULL;
	
	pthread_mutex_unlock(&g_search_lock);
	return 0;
}


