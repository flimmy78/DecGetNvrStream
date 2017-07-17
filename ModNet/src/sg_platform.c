#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <limits.h>
#include <errno.h>

// file description
#include "CmdProc.h"
#include "Cmdcallback.h"
#include "diskmanage.h"
#include "netcommon.h"
#include "mod_config.h"

#include "biz.h"
#include "sg_platform.h"
#include "Mod_syscomplex.h"
#include "ipcamera.h"


static u8 bModInited = 0;//ģ���Ƿ��Ѿ���ʼ��
static time_t inittime = 0;

static PNetCommStateNotify pNotify = NULL;

#define RCV_TIMEOUT	(5)//�룬��ʱ����

extern int ConnectWithTimeout(u32 dwHostIp, u16 wHostPort, u32 dwTimeOut, u16 *pwErrorCode);


static int upload_sg_fd[2] = {-1, -1};
static int Gheart_interval_minute = 5;

static pthread_mutex_t upload_sg_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
��Ϣͷ:
�̶�Ϊ20�ֽ�
6�ֽڣ�QWCMD:
4�ֽڣ���Ϣ���ݳ���(������Ϣͷ����)
10�ֽڣ�����
*/
#define MSGSTR 

#pragma pack(push,1)	//��ԭ�����뷽ʽ����ѹջ�������µ�����Ϊ1

typedef struct
{
	char start[6];
	int msg_size;//������Ϣͷ����Ϣ��
	char reserved[10];
} MSG_HEAD;

#pragma pack(pop)    //�ָ�ԭ�ȵĶ��뷽ʽ
#define BODYSIZE (256<<10)

static s32 upload_msg(int sockfd, SSG_MSG_TYPE *pmsg)
{
	MSG_HEAD head;
	memset(&head, 0, sizeof(head));
	memcpy(head.start, "QWCMD:", 6);
	char msg_body[1024];
	int ret = CTRL_SUCCESS;
	
	switch (pmsg->type)
	{
		case EM_PIC_ALARM_LINK_UPLOAD:		// 1 ��������ͼ��
		case EM_PIC_MAINTAIN_UPLOAD:			// 2 �����ϴ�ͼ��
		case EM_PIC_DAILY_UPLOAD:				// 3 �ճ��ϴ�ͼ��
		case EM_PIC_TEST_UPLOAD:				// 4 �����ϴ�ͼ��
		case EM_PIC_ACCEPT_UPLOAD:			// 33 �����ϴ�ͼ��
		{
		} break;
		case EM_PIC_SERVER_GET:				// 5 ��������ͼ�� 1
		{
		} break;
		case EM_DVR_STARTUP:					// 19 DVRϵͳ����
		case EM_DVR_EXIT_NORMOAL:			// 20 DVRϵͳ�˳�
		case EM_DVR_EXIT_ABNORMAL:			// 21 DVR�쳣�˳�
		case EM_DVR_EXIT_ILLEGALITY:			// 28 DVR�Ƿ��˳�
		{
		} break;
		case EM_DVR_PARAM_SET:				// 22 DVR��������
		case EM_DVR_PARAM_SAVE:				// 23 DVR��������
		{
		} break;
		case EM_DVR_VLOST:					// 24 DVR��Ƶ��ʧ
		case EM_DVR_VMOTION:					// 25 DVR�ƶ����
		case EM_DVR_ALARM_EXT:				// 26 DVR�ⲿ����
		case EM_DVR_MOTION:					// 34 ƫλ���� 1
		case EM_DVR_HDD_ERR:					// 41 DVR���̴���
		{
		} break;
		case EM_DVR_ALARM_RESUME:			// 27 ϵͳ�������
		{
			
		} break;
		case EM_REPAIR_CHECK:					// 29 ϵͳά��ǩ��
		case EM_MAINTAIN_CHECK:				// 30 ϵͳά��ǩ��
		{
		} break;
		case EM_DVR_PLAYBACK:					// 31 DVR���ػط�
		case EM_REMOTE_PLAYBACK:				// 32 DVRԶ�̻ط�
		{
		} break;
		case EM_PIC_ALARM_UPLOAD_UNREC:		// 36 �����ϴ�ͼ��δ¼����ƶ���⣩
		case EM_PIC_MAINTAIN_UPLOAD_UNREC:	// 37 �����ϴ�ͼ��δ¼����ƶ���⣩
		case EM_PIC_DAILY_UPLOAD_UNREC:		// 38 �ճ��ϴ�ͼ�񣨵���δ¼��
		case EM_PIC_TEST_UPLOAD_UNREC:		// 39 �����ϴ�ͼ��δ¼����ƶ���⣩
		case EM_PIC_ACCEPT_UPLOAD_UNREC:		// 40 �����ϴ�ͼ��δ¼����ƶ���⣩
		{
		} break;
		case EM_HEART_TIMEOUT:				// 42 ϵͳ������ʱ
		case EM_HEART_RESUME:				// 43 ϵͳ�����ָ�
		{
		} break;
		case EM_VEDIO_OTHER:					// 44��Ƶ�����¼�
		{
		} break;
		default:
			printf("%s unknow cmd type: %d\n", __FUNCTION__, pmsg->type);
	}
	
	if (head.msg_size != 0)
	{
		if (loopsend(sockfd, (char *)&head, sizeof(head)) != sizeof(head))
		{
			printf("%s send msg head failed\n", __FUNCTION__);
			ret = CTRL_FAILED_NETSND;
		}
		else
		{
			if (loopsend(sockfd, (char *)&msg_body, head.msg_size-sizeof(head)) != head.msg_size-sizeof(head))
			{
				printf("%s send msg body failed\n", __FUNCTION__);
				ret = CTRL_FAILED_NETSND;
			}
		}
	}

	return ret;
}

