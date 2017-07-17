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


#include "klw.h"

#include "VVV_Search.h"
#include "VVV_NET_define.h"
#include "VVV_NET.h"

#include "iconv.h"
#include "libcharset.h"
#include "localcharset.h"
/**********************************************************
����Ҫ����u32DevHandle �ĵط�����Ҫ�������������

**********************************************************/

#define INVALID_DEVHDL	(unsigned int)(-1)
#define INVALID_CHNHDL	(unsigned int)(-1)

typedef struct
{
	unsigned int u32DevHandle;
	unsigned int u32ChnHandle;
	RealStreamCB pStreamCB;
	unsigned int dwContext;
	int nLost;
	int nMDEvent;
	//unsigned int nIPCEstAlarm;
	//unsigned int nIPCCover;
	//SNAP_CALLBACK Snap_CB;
	//ȷ��ץͼ�ص����
	//pthread_mutex_t LockSnapOver;
	//pthread_cond_t CondSnapOver;
	//bool bFlagSnapBusy;
	
	pthread_mutex_t lock;
}klw_client_info;

//volatile SNAP_CALLBACK Snap_CB[32];
//volatile bool bFlagSnapBusy[32];
//���λص������Ƿ�����޸ģ�0�����޸ģ�1�ȴ�



static klw_client_info *g_klwc_info = NULL;

static unsigned int g_klw_client_count = 0;//32: 0-31

static unsigned char g_init_flag = 0;

static unsigned char g_sdk_inited = 0;
/*
static unsigned int ipc_ext_alarm = 0;
static unsigned int ipc_cover_alarm = 0;


unsigned int KLW_get_alarm_IPCExt(void)
{
	unsigned int tmp;

	tmp = ipc_ext_alarm;
	ipc_ext_alarm = 0;

	return tmp;
}
unsigned int KLW_get_alarm_IPCCover(void)
{
	unsigned int tmp;

	tmp = ipc_cover_alarm;
	ipc_cover_alarm = 0;

	return tmp;
}
*/

int KLW_Init(unsigned int max_client_num)
{
	if(max_client_num <= 0)
	{
		return -1;
	}
	
	g_klw_client_count = max_client_num;
	
	g_klwc_info = (klw_client_info *)malloc(g_klw_client_count*sizeof(klw_client_info));
	if(g_klwc_info == NULL)
	{
		return -1;
	}
	memset(g_klwc_info, 0, g_klw_client_count*sizeof(klw_client_info));

/*
CLOCK_MONOTONIC��Represents monotonic time. Cannot be set. ��ʾ����ʱ�䣬
Ϊϵͳ���ʱ�����ڵ�ʱ��,���ܱ����ø��޸�.
*/
	//pthread_condattr_t condattr;
	//pthread_condattr_init(&condattr);
	//pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
	
	int i = 0;
	for(i = 0; i < (int)g_klw_client_count; i++)
	{
		g_klwc_info[i].u32DevHandle = INVALID_DEVHDL;
		g_klwc_info[i].u32ChnHandle = INVALID_CHNHDL;
		
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&g_klwc_info[i].lock, &attr);
		pthread_mutexattr_destroy(&attr);
		//pthread_mutex_init(&g_klwc_info[i].CB_lock, NULL);//�ǵݹ���
		
		/*ȷ��ץͼ�ص����
		pthread_mutex_t LockSnapOver;
		pthread_cond_t CondSnapOver;
		bool bFlagSnapOver;
		*/
		//pthread_mutex_init(&g_klwc_info[i].LockSnapOver, NULL);//�ǵݹ���
		//pthread_cond_init(&g_klwc_info[i].CondSnapOver, &condattr);//��ʼ����������
		//g_klwc_info[i].bFlagSnapBusy = 0;
		//bFlagSnapBusy[i] = 0;
		//Snap_CB[i] = NULL;

		//pthread_mutex_init(&g_klwc_info[i].LockSnapCBChange, NULL);//�ǵݹ���
		//pthread_cond_init(&g_klwc_info[i].CondSnapCBChange, &condattr);//��ʼ����������
		//bFlagSnapCBChange[i] = 0;//�����޸�
	}
	
	g_init_flag = 1;
	
	return 0;
}

int KLW_DeInit()
{
	return 0;
}

int OnEvenFunc2(unsigned int u32Handle,    /* ��� */
               unsigned int u32Event,     /* �¼������LIVE_NET_STAT_E*/
               void* pUserData,           /* �û�����*/
               VVV_STREAM_INFO_S* pStreamInfo)/*��������*/
{
	printf("OnEvenFunc2 handle: %d event: %d\n", u32Handle, u32Event);
	if(u32Event != NETSTAT_TYPE_CONNING)
	{
		
	}
	return 0;
}

