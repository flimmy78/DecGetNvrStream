#ifndef _CTRLPROTOCOL_H_
#define _CTRLPROTOCOL_H_

//#include "iflytype.h"
#include "common_basetypes.h"
#include "camera.h"
//#include "common.h"
#include "public.h"


#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#endif

#ifdef SHENSHIYIN
#define _TOUMING_COM_//csp modify 20100311
#endif

#define IECTRL_NEW

//zlb20090916 begin
#define EMAIL_ALARM_TYPE_MD 0
#define EMAIL_ALARM_TYPE_INPUT 1
#define EMAIL_ALARM_TYPE_EXCEPTION 2
#define EMAIL_ALARM_TYPE_VL 3
#define EMAIL_ALARM_TYPE_VB 4
#define MAX_EMAIL_LENGTH 32
//zlb20090916 end

#define VERSION_OF_9508

//pw 2010/7/26
#if (defined(TL_9508) || defined(TL_9504))
	#if (defined(TIANMIN) && defined(TL_9508))
		#define MAX_ALARM_IN_NUM		8
		#define MAX_ALARM_OUT_NUM		1 	// 4	
	#elif defined(SHENSHIYIN_SP1) && defined(TL_9508)
		#define MAX_ALARM_IN_NUM		8
		#define MAX_ALARM_OUT_NUM	4
	#elif defined(SHENSHIYIN_SP1) && (defined(TL_9504_CIF_8M) || defined(TL_9504_D1_8M))
		#define MAX_ALARM_IN_NUM		4
		#define MAX_ALARM_OUT_NUM	4
	#else
		#define MAX_ALARM_IN_NUM		4
		#define MAX_ALARM_OUT_NUM		1
	#endif
#else
	#define MAX_ALARM_IN_NUM	8 // 16			//fgpa:16 6446:4
	#define MAX_ALARM_OUT_NUM	2 // 4			//fpga:4 6446:4
#endif

#define SPECIAL_DEV_INFO
#define CRUISE_POINT_NUM		128

#ifdef WIN32
#define SOCKHANDLE		SOCKET
#else
#define SOCKHANDLE		int
#define INVALID_SOCKET	(-1)

#define SOCKET_SUCCESS	(0)
#define SOCKET_ERROR	(-1)
#define SOCKET_TIMEOUT	(-2)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define INFINITE	0xffffffff
#endif

//extern u16 server_port;//wrchen 090907
#define CTRL_PROTOCOL_SERVERPORT		8670					//�������˿�//wrchen 090205
#define CTRL_PROTOCOL_MAXLINKNUM		256

#define CTRL_PROTOCOL_CONNECT_BLOCK		INFINITE				//���ӷ�ʽ:����
#define CTRL_PROTOCOL_CONNECT_DEFAULT	3000					//ȱʡ����ʱ��:3��

#define CTRL_VERSION					0x0100

#define CTRL_COMMAND					0
#define CTRL_NOTIFY						1
#define CTRL_ACK						2

//��������
#define CTRL_CMD_BASE					10000
#define CTRL_CMD_LOGIN					CTRL_CMD_BASE+1			//Զ�̵�¼����
#define CTRL_CMD_LOGOFF					CTRL_CMD_BASE+2			//ע���û���¼
#define CTRL_CMD_GETDEVICEINFO			CTRL_CMD_BASE+3			//����豸��Ϣ
#define CTRL_CMD_GETVIDEOPROPERTY		CTRL_CMD_BASE+4			//��Ƶ������Ϣ
#define CTRL_CMD_GETAUDIOPROPERTY		CTRL_CMD_BASE+5			//��Ƶ������Ϣ
#define CTRL_CMD_GETVOIPPROPERTY		CTRL_CMD_BASE+6			//voip������Ϣ
#define CTRL_CMD_GETMDPROPERTY			CTRL_CMD_BASE+7			//�ƶ����������Ϣ
#define CTRL_CMD_STOPVIDEOMONITOR		CTRL_CMD_BASE+8			//ֹͣ��ƵԤ��
#define CTRL_CMD_STOPAUDIOMONITOR		CTRL_CMD_BASE+9			//ֹͣ��ƵԤ��
#define CTRL_CMD_STOPVOIP				CTRL_CMD_BASE+10		//ֹͣvoip
#define CTRL_CMD_STOPDOWNLOAD			CTRL_CMD_BASE+11		//ֹͣ����
#define CTRL_CMD_STOPFILEPLAY			CTRL_CMD_BASE+12		//ֹͣ�ļ��ط�
#define CTRL_CMD_STOPTIMEPLAY			CTRL_CMD_BASE+13		//ֹͣʱ��ط�
#define CTRL_CMD_FASTPLAY				CTRL_CMD_BASE+14		//���ٲ���
#define CTRL_CMD_SLOWPLAY				CTRL_CMD_BASE+15		//���ٲ���
#define CTRL_CMD_SETPLAYRATE			CTRL_CMD_BASE+16		//��������
#define CTRL_CMD_PAUSEPLAY				CTRL_CMD_BASE+17		//��ͣ����
#define CTRL_CMD_RESUMEPLAY				CTRL_CMD_BASE+18		//�ָ�����
#define CTRL_CMD_SINGLEPLAY				CTRL_CMD_BASE+19		//֡��
#define CTRL_CMD_FASTBACKPLAY			CTRL_CMD_BASE+20		//����
#define CTRL_CMD_PLAYPREV				CTRL_CMD_BASE+21		//��һ��
#define CTRL_CMD_PLAYNEXT				CTRL_CMD_BASE+22		//��һ��
#define CTRL_CMD_PLAYSEEK				CTRL_CMD_BASE+23		//��λ
#define CTRL_CMD_PLAYMUTE				CTRL_CMD_BASE+24		//�طž���
#define CTRL_CMD_PLAYPROGRESS 			CTRL_CMD_BASE+25		//���Ž���
#define CTRL_CMD_GETNETWORK				CTRL_CMD_BASE+26		//����������
#define CTRL_CMD_SETNETWORK				CTRL_CMD_BASE+27		//�����������
#define CTRL_CMD_GETVGASOLLIST			CTRL_CMD_BASE+28		//���VGA�ֱ���
#define CTRL_CMD_GETSYSPARAM			CTRL_CMD_BASE+29		//���ϵͳ����
#define CTRL_CMD_SETSYSPARAM 			CTRL_CMD_BASE+30		//����ϵͳ����
#define CTRL_CMD_GETRECPARAM			CTRL_CMD_BASE+31		//���¼�����
#define CTRL_CMD_SETRECPARAM 			CTRL_CMD_BASE+32		//����¼�����
#define	CTRL_CMD_GETSUBSTREAMPARAM		CTRL_CMD_BASE+33		//�������������
#define	CTRL_CMD_SETSUBSTREAMPARAM		CTRL_CMD_BASE+34		//��������������
#define CTRL_CMD_GETIMGPARAM			CTRL_CMD_BASE+35		//���ͼ�����
#define CTRL_CMD_SETIMGPARAM 			CTRL_CMD_BASE+36		//����ͼ�����
#define CTRL_CMD_GETPICADJ 				CTRL_CMD_BASE+37		//��û������
#define CTRL_CMD_SETPICADJ 				CTRL_CMD_BASE+38		//���û������
#define CTRL_CMD_GETALARMINPARAM 		CTRL_CMD_BASE+39		//��ñ����������
#define CTRL_CMD_SETALARMINPARAM 		CTRL_CMD_BASE+40		//���ñ����������
#define CTRL_CMD_GETALARMOUTPARAM 		CTRL_CMD_BASE+41		//��ñ����������
#define CTRL_CMD_SETALARMOUTPARAM 		CTRL_CMD_BASE+42		//���ñ����������
#define	CTRL_CMD_GETALARMNOTICYPARAM	CTRL_CMD_BASE+43		//��ñ���֪ͨ����
#define	CTRL_CMD_SETALARMNOTICYPARAM	CTRL_CMD_BASE+44		//���ñ���֪ͨ����
#define CTRL_CMD_GETRECSCHPARAM 		CTRL_CMD_BASE+45		//���¼�񲼷�����
#define CTRL_CMD_SETRECSCHPARAM 		CTRL_CMD_BASE+46		//����¼�񲼷�����
#define CTRL_CMD_GETMDPARAM 			CTRL_CMD_BASE+47		//����ƶ�������
#define CTRL_CMD_SETMDPARAM 			CTRL_CMD_BASE+48		//�����ƶ�������
#define CTRL_CMD_GETVIDEOLOSTPARAM		CTRL_CMD_BASE+49		//�����Ƶ��ʧ����
#define CTRL_CMD_SETVIDEOLOSTPARAM 		CTRL_CMD_BASE+50		//������Ƶ��ʧ����
#define CTRL_CMD_GETVIDEOBLOCKPARAM 	CTRL_CMD_BASE+51		//�����Ƶ�ڵ�����
#define CTRL_CMD_SETVIDEOBLOCKPARAM 	CTRL_CMD_BASE+52		//������Ƶ�ڵ�����
#define CTRL_CMD_GETPTZPARAM			CTRL_CMD_BASE+53		//�����̨����
#define CTRL_CMD_SETPTZPARAM 			CTRL_CMD_BASE+54		//������̨����
#define CTRL_CMD_SETPRESET 				CTRL_CMD_BASE+55		//����Ԥ�õ����
#define CTRL_CMD_GETCRUISEPARAM 		CTRL_CMD_BASE+56		//���Ѳ��·������
#define CTRL_CMD_SETCRUISEPARAM 		CTRL_CMD_BASE+57		//����Ѳ��·������
#define CTRL_CMD_CTRLCRUISEPATH 		CTRL_CMD_BASE+58		//Ѳ��·������
#define CTRL_CMD_CTRLPTZTRACK 			CTRL_CMD_BASE+59		//�켣����
#define CTRL_CMD_GETHDDINFO 			CTRL_CMD_BASE+60		//��ô�����Ϣ
#define CTRL_CMD_GETUSERINFO			CTRL_CMD_BASE+61		//����û���Ϣ
#define CTRL_CMD_SETUSERINFO 			CTRL_CMD_BASE+62		//�����û���Ϣ
#define CTRL_CMD_SETRESTORE				CTRL_CMD_BASE+63		//�ָ�Ĭ��
#define CTRL_CMD_CLEARALARM				CTRL_CMD_BASE+64		//�������
#define CTRL_CMD_GETSYSTIME				CTRL_CMD_BASE+65		//���ϵͳʱ��
#define CTRL_CMD_SETSYSTIME				CTRL_CMD_BASE+66		//����ϵͳʱ��
#define CTRL_CMD_GETSYSINFO				CTRL_CMD_BASE+67		//���ϵͳ��Ϣ
#define CTRL_CMD_SHUTDOWN				CTRL_CMD_BASE+68		//�ر�ϵͳ
#define CTRL_CMD_REBOOT					CTRL_CMD_BASE+69		//����ϵͳ
#define CTRL_CMD_PTZCTRL				CTRL_CMD_BASE+70		//��̨����
#define CTRL_CMD_GETMANUALREC			CTRL_CMD_BASE+71		//����ֶ�¼��״̬
#define CTRL_CMD_SETMANUALREC			CTRL_CMD_BASE+72		//�����ֶ�¼��״̬
#define CTRL_CMD_LOGSEARCH				CTRL_CMD_BASE+73		//��־�ļ�����
#define CTRL_CMD_RECFILESEARCH			CTRL_CMD_BASE+74		//�ط��ļ�����
#define CTRL_CMD_GETSPSPPS				CTRL_CMD_BASE+75		//���spspps����
#define CTRL_CMD_ALARMUPLOADCENTER		CTRL_CMD_BASE+76		//�����ϴ�����
#ifdef _TOUMING_COM_
#define CTRL_CMD_SERIALSTOP 			CTRL_CMD_BASE+80 		//ֹͣ͸������//csp modify 20100311
#define CTRL_CMD_COM_PROTOCOL_GET 		CTRL_CMD_BASE+78 		//��ô��ڲ���
#define CTRL_CMD_COM_PROTOCOL_SET		CTRL_CMD_BASE+79 		//���ô��ڲ���
#endif
#define CTRL_CMD_GETSYSLANGLIST   		CTRL_CMD_BASE+83  		//���ϵͳ�����б�		//dong	100426
#define CTRL_CMD_GETBITRATELIST 		CTRL_CMD_BASE+84 		//�������λ���б� 
#define CTRL_CMD_GETEMAILSMTP 			CTRL_CMD_BASE+88 		//��ñ���EmailSMTP��Ϣ		EMAIL_OPT
#define CTRL_CMD_SETEMAILSMTP 			CTRL_CMD_BASE+89 		//���ñ���EmailSMTP��Ϣ		EMAIL_OPT

