#ifndef __XM_H__ 
#define __XM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>

#include "ipcamera.h"

#define BROADCAST_IP "255.255.255.255"
#define BROADCAST_PORT 34569

#define LOGIN_REQ		1000	//��¼����			0x3e8
#define LOGIN_RSP		1001	//��¼��Ӧ
#define LOGOUT_REQ		1002	//�ǳ�����
#define LOGOUT_RSP		1003	//�ȳ���Ӧ
#define FORCELOGOUT_REQ	1004	//ǿ�Ƶǳ�����
#define FORCELOGOUT_RSP	1005	//ǿ�Ƶǳ���Ӧ
#define KEEPALIVE_REQ	1006	//��������			0x3ee
#define KEEPALIVE_RSP	1007	//������Ӧ			0x3ef
#define SYSINFO_REQ		1020	//��ȡϵͳ��Ϣ����	0x3fc
#define CONFIG_SET		1040	//������������
#define CONFIG_SET_RSP	1041	//����������Ӧ
#define CONFIG_GET		1042	//��ȡ��������
#define CONFIG_GET_RSP	1043	//��ȡ������Ӧ

#define IPSEARCH_REQ	1530	//IP�Զ���������
#define IPSEARCH_RSP	1531	//IP�Զ�����������Ӧ

#define ABILITY_GET		1360	//��������ȡ����0x550
#define ABILITY_GET_RSP	1361	//��������ȡ��Ӧ0x551

#define PTZ_REQ			1400	//��̨��������
#define PTZ_RSP		1401	//��̨������Ӧ
#define MONITOR_REQ		1410	//ʵʱ��������		0x582
#define MONITOR_RSP		1411	//ʵʱ����������Ӧ	0x583
#define MONITOR_DATA	1412	//ʵʱ��������		0x584
#define MONITOR_CLAIM	1413	//������������		0x585
#define MONITOR_CLAIM_RSP	1414//��������������Ӧ
#define SYSMANAGER_REQ	1450	//ϵͳ��������
#define SYSMANAGER_RSP	1451	//ϵͳ����������Ӧ
#define TIME_SET_REQ	1590	//ʱ����������		0x636
#define TIME_SET_RSP	1591	//ʱ������������Ӧ	0x637

#define DATA_SIZE (16*1024)
#define FAILURE (-1)
#define SUCCESS (0)
#define NETSNDRCVFLAG	MSG_NOSIGNAL

#define VER 0

int XM_Init(unsigned int max_client_num);
int XM_DeInit();
int XM_Search(ipc_node** head, ipc_node **tail, unsigned char check_conflict);
int XM_Start(int chn, RealStreamCB pCB, unsigned int dwContext, char* streamInfo, unsigned int dwIp, unsigned short wPort, char *user, char *pwd, unsigned char rtsp_over_tcp);
int XM_Stop(int chn);

int XM_GetLinkStatus(int chn);
int XM_GetNetworkParam(ipc_unit *ipcam, ipc_neteork_para_t *pnw);
int XM_SetNetworkParam(ipc_unit *ipcam, ipc_neteork_para_t *pnw);

int XM_CMD_Open(int chn);
int XM_CMD_Close(int chn);
int XM_CMD_SetImageParam(int chn, video_image_para_t *para);
int XM_CMD_GetImageParam(int chn, video_image_para_t *para);
int XM_CMD_PtzCtrl(int chn, EMPTZCMDTYPE cmd, void* data);
int XM_CMD_SetTime(int chn, time_t t, int force);
int XM_CMD_Reboot(int chn);
int XM_CMD_RequestIFrame(int chn);

#endif

/*

typedef struct
{
	unsigned char	Head_Flag;		//Э��ͷ��ʶ, 1�ֽڣ��̶�Ϊ0xFF
	unsigned char	VERSION;		//�汾�ţ���ǰΪ1
	unsigned char	RESERVED01;		//����λ
	unsigned char	RESERVED02;		//����λ
	//�ỰID��ÿ��CU��PU�������Ӻ�PU���CU����һ��Ψһ�ĻỰID��
	//PU�����¼ʱ�����ֶ���0����¼�ɹ���PU����Ӧ��Ϣ�������ֶΣ�
	//�˺�˫������Ϣͷ������Ҫ���ỰID
	unsigned int	SessionID;	
	unsigned int	SEQUENCE_NUMBER;//����ţ��������0��ʼ���������������ڴ�0��ʼ
	unsigned char	Total_Packet;	//�ܰ�����Ϊ0��1ʱ����ʾֻ��һ�����ݰ�������1��ʾ�ж�����ݰ���ÿ�����ݰ���С������16K
	unsigned char	CurPacket;	//��ǰ����ţ�ֻ����Total Packet����1ʱ�������壬��ʾ��ǰ�ǵڼ������ݰ����±��0��ʼ
	unsigned short	Message_Id;	//��Ϣ��
	unsigned int	Data_Length;	//����������
	//char	*pData;		//����������󲻳���16K
} ctrl_head_t;

typedef struct
{
	char	ip[16];
	unsigned short	port;
} net_info_t;

#define MAIN_STREAM		0
#define EXTRA_STREAM	1

typedef struct
{
	unsigned int	SessionID;
	int main_sock;
	int stream_sock;
	int stream_type;
	unsigned int dwStreamWidth;
	unsigned int dwStreamHeight;
	RealStreamCB pStreamCB;
	unsigned int dwContext;
	pthread_t capture_tid;
	pthread_mutex_t lock;
} Session_info_t;

typedef struct
{
	net_info_t	tcp_info;
	Session_info_t session[2];//0: main_stream  1:extra_stream
} xm_camera_info_t;	

*/