//����������û�з��ͻ�Ӧֱ�ӹر�SOCK����ʱrecv����0����������Ϊʧ�ܣ�
//�ϲ��߼����Խ���ʧ��
static int looprecv_withtimeout(int sockfd, char *buf, unsigned int rcvsize,int ustimeout)
{
	int ret = CTRL_SUCCESS; 
	int curlen = 0;
	fd_set set;
	struct timeval timeout, starttime, curtime;
	u64 starttm, curtm;

	gettimeofday(&starttime, NULL);
	starttm = starttime.tv_sec*1000*1000 + starttime.tv_usec;
	//printf("%s starttime: %u.%u\n", __func__, starttime.tv_sec, starttime.tv_usec);

	while (curlen < rcvsize)
	{
		FD_ZERO(&set);
		FD_SET(sockfd,&set);
		//timeout.tv_sec = 0;
		//timeout.tv_usec = 10*1000;//10ms
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		ret = select(sockfd+1,&set,NULL,NULL,&timeout);
		if (ret < 0)
		{
			printf("%s select failed\n", __FUNCTION__);
			ret = CTRL_FAILED_NETRCV;
			break;
		}
		else if(ret)
		{
			if (FD_ISSET(sockfd, &set))
			{
				ret = recv(sockfd, buf+curlen, rcvsize-curlen, 0);
				if(ret < 0)
				{
					printf("%s recv failed\n", __FUNCTION__);
					ret = CTRL_FAILED_NETRCV;
					break;
				}
				else if(ret == 0)//�������ر���sock
				{
					ret = CTRL_FAILED_NETRCV;
					printf("%s SG-server close sock, recvlen: %d\n", __FUNCTION__, curlen);
					break;
				}

				curlen += ret;
			}
			else
			{
				printf("%s sockfd not in set\n", __FUNCTION__);
				ret = CTRL_FAILED_NETRCV;
				break;
			}
		}
		
		//�ɹ��ͳ�ʱ��Ҫ������ʱ���Ƿ�ʱ
		gettimeofday(&curtime, NULL);
		curtm = curtime.tv_sec*1000*1000 + curtime.tv_usec;
		//printf("%s curtime: %u.%u\n", __func__, curtime.tv_sec, curtime.tv_usec);
		
		//if ((starttime.tv_sec + ustimeout/(1000*1000) < curtime.tv_sec) 
		//	||((starttime.tv_sec + ustimeout/(1000*1000) == curtime.tv_sec) && (starttime.tv_usec + ustimeout%(1000*1000) < curtime.tv_usec)))
		if (starttm + ustimeout < curtm) 
		{
			printf("%s recv timeout\n", __FUNCTION__);
			ret = CTRL_FAILED_TIMEOUT;
			break;
		}
	}

	if (curlen == rcvsize)
		ret = CTRL_SUCCESS;
	else
		ret = CTRL_FAILED_NETRCV;

	return ret;
}

/*
//0: �ϴ��ɹ�
//1: ����������ʧ��
//2: ����ʧ��
//3: ����ʧ��
//4: DNS����ʧ��
//5: ͼƬ��Ϣ����
//6: ͼƬbase64ת��ʧ��
//7: ��ȡ������Ϣʧ��
//8: �����ڴ�ʧ��
*/

s32 netComm_NotifySGuploadState(u32 chn, EM_MSG_TYPE type, u8 bState )
{
	SNetStateNotify sNotify;

	if (!bModInited )//|| (time(NULL) - inittime < 2*60 ) )
		return 1;

	printf("%s type: %d\n", __func__, type);
	
	memset(&sNotify, 0, sizeof(sNotify));
	
	sNotify.eAct 		= EM_NET_STATE_SG;
	sNotify.sRslt.state 	=  chn;
	sNotify.sRslt.nErr 	= bState;

	if (pNotify != NULL)
	{
		if ( (type == EM_PIC_MAINTAIN_UPLOAD) 
			|| (type == EM_PIC_TEST_UPLOAD)
			|| (type == EM_PIC_ACCEPT_UPLOAD) 
			|| (type == EM_REPAIR_CHECK)
			|| (type == EM_MAINTAIN_CHECK) 
			|| (type == EM_VideoMonitor_Fault)
			//||((type >= EM_SYS_Other) && (type <= EM_ElectronicPatrol_Fault))
			)
		{
			pNotify((void*)&sNotify);
		}
	}
	
	return 0;
}