//_DAYTIME_
#define CTRL_CMD_USEUTCTIME				CTRL_CMD_BASE+93 		//�Ƿ�ʹ��UTC��ʽʱ�� 

#ifdef IECTRL_NEW
#define CTRL_CMD_RESETPICADJUST 		CTRL_CMD_BASE+97 				//�ָ�Ĭ�ϻ������
#define CTRL_CMD_GETFRAMERATELIST 		CTRL_CMD_BASE+98 				//���֡���б�
#define CTRL_CMD_GETMAX_IMGMASKNUM 		CTRL_CMD_BASE+99 				//���֧�ֵ�����ڸ���
#define CTRL_GET_RESOLUTION_LIST 		CTRL_CMD_BASE+100 				//�����/�������ֱ����б�
#endif

#ifdef SPECIAL_DEV_INFO
#define CTRL_CMD_GET_SPECIALDEVICEINFO 	CTRL_CMD_BASE+107 //��������ϵͳ��Ϣ������ĳЩ�źŵļ���
#endif

#define CTRL_CMD_GETMDSENSELIST 		CTRL_CMD_BASE+129 //��ȡ�ƶ�����������б�
#define CTRL_CMD_GETMDALARMDELAYLIST  	CTRL_CMD_BASE+130 //��ȡ�ƶ���ⱨ�������ʱ�б�
#define CTRL_CMD_GETBAUDRATELIST 		CTRL_CMD_BASE+131 //��ȡ�������б�
#define CTRL_CMD_GETPTZPROTOCOLLIST 	CTRL_CMD_BASE+132 //��ȡ��̨Э���б�

#define CTRL_CMD_GETRECSCHPARAMBYTYPE  	CTRL_CMD_BASE+133 //���������ͻ��¼�񲼷�����
#define CTRL_CMD_SETRECSCHPARAMBYTYPE 	CTRL_CMD_BASE+134 //���������ͷֿ�����¼�񲼷�����
#define CTRL_CMD_RECFILESEARCH2 		CTRL_CMD_BASE+139 //2.0�ط��ļ����� 

#define CTRL_CMD_GETDDNSLIST			CTRL_CMD_BASE+152//��ȡDDNS�������б�

//csp modify 20130423
#define CTRL_CMD_SETMONITORINFO			CTRL_CMD_BASE+160//����Զ�̼����Ϣ

//csp modify 20130519
#define CTRL_CMD_GETADVPRIVILEGE		CTRL_CMD_BASE+161//��ȡ�û��߼�Ȩ��

//xdc
#define CTRL_CMD_GETSEACHIPCLIST		CTRL_CMD_BASE+162		//��ȡ����������ͷ�б�
#define CTRL_CMD_GETADDIPCLIST			CTRL_CMD_BASE+163		//��ȡ����ӵ�����ͷ�б�
#define CTRL_CMD_SETIPC				CTRL_CMD_BASE+164		//���ö�Ӧͨ������ͷ
#define CTRL_CMD_ADDIPC				CTRL_CMD_BASE+165		//�������ͷ
#define CTRL_CMD_DELETEIPC			CTRL_CMD_BASE+166		//ɾ������ͷ
#define CTRL_CMD_GETTHEOTHER		CTRL_CMD_BASE+167		//��ȡ����������ͷʣ�µ��б�,������ȡ

//yaogang modify 20141030
#define CTRL_CMD_GET_PATROL_PARA		CTRL_CMD_BASE+168		//��ȡ��Ѳ����
#define CTRL_CMD_SET_PATROL_PARA		CTRL_CMD_BASE+169		//������Ѳ����
#define CTRL_CMD_GET_PREVIEW_PARA		CTRL_CMD_BASE+170		//��ȡԤ������
#define CTRL_CMD_SET_PREVIEW_PARA		CTRL_CMD_BASE+171		//����Ԥ������
#define CTRL_CMD_GET_CHN_PARA		CTRL_CMD_BASE+172		//��ȡͨ������
#define CTRL_CMD_SET_CHN_PARA		CTRL_CMD_BASE+173		//����ͨ������
#define CTRL_CMD_CLEAN_ALARM_ICON		CTRL_CMD_BASE+174	//������汨�����Ǳ�
#define CTRL_CMD_CLOSE_GUIDE		CTRL_CMD_BASE+175		//Ծ������CMS remote CTRL
//yaogang modify 20160122
#define CTRL_CMD_GET_IPCCHN_LINKSTATUS	CTRL_CMD_BASE+176		//��ȡIPCͨ������״̬(һ��IPC������ͨ��������������)

typedef struct
{
	unsigned char max_chn_num;//���ͨ����
	unsigned char chn_status[20];//һ��bit��Ӧһ��ͨ������160ͨ��
}PACK_NO_PADDING ifly_ipc_chn_status_t;


typedef struct
{
	u8 chn; //ͨ��
	u8 weekday; //����
	u8 type;
	struct RecSchTimeField_t
	{
		u32 starttime; //��ʼʱ��
		u32 endtime; //��ֹʱ��
		u8 reserved[6]; //�����ֶ�
	} PACK_NO_PADDING TimeFiled[24];
    u8 copy2Weekday; //���Ƶ�������  ��λ 
    u32 copy2Chnmask; //���Ƶ�����ͨ������λ
	u8 reserved[16]; //�����ֶ�
} PACK_NO_PADDING ifly_RecSchTime_t;

enum PROTOCOLTYPE {
	NETDVR_PROTOCOLTYPE_PELCO_D = 0, //protocol type : Pelco-d
	NETDVR_PROTOCOLTYPE_PELCO_P = 1, //protocol type : Pelco-p
	NETDVR_PROTOCOLTYPE_B01 = 2, //protocol type : B01
	NETDVR_PROTOCOLTYPE_SAMSUNG = 3, //protocol type : Samsung
	NETDVR_PROTOCOLTYPE_HY = 4,
	NETDVR_PROTOCOLTYPE_Panasonic = 5,
	NETDVR_PROTOCOLTYPE_Pelco_9750 = 6,
	NETDVR_PROTOCOLTYPE_PelcoD1 = 7,
	NETDVR_PROTOCOLTYPE_PelcoD_S1 = 8,
	NETDVR_PROTOCOLTYPE_PelcoP1 = 9,
	NETDVR_PROTOCOLTYPE_PelcoP5 = 10,
	NETDVR_PROTOCOLTYPE_Philips = 11,
	NETDVR_PROTOCOLTYPE_Sanli = 12,
	NETDVR_PROTOCOLTYPE_Santachi =13,
	NETDVR_PROTOCOLTYPE_Sharp = 14,
	NETDVR_PROTOCOLTYPE_Sony = 15,
	NETDVR_PROTOCOLTYPE_Yaan = 16, 
};

typedef struct  
{
	u8 maxNum;
	u8 list[19]; //��̨Э���б�
} ifly_PTZProtocolList_t;

typedef struct  
{
	u8 mdsenselist[12]; //�ƶ�����������б�
}ifly_MDSenselist_t;
 
typedef struct  
{
	u16 mdalarmdelaylist[16]; //�ƶ���ⱨ�������ʱ�б�
}ifly_MDAlarmDelaylist_t;
 
typedef struct  
{
	int baudratelist[16]; //�������б�
}ifly_BaudRateList_t;

#define CTRL_CMD_PPPOE_PREUP			CTRL_CMD_BASE+9999
#define CTRL_CMD_PPPOE_UP				CTRL_CMD_BASE+9998
#define CTRL_CMD_PPPOE_DOWN				CTRL_CMD_BASE+9997
#define CTRL_CMD_PPPOE_DISCONNECT		CTRL_CMD_BASE+9996

#define CTRL_CMD_DHCP_DECONFIG			CTRL_CMD_BASE+9989
#define CTRL_CMD_DHCP_BOUND				CTRL_CMD_BASE+9988
#define CTRL_CMD_DHCP_RENEW				CTRL_CMD_BASE+9987
#define CTRL_CMD_DHCP_NAK				CTRL_CMD_BASE+9986




//�¼�֪ͨ����
#define CTRL_NOTIFY_BASE				20000
#define	CTRL_NOTIFY_CONNLOST			CTRL_NOTIFY_BASE+0		//������Ϣ
#define	CTRL_NOTIFY_HEARTBEAT_REQ		CTRL_NOTIFY_BASE+1		//��������
#define	CTRL_NOTIFY_HEARTBEAT_RESP		CTRL_NOTIFY_BASE+2		//�����ظ�
#define CTRL_NOTIFY_PLAYEND				CTRL_NOTIFY_BASE+3		//�������
#define CTRL_NOTIFY_PLAYPROGRESS		CTRL_NOTIFY_BASE+4		//�������֪ͨ
#define CTRL_NOTIFY_HASAUDIO			CTRL_NOTIFY_BASE+5		//�ط��ļ��Ƿ�����Ƶ
#define CTRL_NOTIFY_UPDATEPROGRESS		CTRL_NOTIFY_BASE+6		//��������
#define CTRL_NOTIFY_ALARMINFO			CTRL_NOTIFY_BASE+7		//�첽������Ϣ