int KLW_GetNetworkParam(ipc_unit *ipcam, ipc_neteork_para_t *pnw)
{
	if(!g_init_flag)
	{
		return -1;
	}
	
	if(ipcam == NULL)
	{
		return -1;
	}
	
	if(pnw == NULL)
	{
		return -1;
	}
	
	if(ipcam->net_mask)
	{
		pnw->ip_address = ipcam->dwIp;
		pnw->net_mask = ipcam->net_mask;
		pnw->net_gateway = ipcam->net_gateway;
		pnw->dns1 = ipcam->dns1;
		pnw->dns2 = ipcam->dns2;
		
		return 0;
	}
	
	if(!g_sdk_inited)
	{
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	
	unsigned int u32DevHandle = INVALID_DEVHDL;
	
	char devip[64];
	memset(devip, 0, sizeof(devip));
	
	struct in_addr serv;
	serv.s_addr = ipcam->dwIp;
	sprintf(devip, "%s", inet_ntoa(serv));
	
	userpassword_s stUserAuth;
	VVV_NET_PROTOCOL_S stNetProtocol;
	stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
	stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
	stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
	stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
	int ret = VVV_NET_Login(&u32DevHandle, ipcam->user, ipcam->pwd, devip, ipcam->wPort, stNetProtocol, 5000, OnEvenFunc2, &stUserAuth, NULL);
	if(ret != VVV_SUCCESS)
	{
		printf("KLW_GetNetworkParam: VVV_NET_Login failed\n");
		u32DevHandle = INVALID_DEVHDL;
		return -1;
	}
	
	VVV_SERVER_NETWORK_S stIPCNetworkAttr;
	unsigned int s32Size = sizeof(VVV_SERVER_NETWORK_S);
	memset(&stIPCNetworkAttr, 0, s32Size);
	//stIPCNetworkAttr.bSaveFlash = 1;
	stIPCNetworkAttr.dwSize = s32Size;
	ret = VVV_NET_GetServerConfig(u32DevHandle, VVV_CMD_GET_NETWORK, (void*)&stIPCNetworkAttr, &s32Size);
	
	VVV_NET_Logout(u32DevHandle);
	u32DevHandle = INVALID_DEVHDL;
	
	if(ret != VVV_SUCCESS)
	{
		printf("VVV_NET_GetServerConfig VVV_CMD_GET_NETWORK failed:%d\n",ret);
		return -1;
	}
	
	//printf("VVV_NET_GetServerConfig VVV_CMD_GET_NETWORK success\n");
	
	unsigned int *p = NULL;
	char address[32];
	
	p = stIPCNetworkAttr.IPAddr;
	sprintf(address,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	//printf("VVV_CMD_GET_NETWORK ip=%s\n",address);
	pnw->ip_address = inet_addr(address);
	
	p = stIPCNetworkAttr.NetMask;
	sprintf(address,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	//printf("VVV_CMD_GET_NETWORK netmask=%s\n",address);
	pnw->net_mask = inet_addr(address);
	
	p = stIPCNetworkAttr.GateWay;
	sprintf(address,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	//printf("VVV_CMD_GET_NETWORK gw=%s\n",address);
	pnw->net_gateway = inet_addr(address);
	
	p = stIPCNetworkAttr.DnsFirst;
	sprintf(address,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	//printf("VVV_CMD_GET_NETWORK dns1=%s\n",address);
	pnw->dns1 = inet_addr(address);
	
	p = stIPCNetworkAttr.DnsSecond;
	sprintf(address,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	//printf("VVV_CMD_GET_NETWORK dns2=%s\n",address);
	pnw->dns2 = inet_addr(address);
	
	return 0;
}

//#define COMMAND	 "netconf set -devname eth0 -ipaddr %s -netmask %s -gateway %s -fdnsip %s -dhcp off"
#define COMMAND	 "netconf set -devname eth0 -ipaddr %s -netmask %s -gateway %s -dhcp off"

int CallBackModify(const void* pvHandle,/*���*/char* pszRNPCode,/*����*/VVV_SEARCH_CMD_RES_S* pstruResponseInfo,/*������Ϣ*/void* pvUserData)  /*�û�����*/
{
	printf("CallBackModify:Cmd=%s,ResultInfo=%s,Code=%s\n",pstruResponseInfo->pszCmd,pstruResponseInfo->pszResultInfo,pszRNPCode);
	return 0;
}

int KLW_SetNetworkParam(ipc_unit *ipcam, ipc_neteork_para_t *pnw)
{
	if(!g_init_flag)
	{
		return -1;
	}
	
	if(ipcam == NULL)
	{
		return -1;
	}
	
	if(pnw == NULL)
	{
		return -1;
	}
	
	char ip[20];
	char netmask[20];
	char gateway[20];
	char fdnsip[20];
	char sdnsip[20];
	
	struct in_addr host;
	
	host.s_addr = pnw->ip_address;
	strcpy(ip, inet_ntoa(host));
	
	host.s_addr = pnw->net_mask;
	strcpy(netmask, inet_ntoa(host));
	
	host.s_addr = pnw->net_gateway;
	strcpy(gateway, inet_ntoa(host));
	
	host.s_addr = pnw->dns1;
	strcpy(fdnsip, inet_ntoa(host));
	
	host.s_addr = pnw->dns2;
	strcpy(sdnsip, inet_ntoa(host));
	
	char cmd[320];
	//sprintf(cmd,COMMAND,ip,netmask,gateway,fdnsip);
	sprintf(cmd,COMMAND,ip,netmask,gateway);
	
	int ret = VVV_SEARCH_Init();
	if (0 != ret)
	{
		printf("VVV_SEARCH_Init Erro: %d\n", ret);
		return -1;
	}
	
	VVV_SEARCH_RegistModifyResultProc(CallBackModify, NULL);
	
	int result = 0;
	
	VVV_SEARCH_DEVID_S device;
	device.pszDevID = ipcam->uuid;
	device.pszUserName = ipcam->user;
	device.pszPasswd = ipcam->pwd;
	ret = VVV_SEARCH_Modify(&device, cmd);
	if (0 != ret)
	{
		printf("VVV_SEARCH_Modify Erro: %d\n", ret);
		result = -1;
	}
	else
	{
		printf("VVV_SEARCH_Modify success\n");
	}
	
	ret = VVV_SEARCH_DeInit();
	if (0 != ret)
	{
		printf("VVV_SEARCH_DeInit Erro: %d\n", ret);
		result = -1;
	}
	
	return result;
}

#define MDCOUNT	30

//receive alarm data
int OnAlarmFunc(unsigned int u32ChnHandle,/* ͨ����� */
                   unsigned int u32DataType, /* ��������,���STREAM_TYPE_E
                                                0x04-MD�澯����,
                                                0x08-̽ͷ����澯����,
                                                0x10-��Ƶ�ڵ��澯����,
                                                0x20-��Ƶ��ʧ�澯����,
                                                0x40-���̴���澯*/
                   unsigned char*  pu8Buffer,/* ����,MD�澯����Ϊ
							"Alarm:�������������-���Ͻ�X����-���Ͻ�Y����-��-��|�������������-���Ͻ�X����-���Ͻ�Y����-��-��|",
							����"Alarm:1-558-461-120-120|2-520-0-120-120|3-0-360-120-120|4-520-360-120-120|"*/
                   unsigned int u32Length,          /* ���ݳ��� */
                   VVV_U64      u64TimeStamp,       /* ʱ���*/
                   void* pUserData)                /* �û�����*/
{
	int chn = (int)pUserData;

	if(chn < 0 || chn >= (int)(g_klw_client_count))
	{
		printf("OnAlarmFunc chn%d error\n",chn);
		return -1;
	}
	//printf("OnAlarmFunc Alarm type=%d, Alarm Data=%s, User Data=%d\n",u32DataType,pu8Buffer,(int)pUserData);
	
	//if(u32DataType & 0x4)
	//if(u32DataType == 0x4)
	if (VVV_STREAM_MD_ONLY == u32DataType)//0x4
	{
		g_klwc_info[chn].nMDEvent = MDCOUNT;
	}
	else if (VVV_STREAM_SENSORIN_ONLY == u32DataType)
	{
		//printf("yg ipc_ext_alarm chn: %d\n", chn);
		 IPC_set_alarm_IPCExt(chn);
		
	}
	else if (VVV_STREAM_SHELTER_ONLY == u32DataType)
	{
		//printf("yg ipc_cover_alarm chn: %d\n", chn);
		 IPC_set_alarm_IPCCover(chn);
	}
	
	
	return 0;
}

extern int DoStreamStateCallBack(int chn, real_stream_state_e msg, unsigned int nvrIP, unsigned int alarm_key);

int OnEvenFunc(unsigned int u32Handle,    /* ��� */
               unsigned int u32Event,     /* �¼������LIVE_NET_STAT_E*/
               void* pUserData,           /* �û�����*/
               VVV_STREAM_INFO_S* pStreamInfo)/*��������*/
{
	printf("OnEvenFunc handle: %d event: %d\n", u32Handle, u32Event);
	#if 1
	if ((u32Event != NETSTAT_TYPE_CONNING) && (u32Event != NETSTAT_TYPE_CONNED))
	{
		int i;
		for(i=0; i<(int)g_klw_client_count; i++)
		{
			int chn = i;
			//printf("chn%d u32DevHandle:%d, u32ChnHandle:%d\n", 
				//chn, g_klwc_info[chn].u32DevHandle, g_klwc_info[chn].u32ChnHandle);
			if((g_klwc_info[chn].u32ChnHandle == u32Handle) && (u32Handle != INVALID_CHNHDL))
			{
				//if(KLW_GetLinkStatus(chn))
				{
					//yaogang modify 20140918
					printf("OnEvenFunc stream lost chn%d\n", chn);
					DoStreamStateCallBack(chn, REAL_STREAM_STATE_LOST, 0, 0);
					//������Bug
					//�����������ᷢ���쳣�˳�
					//KLW_Stop(chn);
					//csp modify 20140520
					g_klwc_info[chn].nLost = 1;
				}
			}
		}
	}
	#endif
	return 0;
}

typedef struct video_frameinfo_t
{
  int format_type;                    /*  ��Ƶ�����ʽ  */
  int stream_flag;					          /*	�������ͣ�0����������1��С������2���ֻ�����*/	
  int channel;						            /*	ͨ����	*/
  VVV_U64  u64TimeStamp;               /*  ʱ��� */
  time_t  frame_time;
  int msecond;
  int width;
  int height;
}video_frameinfo_t;

extern "C" unsigned int getTimeStamp();

#if 0
#define TestFileLen	(20*1024*1024)
static char *ptest_data = NULL;
static unsigned int test_data_len = 0;
static unsigned int test_min_len = 256*1024;
static unsigned char test_status = 0;
static FILE *pf_test = NULL;
#endif

int KLW_DataCB(unsigned int u32ChnHandle,/* ͨ����� */
                     unsigned int u32DataType,/* �������ͣ����VVV_STREAM_TYPE_E 0x01-��Ƶ��0x02-��Ƶ*/
                     unsigned char* pu8Buffer,/* ���ݰ���֡ͷ */
                     unsigned int u32Length,  /* ���ݳ��� */
                     VVV_U64  u64TimeStamp,    /* ʱ���*/
                     VVV_STREAM_INFO_S *pStreamInfo,/*��������*/
                     void* pUserData)         /* �û�����*/
{
	int i;
	struct timeval tv1, tv2;
	static unsigned int max_interval = 0;
	unsigned int yg_interval = 0;
	static unsigned int cnt = 0;
	
	for(i=0; i<(int)g_klw_client_count; i++)
	{
		int chn = i;
		if(g_klwc_info[chn].u32ChnHandle == u32ChnHandle)
		{
			if(g_klwc_info[chn].pStreamCB != NULL)
			{
				if(u32DataType == VVV_STREAM_TYPE_VIDEO && pStreamInfo->struVencChAttr.enVedioFormat == VVV_VIDEO_FORMAT_H264)
				{
					real_stream_s stream;
					//memset(&stream, 0, sizeof(stream));
					
					unsigned int t;

					#if 0
					if (chn == 0)
					{
						if (u32Length > 5)
						{
							printf("\t IpcPts: %llu, LocalPts: %u,length: %d, 0x%x 0x%x 0x%x 0x%x 0x%x\n",	\
								u64TimeStamp,	\
								getTimeStamp(),	\
								u32Length,	\
								pu8Buffer[0], pu8Buffer[1], pu8Buffer[2], pu8Buffer[3], pu8Buffer[4]);
						}
					}

					//if ( (pu8Buffer[4] == 0x61) && test_min_len > u32Length )
							//test_min_len = u32Length;
					
					#endif
					/*
					//if (chn < 16)
					if (chn > 15)
					{
						t = getTimeStamp(); //ms
						printf("%s IpcPts: %llu, LocalPts: %u, u32Length: %d\n", __func__, u64TimeStamp, t, u32Length);
					}
					*/
					#if 0
					if (chn == 0)
					{
						switch (test_status)
						{
							case 0:
							{
								ptest_data = (char *)malloc(TestFileLen);
								if (NULL == ptest_data)
								{
									exit(1);
								}

								pf_test = fopen("/mnt/aaa.h264", "w");
								if (NULL == pf_test)
								{
									perror("pf_test: ");
									exit(1);
								}

								printf("pf_test init ok\n");
								test_data_len = 0;
								test_status++;
							}
							case 1:
							{
								if ((test_data_len + u32Length) < TestFileLen)
								{
									memcpy(ptest_data+test_data_len, pu8Buffer, u32Length);
									test_data_len += u32Length;
								}
								else
								{
									test_status++;
								}
							} break;
							case 2:
							{
								if (fwrite(ptest_data, 1, test_data_len, pf_test) != test_data_len)
								{
									perror("fwrite: ");
									exit(1);
								}
								fclose(pf_test);
								printf("pf_test write over, len: %d\n", test_data_len);
								test_status++;
							} break;
							case 3:
							{
								
							} break;
							default:
							{
								//printf("pf_test test_status: %d, invalid\n", test_status);
							}
						}
					}
					
					#endif
					
					stream.chn = chn;
					stream.data = (unsigned char *)pu8Buffer;
					stream.len = u32Length;
					stream.pts = u64TimeStamp;
					stream.pts *= 1000;
					stream.media_type = MEDIA_PT_H264;
					if((pu8Buffer[4]&0x1F) == 0x07)
					{
						stream.frame_type = REAL_FRAME_TYPE_I;
					}
					else
					{
						stream.frame_type = REAL_FRAME_TYPE_P;
					}
					stream.rsv = 0;
					/*
					if (chn == 0)
					{
						printf("pts: %lld, len: %u, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", \
							stream.pts, stream.len, pu8Buffer[0], pu8Buffer[1], pu8Buffer[2], pu8Buffer[3], pu8Buffer[4]);
					}
					*/
					if(g_klwc_info[chn].nMDEvent > 0)
					{
						//printf("chn%d klw has md event\n",chn);
						stream.mdevent = 1;
						unsigned char head[4] = {0x00,0x00,0x00,0x01};
						if(memcmp(pu8Buffer, head, 4) == 0)
						{
							g_klwc_info[chn].nMDEvent--;
						}
						else
						{
							printf("klw no first packet\n");
						}
					}
					else
					{
						//printf("chn%d klw no md event\n",chn);
						stream.mdevent = 0;
						if(g_klwc_info[chn].nMDEvent < 0)
						{
							g_klwc_info[chn].nMDEvent = 0;
						}
					}
					
					stream.width = pStreamInfo->struVencChAttr.u32PicWidth;
					stream.height = pStreamInfo->struVencChAttr.u32PicHeight;
					
					#if 0
					if (chn == 0)
					{
						gettimeofday(&tv1, NULL);
						//printf("\t1 tv.tv_sec: %u, tv.tv_usec: %u\n", tv.tv_sec, tv.tv_usec);
					}
					#endif
					
					g_klwc_info[chn].pStreamCB(&stream, g_klwc_info[chn].dwContext);

					#if 0
					if (chn == 0)
					{
						gettimeofday(&tv2, NULL);
						//printf("\t2 tv.tv_sec: %u, tv.tv_usec: %u\n", tv.tv_sec, tv.tv_usec);
						if (tv2.tv_sec != tv1.tv_sec)
						{
							yg_interval = 1000000+tv2.tv_usec - tv1.tv_usec;
						}
						else
						{
							yg_interval = tv2.tv_usec - tv1.tv_usec;
						}

						if (yg_interval > max_interval)
							max_interval = yg_interval;
						
						if (cnt++ > 25)
						{
							cnt = 0;
							//printf("\t max_interval: %u\n", max_interval);
							//printf("\t max_interval: %u, test_min_len: %u\n", max_interval, test_min_len);
						}
					}
					#endif
					
				}
				//yaogang modify for yuetian audio
				if ((u32DataType == VVV_STREAM_TYPE_AUDIO) )//&& (chn < g_klw_client_count/2))
				{	
					//IPC��Ƶ����main_audio_sw
					//printf("yg get audio package chn:%d\n", chn);
					
					ipc_unit ipcam;
					if(IPC_Get(chn, &ipcam))
					{
						return -1;
					}
					//printf("yg klw main_audio_sw: %d\n", ipcam.main_audio_sw);
					/*
					if (ipcam.main_audio_sw == 0)
					{
						return 0;
					}
					*/
					real_stream_s stream;
					memset(&stream, 0, sizeof(stream));
					stream.chn = chn;
					stream.data = (unsigned char *)pu8Buffer + 4;
					stream.len = u32Length - 4;
					stream.pts = u64TimeStamp;
					stream.pts *= 1000;
					/*
					printf("yg chn%d u32Length: %d,AudioFormat: %d, SampleRate: %d, BitWidth: %d, ChannMode: %d\n", \
						chn, u32Length, \
						pStreamInfo->struAencChAttr.enAudioFormat, \
						pStreamInfo->struAencChAttr.enSampleRate, \
						pStreamInfo->struAencChAttr.enBitWidth, \
						pStreamInfo->struAencChAttr.enChannMode);
					*/
					switch (pStreamInfo->struAencChAttr.enAudioFormat)
					{
						case VVV_AUDIO_FORMAT_G711A :
						{
							stream.media_type = MEDIA_PT_G711;
						} break;
						case VVV_AUDIO_FORMAT_G711Mu :
						{
							stream.media_type = MEDIA_PT_PCMU;
						} break;
						case VVV_AUDIO_FORMAT_G726 :
						{
							stream.media_type = MEDIA_PT_G726;
						} break;
						default :
							printf("chn%d Unknow Audio ENC Format.\n", chn);
							return 0;
					}
					
					g_klwc_info[chn].pStreamCB(&stream, g_klwc_info[chn].dwContext);
					
				}
			}
		}
	}
	return 0;
}
/* //demo
int VVV_SRDK_SAMPLE_AENC(unsigned int u32DevHandle)//ok
{
	int ret = -1;
    unsigned int s32Size = 0;
	int reboot = -1;
	VVV_AENC_CONFIG_S stAENC;
	printf("Get AENC Attr \n");
	s32Size = sizeof(stAENC);
	memset(&stAENC, 0, s32Size);
	stAENC.dwSize = s32Size;
	stAENC.chn = 0;
	stAENC.bindvideochn= 0;	
	ret = VVV_NET_GetServerConfig(u32DevHandle, VVV_CMD_GET_AENC_CONFIG, (void*)&stAENC, &s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_AENC_CONFIG fail ret:%d\n", ret);
		return 1;
	} 
	printf("VVV_CMD_GET_AENC_CONFIG: ret=%d,chn=%d\nbindvideochn=%d\nbEncodeAudio\t%d\nAudioFormat\t%d\n",
		ret,stAENC.chn,stAENC.bindvideochn, stAENC.bEncodeAudio,stAENC.AudioFormat);

	stAENC.dwSize= sizeof(VVV_AENC_CONFIG_S);
	stAENC.chn = 0;
	stAENC.bindvideochn = 0;//0-main stream ,1-second stream
	stAENC.bSaveFlash = 1;
#if 0		 //����������Ƶ
	stAENC.bEncodeAudio = 0;//G726
	if (stAENC.AudioFormat != 4) //��Ƶ�����ʽ:1-G711A,2-G711U,3-ADPCM,4-G726,���VVV_AUDIO_FORMAT_E
	{
		reboot = 1;//if AudioFormat change , reboot to become effective
		stAENC.AudioFormat = 4;
	}
#else	//����������Ƶ
	stAENC.bEncodeAudio = 1;//G711A
	if (stAENC.AudioFormat != 1) //��Ƶ�����ʽ:1-G711A,2-G711U,3-ADPCM,4-G726,���VVV_AUDIO_FORMAT_E
	{
		reboot = 1;//if AudioFormat change , reboot to become effective
		stAENC.AudioFormat = 1;
	}
#endif
	ret = VVV_NET_SetServerConfig(u32DevHandle, VVV_CMD_SET_AENC_CONFIG, (void*)&stAENC, s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_SET_VENC_CONFIG fail ret:%d\n", ret);
		return 2;
	} 	

	printf("After Set => Get AENC Attr \n");
	s32Size = sizeof(stAENC);
	memset(&stAENC, 0, s32Size);
	stAENC.dwSize = s32Size;
	stAENC.chn = 0;
	stAENC.bindvideochn= 0;	
	ret = VVV_NET_GetServerConfig(u32DevHandle, VVV_CMD_GET_AENC_CONFIG, (void*)&stAENC, &s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_AENC_CONFIG fail ret:%d\n", ret);
		return 1;
	} 
	printf("VVV_CMD_GET_AENC_CONFIG: ret=%d,chn=%d\nbindvideochn=%d\nbEncodeAudio\t%d\nAudioFormat\t%d\n",
		ret,stAENC.chn,stAENC.bindvideochn, stAENC.bEncodeAudio,
		stAENC.AudioFormat);

	if (reboot == 1)		
	{
		VVV_NET_SetServerConfig(u32DevHandle, VVV_CMD_REBOOT, NULL, 0);		
	}
	return 0;
}
*/
int KLW_Start(int chn, RealStreamCB pCB, unsigned int dwContext, char* streamInfo, unsigned int dwIp, unsigned short wPort, char *user, char *pwd, unsigned char rtsp_over_tcp)
{
	int ret;
	
	if(!g_init_flag)
	{
		printf("%s KLW_Start not inited\n", __func__);
		return -1;
	}
	
	if(chn < 0 || chn >= (int)(g_klw_client_count))
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return -1;
	}
	
	if(KLW_GetLinkStatus(chn))
	{
		KLW_Stop(chn);
	}
	
	if(!g_sdk_inited)
	{
		ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	
	char devip[64];
	memset(devip, 0, sizeof(devip));
	
	struct in_addr serv;
	serv.s_addr = dwIp;
	sprintf(devip, "%s", inet_ntoa(serv));

	//printf("%s lock1 chn%d\n", __func__, chn);
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	//printf("%s lock2 chn%d\n", __func__, chn);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		//printf("user: %s, pwd: %s, devip: %s, wPort: %d\n", user, pwd, devip, wPort);
		ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, user, pwd, devip, wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("%s chn%d VVV_NET_Login failed\n", __func__, chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			
			//printf("%s unlock1 chn%d\n", __func__, chn);
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			//printf("%s unlock2 chn%d\n", __func__, chn);
			return -1;
		}
		//printf("%s chn%d, DevHandle: %d\n", __func__, chn, g_klwc_info[chn].u32DevHandle);
	}
	
	unsigned int u32StreamFlag = 0;//����������
	if(chn >= (int)(g_klw_client_count/2))
	{
		u32StreamFlag = 1;
	}
	
	g_klwc_info[chn].nLost = 0;
	
	ret = VVV_SUCCESS;
	VVV_STREAM_INFO_S StreamInfo;
	md_para_t md;
	memset(&md, 0, sizeof(md));
	
	if(u32StreamFlag)//������
	{		
		md.flag = 0;//�ƶ���ⲻʹ��
		KLW_CMD_SetMD(chn, &md);
		//ret = VVV_NET_StartStream(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_TYPE_VIDEO, u32StreamFlag, &StreamInfo, KLW_DataCB, /*OnAlarmFunc*/NULL, (void *)chn);
		//ret = VVV_NET_StartStream(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_ALL, u32StreamFlag, &StreamInfo, KLW_DataCB, /*OnAlarmFunc*/NULL, (void *)chn);
		ret = VVV_NET_StartStream_EX(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_ALL, u32StreamFlag, &StreamInfo, KLW_DataCB, /*OnAlarmFunc*/NULL, (void *)chn, 300*1024);
		//VVV_STREAM_ALL
	}
	else
	{//������
		md.flag = 1;//ʹ��
		KLW_CMD_SetMD(chn, &md);
		//ret = VVV_NET_StartStream(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_VIDEO_AUDIO, u32StreamFlag, &StreamInfo, KLW_DataCB, OnAlarmFunc, (void *)chn);
		//�����澯
		//ret = VVV_NET_StartStream(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_ALL, u32StreamFlag, &StreamInfo, KLW_DataCB, OnAlarmFunc, (void *)chn);
		ret = VVV_NET_StartStream_EX(&g_klwc_info[chn].u32ChnHandle, g_klwc_info[chn].u32DevHandle, 0, VVV_STREAM_ALL, u32StreamFlag, &StreamInfo, KLW_DataCB, OnAlarmFunc, (void *)chn, 800*1024);
	}
	if(ret != VVV_SUCCESS)
	{
		printf("%s chn%d VVV_NET_StartStream failed\n", __func__, chn);
		g_klwc_info[chn].u32ChnHandle = INVALID_CHNHDL;
		
		if(g_klwc_info[chn].u32DevHandle != INVALID_DEVHDL)
		{
			VVV_NET_Logout(g_klwc_info[chn].u32DevHandle);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
		}
		//printf("%s unlock1 chn%d\n", __func__, chn);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		//printf("%s unlock2 chn%d\n", __func__, chn);
		
		return -1;
	}
	g_klwc_info[chn].pStreamCB = pCB;
	g_klwc_info[chn].dwContext = dwContext;

	//printf("%s unlock1 chn%d\n", __func__, chn);
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	//printf("%s unlock2 chn%d\n", __func__, chn);

	printf("%s chn%d success\n", __func__, chn);
	return 0;
}

int KLW_Startbyurl(int chn, RealStreamCB pCB, unsigned int dwContext, char* rtspURL, char *user, char *pwd, unsigned char rtsp_over_tcp)
{
	return -1;
}

#define nSperS  (1000000000) // 1��= 1000000000����

int KLW_Stop(int chn)
{
	int ret;
	
	if(!g_init_flag)
	{
		printf("%s KLW_Start not inited\n", __func__);
		return -1;
	}
	
	if(chn < 0 || chn >= (int)(g_klw_client_count))
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return -1;
	}
	//printf("%s lock1 chn%d\n", __func__, chn);
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	//printf("%s lock2 chn%d\n", __func__, chn);
#if 0
	//���֮ǰ������KLW_Snapshot_RegisterCB()��Ҫ�ȴ���ص�����
	struct timespec tv;
	//long delayns = 50*1000*1000;// 50ms
	clock_gettime(CLOCK_MONOTONIC, &tv);
	tv.tv_sec += 5;//��ʱ5s
	
	pthread_mutex_lock(&g_klwc_info[chn].LockSnapOver);
	//if (g_klwc_info[chn].bFlagSnapBusy) 
	if (bFlagSnapBusy[chn]) 
	{
		printf("%s pthread_cond_timedwait chn%d\n", __func__, chn);
		ret = pthread_cond_timedwait(&g_klwc_info[chn].CondSnapOver, &g_klwc_info[chn].LockSnapOver, &tv);
		if (ret)
		{
			if (ETIMEDOUT == ret)
			{
				printf("%s pthread_cond_timedwait chn%d timeout\n", __func__, chn);
			}
			else
			{
				printf("%s pthread_cond_timedwait chn%d failed, ret: %d\n", __func__, chn, ret);
			}
		}
	}

	pthread_mutex_unlock(&g_klwc_info[chn].LockSnapOver);
	//
	usleep(100*1000);// 100ms �ȴ��ص�����
#endif

	g_klwc_info[chn].nLost = 0;
	
	if(g_klwc_info[chn].u32ChnHandle != INVALID_CHNHDL)
	{
		VVV_NET_StopStream(g_klwc_info[chn].u32ChnHandle);
		g_klwc_info[chn].u32ChnHandle = INVALID_CHNHDL;
	}

	if(g_klwc_info[chn].u32DevHandle != INVALID_DEVHDL)
	{
		VVV_NET_Logout(g_klwc_info[chn].u32DevHandle);
		g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
	}
	//printf("%s unlock1 chn%d\n", __func__, chn);	
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	//printf("%s unlock2 chn%d\n", __func__, chn);
	printf("%s chn%d success\n", __func__, chn);
	return 0;
}

//return value : 1 - Link; 0 - Lost
int KLW_GetLinkStatus(int chn)
{
	if(!g_init_flag)
	{
		printf("%s KLW_Start not inited\n", __func__);
		return -1;
	}
	
	if(chn < 0 || chn >= (int)(g_klw_client_count))
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return -1;
	}
	
	int status = 0;
	
	if(g_klwc_info[chn].nLost)
	{
		usleep(10*1000);
		KLW_Stop(chn);
	}
	
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	status = (g_klwc_info[chn].u32ChnHandle != INVALID_CHNHDL);
	
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	
	return status;
}

int SnapCallBack(unsigned int u32ChnHandle,/* ͨ����� */
                                  char *SnapData,         /*ץ������ָ��*/
                                  unsigned int DataSize,  /*ץ�����ݳ���*/
                                  int width,
                                  int height,/*ץ�ķ��ص�ͼ����*/
                                  int count, /*ץ�ĵĵڼ���*/
                                  void* pUserData)
{
	klw_client_info *pChnInfo = (klw_client_info *)pUserData;
	//int chn = (int)pUserData;//ץͼ�ô�����
	int chn;
	int main_chn;
	SNAP_CALLBACK pSnapCB = NULL;
	int ret = -1;

	#if 0
	chn = pChnInfo->dwContext;
	main_chn = chn < (int)(g_klw_client_count/2) ? chn: chn-(int)(g_klw_client_count/2);
	if (DataSize > (128<<10))
	{
		printf("%s: chn%d, main_chn%d, datasize: %d\n", __func__, chn, main_chn, DataSize);
	}
	
	pSnapCB = Snap_CB[chn];
	
	if (SnapData != NULL && DataSize > 0 && DataSize < 1920*1088*3/4)
	{
		if (NULL == pSnapCB)
		{
			printf("%s: chn%d pSnap_CB == NULL!!!! impossible\n", __FUNCTION__, chn);
			ret = -1;
		}
		else
		{	
			//�ص�ҪдͼƬ��Ӳ�̣�Ҫ����������
			printf("%s: chn%d pSnap_CB: %p\n", __FUNCTION__, chn, pSnapCB);
			ret = pSnapCB(SnapData, DataSize, width, height, count, (void *)main_chn);
		}
	}
	else
	{
		printf("%s: chn%d callback param invalid\n", __FUNCTION__, chn);
		ret = -1;
	}

	pthread_mutex_lock(&g_klwc_info[chn].LockSnapOver);
	
	if (bFlagSnapBusy[chn])
	{
		bFlagSnapBusy[chn] = 0;
		pthread_cond_broadcast(&g_klwc_info[chn].CondSnapOver); 
	}
	
	pthread_mutex_unlock(&g_klwc_info[chn].LockSnapOver);
	#endif
	return 0;
}

//yaogang modify 20141225
/**********************
chn: ����ͨ��0-31����ipcamera ��IPCͨ������StreamType����
StreamType: ץͼ������0:������,1:������,2:��������,3:��������
**********************/

int KLW_Snapshot_RegisterCB(int chn, int StreamType, SNAP_CALLBACK pSnapCB)
{
#if 0
	int ret;

	if(!g_init_flag)
	{
		printf("%s KLW_Start not inited\n", __func__);
		return -1;
	}
	
	if(chn < 0 || chn >= (int)(g_klw_client_count))
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		printf("%s: chn%d g_sdk_inited == 0\n", __FUNCTION__, chn);
		return -1;
	}
	
	if(pSnapCB == NULL)
	{
		printf("%s: chn%d pSnapCB == NULL\n", __FUNCTION__, chn);
		return -1;
	}	
	
	pthread_mutex_lock(&g_klwc_info[chn].lock);

	ret = -1;
	VVV_SNAP_CHN_REQ_S stSnapAttr;
	memset(&stSnapAttr, 0, sizeof(stSnapAttr));
	//stSnapAttr.chn = snap_chn < (int)(g_klw_client_count/2) ? 1:2;	/* 1:main stream 2:sub stream 3:mobile stream */
	stSnapAttr.chn = 2;	
	stSnapAttr.snapformat = 0;	   /*ץ�ĸ�ʽ,0-jpeg,1-bmp,Ĭ��jpeg*/
	stSnapAttr.count = 1;		   
	stSnapAttr.timeinterval = 500;
	/*
	if (NULL == g_klwc_info[chn].Snap_CB)
	{
		g_klwc_info[chn].Snap_CB = pSnapCB;
	}
	*/

	if (Snap_CB[chn] != pSnapCB)
	{
		Snap_CB[chn] = pSnapCB;
	}
	
	if (g_klwc_info[chn].u32ChnHandle == INVALID_CHNHDL)
	{		
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s: chn%d u32ChnHandle invalid\n", __FUNCTION__, chn);
		return -1;
	}

	pthread_mutex_lock(&g_klwc_info[chn].LockSnapOver);
	//bFlagSnapBusy[chn] = 1;
	//pthread_mutex_unlock(&g_klwc_info[chn].LockSnapOver);
	
	printf("%s chn%d, 1\n", __func__, chn);
	ret = VVV_NET_Snap_Chn(g_klwc_info[chn].u32ChnHandle,	/* ����ͨ�����*/
				stSnapAttr,		/* ץ������*/
				SnapCallBack,		/* ץ�����ݻص�*/
				(void *)&g_klwc_info[chn]);/* pUserData  snap_chn*/

	printf("%s chn%d, 2\n", __func__, chn);
	if(ret != 0)
	{
		//pthread_mutex_lock(&g_klwc_info[chn].LockSnapOver);
		//if (bFlagSnapBusy[chn])
		{
		//	bFlagSnapBusy[chn] = 0;
		//	pthread_cond_broadcast(&g_klwc_info[chn].CondSnapOver); 
		}
		pthread_mutex_unlock(&g_klwc_info[chn].LockSnapOver);
	
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s: chn%d VVV_NET_Snap_Chn failed, ret:%d\n", __FUNCTION__, chn, ret);
		return -1;
	}

	bFlagSnapBusy[chn] = 1;
	pthread_mutex_unlock(&g_klwc_info[chn].LockSnapOver);
	
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	
#endif
	
	return 0;
}

//CMD ����ĺ�����IPC �йأ������޹� 
int KLW_CMD_Open(int chn)
{
	return 0;
}

int KLW_CMD_Close(int chn)
{
	return 0;
}

int KLW_CMD_SetImageParam(int chn, video_image_para_t *para)
{
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	return 0;
}

int KLW_CMD_GetImageParam(int chn, video_image_para_t *para)
{
	return -1;
}

int KLW_CMD_PtzCtrl(int chn, EMPTZCMDTYPE cmd, void* data)
{
	if(chn < 0 || chn >= (int)(g_klw_client_count/2))
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	unsigned int chnNo = 0;
	unsigned int dwPTZCmd = 0;
	unsigned int dwValue = 0;
	
	if(cmd == EM_PTZ_CMD_START_TILEUP)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_UP;
		dwValue = (tiltSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_TILEDOWN)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_DOWN;
		dwValue = (tiltSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_PANLEFT)
	{
		unsigned int panSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_LEFT;
		dwValue = (panSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_PANRIGHT)
	{
		unsigned int panSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_RIGHT;
		dwValue = (panSpeed - 1) / 2;
	}
	/*
	else if(cmd == EM_PTZ_CMD_START_LEFTUP)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_UPLEFT;
		dwValue = (tiltSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_LEFTDOWN)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_DOWNLEFT;
		dwValue = (tiltSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_RIGHTUP)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_UPRIGHT;
		dwValue = (tiltSpeed - 1) / 2;
	}
	else if(cmd == EM_PTZ_CMD_START_RIGHTDOWN)
	{
		unsigned int tiltSpeed = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_DOWNRIGHT;
		dwValue = (tiltSpeed - 1) / 2;
	}
	*/
	else if(cmd == EM_PTZ_CMD_START_ZOOMTELE)
	{
		dwPTZCmd = VVV_CMD_PTZ_ZOOM_ADD;
	}
	else if(cmd == EM_PTZ_CMD_START_ZOOMWIDE)
	{
		dwPTZCmd = VVV_CMD_PTZ_ZOOM_SUB;
	}
	else if(cmd == EM_PTZ_CMD_START_FOCUSNEAR)
	{
		dwPTZCmd = VVV_CMD_PTZ_FOCUS_ADD;
	}
	else if(cmd == EM_PTZ_CMD_START_FOCUSFAR)
	{
		dwPTZCmd = VVV_CMD_PTZ_FOCUS_SUB;
	}
	else if(cmd == EM_PTZ_CMD_START_IRISSMALL)
	{
		dwPTZCmd = VVV_CMD_PTZ_IRIS_SUB;
	}
	else if(cmd == EM_PTZ_CMD_START_IRISLARGE)
	{
		dwPTZCmd = VVV_CMD_PTZ_IRIS_ADD;
	}
	else if(cmd >= EM_PTZ_CMD_STOP_TILEUP && cmd < EM_PTZ_CMD_PRESET_SET)
	{
		dwPTZCmd = VVV_CMD_PTZ_STOP;
	}
	else if(cmd == EM_PTZ_CMD_PRESET_SET)
	{
		unsigned int preset = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_PRESET;
		dwValue = preset;
	}
	else if(cmd == EM_PTZ_CMD_PRESET_GOTO)
	{
		unsigned int preset = *((unsigned int *)data);
		dwPTZCmd = VVV_CMD_PTZ_CALL;
		dwValue = preset;
	}
	else if(cmd == EM_PTZ_CMD_PRESET_CLEAR)
	{
		//unsigned int preset = *((unsigned int *)data);
	}
	else if(cmd == EM_PTZ_CMD_LIGHT_ON)
	{
		dwPTZCmd = VVV_CMD_PTZ_LIGHT_OPEN;
	}
	else if(cmd == EM_PTZ_CMD_LIGHT_OFF)
	{
		dwPTZCmd = VVV_CMD_PTZ_LIGHT_CLOSE;
	}
	else if(cmd == EM_PTZ_CMD_AUX_ON)
	{
		unsigned int id = *((unsigned int *)data);
		if(id == 1)//Ĭ��1�Ÿ���Ϊ��ˢ
		{
			dwPTZCmd = VVV_CMD_PTZ_BRUSH_START;
		}
	}
	else if(cmd == EM_PTZ_CMD_AUX_OFF)
	{
		unsigned int id = *((unsigned int *)data);
		if(id == 1)//Ĭ��1�Ÿ���Ϊ��ˢ
		{
			dwPTZCmd = VVV_CMD_PTZ_BRUSH_STOP;
		}
	}
	else if(cmd == EM_PTZ_CMD_AUTOPAN_ON)
	{
		dwPTZCmd = VVV_CMD_PTZ_AUTO_STRAT;
	}
	else if(cmd == EM_PTZ_CMD_AUTOPAN_OFF)
	{
		dwPTZCmd = VVV_CMD_PTZ_AUTO_STOP;
	}
	
	if(dwPTZCmd != 0)
	{
		pthread_mutex_lock(&g_klwc_info[chn].lock);
		
		if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
		{
			char devip[64];
			memset(devip, 0, sizeof(devip));
			
			struct in_addr serv;
			serv.s_addr = ipcam.dwIp;
			sprintf(devip, "%s", inet_ntoa(serv));
			
			userpassword_s stUserAuth;
			VVV_NET_PROTOCOL_S stNetProtocol;
			stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
			stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
			stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
			stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
			int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
			if(ret != VVV_SUCCESS)
			{
				printf("chn%d VVV_NET_Login failed\n",chn);
				g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
				pthread_mutex_unlock(&g_klwc_info[chn].lock);
				return -1;
			}
		}
		
		int rtn = VVV_NET_PTZControl(g_klwc_info[chn].u32DevHandle, chnNo, dwPTZCmd, dwValue);
		if(rtn != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_PTZControl failed\n",chn);
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
		
		//printf("chn%d VVV_NET_PTZControl success,PTZCmd=0x%08x & Value=%d\n",chn,dwPTZCmd,dwValue);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
	}
	
	return 0;
}

static const char* szTimeZoneInfo[] = 
{
	"Etc/GMT-12",//"GMT-12:00",
	"Pacific/Apia",//"GMT-11:00",
	"Pacific/Honolulu",//"GMT-10:00",
	"America/Anchorage",//"GMT-09:00",
	"America/Los_Angeles",//"GMT-08:00",
	"America/Phoenix",//"GMT-07:00",
	"America/Chicago",//"GMT-06:00",
	"America/New_York",//"GMT-05:00",
	"America/Caracas",//"GMT-04:30",//������˹��ί�������׶���
	"America/Santiago",//"GMT-04:00",
	"America/St_Johns",//"GMT-03:30",//Ŧ����
	"America/Buenos_Aires",//"GMT-03:00",
	"Atlantic/South_Georgia",//"GMT-02:00",
	"Atlantic/Cape_Verde",//"GMT-01:00",
	"Europe/Dublin",//"GMT",//"GMT+00:00",
	"Europe/Amsterdam",//"GMT+01:00",
	"Europe/Athens",//"GMT+02:00",
	"Asia/Baghdad",//"GMT+03:00",
	"Asia/Tehran",//"GMT+03:30",//�����׶��º���
	"Asia/Dubai",//"GMT+04:00",
	"Asia/Kabul",//"GMT+04:30",//������
	"Asia/Karachi",//"GMT+05:00",
	"Asia/Calcutta",//"GMT+05:30",//�����˹���Ӷ����������µ���
	"Asia/Katmandu",//"GMT+05:45",//�ӵ�����
	"Asia/Almaty",//"GMT+06:00",
	"Asia/Rangoon",//"GMT+06:30",//����
	"Asia/Bangkok",//"GMT+07:00",
	"Asia/Hong_Kong",//"GMT+08:00",
	"Asia/Tokyo",//"GMT+09:00",
	"Australia/Adelaide",//"GMT+09:30",//��������
	"Australia/Brisbane",//"GMT+10:00",
	"Asia/Magadan",//"GMT+11:00",
	"Pacific/Auckland",//"GMT+12:00",
	"Pacific/Tongatapu",//"GMT+13:00",
};

static const char* GetTZInfo(int index)
{
	if(index < 0 || index >= (int)(sizeof(szTimeZoneInfo)/sizeof(szTimeZoneInfo[0])))
	{
		return "Asia/Hong_Kong";
	}
	
	return szTimeZoneInfo[index];
}

static int TimeZoneOffset[] = 
{
	-12*3600,
	-11*3600,
	-10*3600,
	-9*3600,
	-8*3600,
	-7*3600,
	-6*3600,
	-5*3600,
	-4*3600-1800,
	-4*3600,
	-3*3600-1800,
	-3*3600,
	-2*3600,
	-1*3600,
	0,
	1*3600,
	2*3600,
	3*3600,
	3*3600+1800,
	4*3600,
	4*3600+1800,
	5*3600,
	5*3600+1800,
	5*3600+2700,
	6*3600,
	6*3600+1800,
	7*3600,
	8*3600,
	9*3600,
	9*3600+1800,
	10*3600,
	11*3600,
	12*3600,
	13*3600,
};

static int GetTZOffset(int index)
{
	if(index < 0 || index >= (int)(sizeof(TimeZoneOffset)/sizeof(TimeZoneOffset[0])))
	{
		return 0;
	}
	
	return TimeZoneOffset[index];
}

int KLW_CMD_SetTime(int chn, time_t t, int force)
{
	if(chn < 0 || chn >= (int)(g_klw_client_count/2))
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!force)
	{
		if(!ipcam.enable || !KLW_GetLinkStatus(chn))
		{
			//printf("KLW_CMD_SetTime: chn%d vlost!!!\n",chn);
			return -1;
		}
	}
	
	if(!g_sdk_inited)
	{
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	
	if(t == 0)
	{
		t = time(NULL);// + 1;//???
	}
	
	int nTimeZone = IPC_GetTimeZone();
	t += GetTZOffset(nTimeZone);
	
	struct tm now;
	localtime_r(&t,&now);
	
	/*char date[64];
	sprintf(date,"%04d.%02d.%02d.%02d.%02d.%02d",
		now.tm_year+1900,
		now.tm_mon+1,
		now.tm_mday,
		now.tm_hour,
		now.tm_min,
		now.tm_sec);*/
	
	VVV_Server_Time_S qv_time;
	memset(&qv_time, 0, sizeof(qv_time));
	qv_time.dwSize = sizeof(qv_time);
	qv_time.year = now.tm_year+1900;
	qv_time.month = now.tm_mon+1;
	qv_time.day = now.tm_mday;
	qv_time.hour = now.tm_hour;
	qv_time.minute = now.tm_min;
	qv_time.second = now.tm_sec;
	qv_time.DateMode = 1;
	qv_time.TimeMode = 0;
	qv_time.bdstmode = 0;
	qv_time.timezone = GetTZOffset(nTimeZone) / 3600;
	if(qv_time.timezone > 12)
	{
		qv_time.timezone = 12;
	}
	if(qv_time.timezone < -12)
	{
		qv_time.timezone = -12;
	}
	strcpy(qv_time.citytimezone, GetTZInfo(nTimeZone));

	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
	}
	
	int ret = VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_SET_TIME, &qv_time, sizeof(qv_time));
	if(ret != VVV_SUCCESS)
	{
		printf("chn%d VVV_NET_SetServerConfig VVV_CMD_SET_TIME failed:%d\n",chn,ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	}
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	//printf("chn%d VVV_NET_SetServerConfig VVV_CMD_SET_TIME success\n",chn);
	
	return 0;
}

int KLW_CMD_SetMD(int chn, md_para_t *para)
{
	if(para == NULL)
	{
		return -1;
	}

	if(chn < 0 || chn >= (int)(g_klw_client_count/2))
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
	}
	
	//�ƶ����
	int ret = 0, i = 0, j = 0;
	unsigned int s32Size = sizeof(VVV_CHN_MD_ALARM_S);
	VVV_CHN_MD_ALARM_S md;
	memset(&md, 0, sizeof(md));
	md.dwSize = sizeof(VVV_CHN_MD_ALARM_S);
	md.chn = 0xff;
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle,VVV_CMD_GET_MD_ALARM,&md,&s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_MD_ALARM error! \n");
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	}
	if(para->flag)
	{
		md.bEnable = 1;
		for(i = 0; i < 32; i++)
		{
			md.RegionSetting[i] = 0xff;
		}
		for(i = 0; i < 7; i++)
			for(j = 0; j < 24; j++)
				md.hour[i][j] = 1;
	}
	else
	{
		md.bEnable = 0;	
		for(i = 0; i < 32; i++)
		{
			md.RegionSetting[i] = 0;
		}
		for(i = 0; i < 7; i++)
			for(j = 0; j < 24; j++)
				md.hour[i][j] = 0;
	}
	s32Size = sizeof(VVV_CHN_MD_ALARM_S);
	ret = VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle,VVV_CMD_SET_MD_ALARM,&md,s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_SET_MD_ALARM error! \n");
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	}

	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	return 0;
}