//���ƽ̨�����ϴ�
s32 netComm_NotifySGReqSnap(u32 chn_mask)
{
	SNetStateNotify sNotify;

	if (!bModInited )//|| (time(NULL) - inittime < 2*60 ) )
		return 1;

	printf("%s chn mask: 0x%x\n", __func__, chn_mask);
	
	memset(&sNotify, 0, sizeof(sNotify));
	
	sNotify.eAct			= EM_NET_STATE_SGReqSnap;
	sNotify.SGReqChnMask	= chn_mask;

	if (pNotify != NULL)
	{
		pNotify((void*)&sNotify);		
	}
	
	return 0;
}

/*
���յ�����������GetImage�ֶΣ�
DVR��������OperationCmd����ָ������ͨ����ʵʱͼƬ��
OperationCmd��GUID������д�������ش���GUID��ţ�
Type����Ϊ5(Type=5);���ϴ�ͨ��Ϊ"��"������ͨ����š�

ȡ��ͨ����
<GetImage Channel="1,2,3" GUID="1C164A158FFDED4FAFC6A37DC57017BF"/>
*/
s32 proc_svr_req(char *pmsg)
{
	int ret = 0;
	char *s; 
	char *e; //start end
	int chn, chn_mask;
	
	s = strstr(pmsg, "GetImage");
	if (NULL == s)
	{
		printf("%s \"GetImage\" not found\n", __func__);
		return 1;
	}

	s = strchr(s, '\"');  // " ������
	if (NULL == s)
	{
		printf("%s \" not found\n", __func__);
		return 1;
	}

	chn_mask = 0;
	s += 1;
	do {		
		if ((*s >= '0') && (*s <= '9'))
		{
			chn = strtol(s, &e, 10);
			if ((errno == ERANGE && (chn == LONG_MAX || chn == LONG_MIN)) || (errno != 0 && chn == 0)) 
			{
				printf("%s strtol %s\n", __func__, strerror(errno));
				break;
			}

			printf("%s strtol chn: %d\n", __func__, chn);
			chn_mask |= 1<<chn;

			if (*e != ',') //"1,2,3"  ���ż��
			{
				break;				
			}

			s = e+1;
		}
		else
		{
			break;
		}
	} while(1);

	if (chn_mask)
	{
		ret = netComm_NotifySGReqSnap(chn_mask);
	}
	
	return ret;
}

static char SvrGuid[128];
//ʱ�������Ϊ����������ָ������ΪҪ�ϴ�֮ǰ��Ԥ¼ͼƬ
/*
tm:
	= 0�����õ�ǰʱ��
	!=0���Ǿ�ͼƬʱ��
SnapData, DataSize
	ͼƬ����λ�úͳ���
*/
s32 upload_sg_proc(SSG_MSG_TYPE *pmsg, time_t tm, void *SnapData, unsigned int DataSize, int GUID)
{
	int ret = CTRL_SUCCESS;
	u32 dwServerIp;
	SModConfigSGParam para;
	//printf("%s type: %d\n", __func__, pmsg->type);

	//�õ�������IP
	ret = ModConfigGetParam(EM_CONFIG_PARA_SG, &para, 0);
	if (ret == 0)
	{
		if (!para.RunEnable)
		{
			return CTRL_SUCCESS;
		}
		
		//printf("%s SG Platform ip: %s\n", __func__, para.CenterIP);
	
		if ((para.CenterIP[0] >= '0') && (para.CenterIP <= '9'))//���10����IP
		{
			dwServerIp = (u32)inet_addr(para.CenterIP);
		}
		else
		{
			struct hostent *hptr;
			//printf("%s gethostbyname 1\n", __func__);
			if((hptr = gethostbyname(para.CenterIP)) == NULL)
			{
				printf("%s gethostbyname error for host:%s, h_errno: %d\n", __func__, para.CenterIP, h_errno);
				netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 4);
				return CTRL_FAILED_PARAM;
			}
			//printf("SG ServerIP Address :%s\n",inet_ntoa(*((struct in_addr *)hptr->h_addr)));

			dwServerIp = *((u32 *)hptr->h_addr);
		}
		
		u16 wServerPort = 5901;
		u16 wErrCode;
		u32 dwTimeOut = RCV_TIMEOUT * 1000;//ms

		//��������
		//printf("%s connect 1, %d\n", __func__, time(NULL));
		int sockfd = ConnectWithTimeout(dwServerIp,wServerPort,dwTimeOut,&wErrCode);//88888888888888888888888888888
		if(sockfd < 0)
		{
			printf("%s connect ServreIp: %s, Port: %d failed\n",__FUNCTION__, 
				inet_ntoa(*((struct in_addr *)&dwServerIp)), wServerPort);
			//printf("%s connect 2, %d\n", __func__, time(NULL));
			netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 1);
			return CTRL_FAILED_PARAM; 
		}
		//printf("%s connect ServreIp: %s, Port: %d success\n",__FUNCTION__, 
		//		inet_ntoa(*((struct in_addr *)&dwServerIp)), wServerPort);
		