//Ӧ������
#define CTRL_SUCCESS					0						//�ɹ�
#define CTRL_FAILED_BASE				30000					//������ƫ����
#define CTRL_FAILED_USER				CTRL_FAILED_BASE+1		//�����ڵ��û���
#define CTRL_FAILED_PASSWORD			CTRL_FAILED_BASE+2		//�������
#define CTRL_FAILED_COMMAND				CTRL_FAILED_BASE+3		//δ�Ͽɵ�����
#define CTRL_FAILED_PARAM				CTRL_FAILED_BASE+4		//��Ч����
#define CTRL_FAILED_OUTOFMEMORY			CTRL_FAILED_BASE+5		//�ڴ治��
#define CTRL_FAILED_RESOURCE			CTRL_FAILED_BASE+6		//��Դ����
#define CTRL_FAILED_FILENOTEXIST		CTRL_FAILED_BASE+7		//�ļ�������
#define CTRL_FAILED_DATABASE			CTRL_FAILED_BASE+8		//���ݿ����
#define CTRL_FAILED_RELOGIN				CTRL_FAILED_BASE+9		//�ظ���¼
#define CTRL_FAILED_BAUDLIMIT			CTRL_FAILED_BASE+10		//ÿһ·ͨ�����֧��5·ʵʱ���			
#define CTRL_FAILED_CREATESOCKET		CTRL_FAILED_BASE+11		//�����׽���ʧ��
#define CTRL_FAILED_CONNECT				CTRL_FAILED_BASE+12		//��������ʧ��
#define CTRL_FAILED_BIND				CTRL_FAILED_BASE+13		//��ʧ��
#define CTRL_FAILED_LISTEN				CTRL_FAILED_BASE+14		//����ʧ��
#define CTRL_FAILED_NETSND				CTRL_FAILED_BASE+15		//���緢�ͳ���
#define CTRL_FAILED_NETRCV				CTRL_FAILED_BASE+16		//������ճ���
#define CTRL_FAILED_TIMEOUT				CTRL_FAILED_BASE+17		//�������ӳ�ʱ
#define CTRL_FAILED_CHNERROR			CTRL_FAILED_BASE+18		//����ͨ������
#define CTRL_FAILED_DEVICEBUSY			CTRL_FAILED_BASE+19		//�豸����æ
#define CTRL_FAILED_WRITEFLASH			CTRL_FAILED_BASE+20		//��дflash����
#define CTRL_FAILED_VERIFY				CTRL_FAILED_BASE+21		//У���
#define CTRL_FAILED_CONFLICT			CTRL_FAILED_BASE+22		//ϵͳ��Դ��ͻ
#define CTRL_FAILED_BUSY				CTRL_FAILED_BASE+23		//ϵͳ����æ
#define CTRL_FAILED_LINKLIMIT			CTRL_FAILED_BASE+24		//�Ѵﵽ��������
#define CTRL_FAILED_USER_SAME			CTRL_FAILED_BASE+25		//�û�����ͬ07-08-02
#define CTRL_FAILED_MACADDR				CTRL_FAILED_BASE+26		//Զ�̷��ʵ�pc mac��ַ����
#define CTRL_FAILED_NOINIT				CTRL_FAILED_BASE+27		//ģ��δ��ʼ��
#define CTRL_FAILED_USER_MAX			CTRL_FAILED_BASE+28		//�û������//wrchen 080529
#define CTRL_FAILED_UNKNOWN				CTRL_FAILED_BASE+9999	//δ֪����
//����
#define CTRL_CONNECTION_NULL			0x0
#define CTRL_CONNECTION_TCPCLIENT		0x1
#define CTRL_CONNECTION_TCPSERVER		0x2

//�鲥�����豸
#define CTRL_DEVICESEARCH_ACKCLIENT		0X1
#define CTRL_DEVICESEARCH_ACKSERVER		0x2
#define SEARCHPORT						6666
#define ACKSEARCHPORT					SEARCHPORT+1
#define MULTICASTGROUP					"224.0.1.2"

//��������
#define VIDEO_CHANNELS			16
#define AUDIO_CHANNELS			16
#define EACH_STREAM_TCP_LINKS	5

#define MAX_REMOTE_PLAYER_NUM	4//8//csp modify 20121018

#define VOIP_NUM				1
#define DOWNLOAD_NUM			1
#define UPDATE_NUM				1
#define MEDIA_LINKMAX_SVR		(VIDEO_CHANNELS+AUDIO_CHANNELS)*(EACH_STREAM_TCP_LINKS)+MAX_REMOTE_PLAYER_NUM+VOIP_NUM
#define STREAM_LINK_MAXNUM		MEDIA_LINKMAX_SVR+DOWNLOAD_NUM+UPDATE_NUM

//2010/01/06/
//   ***************************************************************
//˵��
//�ָ�Ĭ�ϻ������
//�ͻ��˷���CTRL_CMD_RESETPICADJUST+ifly_reset_picadjust_t;

//���֡���б�
//�ͻ��˷���CTRL_CMD_GETFRAMERATELIST+chn��1�ֽڣ�+type��1�ֽڣ����豸�˷���ifly_framerate_list_t

//���֧�ֵ�����ڸ���
//�ͻ��˷���CTRL_CMD_GETMAX_IMGMASKNUM���豸�˷�����Ŀ��1�ֽڣ�ȡֵ <=4��

//�����/�������ֱ����б�
//�ͻ���CTRL_GET_RESOLUTION_LIST +chn��1�ֽڣ�+type��1�ֽڣ����豸����ifly_videoresolu_list_t

//�ָ�Ĭ�ϻ��������CTRL_CMD_RESETPICADJUST
typedef struct  
{
	u8 chn;  //ͨ�� 0��ʼ
	u8 adjtype; //��λ��ʾ���ͣ���1-4λ�ֱ��ʾ���ȣ��Աȶȣ�ɫ�������Ͷȣ���Ϊ1�ı�ʾ�ָ�Ĭ��ֵ�� 0xf��ʾȫ�ָ�
}ifly_reset_picadjust_t;

//֡���б� CTRL_CMD_GETFRAMERATELIST
typedef struct
{
	u8 chn; //ͨ��
	u8 type; // 0 ������ 1 ������
	u8 framerate[10]; //֡���б� δʹ�õ���0
}ifly_framerate_list_t;

//��/�������ֱ����б� CTRL_GET_RESOLUTION_LIST
#define VIDEORESOLU_BEGIN 1
#define VIDEORESOLU_QCIF 1
#define VIDEORESOLU_CIF 2
#define VIDEORESOLU_HD1 3
#define VIDEORESOLU_D1 4 // 20111014 by lanston, org:3
#define VIDEORESOLU_720P 5 // 20120502 by yzw
#define VIDEORESOLU_1080P 6
#define VIDEORESOLU_960H 7
#define VIDEORESOLU_END VIDEORESOLU_960H

typedef struct  
{
	u8 chn; //ͨ��
	u8 type; // 0 ������ 1 ������
	u8 videoresolu[8]; //�ֱ����б� δʹ�õ���0 VIDEORESOLU_BEGIN ~ VIDEORESOLU_END
} PACK_NO_PADDING ifly_videoresolu_list_t;


//  ********************************************************************
//#pragma pack( push, 1 )

typedef struct
{
	u8 bUseUTC;
	u8 reserved[16]; //Ԥ��
}PACK_NO_PADDING ifly_utctime_t;//�Ƿ�ʹ��UTC��ʽʱ�� 


//¼�񲼷�

typedef struct
{
	u8	data[6];
}PACK_NO_PADDING CPGuid;

typedef struct
{
	u32         ip;
	u16			port;
	CPGuid		guid;
	SOCKHANDLE	sockfd;
	u8			conntype;
	u8          newmsgcome;
	u8          nolnkcount;
}PACK_NO_PADDING ifly_cp_t,*CPHandle;



typedef struct
{
	u32		length;						//��Ϣ����		
	u16		type;						//��Ϣ����		
	u16		event;						//��Ϣ��		
	u16		number;						//��ˮ��		
	u16		version;					//�汾��		
}PACK_NO_PADDING ifly_cp_header_t;

//Э��ͷ�ṹ
typedef struct 
{
	u8	safeguid[16];					// PROTOGUID
	u8  protocolver;					//Э��汾
	u8	byConnType;						//�������ͣ�0x1���������0x2���������䣻0x3���㲥������0x4  ��Ѳͬ��(ֻ���Ѿ�ʹ����Ѳ���豸��Ч)
	u8	reserved[2];					//�����ֶ�
}PACK_NO_PADDING ifly_ProtocolHead_t;


//�豸��Ϣ
typedef struct 
{
	u32	deviceIP; 						//�豸IP  
	u16	devicePort;						//�豸�˿� 
	char device_name[32];				//�豸����
	char device_mode[32];				//�豸�ͺ�
	u8	maxChnNum;						//���ͨ����
	u8	maxAduioNum;					//�����Ƶ��
	u8	maxSubstreamNum;				//�����������
	u8	maxPlaybackNum;					//���ط���
	u8	maxAlarmInNum;					//��󱨾�������
	u8	maxAlarmOutNum;					//��󱨾������
	u8	maxHddNum;						//���Ӳ����
	u8 	nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	u8	reserved[15];					//Ԥ��
}PACK_NO_PADDING ifly_DeviceInfo_t;


//��Ƶ����
typedef struct  
{
	u8	videoEncType;					//��Ƶ��������
	u16	max_videowidth;					//
	u16	max_videoheight;					
	u8	reserved[3];					//Ԥ��
}PACK_NO_PADDING ifly_Video_Property_t;

//��Ƶ����
typedef struct  
{
	u8	audioEnctype;					//Ԥ����Ƶ��������
	u8	audioBitPerSample;				//Ԥ����Ƶλ��
	u16 audioSamplePerSec;				//Ԥ����Ƶ������
	u16 audioFrameSize;					//Ԥ����Ƶÿ֡��С
	u16	audioFrameDurTime;				//Ԥ����Ƶÿ֡���
	u8	reserved[4];					//Ԥ��
}PACK_NO_PADDING ifly_Audio_Property_t;

//VOIP����
typedef struct  
{
	u8	VOIPBitPerSample;				//�����Խ�λ��
	u16 VOIPSamplePerSec;				//�����Խ�������
	u16 VOIPFrameSize;					//�����Խ�ÿ֡��С
	u8	reserved[3];					//Ԥ��
}PACK_NO_PADDING ifly_VOIP_Property_t;

//�ƶ��������
typedef struct  
{
	u8	MDCol;							//�ƶ��������
	u8	MDRow;							//�ƶ��������
	u8	reserved[4];					//Ԥ��
}PACK_NO_PADDING ifly_MD_Property_t;

#ifdef _TOUMING_COM_
//͸�����ڷ�������ͷ
typedef struct  
{
	u8	channel; //����ͨ����ʹ��485����ʱ��Ч����1��ʼ��232������Ϊ͸��ͨ��ʱ��ֵ����Ϊ0
	u32 	dwDataSize; //���͵����ݳ���
	u8 	reserved[16]; //Ԥ��
}PACK_NO_PADDING ifly_serial_hdr;