//chn: 0-16
//stream: 0-������,1-������					
int KLW_CMD_GetVENC(int chn, int stream, VideoEncoderParam *para)
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//printf("yg time: %d.%d\n", tv.tv_sec, tv.tv_usec);
#if 1
	int ret;
	
	if(para == NULL)
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		/*
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
		*/
		return -1;
	}
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		#if 1
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
		#endif
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s chn%d, DevHandle invalid\n", __func__, chn);
		return -1;
	}

	VVV_VENC_CONFIG_S stVENC;
	unsigned int s32Size = sizeof(stVENC);

	//printf("Get Osd Attr chn%d, DevHandle: %d\n", chn, g_klwc_info[chn].u32DevHandle);
	memset(&stVENC, 0, s32Size);
	stVENC.dwSize = s32Size;
	stVENC.chn = 0;//���ֶα���Ϊ0
	stVENC.Stream = stream;
	
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_VENC_CONFIG, (void*)&stVENC, &s32Size);
	//printf("VVV_CMD_GET_VENC_CONFIG stream: %s ret=%d,chn=%d\nVideoFormat\t%d\nRateType\t%d\nresolution\t%d\ndwBitRate\t%d\n",
		//stream?"sub":"main", ret,stVENC.chn,stVENC.VideoFormat, stVENC.RateType,
		//stVENC.resolution, stVENC.dwBitRate);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_VENC_CONFIG failed ret:%d \n", ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	} 

	pthread_mutex_unlock(&g_klwc_info[chn].lock);

	para->RateType = stVENC.RateType;
	para->BitRate = stVENC.dwBitRate;