#if 1
		//�����Ϣͷ
		MSG_HEAD head;
		memset(&head, 0, sizeof(head));
		memcpy(head.start, "QWCMD:", 6);
		//��Ϣͷ�����ݳ�����֮��ֵ

		//�����Ϣ��
		int len = 0;
		char *msg_body = (char *)malloc(BODYSIZE);//88888888888888888888888888888888888888888888888888
		
		if (msg_body == NULL)
		{
			printf("%s msg_body malloc failed\n", __func__);
			close(sockfd);
			netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 8);
			
			return CTRL_FAILED_OUTOFMEMORY;
		}
		memset(msg_body, 0, BODYSIZE);
		//�����ʼ�ֽ�
		strcpy(msg_body, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
		len = strlen(msg_body);
		sprintf(msg_body+len, "<Agent ID=\"%s\" Type=\"SG\" Ver=\"1.3.0.0\">\n", para.AgentID);
		strcat(msg_body, "<GetTicks/>\n");
		
		//׼�����ں�ʱ��
		SDateTime TriggerTime;
		char sTriggerTime[30];
		
		if (tm == 0)//û��ָ�������õ�ǰʱ��
		{
			ModSysComplexDTGet_TZ(&TriggerTime);
		}
		else	//ָ����ʱ�䣬������֮ǰͼƬԤ¼��ʱ��
		{
			ModSysComplexConvertToDT(tm, &TriggerTime);
		}
		
		sprintf(sTriggerTime, "%d-%d-%d %d:%d:%d",\
						TriggerTime.nYear, \
						TriggerTime.nMonth, \
						TriggerTime.nDay, \
						TriggerTime.nHour, \
						TriggerTime.nMinute, \
						TriggerTime.nSecode);
		
		//׼��GUID
		int tmpGUID;
		char sGUID[32];
		if (GUID == 0)
		{
			srand(time(NULL));
			do {
				tmpGUID = rand();
			} while(tmpGUID == 0);
		}
		else
		{
			tmpGUID = GUID;
		}
		
		sprintf(sGUID, "%d", tmpGUID);

		//׼��Note
		//Note ת��base64
		char *tmp = NULL;
		if (pmsg->note[0] != '\0')
		{
			tmp = base64_encode_v1(pmsg->note, strlen(pmsg->note));
			if (NULL == tmp)
			{
				close(sockfd);
				free(msg_body);
				return -1;
			}
			
			strcpy(pmsg->note, tmp);
			free(tmp);
			tmp = NULL;
		}
		//printf("base64: %s\n", pmsg->note);

		len = strlen(msg_body);
		switch (pmsg->type)
		{
			case EM_PIC_ALARM_LINK_UPLOAD:		// 1 ��������ͼ��
			case EM_PIC_MAINTAIN_UPLOAD:		// 2 �����ϴ�ͼ��
			case EM_PIC_DAILY_UPLOAD:			// 3 �ճ��ϴ�ͼ��
			case EM_PIC_TEST_UPLOAD:			// 4 �����ϴ�ͼ��
			case EM_PIC_SERVER_GET:				// 5 ��������ͼ�� 
			case EM_PIC_ACCEPT_UPLOAD:			// 33 �����ϴ�ͼ��
			{
				if (EM_PIC_SERVER_GET == pmsg->type)
				{
					strcpy(sGUID, SvrGuid);
				}

				int yg = pmsg->type;
				if (pmsg->type == EM_PIC_ALARM_LINK_UPLOAD)
				{
					//yg = 4;
					//strcpy(pmsg->note, "5rWL6K+V5Zu+5YOP5LiK5Lyg");
				}
				
				sprintf(msg_body+len, "<OperationCmd Type=\"%d\" Channel=\"%d\" TriggerTime=\"%s\" Note=\"%s\" GUID=\"%s\">\n", \
									yg, /*pmsg->type, */ \
									pmsg->chn, \
									sTriggerTime, \
									pmsg->note, \
									sGUID);
				len = strlen(msg_body);

				//���ͼƬ
				//ͼƬת��base64
			#if 0
				if (SnapData == NULL)
				{
					printf("%s chn%d, type: %d, SnapData == NULL\n", __func__, pmsg->chn, pmsg->type);
					close(sockfd);
					free(msg_body);
					netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 5);
					return -1;
				}
				if (DataSize == 0)
				{
					printf("%s chn%d, type: %d, DataSize == 0\n", __func__, pmsg->chn, pmsg->type);
					close(sockfd);
					free(msg_body);
					netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 5);
					return -1;
				}
				//printf("%s invoke base64_encode_v1\n", __func__);
				tmp = base64_encode_v1(SnapData, DataSize);
				if (NULL == tmp)
				{
					printf("%s chn%d, type: %d, base64_encode_v1 failed\n", __func__, pmsg->chn, pmsg->type);
					close(sockfd);
					free(msg_body);
					netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 6);
					return -1;
				}
				//printf("type: %d,pic-base64: %d-%d\n", pmsg->type, DataSize, strlen(tmp));
				
				strcat(msg_body, tmp);
				free(tmp);
				tmp = NULL;
			#else
				if (SnapData && DataSize)
				{
					tmp = base64_encode_v1(SnapData, DataSize);
					if (NULL == tmp)
					{
						printf("%s chn%d, type: %d, base64_encode_v1 failed\n", __func__, pmsg->chn, pmsg->type);
						close(sockfd);
						free(msg_body);
						netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 6);
						return -1;
					}					
					
					strcat(msg_body, tmp);
					free(tmp);
					tmp = NULL;
				}
				else
				{
					printf("%s type :%d snap to base64 failed, pSnapData: %p, len: %d\n", \
						__func__, pmsg->type, SnapData, DataSize);
					strcat(msg_body, "\n");
				}
			#endif
				
				//len = strlen(msg_body);
				strcat(msg_body, "</OperationCmd>\n");
				strcat(msg_body, "</Agent>\n");
				head.msg_size = sizeof(head)+strlen(msg_body);
			} break;
			case EM_SYS_Other:					// 11 ϵͳ��������
			case EM_Perimeter_Fault:			// 12 �ܽ籨������
			case EM_Networking_Fault:			// 13 ������������
			case EM_LocalAlarm_Fault:			// 14 ���ر�������
			case EM_VideoMonitor_Fault:			// 15 ��Ƶ��ع���
			case EM_BuildingIntercom_Fault:		// 16 ¥��Խ�����
			case EM_AccessControl_Fault:		// 17 ������ƹ���
			case EM_ElectronicPatrol_Fault:		// 18 ����Ѳ������
			case EM_DVR_STARTUP:				// 19 DVRϵͳ����
			case EM_DVR_EXIT_NORMOAL:			// 20 DVRϵͳ�˳�
			case EM_DVR_EXIT_ABNORMAL:			// 21 DVR�쳣�˳�
			case EM_DVR_PARAM_SET:				// 22 DVR��������
			case EM_DVR_PARAM_SAVE:				// 23 DVR��������
			case EM_DVR_VLOST:					// 24 DVR��Ƶ��ʧ
			case EM_DVR_VMOTION:				// 25 DVR�ƶ����
			case EM_DVR_ALARM_EXT:				// 26 DVR�ⲿ����
			case EM_DVR_ALARM_RESUME:			// 27 ϵͳ�������
			case EM_DVR_EXIT_ILLEGALITY:		// 28 DVR�Ƿ��˳�
			case EM_REPAIR_CHECK:				// 29 ϵͳά��ǩ��
			case EM_MAINTAIN_CHECK:				// 30 ϵͳά��ǩ��
			{
				sprintf(msg_body+len, "<OperationCmd Type=\"%d\" Channel=\"%d\" TriggerTime=\"%s\" Note=\"%s\" GUID=\"%s\">\n", \
									pmsg->type, \
									pmsg->chn, \
									sTriggerTime, \
									pmsg->note, \
									sGUID);
				strcat(msg_body, "</OperationCmd>\n");
				strcat(msg_body, "</Agent>\n");
				head.msg_size = sizeof(head)+strlen(msg_body);
				
			} break;
			
			case EM_DVR_MOTION:					// 34 ƫλ���� 1
			case EM_DVR_HDD_ERR:				// 41 DVR���̴���
			{
			} break;
			case EM_DVR_PLAYBACK:				// 31 DVR���ػط�
			case EM_REMOTE_PLAYBACK:			// 32 DVRԶ�̻ط�
			{
			} break;
			case EM_PIC_ALARM_UPLOAD_UNREC:		// 36 �����ϴ�ͼ��δ¼����ƶ���⣩
			case EM_PIC_MAINTAIN_UPLOAD_UNREC:	// 37 �����ϴ�ͼ��δ¼����ƶ���⣩
			case EM_PIC_DAILY_UPLOAD_UNREC:		// 38 �ճ��ϴ�ͼ�񣨵���δ¼��
			case EM_PIC_TEST_UPLOAD_UNREC:		// 39 �����ϴ�ͼ��δ¼����ƶ���⣩
			case EM_PIC_ACCEPT_UPLOAD_UNREC:	// 40 �����ϴ�ͼ��δ¼����ƶ���⣩
			{
			} break;
			case EM_HEART_TIMEOUT:				// 42 ϵͳ������ʱ
			case EM_HEART_RESUME:				// 43 ϵͳ�����ָ�
			{
			} break;
			case EM_VEDIO_OTHER:					// 44��Ƶ�����¼�
			{
			} break;
			case EM_HEART:	// ��������SGƽ̨��Ҫ���ͣ����ڱ�ϵͳ
			{
				//����Ӳ�̿ռ�
				u64 nTotal, nFree;
				nTotal = nFree = 0;
				int i;
				SStoreDevManage sStDevMgr;
				memset( &sStDevMgr, 0, sizeof(sStDevMgr) );
				
				SDevInfo sDevInfo[64]; //zlb20111117 ȥ������malloc
				sStDevMgr.psDevList = sDevInfo;
				memset(sStDevMgr.psDevList, 0, sizeof(SDevInfo)*64);
				
				if (0 == ModSysComplexDMGetInfo(&sStDevMgr, 8) )
				{
					if(sStDevMgr.nDiskNum>0)
					{
						for( i=0; i<sStDevMgr.nDiskNum; i++ )
						{
							nTotal += sStDevMgr.psDevList[i].nTotal;
							nFree += sStDevMgr.psDevList[i].nFree;
						}
					}
					nTotal *= 1024;
					nFree *= 1024;
					//printf("upload heart nTotal: %lld, nFree: %lld\n", nTotal, nFree);
					//ȥ��֮ǰ�����"<GetTicks/>\r"
					len -= strlen("<GetTicks/>\n");
					sprintf(msg_body+len, "<DVRHeart State=\"0\" TotalSpace=\"%lld\" FreeSpace=\"%lld\">\n", \
								nTotal, \
								nFree);

					strcat(msg_body, "init,hicore,pppoed\n");
					strcat(msg_body, "</DVRHeart>\n");
					strcat(msg_body, "<GetTicks/>\n");
					strcat(msg_body, "</Agent>\n");
					head.msg_size = sizeof(head)+strlen(msg_body);
				}
				else
				{
					printf("%s: ModSysComplexDMGetInfo() failed\n", __FUNCTION__);
					close(sockfd);
					free(msg_body);
					netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 7);
					return -1;
				}
				
			} break;
			default:
				printf("%s unknow cmd type: %d\n", __FUNCTION__, pmsg->type);
		}