typedef struct
{
	u8 com_protocol; //Э���� 0:N5,1:D4,2:S8,3:S7,4:F2,5:G2E,6:G2N,7:CR,8:TY,9:YH
	u32 com_baudrate; //������
	u8 com_checkbit; //У��λ 0:NONE,1:��У�飬2:żУ��
	u8 com_databit; //����λ 8��7��6
	u8 com_stopbit; //ֹͣλ 1��2
	u8 serialport;// 1-232, 2-485
	u8 reserved[23]; //����
}ifly_com_param_t;
#endif

//������������ṹ
typedef struct 
{
	u8 command;							//0��Ԥ�� 1���ļ��ط� 2��ʱ��ط� 3���ļ����� 4������ 5 VOIP
	union
	{
		struct
		{
			u8		chn;				//Ԥ��ͨ��
			u8		type;				//0:��������Ƶ 1����������Ƶ 2����������Ƶ
		}PACK_NO_PADDING Monitor_t;						//Ԥ�� command = 0
		
		struct
		{
			char	filename[64];		//�طŵ��ļ�
			u32		offset;				//�ļ�ƫ��
		}PACK_NO_PADDING FilePlayBack_t; 				//�ļ��ط� command = 1
		
		struct
		{
			u8		channel;			//ͨ����
			u16		type;				//����
			u32		start_time;			//��ʼʱ��
			u32		end_time;			//��ֹʱ��
		}PACK_NO_PADDING TimePlayBack_t ;				//ʱ��ط� command = 2
		
		struct
		{
			char	filename[64];		//���ص��ļ�
			u32		offset;				//�ļ�ƫ��
			u32		size;				//�ļ���С
		}PACK_NO_PADDING FileDownLoad_t;				//�ļ����� command = 3
		
		struct
		{
			u32		size;				//�ļ�����
			u32		verify;				//У���
			u16		version;			//�ļ��汾��
			u16		updateType;			//0:�������� 1���������, 2: ����LOGO, 3: APP LOGO
		}PACK_NO_PADDING Update_t;						//Զ������  command = 4��
		
		//VOIP command
		u8 voipreserved;				//VOIPԤ��
	#ifdef _TOUMING_COM_
		//͸��ͨ��
		u8 SerialPort; //͸��ͨ�� 1-232����  2-485���� 
	#endif
		
		//Զ�̸�ʽ��Ӳ��
		u8 formatHddIndex; //Ҫ��ʽ����Ӳ�̺� 0��ʼ
		struct
		{
			u16 chnMask; //ͨ���� ��λ
			u16 type; //����
			u32 start_time; //��ʼʱ��
			u32 end_time; //��ֹʱ��
		}PACK_NO_PADDING MultiTimePlayBack_t ; //��·ʱ��ط� command = 11
	};
	u8	reserved[7];					//Ԥ��	
}PACK_NO_PADDING ifly_TCP_Stream_Req;					 


//�������������˻�Ӧ�ṹ
typedef struct
{
	u32	errcode;						//���ӳɹ�����0�� ���򷵻�����������
	u32	ackid;							//�����ļ��طţ�ʱ��طš� ���ں����ͻ��˿��ƻطŲ��������ֽ���, Ԥ������
}PACK_NO_PADDING ifly_TCP_Stream_Ack;

typedef struct  
{
	u16 errcode;
	u8 pos;
}PACK_NO_PADDING ifly_TCP_Pos;

//Ԥ�����طź� VOIP֡�ṹ
typedef struct
{
	u8	m_byMediaType;					//����
	u8	m_byFrameRate;					//֡��
	u8	m_bKeyFrame;					//�Ƿ�ؼ�֡
	int m_nVideoWidth;					//��
	int m_nVideoHeight;					//��
	u32 m_dwDataSize;					//���ݴ�С
	u32 m_dwFrameID;					//֡ID
	u32 m_dwTimeStamp;					//ʱ���
}PACK_NO_PADDING ifly_MediaFRAMEHDR_t;

//��¼
typedef struct
{
	char	username[12];				//�û���
	char	loginpass[12];				//����
	char	macAddr[18];				//MAC��ַ		
	u32		ipAddr;						//IP��ַ
}PACK_NO_PADDING ifly_loginpara_t;