#endif
	
	return 0;
}

//chn: 0-16
//stream: 0-������,1-������	
int KLW_CMD_SetVENC(int chn, int stream, VideoEncoderParam *para)
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//printf("yg time: %d.%d\n", tv.tv_sec, tv.tv_usec);
#if 1
	int ret;
	
	if(para == NULL)
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		/*
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
		*/
		return -1;
	}
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		#if 1
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}

		//printf("chn%d, DevHandle invalid\n", chn);
		#endif
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s chn%d, DevHandle invalid\n", __func__, chn);
		return -1;
	}

	VVV_VENC_CONFIG_S stVENC;
	unsigned int s32Size = sizeof(stVENC);

	//printf("Get Osd Attr chn%d, DevHandle: %d\n", chn, g_klwc_info[chn].u32DevHandle);
	memset(&stVENC, 0, s32Size);
	stVENC.dwSize = s32Size;
	stVENC.chn = 0;//���ֶα���Ϊ0
	stVENC.Stream = stream;
	
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_VENC_CONFIG, (void*)&stVENC, &s32Size);
	//printf("VVV_CMD_GET_VENC_CONFIG stream: %s ret=%d,chn=%d\nVideoFormat\t%d\nRateType\t%d\nresolution\t%d\ndwBitRate\t%d\n",
		//stream?"sub":"main", ret,stVENC.chn,stVENC.VideoFormat, stVENC.RateType,
		//stVENC.resolution, stVENC.dwBitRate);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_VENC_CONFIG failed ret:%d \n", ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	} 

	//gettimeofday(&tv, NULL);
	//printf("yg 1 time: %d.%d\n", tv.tv_sec, tv.tv_usec);

	if ((stVENC.RateType == para->RateType) && (stVENC.dwBitRate == para->BitRate))
	{
		printf("chn%d %s-stream VENC param not change\n", chn, stream?"sub":"main");
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return 0;
	}
	
	stVENC.RateType = para->RateType;
	stVENC.dwBitRate = para->BitRate;
	stVENC.bSaveFlash = 1;
	
	ret = VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_SET_VENC_CONFIG, (void*)&stVENC, s32Size);
	//gettimeofday(&tv, NULL);
	//printf("yg 2 time: %d.%d\n", tv.tv_sec, tv.tv_usec);
	if (ret != 0)
	{
		printf("VVV_CMD_SET_VENC_CONFIG error!\n");
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	}
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	
#endif
	
	return 0;
}