#endif
		//��ӡmsgbody������
		if ( ((pmsg->type >= EM_PIC_ALARM_LINK_UPLOAD) &&(pmsg->type <= EM_PIC_TEST_UPLOAD)) \
					|| (pmsg->type == EM_PIC_ACCEPT_UPLOAD) ) 
		{
			
		}
		else
		{
			//printf("%s msgbody: \n%s\n", __func__, msg_body);
		}
		//printf("%s msgbody: \n%s\n", __func__, msg_body);
		//������Ϣ
		if (head.msg_size != 0)
		{
			if (loopsend(sockfd, (char *)&head, sizeof(head)) != sizeof(head))
			{
				printf("%s send msg head failed\n", __FUNCTION__);
				close(sockfd);
				free(msg_body);
				netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 2);
				return CTRL_FAILED_NETSND;
			}
			else
			{
				//printf("%s msg_head success\n", __func__);
				//printf("%s msg_body size: %d\n", __func__, head.msg_size-sizeof(head));
				if (loopsend(sockfd, (char *)msg_body, head.msg_size-sizeof(head)) != head.msg_size-sizeof(head))
				{
					printf("%s send msg body failed\n", __FUNCTION__);
					close(sockfd);
					free(msg_body);
					netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 2);
					return CTRL_FAILED_NETSND;
				}
				//printf("%s msg_body success\n", __func__);
			}
			
			
		}
		//printf("yg sg send msg success\n");

	//����
		//��Ϣͷ
		ret = CTRL_SUCCESS;

		//printf("%s recv head 1, %d\n", __func__, time(NULL));
		ret = looprecv_withtimeout(sockfd, (char *)&head, sizeof(head), RCV_TIMEOUT *1000*1000);
		if (ret != CTRL_SUCCESS)
		{
			printf("%s recv msg head failed\n", __FUNCTION__);
			printf("%s recv head 2, %d\n", __func__, time(NULL));
			close(sockfd);
			free(msg_body);
			//���Խ���ʧ��
			//netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 3);
 			return ret;
		}
		
		//printf("%s recv msg head success, msg_size: %d\n", __FUNCTION__, head.msg_size);
		//printf("%s recv head 2, %d\n", __func__, time(NULL));
		//��Ϣ��
		int recvsize = head.msg_size - sizeof(head);
		char *recvbuf = (char *)malloc(recvsize+1);//88888888888888888888888888888888888888888888
		if (recvbuf == NULL)
		{
			printf("%s malloc recvbuf failed\n", __FUNCTION__);
			close(sockfd);
			free(msg_body);
			//���Խ���ʧ��
			//netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 8);
			return CTRL_FAILED_OUTOFMEMORY;
		}
		recvbuf[recvsize] = '\0';//

		//printf("%s recv body 1, %d\n", __func__, time(NULL));
		ret = looprecv_withtimeout(sockfd, recvbuf, recvsize, RCV_TIMEOUT *1000*1000);
		if (ret != CTRL_SUCCESS)
		{
			printf("%s recv msg body failed\n", __FUNCTION__);
			printf("%s recv body 2, %d\n", __func__, time(NULL));
			close(sockfd);
			free(msg_body);
			free(recvbuf);

			//���Խ���ʧ��
			//netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 3);
			return ret;
		}
		//printf("%s recv msg body success\n", __FUNCTION__);
		//printf("%s recv body 2, %d\n", __func__, time(NULL));
		//printf("recv msg body: %s\n", recvbuf);
		
		char *p, *q;
		
		if (EM_HEART == pmsg->type)
		{
			p = strstr(recvbuf, "Ticks Value=");
			if (p != NULL)
			{
				Gheart_interval_minute = atoi(p+strlen("Ticks Value=")+1);
				printf("yg Gheart_interval_minute: %d\n", Gheart_interval_minute);
			}

			//������ƽ̨��������ͼ��
			proc_svr_req(recvbuf);
			//��¼SvrGuid
			q = SvrGuid;
			p = strstr(recvbuf, "GUID");
			if (p != NULL)
			{
				p += strlen("GUID")+2;
				while( *p != '\"')
				{
					*q++ = *p++;
				}
				*q = '\0';
			}
			else
			{
				printf("%s \"GUID\" not found\n", __func__);
				memset(SvrGuid, 0, sizeof(SvrGuid));
			}
		}

		
		
		close(sockfd);
		free(msg_body);
		free(recvbuf);
		
	}
	else
	{
		printf("%s: ModConfigGetParam failed\n", __FUNCTION__);
	}
	netComm_NotifySGuploadState(pmsg->chn, pmsg->type, 0);

	//printf("%s upload sg platform success\n", __func__);
	return ret;
}