//�������
typedef struct
{
	char	mac_address[18];			//mac��ַ
	u32		ip_address;					//ip��ַ
	u16		server_port;				//�豸�˿�
	u32		net_mask;					//����
	u32		net_gateway;				//����
	u32		dns;						//dns
	u8		flag_multicast;				//�鲥���ñ��
	u32		multicast_address;			//�鲥��ַ
	u16		http_port;					//http�˿�
	u8		flag_pppoe;					//pppoe���ñ��
	char	pppoe_user_name[64];		//pppoe�û���
	char	pppoe_passwd[64];			//pppoe����
	u8      flag_dhcp;					//dhcp���ñ�־
	u8		ddnsserver;					//ddns������
	u8		flag_ddns;					//ddns���ñ�־
	char	ddns_domain[64];			//ddns����
	char	ddns_user[64];				//ddns�û���
	char	ddns_passwd[64];			//ddns����
	u32		centerhost_address;			//���ķ�������ַ
	u16		centerhost_port;			//���ķ������˿�
	u16		mobile_port;				//�ֻ���ض˿�
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_NetWork_t;

//�ֱ������֧��16�ֲ�֧�ֵ�����0
typedef struct
{
	u16		width;						//�ֱ��ʿ�
	u16		height;						//�ֱ��ʸ�
	u8		flashrate;					//�ֱ���ˢ����
}PACK_NO_PADDING ifly_VGA_Pro;

typedef struct 
{
	ifly_VGA_Pro vgapro[16];
}PACK_NO_PADDING ifly_VGA_Solution;

typedef struct 
{
	char DDNSlist[20][20];
}PACK_NO_PADDING ifly_DDNSList_t;

//csp modify 20130423
typedef struct
{
	u32	id;
	u8	chn;
	u8	wnd_num;
	u8	wnd_index;
	u8	reserved[9];
}PACK_NO_PADDING ifly_wndinfo_t;

//csp modify 20130519
typedef struct
{
	char	username[12];				//�û���
	char	reserved1[4];
	u64		nRemoteView[1];				//Զ��Ԥ��Ȩ��
	u64		reserved2[29];
}PACK_NO_PADDING ifly_AdvPrivilege_t;

//ϵͳ����
typedef struct
{
	u16		device_id;					//�豸id
	char	device_name[32];			//�豸����
	u8		flag_overwrite;				//Ӳ����ʱ���Ǳ��
	u16		lock_time;					//��������ʱ��
	u16		switch_time;				//�л�ʱ��
	u8		flag_statusdisp;			//״̬��ʾ���
	u8		video_format;				//��Ƶ��ʽ
	u8		vga_solution;				//VGA�ֱ��� ifly_VGA_Solution[16];
	u8		transparency;				//�˵�͸����
	u8		language;					//ϵͳ����
	u8		disable_pw;					//�������
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_SysParam_t;

//¼�����
typedef struct									
{
	u8		channelno;					//ͨ����
	u8		code_type;					//��������
	u8		bit_type;					//λ������
	u32		bit_max;					//λ������
	u16		intraRate;					//�ؼ�֡���
	u8		qi;							//�ؼ�֡��������
	u8		minQ;						//��С��������
	u8		maxQ;						//�����������
	u8		quality;					//ͼ������
	u8		frame_rate;					//��Ƶ֡��
	u16		pre_record;					//Ԥ¼ʱ��
	u16		post_record;				//¼����ʱ
	u32		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
 #ifdef IECTRL_NEW
	u8 		supportdeinter;    //�Ƿ�֧��deinter���� 1�� 0�� (����)
	u8  		deinterval;     //deinterǿ�� 0-4 ���ã������У�ǿ����ǿ
 	u8  		supportResolu;    //�Ƿ�֧������¼��ֱ���
 	u8  		resolutionpos;    //�ֱ���ѡ��ֵ
 	u8		reserved[12];				//�����ֶ�
 #endif

}PACK_NO_PADDING ifly_RecordParam_t;

//����������
typedef struct									
{
	u8		channelno;					//ͨ����
	u8		sub_flag;					//���ֶ������������� 0 cif 1 qcif
	u8		sub_bit_type;				//������λ������
	u16		sub_intraRate;				//�ؼ�֡���
	u8		sub_qi;						//�ؼ�֡��������
	u8		sub_minQ;					//��С��������
	u8		sub_maxQ;					//�����������
	u8		sub_quality;				//������ͼ������
	u8 		sub_framerate;				//��������֡��
	int 	sub_bitrate;				//��������λ��
	u32		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_SubStreamParam_t;

//ͼ�����
typedef struct
{
	u8		channel_no;					//ͨ����
	char	channelname[32];			//ͨ������������UTF-8��ʽ
	u8		flag_name;					//����λ����ʾ
	u16		chnpos_x;					//����x����
	u16		chnpos_y;					//����y����
	u8		flag_time;					//ʱ��λ����ʾ
	u16		timepos_x;					//ʱ��x����
	u16		timepos_y;					//ʱ��y����
	u8		flag_mask;					//�ڸ�
	struct  Mask_t
	{
		u16	 	x;
		u16		y;
		u16		width;
		u16		height;
	}PACK_NO_PADDING MaskInfo[4];
	u8		flag_safechn;				//��ȫͨ�����
	u32		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_ImgParam_t;

//�������
typedef	struct  
{
	u8		channel_no;					//ͨ����
	u8		flag;						//���ڱ�־:0-3
	u8		val;						//����ֵ
	u32		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
}PACK_NO_PADDING ifly_PicAdjust_t;


typedef struct _AlarmPtzDeal
{
	u8		flag_preset;			//Ԥ�õ�
	u8		preset;
	u8		flag_cruise;			//Ѳ����
	u8		cruise;
	u8		flag_track;				//�켣
} PACK_NO_PADDING AlarmPtzDeal_t;

//�����������
typedef struct									
{
	u8		inid;						//���������� 
	u8		typein;						//������������
	u8		flag_deal;					//�Ƿ񴥷��������봦��
	u32		triRecChn;					//����¼��ͨ����ÿһλһͨ��
	u32		triAlarmoutid;				//���������������λ
	u8		flag_buzz;					//����������
	u8		flag_email;					//����emaill
	u8		flag_mobile;				//�����ֻ�����
	u16		delay;						//���������ʱ
	AlarmPtzDeal_t AlarmInPtz [32];
	u32		copy2AlarmInmask;			//���Ƶ������������롣��λ
	u8		flag_enable;				//�������ñ�־
	u8		flag_showfullscreen;		//�����Ŵ��־
	u8		reserved[14];				//�����ֶ�
}PACK_NO_PADDING ifly_AlarmInParam_t;

//�����������
typedef struct									
{
	u8		outid;						//��������� 
	u8		typeout;					//�����������
	u32		copy2AlarmOutmask;			//���Ƶ����������������λ
	u16		alarmoutdelay;				//���������ʱ
	u8		flag_buzz;					//����������
	u16		buzzdelay;					//��������ʱ
	u8		reserved[11];
}PACK_NO_PADDING ifly_AlarmOutParam_t;

//����֪ͨ����: email�����ֻ�����
typedef struct									
{

	char	alarm_email[32];			//����email��ַ
	char	alarm_mobile[32];			//�����ֻ���ַ
	u8		reserved[32];				//�����ֶ�
}PACK_NO_PADDING ifly_AlarmNoticeParam_t;

//����EMail SMTP��Ϣ							EMAIL_OPT
typedef struct  
{
 char	alarm_email1[32];	//��������1
 char	alarm_email2[32];	//��������2
 char	alarm_email3[32];	//��������3
 char 	SMTP_svr[32];  		//emali��SMTP������
 char 	username[32];  		//�����û���
 char 	userpw[32];   		//��������
 u16  smtp_port;			//SMTP�˿�pw 2010/11/18
 u8  reserved[13];  		//Ԥ��
} PACK_NO_PADDING ifly_AlarmEmail_SMTP_t;

//¼�񲼷�
typedef struct
{
	u8		chn;						//ͨ��
	u8		weekday;					//����
	struct RecTimeField_t
	{
		u32 starttime;					//��ʼʱ��
		u32 endtime;					//��ֹʱ��
		u8	flag_sch;					//��ʱ¼��
		u8 	flag_md;					//�ƶ����¼��
		u8  flag_alarm;					//����¼��
		u8	reserved[4];				//�����ֶ�
	}PACK_NO_PADDING TimeFiled[4];
	u8		copy2Weekday;				//���Ƶ�������  ��λ 
	u32		copy2Chnmask;				//���Ƶ�����ͨ������λ
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_RecordSCH_t;

//�ƶ����
typedef struct
{
	u8 		chn;						//ͨ��
	u32 	trigRecChn;					//����¼��ͨ�� ��λ
	u32 	trigAlarmOut;				//����������� ��λ
	u8 		flag_buzz;					//������
	u8		flag_email;					//����emaill
	u8		flag_mobile;				//�����ֻ�����
	u16 	delay;						//��ʱ
	u8		sense;						//������
	u8		block[44*36];				//����
	AlarmPtzDeal_t AlarmInPtz[32];		//��̨����
	u32		copy2Chnmask;				//���Ƶ�����ͨ������λ
	u8		flag_enable;				//�������ñ�־
	u8		flag_showfullscreen;		//�����Ŵ��־
	u8		reserved[10];				//�����ֶ�
} PACK_NO_PADDING ifly_MDParam_t;

//��Ƶ��ʧ
typedef struct
{
	u8 		chn;						//ͨ��
	u32 	trigRecChn;					//����¼��ͨ�� ��λ
	u32 	trigAlarmOut;				//����������� ��λ
	u8 		flag_buzz;					//������
	u8		flag_email;					//����emaill
	u8		flag_mobile;				//�����ֻ�����
	u16 	delay;						//��ʱ
	AlarmPtzDeal_t AlarmInPtz [32];		//��̨����
	u32		copy2Chnmask;				//���Ƶ�����ͨ������λ
	u8		flag_showfullscreen;		//�����Ŵ��־
	u8		reserved[11];				//�����ֶ�
} PACK_NO_PADDING ifly_VideoLostParam_t;


//��Ƶ�ڵ�
typedef struct
{
	u8 		chn;						//ͨ��
	u32 	trigRecChn;					//����¼��ͨ�� ��λ
	u32 	trigAlarmOut;				//����������� ��λ
	u8 		flag_buzz;					//������
	u8		flag_email;					//����emaill
	u8		flag_mobile;				//�����ֻ�����
	u16 	delay;						//��ʱ
	u8		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ifly_VideoBlockParam_t;

//��̨����
typedef struct
{
	u8		chn;						//ͨ��
	u16		address;					//��������ַ
	u8		baud_ratesel;				//������ ѡ��ֵ
	u8		data_bitsel;				//����λ ѡ��ֵ
	u8		stop_bitsel;				//ֹͣλ ѡ��ֵ
	u8		crccheck;					//У��
	u8		flow_control;				//����
	u8		protocol;					//Э������
	u32		copy2Chnmask;				//���Ƶ�����ͨ�� ��λ	
	u8		comstyle;					//��������(232 or 485)
	u8		enableptz;					//ͨ����̨(����)ʹ��
}PACK_NO_PADDING ifly_PTZParam_t;

//Ԥ�õ����
typedef struct
{
	u8		chn;						//ͨ��
	u8		presetpoint;				//Ԥ�õ��
	u8		option;						//���� 0��� 1ɾ�� 2 ��Ԥ�õ�
}PACK_NO_PADDING ifly_PtzPresetCtr_t;

//Ѳ��·������
typedef struct
{
	u8		chn;						//ͨ��
	u8		cruise_path;				//Ѳ��·��
	struct Cruise_point_t
	{
		u8  preset;						//Ԥ�õ�
		u8  weeltime;					//ͣ��ʱ��
		u8  rate;						//�ٶ�
		u8  flag_add;					//0ɾ��  1���
	}PACK_NO_PADDING Cruise_point[CRUISE_POINT_NUM];
}PACK_NO_PADDING ifly_PtzCruisePathParam_t;

//Ѳ��·������
typedef struct
{
	u8		chn;						//ͨ��
	u8		cruisepath;					//Ѳ��·����
	u8		flagoption;					//���� 0ֹͣ 1��ʼ 
}PACK_NO_PADDING ifly_CruisePathCtr_t;

//�켣����
typedef struct
{
	u8		chn;						//ͨ��
	u8		flagoption;					//���� 0��¼ 1ֹͣ��¼  2��ʼ�켣 3ֹͣ�켣 
}PACK_NO_PADDING ifly_TrackCtr_t;

//Ӳ�̹���
typedef struct 
{
	u8		hdd_index;					//Ӳ�����
	u8		hdd_exist;  				//1 exist; 0 not exist
	u32		capability;					//MB
	u32		freesize;					//MB
	u8		reserved[2];				//Ԥ��
}PACK_NO_PADDING  ifly_hddInfo_t;

//�û���Ϣ
typedef struct
{
	char	name[12];					//�û���
	char	passwd[12];					//����
	char	macaddr[18];				//�����ַ
	u8		rcamer;						//Զ����̨Ȩ��
	u8		rrec;						//Զ��¼��Ȩ��
	u8		rplay;						//Զ�̻ط�Ȩ��
	u8		rsetpara;					//Զ�����ò���
	u8		rlog;						//Զ����־Ȩ��
	u8		rtool;						//Զ�̹���Ȩ��
	u8		rpreview;					//Զ��Ԥ��Ȩ��
	u8		ralarm;						//Զ�̱���Ȩ��
	u8		rvoip;						//�����Խ�Ȩ��
	u8		lcamer;						//������̨Ȩ��
	u8		lrec;						//����¼��Ȩ��
	u8		lplay;						//���ػط�Ȩ��
	u8		lsetpara;					//�������ò���
	u8		llog;						//������־Ȩ��
	u8		ltool;						//���ع���Ȩ��
}PACK_NO_PADDING ifly_user_t;

typedef struct
{
	ifly_user_t	userNum[8];		//���8 ���û�
}PACK_NO_PADDING ifly_userNumber_t;

typedef struct
{
	ifly_user_t	userInfo;		//�û���Ϣ
	u8		flagOption;					//0����û� 1�༭�û� 2ɾ���û�
}PACK_NO_PADDING ifly_userMgr_t;

//ϵͳʱ��
typedef struct
{
	u32		systemtime;					//ϵͳʱ��
	u8		format;						//ʱ���ʽ ѡ��ֵ
	u8		flag_time;					//Ԥ��ʱ��λ����ʾ
	u16		timepos_x;					//Ԥ��ʱ��x����
	u16		timepos_y;					//Ԥ��ʱ��y����
}PACK_NO_PADDING ifly_sysTime_t;

//ϵͳ��Ϣ
typedef struct
{
	char	devicename[32];				//�豸��
	char	devicemodel[32];			//�豸�ͺ�
	char	deviceser[32];				//�豸���к�
	char	version[64];				//����汾
}PACK_NO_PADDING ifly_sysinfo_t;

//��̨����
//��������:
//0-ֹͣ 1�� 2�� 3 �� 4�� 5��ת 
//6�䱶�� 7�䱶�� 8����� 9����� 10��Ȧ�� 
//11��Ȧ�� 12�ƹ⿪ 13�ƹ�� 14��ˢ�� 15��ˢ�� 
//16 ���� 17���� 18����
typedef struct
{
	u8  chn;							//ͨ��
	u8  cmd;							
	u8	reserved;
}PACK_NO_PADDING ifly_PtzCtrl_t;

//�ֶ�¼��
typedef struct
{
	u32  chnRecState;					//ͨ���ֶ�¼��״̬ ��λ	
}PACK_NO_PADDING ifly_ManualRecord_t;

//��־����
typedef struct
{
	u8		query_mode;					//��ѯ��ʽ		
	u8		main_type;					//������		
	u8		slave_type;					//������		
	u16		max_return;					//��󷵻���		
	u16		startID;					//���شӵ�һ����¼��ʼ		
	u32		start_time;					//��ʼʱ��		
	u32		end_time;					//����ʱ��			
	u8      reserved[16];				//Ԥ��		
}PACK_NO_PADDING ifly_search_log_t;

typedef struct
{
	u32		startTime;
	u8		main_type;					//������		
	u8		slave_type;					//������	
	u8 		chn;							//ͨ���Ż����
	char	loginfo[32];				//������Ϣ		
}PACK_NO_PADDING ifly_logInfo_t;

typedef struct
{
	u16		sum;						//�ܼ�¼��		
	u16		startID;					//��ʼ�ļ�¼,��ַΪ1.���޷�������ʱ,startID��ֵΪ0		
	u16		endID;						//�����ļ�¼,��ַΪ1.��endID��startID���ʱ,��ʾ֮����һ����¼		
}PACK_NO_PADDING ifly_search_desc_t;


//xdc ��������ͷ
typedef struct
{
	u32 	protocol_type;//0 all,1 onvif, 2 i13 
	u16		max_return;					//��󷵻���	
}PACK_NO_PADDING ifly_search_ipc_t;


//����ͷ����Ϣ
typedef struct
{
	unsigned char channel_no;
	unsigned char enable;
	unsigned char stream_type;
	unsigned char trans_type;
	unsigned int protocol_type;
	unsigned int dwIp;
	unsigned short wPort;
	unsigned char force_fps;
	unsigned char frame_rate;
	char user[32];
	char pwd[32];
	char name[32];
	char uuid[64];
	char address[64];//onvifʹ��
	char ipc_type;
	char reserved2[2];
	//NVR�����ͨ����
	char max_nvr_chn;
	//Ҫ����NVR ����һ·����
	char req_nvr_chn;
	char reserved[43];
	unsigned int net_mask;
	unsigned int net_gateway;
	unsigned int dns1;
	unsigned int dns2;
}PACK_NO_PADDING ifly_ipc_info_t;
//yaogang modify 20141030
//��Ѳ����
typedef struct
{
	unsigned char nIsPatrol; //��ǰ��Ѳ�Ƿ�����(����:0=��;1=��)
	unsigned char nInterval; //��ǰ��Ѳ�л�ʱ��(����:��λ��s)
	unsigned char nPatrolMode; //��ǰ��Ѳʱ���õ�Ԥ��ģʽ(����:�ο�Mode 1 4 9 16)
	unsigned char nPatrolChnNum;//��Ѳͨ����16 32 48 64
	unsigned char nInterval_num;//��Ѳ�л�ʱ��ɹ�ѡ�������
	unsigned char nPatrolMode_num;//��ѲԤ��ģʽ�ɹ�ѡ�������
	unsigned char value[1];//�����Ѳʱ���ģʽ��ֵ��eg: 5 10 20 30 60 1 4 9 16

} ifly_patrol_para_t;

//Ԥ������
typedef struct
{
	unsigned char nPreviewMode; //��ǰ���õ�Ԥ��ģʽ(����:�ο�Mode 1 4 9 16)	
	unsigned char ModePara;//Ԥ����ʼͨ��0-15
} ifly_preview_para_t;

//�ط��ļ�����
#if 1
typedef struct
{
	u16		channel_mask;				//ͨ�� ��λ��� ��ָͨ��1-16
	u16		type_mask;					//���� ��λ���
	u32		start_time;					//��ʼʱ��
	u32		end_time;					//��ֹʱ��
	u16		startID;					//���صĵ�һ����¼,��1��ʼ
	u16		max_return;					//ÿ�η��ص�����¼��
	u16		chn17to32mask;				//ͨ��17-32 ��λ���
	u8		reserved[5];				//Ԥ��
	char	bankcardno[21];				//����
}ifly_recsearch_param_t;
#else
typedef struct
{
	u16		channel_mask;				//ͨ�� ��λ���
	u16		type_mask;					//���� ��λ���
	u32		start_time;					//��ʼʱ��
	u32		end_time;					//��ֹʱ��
	u16		startID;					//���صĵ�һ����¼,��1��ʼ
	u16		max_return;					//ÿ�η��ص�����¼��
	u8		reserved[4];				//Ԥ��
}PACK_NO_PADDING ifly_recsearch_param_t;
#endif

////Զ�̻طŷ��ص���Ϣ�ṹ
typedef struct
{
	u8		channel_no;
	u8		type;
	u32		start_time;
	u32		end_time;
	u8		image_format;				//3:cif;4:4cif
	u8		stream_flag;				//0:��Ƶ��;1:������
	u32		size;
	u32		offset;
	char	filename[64];
}PACK_NO_PADDING ifly_recfileinfo_t;

//�������֪ͨ
typedef struct
{
	u32	 id;							//�ط�id
	u32  currPos;						//��ǰ����
	u32  totallen;						//��ʱ��
}PACK_NO_PADDING ifly_progress_t;

//�ط��ļ��Ƿ�����Ƶ
typedef struct  
{
	u8 bHasAudio;					 	// 1����Ƶ��0����Ƶ
}PACK_NO_PADDING ifly_play_hasaudio_t;

//�첽������Ϣ
typedef struct
{
	u32	 time;							//��������ʱ��
	u32  alarmInfo;						//����ֵ
	u32  reserved;						//Ԥ��
}PACK_NO_PADDING ifly_alarminfo_t;

#ifdef ALARM_UPLOAD
typedef struct
{
	CPHandle g_cph;
	u8 g_state;
}PACK_NO_PADDING ifly_AlarmUploadCenter_t;


#if 1
typedef struct 
{
 u8 type;       //0-�ź�������,1-Ӳ����,2-�źŶ�ʧ,3���ƶ����,4��Ӳ��δ��ʽ��,5-��дӲ�̳���,6-�ڵ�����,7-��ʽ��ƥ��, 8-�Ƿ�����
 u8 state;       //  1���� 2�ָ�
 u8 id;        //ͨ��,Ӳ�̺�,���������,ȡ����type 
 u16 reserved1;      //Ԥ��
 u32  reserved2;      //Ԥ��
}PACK_NO_PADDING ifly_alarmstate_t;
#else
typedef struct 
{
	u8 type;
	u32 state;
	u32  reserved;						//Ԥ��
}PACK_NO_PADDING ifly_alarmstate_t;
#endif
#endif


typedef struct
{
	u8 m_byMediaType;
	u8 m_byFrameRate;
	u8 m_bKeyFrame;
	int m_wVideoWidth;
	int m_wVideoHeight;
	u32 m_dwDataSize;
	u32 m_dwFrameID;
	u32 m_dwTimeStamp;
}PACK_NO_PADDING IFLY_MediaFRAMEHDR_t;




typedef struct
{
	char	ifrname[16];
	u32		ip;
	u32		subnet;
	u32		broadcast;
	u32		gateway;
	u32		dns;
}PACK_NO_PADDING ifly_cp_dhcp_t;

/************************/
//07-09-03
typedef struct  
{
 	u16		width;
 	u16		height;
}PACK_NO_PADDING ifly_pic_resolution_t;							//�������͸��ͻ��˵�ͼ��ֱ���

typedef struct
{
 	u8		channel_no;
}PACK_NO_PADDING ifly_need_resolution_t;						//��ͨ����Ҫ��������ͼ��ֱ���
/*************************/

typedef struct 
{
	char	usrname[12];
}PACK_NO_PADDING ifly_usrname_header;

typedef struct
{
	u16		startID;
	u16		max_return;
}PACK_NO_PADDING ifly_remusr_send_t;

//pw 2010/7/9
#if 0
#ifndef MAX_CHN_NUM
#define MAX_CHN_NUM		8//16//fpga:16 6446:4
#endif
#endif

//Զ���ֶ�¼��ṹ
typedef struct
{
	//u8		chan_rec[MAX_CHN_NUM];
	u8		chan_rec[16];
	u8		allrec;
	u8		allstop;
}PACK_NO_PADDING ifly_hand_rec_t;

//Զ����־��ѯ
typedef struct
{
	u8		query_mode;							//��ѯ��ʽ
	u8		main_type;							//������
	u8		slave_type;							//������
	u16		max_return;							//��󷵻���
	u16		startID;							//���شӵ�һ����¼��ʼ
	u32		start_time;							//��ʼʱ��
	u32		end_time;							//����ʱ��	
	u8 langID;									//��ѯ�����ʾ����
}PACK_NO_PADDING ifly_research_log_t;

typedef struct
{
	u32		startTime;
	char	main[64];							//������
	char	slave[64];							//������
	char	loginfo[32];						//������Ϣ07-10-09
}PACK_NO_PADDING ifly_log_back_t;

#if 0//csp modify 20130519
typedef struct
{
	char	username[12];
	char	loginpass[12];
}PACK_NO_PADDING ifly_logininfo_t;
#endif

typedef struct
{
	char	usrname[12];
	char	remote_privilege[16];
}PACK_NO_PADDING ifly_remusr_limit;

typedef struct
{
	int		link_online;
}PACK_NO_PADDING ifly_link_online;

//Զ���û�������Ϣ�ṹ
typedef struct
{
	char	name[12];
	char	password[12];
	char	local_privilege[16];
	char	remote_privilege[16];
	char	mac_address[18];
}PACK_NO_PADDING ifly_userinfo_t;

//Զ���û��༭�����ṹ
typedef struct
{
	char	name[12];							//�û���
	char	passwd[12];							//����
	char	confirm[12];						//У������
	char	macaddr[18];						//�����ַ
	u32		index;								//�û�Ȩ��
	u32		rcamer;								//Զ����̨Ȩ��
	u32		rrec;								//Զ��¼��Ȩ��
	u32		rplay;								//Զ�̻ط�Ȩ��
	u32		rsetpara;							//Զ�����ò���
	u32		rlog;								//Զ����־Ȩ��
	u32		rtool;								//Զ�̹���Ȩ��
	u32		rpreview;							//Զ��Ԥ��Ȩ��
	u32		ralarm;								//Զ�̱���Ȩ��
	u32		rvoip;								//�����Խ�Ȩ��
	u32		lcamer;								//������̨Ȩ��
	u32		lrec;								//����¼��Ȩ��
	u32		lplay;								//���ػط�Ȩ��
	u32		lsetpara;							//�������ò���
	u32		llog;								//������־Ȩ��
	u32		ltool;								//���ع���Ȩ��
}PACK_NO_PADDING ifly_useredit_t;

//Զ���û�ɾ�������ṹ
typedef struct
{
	u32		user_index;							//��ǰɾ���û���¼
	char	name[12];							//��ǰ��ɾ���û���
}PACK_NO_PADDING ifly_userdel_t;

//Զ���û������޸Ĳ���
typedef struct 
{
	char	name[12];
	char	oldpass[12];
	char	newpass[12];
	char	confirm[12];
}PACK_NO_PADDING ifly_editpass_t;

//Զ�̷���Ӳ����Ϣ�ṹ
typedef	struct 
{
	s64		capability;//Byte
	s64		freesize;//Byte
}PACK_NO_PADDING ifly_remhdd_t;

typedef struct
{
	//u8		hdd_exist[MAX_HDD_NUM];
	u8		hdd_exist[8];
}PACK_NO_PADDING ifly_hdd_desc_t;

//Զ��ϵͳ��Ϣ���� 
typedef struct
{
	char	devicename[32];
	char	devicemodel[32];
	char	deviceser[32];
	char	version[64];
}PACK_NO_PADDING ifly_recsysinfo_t;
	
															//dong	100424
enum SYS_LANGUAGE{		
 SYS_LANGUAGE_CHS = 0,	  //��������
 SYS_LANGUAGE_ENU = 1,    //��ʽӢ��
 SYS_LANGUAGE_CHT = 2, 	//�����G��
 SYS_LANGUAGE_GER = 3,    //����
 SYS_LANGUAGE_FRE = 4,    //����
 SYS_LANGUAGE_SPA = 5,    //��������
 SYS_LANGUAGE_ITA = 6,    //�����
 SYS_LANGUAGE_POR = 7,    //��������
 SYS_LANGUAGE_RUS = 8, 	//����
 SYS_LANGUAGE_TUR = 9, 	//��������
 SYS_LANGUAGE_THA = 10,	//̩����
 SYS_LANGUAGE_JAP = 11,	//����
 SYS_LANGUAGE_HAN = 12,	//����
 SYS_LANGUAGE_POL = 13,	//������
 SYS_LANGUAGE_HEB = 14,   //ϣ������Hebrew
 SYS_LANGUAGE_HUN = 15,	//��������Hungarian
 SYS_LANGUAGE_ROM = 16,	//������Roma
 SYS_LANGUAGE_IRA = 17, //������
 SYS_LANGUAGE_CZE = 18, //�ݿ���
 SYS_LANGUAGE_VIE = 19, //Խ����
 SYS_LANGUAGE_LIT = 20, //������
 SYS_LANGUAGE_SLO = 21, //˹�工��
 SYS_LANGUAGE_ARA = 22, //��������
 SYS_LANGUAGE_INVALID = -1  //���������Чֵ��
};

typedef struct  
{
 u8 max_langnum;  //���֧�������� <=32
 u8 langlist[32]; // language list, ���32��,ÿ��ֵΪSYS_LANGUAGE(תu8)
 u8 reserved[16]; //Ԥ��
}PACK_NO_PADDING ifly_SysLangList_t;

#if 0//csp modify
//Զ��ϵͳ�����ṹ added by kmli
typedef struct
{
	u16		device_id;
	char	device_name[32];
	u8		flag_overlap;
	u32		time;
	u8		lock_time;
	u8		switch_time;
	u8		flag_statusdisp;
	u8		video_format;
	u8		vga_solution;
	u8		transparency;//07-10-16
	u8		language;//07-10-16
}PACK_NO_PADDING ifly_syspara_send_t;
#endif

//zlb20090916 begin
typedef struct {
	u8 chn;
	u8 b_email;
	u8 alarm_type;
}PACK_NO_PADDING emailalarm_send_t;

typedef struct
{
	u8 preview_mode;
	u16 buzz_delay;
	u8 reserved1[21];
	u8 reserved2[28];
	char alarm_email[MAX_EMAIL_LENGTH];
	u16 md_emailalarm;
	u16 in_emailalarm;
	u16 vl_emailalarm;
	u16 vb_emailalarm;
	u8 reserved[68 - MAX_EMAIL_LENGTH];
}PACK_NO_PADDING ifly_reserved_send_t;
//zlb20090916 end


//Զ����̨����
typedef struct
{
	u8		channel_no;
	u16		address;
	u8		baud_ratesel;
	u8		data_bitsel;
	u8		stop_bitsel;
	u8		crccheck;
	u8		flow_control;
	u8		protocol;
}PACK_NO_PADDING ifly_yuntai_send_t;

typedef struct
{
	CPGuid	guid;								//����mac��ַ
	u32		dwIp;								//����ip
	u16		wPort;								//���ؽ������ݵĶ˿�
	u8		byChn;								//ͨ����,��ַΪ0
	u8		byFlag;								//0:audio;1:video
}PACK_NO_PADDING ifly_nat_t;

//Զ�̼��
typedef struct
{
	CPGuid	guid;								//����mac��ַ
	u32		dwIp;								//����ip
	u16		wPort;								//���ؽ������ݵĶ˿�
	u8		byChn;								//ͨ����,��ַΪ0
	u8 		byFlagSubcode;						//�Ƿ���������
}PACK_NO_PADDING ifly_monitor_param_t;

//���ļ�Զ�̻ط�
typedef struct
{
	u32		dwIp;								//����ý�����ݵ�ip
	u16		wVideoPort;							//������Ƶ���ݵĶ˿�
	u16		wAudioPort;							//������Ƶ���ݵĶ˿�
	char	filename[64];						//�طŵ��ļ�
	u32		offset;								//�ļ�ƫ��
}PACK_NO_PADDING ifly_remote_playback_t;

typedef struct
{
	u32		id;
}PACK_NO_PADDING ifly_play_handle_t;

typedef struct
{
	u32		curpos;
	u32		totaltime;
}PACK_NO_PADDING ifly_play_progress_t;

typedef struct
{
	u32		dwIp;								//�����ļ����ݵ�ip
	u16		wPort;								//�����ļ����ݵĶ˿�
	char	filename[64];						//���ص��ļ�
	u32		offset;								//�ļ�ƫ��
	u32		size;								//�ļ���С
}PACK_NO_PADDING ifly_remote_download_t;

typedef struct
{
	u32		id;
}PACK_NO_PADDING ifly_download_handle_t;

/*********************/
typedef struct
{
	u32		dwIp;
	u16		wPort;
}PACK_NO_PADDING ifly_remote_layout_head;

typedef struct
{
	char	filename[64];
	u32		size;
	u16		reserved;
}PACK_NO_PADDING ifly_remote_layout_t;

typedef struct  
{
	u8		channel_no;							//ͨ����
	u8		flag_mask;							//�Ƿ���ʾ
	u8		mask_index;							//�ڸ���������		
	u32		x;									//�ڸ���������x����
	u32		y;									//�ڸ���������y����
	u32   	width;								//�ڸ�������
	u32   	height;								//�ڸ�����߶�
}PACK_NO_PADDING ifly_maskset_t;

typedef	struct  
{
	u8		channel_no;							//ͨ����
	u8		flag;								//���ڱ�־:0-3
	u8		val;								//����ֵ
}PACK_NO_PADDING ifly_picset_t;
/*********************/

typedef struct
{
	char	filename[64];						//�ļ���
	u32		size;								//�ļ�����
	u32		verify;								//У���
	u16		version;							//�ļ��汾��
	u16		reserved;
}PACK_NO_PADDING ifly_remote_upload_t;

typedef struct
{
	u32		dwIp;
	u16		wPort;
}PACK_NO_PADDING ifly_voip_t;



typedef struct
{
	u16		sum;								//�ܼ�¼��
	u16		startID;							//��ʼ�ļ�¼,��ַΪ1.���޷�������ʱ,startID��ֵΪ0
	u16		endID;								//�����ļ�¼,��ַΪ1.��endID��startID���ʱ,��ʾ֮����һ����¼
}PACK_NO_PADDING ifly_recfile_desc_t;

typedef struct
{
	char	mac_address[18];
	u32		ip_address;
	u16		server_port;
	u32		net_mask;
	u32		net_gateway;
	u32		dns;
	u32		multicast_address;
	u32		admin_host;
	u16		host_port;
	u16		http_port;
	//u16		port2;			//pw:26/05/2010
	u8		flag_pppoe;
	char	user_name[64];
	char	passwd[64];
	char	pwdcheck[64];
	u8    	flag_dhcp;//cyl commet  ��4λ��ʾ�Ƿ����öಥ..����λ��ʾDHCP
	u8		flag_ddns;
	char	ddns_domain[64];
	char	ddns_user[64];
	char	ddns_passwd[64];
}PACK_NO_PADDING ifly_network_t;

//Զ��ͼ����������ṹ
typedef struct
{
	u8		channel_no;							//ͨ����
	char	channelname[32];					//ͨ����
	//��λ����ص�
	u8		flag_name;							//����λ����ʾ
	//ʱ����ص�
	u8		flag_time;							//ʱ��λ����ʾ
	u8		flag_mask;							//�ڸ�
	u8		overlay;							//�ڵ�����
	u8		lost;								//��Ƶ��ʧ
	u8		motion;								//�ƶ����
	//����ʽ
	//char	record_channel[8];					//������¼��ͨ��
	char	record_channel[16];					//������¼��ͨ��
	u8		flag_buzz;							//��������
	u8		flag_send;							//�ϴ�����
	//char	alarm_out[2];						//�����������
	char	alarm_out[16];						//�����������
}PACK_NO_PADDING ifly_imagset_t;

//OSDλ������ 07-07-27
typedef struct 
{
	u8		channel_no;							//ͨ����
	u8		flag_name;							//����λ���Ƿ���ʾ
	u16		x;									//����λ�����Ͻ�x����
	u16		y;									//����λ�����Ͻ�y����
}PACK_NO_PADDING ifly_channelset_t;

typedef struct 
{
	u8		channel_no;							//ͨ����
	u8		flag_time;							//ʱ��λ���Ƿ���ʾ
	u16		x;									//ʱ��λ�����Ͻ�x����
	u16		y;									//ʱ��λ�����Ͻ�y����
}PACK_NO_PADDING ifly_timeset_t;

typedef struct									//�ڵ�����Ƶ��ʧ���ƶ���Ⲽ�������ṹ
{
	u8		channel_no;							//ͨ����
	u8		week;								//����
	u8		flag_alltime;						//�Ƿ�ȫ�첼��						
	u32		starttime[4];						//ʱ���1~4����ʼʱ��
	u32		endtime[4];							//ʱ���1~4����ֹʱ��
}PACK_NO_PADDING ifly_valid_t;

typedef struct									//Զ���ƶ���ⱨ����������ṹ
{
	u8		channel_no;							//ͨ����
	//u8		recchan[MAX_CHN_NUM];						//������¼��ͨ��
	u8		recchan[16];						//������¼��ͨ��
	u8		flag_buzz;							//�Ƿ���������
	u8		flag_send;							//�Ƿ��ϴ�����
	u8		alarmout;							//�Ƿ񴥷��������
	u8		alarm_out[4];						//����ĳ·�ı������
}PACK_NO_PADDING ifly_motion_t; 

typedef struct									//Զ���쳣����ҳ������ṹ
{
	u8		exceptiontype;						//�쳣����
	u8		flag_display;						//�Ƿ�������ʾ
	u8		flag_buzz;							//�Ƿ���������
	u8		flag_send;							//�Ƿ��ϴ�����
	u8		alarmout;							//�Ƿ񴥷��������
	u8		alarm_out[4];						//����ĳ·�ı������
}PACK_NO_PADDING ifly_exceptioninfo_t;

//λ���б�											//dong	100424
typedef struct  
{
 u8 chn;    //ͨ��
 u8 videotype;   //0 ������, 1 ������
 u32 bitratelist[16]; //λ���б�, ��λkb, δʹ�õ���0
 u8 reserved[16];  //Ԥ��
}PACK_NO_PADDING ifly_bitRateList_t;


typedef struct									//Զ��¼������ҳ������ṹ
{
	u8		channelno;							//ͨ����
	char	channelname[32];					//ͨ����
	u8		code_type;							//��Ƶ����
	u8		video_resolution;					//�ֱ���
	u8		bit_type;							//λ������
	u32		bit_max;							//λ������
	u8		quality;							//ͼ������
	u8		frame_rate;							//��Ƶ֡��
	u16		pre_record;							//Ԥ¼ʱ��
	u16		post_record;						//¼����ʱ
	u8		flag_record;						//�Ƿ�����¼��
	//wrchen 090210
	u8 		yfactor;							//�жϵ�ǰ��Ƶ��ʽ05-13

	u8 		sub_size;									//�������Ĵ�С qcif cif
	u8 		sub_framerate;								//��������֡��
	int 		sub_bitrate;								//��������λ��
}PACK_NO_PADDING ifly_recordinfo_t;

typedef struct									//Զ��¼������ҳ������ṹ
{
	u8		channelno;							//ͨ����
	u8		week;								//����
	u8		flag_alltime;						//�Ƿ�ȫ�첼��
	u8		alltype;							//ȫ�첼��ʱ��¼������
	u32		start_time[4];						//ʱ���1~4����ʼʱ��
	u32		end_time[4];						//ʱ���1~4����ֹʱ��
	u8		type[4];							//ʱ���1~4��¼������
}PACK_NO_PADDING ifly_recordtime_t;

typedef struct									//Զ�̱�������ҳ��
{
	u8		inid;								//���������� 
	u8		typein;								//������������
	u8		flag_deal;							//�Ƿ񴥷��������봦��
	u8		copyin;
	u8		in_copy;							//�Ƿ�ѡ�񱨾����ĸ���
	u8		outid;								//ѡ�񱨾����
	u8		typeout;							//����������� 
	u8		out_copy;							//�����������
	u8		copyout;
	u16		delay;								//���������ʱ	
}PACK_NO_PADDING ifly_alarmset_t;

struct _alarmset_in_
{
	u8		typein;								//������������
	u8		flag_deal;							//�Ƿ񴥷��������봦��
}PACK_NO_PADDING;

struct _alarmset_out_
{
	u8		typeout;							//����������� 
	u16		delay;								//���������ʱ	
}PACK_NO_PADDING;

typedef struct													//Զ�̱�������ҳ��
{
	//struct _alarmset_in_ alarmset_in[MAX_ALARM_IN_NUM];			//�����������
	struct _alarmset_in_ alarmset_in[16];			//�����������
	u8 copyin;													//�������븴��Ŀ���
	u8 in_copy;													//�Ƿ�ѡ�񱨾����ĸ���	
	//struct _alarmset_out_ alarmset_out[MAX_ALARM_OUT_NUM];		//�����������
	struct _alarmset_out_ alarmset_out[4];		//�����������
	u8 copyout;													//����������ø���Ŀ���
	u8 out_copy;												//�Ƿ�ѡ�񱨾�������ĸ���	
} PACK_NO_PADDING alarmset_t;

typedef struct									//Զ��ptz����
{
	u8		inid;								//����������
	u8		channo;								//ͨ����
	u8		flag_preset;						//Ԥ�õ�
	u8		preset;
	u8		flag_cruise;						//Ѳ����
	u8		cruise;
	u8		flag_track;							//�켣
}PACK_NO_PADDING ifly_alarmin_ptz;

typedef struct									//��������
{
	u8		id;									//ѡ�񱨾���
	u8		week;								//����	
	u8		flag_alltime;						//ȫ�첼��
	u32		starttime[4];						//ʱ���1~4����ʼʱ��
	u32		endtime[4];							//ʱ���1~4����ֹʱ��
}PACK_NO_PADDING ifly_alarm_settime;

#define MD_ROW			18
#define MD_COL			22
#define MD_MIN_SENSE	0						//�����
#define MD_MAX_SENSE	5						//������

typedef struct
{
	u8	channel_no;
	u8	flag;									//�Ƿ����ƶ����
	u8	sense;									//������ MD_MIN_SENSE-MD_MAX_SENSE
	u8	block[MD_ROW*MD_COL];					//��0ѡ�� 0��ѡ��
}PACK_NO_PADDING ifly_md_param_t;

typedef struct
{
	u8	channel_no;
	u8	byCmdId;								//����ͷ������������
	u8	abyParam[MAX_PARAM_NUM];				//����
	u8	byCtrlWay;								//����ͷ��������Ʒ�ʽ CAM_CTRLWAY_GENERAL:����;CAM_CTRLWAY_REVERSE:����
}PACK_NO_PADDING ifly_camera_ctrl_t;


//#pragma pack( pop )

typedef enum									//��������ö��
{
	PLAYRATE_1 = 1,								//��������
		PLAYRATE_2 = 2,								//2x���
		PLAYRATE_4 = 4,								//4x���
		PLAYRATE_MAX = 8,							//8x���
		PLAYRATE_1_2 = -2,							//1/2x����
		PLAYRATE_1_4 = -4,							//1/4x����
		PLAYRATE_MIN = -8,							//1/8x����
		PLAYRATE_SINGLE = 0,						//֡��
		PLAYRATE_BACK_1 = -1						//1x�˷�
}re_em_play_rate;


typedef struct
{
	SOCKHANDLE	sockfd;
	u8	bStart;
	u32 linkid;
	ifly_TCP_Stream_Req req;
	union
	{
		struct  
		{
			u32 frameid;
		}PACK_NO_PADDING Monitor_t;
		
		struct
		{
			CPHandle cph;
			u8   byFlag;//0:���ļ�����;1:��ʱ��ط�
			u8   byUse;
			u8   byStop;
			u8   byPause;
			u8   byMute;
			u8   byStatus;
			u8   bySkip;
			re_em_play_rate rate;
			u32  curPos;
			u32  totalTime;
			u32  refTime;
			u32  seekPos;
		}PACK_NO_PADDING PlayBack_t;
		
		struct
		{
			u8   byUse;
			u8   byStop;
		}PACK_NO_PADDING DonwLoad_t;
		
		struct  
		{
			CPHandle cph;
		}PACK_NO_PADDING Update_t;
	};
	
}PACK_NO_PADDING ifly_stearmsnd_t;


#ifdef SPECIAL_DEV_INFO
typedef struct
{
 u8 mainbordmode; //�����ͺ� Ԥ��
 u8 characterset; //�ַ���: 0-ascii��1-GB2312, 2-UTF8, 3-UTF16 
 u8 reserved[62]; //Ԥ��
}PACK_NO_PADDING ifly_specialinfo_t;
#endif

typedef u16 (*pMESSAGE_CB)(CPHandle cph,u16 event,u8 *pbyMsgBuf,int msgLen,u8 *pbyAckBuf,int *pAckLen,void* pContext);
void SetMsgCallBack(pMESSAGE_CB pCB,void *pContext);

//�Զ������豸
//sever�ˣ������豸��Ϣ
void SetDeviceInfo(ifly_DeviceInfo_t deviceinfo);

//client�ˣ�ע�������ص�����
typedef void (*pFindDeivce_CB)(ifly_DeviceInfo_t deviceinfo, void* pContext);
void SetFindDeviceCB(pFindDeivce_CB pCB, void *pContext);
//client�ˣ���ʼ����
int SearchDevice();

//yaogang modify 20151222
//�����������ջ�Ӧ�߳�
//return: -1 ʧ�ܣ�0 �ɹ�
int CreateAckSearchProc(pthread_t *phandle);


//sever�ˣ�ע���������ӻص�����
//�����������������ʱ���ڻص�����д���
//pAddStreamTCPLink�� �ɹ�ʱ����0������-1
typedef int (*pAddStreamTCPLink)(SOCKHANDLE hSock, ifly_TCP_Stream_Req req, void* pContext);
void SetAddStreamLinkCB(pAddStreamTCPLink pCB, void *pContext);

//����tcp����ͷ�������������������������
u16 SetSockLinkOpt(SOCKHANDLE hSock);

int SendTcpConnHead(SOCKHANDLE hSock, u8 type);
void SetCliAckPort(u16 wPort);

u16  CPLibInit(u16 wPort);
u16  CPLibCleanup();

/************************
**07-08-21 verifying connecting req from custorm
*************************/
CPHandle CurConnect_out(u32 i);
//���ӷ�����
CPHandle CPConnect(u32 dwServerIp, u16 wServerPort, u32 dwTimeOut, u16 *pwErrorCode);
//�崦����
u16  CleanCPHandle(CPHandle cph);
//�����¼�֪ͨ
u16  CPPost(CPHandle cph, u16 event, const void *content, int length);
//��������
u16  CPSend(CPHandle cph, u16 event, const void *content, int length, void* ackbuf, int ackbuflen, int *realacklen, u32 dwTimeOut);

//�õ�TCP���ӵı���ip��ַ��˿�
BOOL GetCPHandleLocalAddr(CPHandle cph, struct sockaddr_in *pAddr);

//����NAT̽���
BOOL SendSTUNPacket(u32 dwLocalIpAddr, u16 wLocalPort, u32 dwRemoteIpAddr, u16 wRemotePort, u8 *pbyBuf, u32 dwBufLen);

u16  GetTransactionNum();



/***************************************************************************************************************/
//���ͽ�������������������ؿ��ƴ�������ս��
//��������Ϊ��
//	1.client�˽�Ҫ���͵Ĳ������ض��ĸ�ʽ�����Ȼ����ñ�����
//	2.server�˽��յ�����󣬽�����Ӧ�Ĵ���
//	3.server���ݴ��������ض�Ӧ����Ϣ��client�����ر�socket ���ӣ�

//���������
//	dwHostIp��������IP��ַ
//	dwTimeOut�������ĳ�ʱʱ��
//	pBuf�����͵�����ͷ��������������Ϣ�������������У�pBuf = ifly_cp_header_t + ifly_monitor_param_t
//	dwBufLen��pBuf�ĳ���
//����ֵ��
//	0�����óɹ�
//	����������ʧ��
/***************************************************************************************************************/
u16  SendToHost(u32 dwHostIp, u32 dwTimeOut, void *pBuf, u32 dwBufLen);

/***************************************************************************************************************/
//���ͽ������������������Ҫ��������ö������Ϣ
//��������Ϊ��
//	1.client�˽�Ҫ���͵Ĳ������ض��ĸ�ʽ�����Ȼ����ñ�����
//	2.server�˽��յ�����󣬽�����Ӧ�Ĵ���
//	3.server���ݴ��������ض�Ӧ����Ϣ��client�����ر�socket ���ӣ�

//���������
//	dwHostIp��������IP��ַ
//	dwTimeOut�������ĳ�ʱʱ��
//	pinBuf�����͵�����ͷ��������������Ϣ�������������У�pinBuf = ifly_cp_header_t + ifly_monitor_param_t
//	dwinBufLen��pinBuf�ĳ���
//	dwoutBufLen��poutBuf��󳤶ȣ���ֹ�ڴ����
//���������
//	poutBuf�����������յ�����ͷ��������������Ϣ������Զ�̻طŲ�ѯ�У�poutBuf = ifly_cp_header_t + ��ѯ�Ľ��
//	dwoutBufLen��ʵ�ʽ��յ���buf�ĳ���
//����ֵ��
//	0�����óɹ�
//	����������ʧ��
//ע�⣺
//	����Ӧ���������dwoutBufLen�����Ʒ��ظ�client��������ݣ�������ܵ����������
/***************************************************************************************************************/
u16  SendToHost2(u32 dwHostIp, u32 dwTimeOut, void *pinBuf, u32 dwinBufLen, void *poutBuf, u32 *dwoutBufLen);



int check_link_stat();

//pw 2010/9/10
typedef int SOCKET;
int loopsend(SOCKET s, char *buf, unsigned int sndsize);
int looprecv(SOCKET s, char *buf, unsigned int rcvsize);
//ns : ��ʱʱ�䵥λ:�룬�������0
int loopRcvTimeoutNS(SOCKET s, char * buf, unsigned int rcvsize, int ns);



#ifdef __cplusplus
}
#endif

#endif