int KLW_CMD_SetOSD(int chn, char *name)
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//printf("yg time: %d.%d\n", tv.tv_sec, tv.tv_usec);
#if 1
	int ret;
	
	if(name == NULL)
	{
		return -1;
	}
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		#if 1
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
		#endif
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s chn%d, DevHandle invalid\n", __func__, chn);
		return -1;
	}

	VVV_CHN_OSDINFO_S stOsdInfo;
	unsigned int s32Size = sizeof(stOsdInfo);

	//printf("Get Osd Attr chn%d, DevHandle: %d, name: %s\n", chn, g_klwc_info[chn].u32DevHandle, name);	
	memset(&stOsdInfo, 0, s32Size);
	stOsdInfo.chn = 0;//���ֶα���Ϊ0
	stOsdInfo.dwSize = s32Size;
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_CHN_OSDINFO, (void*)&stOsdInfo, &s32Size);
	//printf("VVV_CMD_GET_CHN_OSDINFO: ret=%d,chn=%d\nbShowTime\t%d\nTimePosition\t%d\nbShowString\t%d\nStringPosition\t%d\ncsString\t%s\n",
		//ret,stOsdInfo.chn,stOsdInfo.bShowTime, stOsdInfo.TimePosition,
		//stOsdInfo.bShowString, stOsdInfo.StringPosition,
		//stOsdInfo.csString);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_CHN_OSDINFO failed ret:%d \n", ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	} 


	printf("KLW_CMD_SetOSD: chn%d\n", chn);
	int i;
	for(i=0; i<6; i++)
		printf("0x%x\n", name[i]);

	
	//yaogang modify 20140918 utf8 to gb2312
	//gettimeofday(&tv, NULL);
	//printf("yg 1 time: %d.%d\n", tv.tv_sec, tv.tv_usec);
	
	char outbuf[48];
	memset(outbuf, 0, sizeof(outbuf));
	size_t inbytesleft = strlen(name);
	size_t outbytesleft = sizeof (outbuf);
	char *inptr = name;
	char *outptr = (char *) outbuf;
	