void UploadSGPlatformFxn(void *para)
{
	int ret;
	SSG_MSG_TYPE msg, heartmsg;
	
	fd_set action_fds;
	struct timeval tv;
	time_t cur, nextheart;

	time(&nextheart);
	
	while(1)
	{
		FD_ZERO(&action_fds);
		FD_SET(upload_sg_fd[0], &action_fds);
		
		tv.tv_sec  = 1;
		tv.tv_usec = 0;
		
		ret = select(upload_sg_fd[0] + 1, &action_fds, NULL, NULL, &tv);

		if (ret > 0) 
		{
			if(FD_ISSET(upload_sg_fd[0], &action_fds)) 
			{
				if (read(upload_sg_fd[0], &msg, sizeof(msg)) != sizeof(msg))
				{
					printf("%s: read pipo failed\n", __FUNCTION__);
					continue;
				}
				printf("%s msgtype: %d\n",__func__, msg.type);
				upload_sg_proc(&msg, 0, NULL, 0, 0);//ֻ������Ϣ��ͼƬ�ϴ�ר�Ŵ���
				/*
				if ( ((msg.type >= EM_PIC_ALARM_LINK_UPLOAD) &&(msg.type <= EM_PIC_TEST_UPLOAD)) \
					|| (msg.type == EM_PIC_ACCEPT_UPLOAD) ) 
				{
					//printf("%s pic\n",__func__);
					upload_sg_pic(&msg);
				}
				else
				{
					//printf("%s other\n",__func__);
					upload_sg_proc(&msg, NULL, 0);
				}
				*/
			}
		}
		
		time(&cur);
		if (cur > nextheart + Gheart_interval_minute*60-10)//��ǰ10��
		{
			nextheart = cur;
					
			memset(&heartmsg, 0, sizeof(heartmsg));
			heartmsg.type = EM_HEART;
			heartmsg.chn = 0;
			//upload_sg(&heartmsg);
			upload_sg_proc(&heartmsg, 0, NULL, 0, 0);
			
		}
	}
}