//iconv_t iconv_open (const char* tocode, const char* fromcode);
	iconv_t cd = iconv_open ("GB18030", "UTF-8");
	if (cd == (iconv_t)(-1)) 
	{
		printf("iconv_open failed\n");
	} 
	else 
	{
		size_t r = iconv (cd,
		                  (char **) &inptr, &inbytesleft,
		                  (char **) &outptr, &outbytesleft);

		//if (!(r == (size_t)(-1) && errno == EINVAL))
		if (r == (size_t)(-1) )
		{
			//"UTF-8" to "GB18030" ʧ�ܣ�
			//��ô��Ϊ֮ǰ�ı������"GB18030" 
			//����ת��
			printf("KLW_CMD_SetOSD iconv() UTF-8 to GB18030 failed r: %d\n", r);
			strcpy(outbuf, inptr);
		}
		else
		{
			printf("KLW_CMD_SetOSD iconv UTF-8 to GB18030 success\n");
		}
	}
	iconv_close(cd);
	
/*
	printf("yg outbuf: \n");
	int i;
	for (i=0; i<48; i++)
	{
		printf("0x%02x ", outbuf[i]);
	}
	printf("\n");
*/

	if (strcmp(outbuf, stOsdInfo.csString) == 0)
	{
		printf("chn%d same name\n", chn);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return 0;
	}
	
	//yaogang modify 20140918 end
	//gettimeofday(&tv, NULL);
	//printf("yg 2 time: %d.%d\n", tv.tv_sec, tv.tv_usec);

	
	strcpy(stOsdInfo.csString, outbuf);
	//strcpy(stOsdInfo.csString, name);
	stOsdInfo.bShowString = 1;
	stOsdInfo.StringPosition = 0;
	stOsdInfo.bShowTime = 1;
	stOsdInfo.TimePosition = 2;	
	ret = VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_SET_CHN_OSDINFO, (void*)&stOsdInfo, s32Size);
	//gettimeofday(&tv, NULL);
	//printf("yg 3 time: %d.%d\n", tv.tv_sec, tv.tv_usec);
	if (ret != 0)
	{
		printf("VVV_CMD_SET_MD_ALARM error!\n");
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	}
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	
#endif
	
	return 0;
}