s32 net_sg_init(PSNetCommCfg pCfg)
{
	int ret, i;
	pthread_t tid;
	//SModConfigUserPara para;

	//ret = ModConfigGetParam(EM_CONFIG_PARA_SG, &para, 0);
	ret = pipe(upload_sg_fd);
	if(ret < 0)//init pipe fd for transFd
	{
		printf("upload_sg_fd pipe:failed,errno=%d,result=%s\n", errno, strerror(errno));
		return ret;
	}
	ret = pthread_create(&tid, NULL, (void*)UploadSGPlatformFxn, NULL);
	if(0 != ret)
	{
		printf("error: pthread_create,  func:%s\n", __FUNCTION__);		
		return ret;
	}

	pNotify = pCfg->pNotify;
	if ( NULL == pNotify )
	{
		printf("%s NULL == pNotify, Initialization failed\n", __func__);
		return -1;
	}

	inittime = time(NULL);
	bModInited = 1;
	
	printf("%s init ok!!!!!!!!!!!!!!\n", __func__);
	
	return ret;	
}

inline void net_write_upload_sg(SSG_MSG_TYPE *pmsg)
{
	pthread_mutex_lock(&upload_sg_mutex);
	//printf("write_pipe:key=%d\n",key);
	write(upload_sg_fd[1], pmsg, sizeof(SSG_MSG_TYPE));
	pthread_mutex_unlock(&upload_sg_mutex);
}

void upload_sg(SSG_MSG_TYPE *pmsg)
{
	if (bModInited == 0)
	{
		printf("%s msg type: %d, sg_platform module has not been initialized, wait......\n", __func__, pmsg->type);
		return;
	}
	//printf("%s msg type: %d\n", __func__, pmsg->type);
	net_write_upload_sg(pmsg);
}

//base64 �����
#if 1
const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
char *base64_encode_v1(const char* data, int data_len)
{ 
	//int data_len = strlen(data); 
	int prepare = 0; 
	int ret_len; 
	int temp = 0; 
	char *ret = NULL; 
	char *f = NULL; 
	int tmp = 0; 
	char changed[4]; 
	int i = 0; 
	ret_len = data_len / 3; 
	temp = data_len % 3; 
	
	if (temp > 0) 
	{ 
		ret_len += 1; 
	} 
	ret_len = ret_len*4 + 1; 
	ret = (char *)malloc(ret_len); 
	//printf("%s ret_len: %d\n", __func__, ret_len);
	if ( ret == NULL) 
	{ 
		printf("No enough memory.\n"); 
		return ret; 
	} 
	memset(ret, 0, ret_len); 
	f = ret; 
	while (tmp < data_len) 
	{ 
		temp = 0; 
		prepare = 0; 
		memset(changed, '\0', 4); 
		while (temp < 3) 
		{ 
		//printf("tmp = %d\n", tmp); 
			if (tmp >= data_len) 
			{ 
				break; 
			} 
			prepare = ((prepare << 8) | (data[tmp] & 0xFF)); 
			tmp++; 
			temp++; 
		} 
		prepare = (prepare<<((3-temp)*8)); 
		//printf("before for : temp = %d, prepare = %d\n", temp, prepare); 
		for (i = 0; i < 4 ;i++ ) 
		{ 
			if (temp < i) 
			{ 
				changed[i] = 0x40; 
			} 
			else 
			{ 
				changed[i] = (prepare>>((3-i)*6)) & 0x3F; 
			} 
			*f = base[changed[i]]; 
			//printf("%.2X", changed[i]); 
			f++; 
		} 
	} 
	*f = '\0'; 

	return ret; 

} 
/* */ 
static char find_pos(char ch)   
{ 
    char *ptr = (char*)strrchr(base, ch);//the last position (the only) in base[] 
    return (ptr - base); 
} 
/* */ 
char *base64_decode_v1(const char *data, int data_len) 
{ 
    int ret_len = (data_len / 4) * 3; 
    int equal_count = 0; 
    char *ret = NULL; 
    char *f = NULL; 
    int tmp = 0; 
    int temp = 0; 
    char need[3]; 
    int prepare = 0; 
    int i = 0; 
    if (*(data + data_len - 1) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 2) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 3) == '=') 
    {//seems impossible 
        equal_count += 1; 
    } 
    switch (equal_count) 
    { 
    case 0: 
        ret_len += 4;//3 + 1 [1 for NULL] 
        break; 
    case 1: 
        ret_len += 4;//Ceil((6*3)/8)+1 
        break; 
    case 2: 
        ret_len += 3;//Ceil((6*2)/8)+1 
        break; 
    case 3: 
        ret_len += 2;//Ceil((6*1)/8)+1 
        break; 
    } 
    ret = (char *)malloc(ret_len); 
    if (ret == NULL) 
    { 
        printf("No enough memory.\n"); 
        exit(0); 
    } 
    memset(ret, 0, ret_len); 
    f = ret; 
    while (tmp < (data_len - equal_count)) 
    { 
        temp = 0; 
        prepare = 0; 
        memset(need, 0, 4); 
        while (temp < 4) 
        { 
            if (tmp >= (data_len - equal_count)) 
            { 
                break; 
            } 
            prepare = (prepare << 6) | (find_pos(data[tmp])); 
            temp++; 
            tmp++; 
        } 
        prepare = prepare << ((4-temp) * 6); 
        for (i=0; i<3 ;i++ ) 
        { 
            if (i == temp) 
            { 
                break; 
            } 
            *f = (char)((prepare>>((2-i)*8)) & 0xFF); 
            f++; 
        } 
    } 
    *f = '\0'; 
    return ret; 
}

#endif