int KLW_CMD_Reboot(int chn)
{
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
		int ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	}
	
	unsigned int u32DevHandle = INVALID_DEVHDL;
	
	char devip[64];
	memset(devip, 0, sizeof(devip));
	
	struct in_addr serv;
	serv.s_addr = ipcam.dwIp;
	sprintf(devip, "%s", inet_ntoa(serv));
	
	userpassword_s stUserAuth;
	VVV_NET_PROTOCOL_S stNetProtocol;
	stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
	stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
	stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
	stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
	int ret = VVV_NET_Login(&u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc2, &stUserAuth, NULL);
	if(ret != VVV_SUCCESS)
	{
		printf("KLW_CMD_Reboot: VVV_NET_Login failed\n");
		u32DevHandle = INVALID_DEVHDL;
		return -1;
	}
	
	ret = VVV_NET_SetServerConfig(u32DevHandle, VVV_CMD_REBOOT, NULL, 0);
	
	VVV_NET_Logout(u32DevHandle);
	u32DevHandle = INVALID_DEVHDL;
	
	if(ret != VVV_SUCCESS)
	{
		printf("KLW_CMD_Reboot failed:%d\n",ret);
		return -1;
	}
	
	printf("KLW_CMD_Reboot success\n");
	
	return 0;
}
int KLW_CMD_GetAudioSwitchStatus(int chn)
{
#if 1
	int ret;
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
	
		ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	
		return -1;
	}
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		#if 1
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
		#endif
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s chn%d, DevHandle invalid\n", __func__, chn);
		return -1;
	}
	
	VVV_AENC_CONFIG_S stAENC;
	printf("Get AENC Attr \n");
	unsigned int s32Size = sizeof(stAENC);
	memset(&stAENC, 0, s32Size);
	stAENC.dwSize = s32Size;
	stAENC.chn = 0;
	stAENC.bindvideochn= 0;	
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_AENC_CONFIG, (void*)&stAENC, &s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_AENC_CONFIG failed ret:%d \n", ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	} 
	pthread_mutex_unlock(&g_klwc_info[chn].lock);

	if ((stAENC.bEncodeAudio == 1) && (stAENC.AudioFormat == 1))
	{
		return 1;//��
	}
	if ((stAENC.bEncodeAudio == 0) && (stAENC.AudioFormat == 4))
	{
		return 0;//��
	}
	
#endif
	
	return 0;
}
int KLW_CMD_SetAudioSwitchStatus(int chn, int bswitch)
{
	printf("yg KLW_CMD_SetAudioSwitchStatus  bswitch: %d\n", bswitch);
#if 1
	int ret;
	
	ipc_unit ipcam;
	if(IPC_Get(chn, &ipcam))
	{
		return -1;
	}
	
	if(!g_sdk_inited)
	{
	
		ret = VVV_NET_Init();
		if(ret != VVV_SUCCESS)
		{
			printf("VVV_NET_Init failed\n");
			return -1;
		}
		g_sdk_inited = 1;
	
		return -1;
	}
	pthread_mutex_lock(&g_klwc_info[chn].lock);
	
	if(g_klwc_info[chn].u32DevHandle == INVALID_DEVHDL)
	{
		#if 1
		char devip[64];
		memset(devip, 0, sizeof(devip));
		
		struct in_addr serv;
		serv.s_addr = ipcam.dwIp;
		sprintf(devip, "%s", inet_ntoa(serv));
		
		userpassword_s stUserAuth;
		VVV_NET_PROTOCOL_S stNetProtocol;
		stNetProtocol.eNetProtocol = NET_PROTOCOL_QV;
		stNetProtocol.eStreamTransProtocol = TRANS_PROTOCOL_HTTP;
		stNetProtocol.eSocketType = SOCKET_TYPE_TCP;
		stNetProtocol.eControlProtocol = CTL_PROTOCOL_TYPE_PRIVATE;
		int ret = VVV_NET_Login(&g_klwc_info[chn].u32DevHandle, ipcam.user, ipcam.pwd, devip, ipcam.wPort, stNetProtocol, 5000, OnEvenFunc, &stUserAuth, NULL);
		if(ret != VVV_SUCCESS)
		{
			printf("chn%d VVV_NET_Login failed\n",chn);
			g_klwc_info[chn].u32DevHandle = INVALID_DEVHDL;
			pthread_mutex_unlock(&g_klwc_info[chn].lock);
			return -1;
		}
		#endif
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("%s chn%d, DevHandle invalid\n", __func__, chn);
		return -1;
	}
	
	VVV_AENC_CONFIG_S stAENC;
	printf("Get AENC Attr \n");
	unsigned int s32Size = sizeof(stAENC);
	memset(&stAENC, 0, s32Size);
	stAENC.dwSize = s32Size;
	stAENC.chn = 0;
	stAENC.bindvideochn= 0;	
	int reboot = 0;
	
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_AENC_CONFIG, (void*)&stAENC, &s32Size);
	if(ret != 0)
	{
		printf("VVV_CMD_GET_AENC_CONFIG failed ret:%d \n", ret);
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		return -1;
	} 
	//pthread_mutex_unlock(&g_klwc_info[chn].lock);

	stAENC.dwSize= sizeof(VVV_AENC_CONFIG_S);
	stAENC.chn = 0;
	stAENC.bindvideochn = 0;//0-main stream ,1-second stream
	stAENC.bSaveFlash = 1;
	if (0 == bswitch)		 //����������Ƶ
	{
		stAENC.bEncodeAudio = 0;//G726
		if (stAENC.AudioFormat != 4) //��Ƶ�����ʽ:1-G711A,2-G711U,3-ADPCM,4-G726,���VVV_AUDIO_FORMAT_E
		{
			reboot = 1;//if AudioFormat change , reboot to become effective
			stAENC.AudioFormat = 4;
		}
	}
	else	//����������Ƶ
	{
		stAENC.bEncodeAudio = 1;//G711A
		if (stAENC.AudioFormat != 1) //��Ƶ�����ʽ:1-G711A,2-G711U,3-ADPCM,4-G726,���VVV_AUDIO_FORMAT_E
		{
			reboot = 1;//if AudioFormat change , reboot to become effective
			stAENC.AudioFormat = 1;
		}
	}

	ret = VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_SET_AENC_CONFIG, (void*)&stAENC, s32Size);
	if(ret != 0)
	{
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("VVV_CMD_SET_VENC_CONFIG fail ret:%d\n", ret);
		return 2;
	} 	

	printf("After Set => Get AENC Attr \n");
	s32Size = sizeof(stAENC);
	memset(&stAENC, 0, s32Size);
	stAENC.dwSize = s32Size;
	stAENC.chn = 0;
	stAENC.bindvideochn= 0;	
	ret = VVV_NET_GetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_GET_AENC_CONFIG, (void*)&stAENC, &s32Size);
	if(ret != 0)
	{
		pthread_mutex_unlock(&g_klwc_info[chn].lock);
		printf("VVV_CMD_GET_AENC_CONFIG fail ret:%d\n", ret);
		return 1;
	} 
	printf("VVV_CMD_GET_AENC_CONFIG: ret=%d,chn=%d\nbindvideochn=%d\nbEncodeAudio\t%d\nAudioFormat\t%d\n",
		ret,stAENC.chn,stAENC.bindvideochn, stAENC.bEncodeAudio, stAENC.AudioFormat);

	if (reboot == 1)		
	{
		VVV_NET_SetServerConfig(g_klwc_info[chn].u32DevHandle, VVV_CMD_REBOOT, NULL, 0);		
	}
	pthread_mutex_unlock(&g_klwc_info[chn].lock);
	
#endif
	
	return 0;
}


int KLW_CMD_RequestIFrame(int chn)
{
	if(!g_init_flag)
	{
		return -1;
	}
	
	return 0;
}




