#ifndef _COMMON_H_
#define _COMMON_H_

//#define PW_TEST

// 2011-05-10 spliang add tw2868 driver test
#define R9508_TW2868

//model
//#define TL_9508_8M
#define TL_9504_D1_8M
//#define TL_9504_CIF_8M
//#define TL_9508_D1_8M
//#define TL_9506_D1_8M



//#define TL_9508
//#define TL_9504_D1
//#define TL_9504_CIF

//#define TL_9508_D1
//#define TL_9508_D1_8M
//#define TL_9508_RTL
//#define TL_9504_D1_RTL
//#define TL_9504_CIF_RTL
//#define TL_9504_CIF_8M_RTL

#define CHECK_UPDATE     //����flash��֤ 
#define IECTRL_NEW  //wangcong 2011/01/06  IE�޸�ͼ��ָ��������������ֱ���ѡ�� ֡������ ��ȡ����ڸ���
#define ONLY_UNICODE     //���ֿ�
#define SHOW_TWICE
#if defined(TL_9506_D1_8M) || defined(TL_9508_D1_8M)
#else
#define VGA_RESOL_FIX//zlb20101126
#define PIC_TWINKLE_OPT
#define MSGQ_OPT
#endif

#if defined(TL_9506_D1_8M)
#define PREVIEW_6
#define CHNS_6
#define D1_8
#define TL_9508_8M
#endif


//pw 2010/9/27
#ifdef TL_9504_D1_8M
#define TL_9504_D1
#define CFG_SAVE_IN_FLASH
#endif


//pw 2010/9/6
#ifdef TL_9504_CIF_8M
#define TL_9504_CIF
#define CFG_SAVE_IN_FLASH
#endif

//pw 2010/9/17
#if defined(TL_9508_8M) || defined(TL_9508_D1_8M)
#define TL_9508
#define CFG_SAVE_IN_FLASH
#endif

#if defined(TL_9508_D1)
#define TL_9508
#endif

//pw 2010/9/7
//��������Ƶ˳��
#if defined(TL_9508_RTL)
#define TL_9508
#define CHN_ORDER
#endif

//pw 2010/10/11
#if defined(TL_9504_CIF_RTL)
#define TL_9504_CIF
#define CHN_ORDER
#endif

//pw 2010/10/11
#if defined(TL_9504_D1_RTL)
#define TL_9504_D1
#define CHN_ORDER
#endif

//pw 2010/10/13
#if defined(TL_9504_CIF_8M_RTL)
#define TL_9504_CIF_8M
#define CHN_ORDER
#endif

#if defined(TL_9508_D1_8M)
#define TL_9508_8M
#define D1_8
#endif

//pw 2010/9/13
#if defined(TL_9504_CIF) || defined(TL_9504_D1) || defined(TL_9504_CIF_8M)
#define TL_9504
#endif

#if defined(TL_9506_D1_8M)
	#define D1_CHN_NUM 6
#elif defined(TL_9508_D1_8M) || defined(TL_9508_D1)	//DDR_128M
	#define D1_CHN_NUM 8
#else
	#ifdef TL_9504_D1
		#define D1_CHN_NUM 4
	#elif defined(TL_9504_CIF)
		#define D1_CHN_NUM 1
	#else
		#define D1_CHN_NUM 2
	#endif
	#define DDR_128M
#endif


extern int BOARD_TYPE;

#define MODEL_9508			0
#define MODEL_9504_D1		1

//client
//#define ANGESHI
//#define BAOHENGAN
//#define DONGJIA
//#define GAOSIBEIER

//#define HANKE
//#define HANKE_SP1				//����ץͼ USB_REC
//#define HANKE_SP2				//�¼�������	����Ϊ�� IP:192.168.1.163	ͨ��������ɫ
//#define HANKE_SP3
//#define HANKE_SP4         	//4 IN 4 OUT
//#define HANKE_SP5         	//4 IN 1 OUT  
//#define HANKE_SP6     		//++++++++ 0 PTZ����
//#define HAOWEI           	 	
//#define HAOWEI_ZX
//#define HISHARP
//#define HUANYUWEISHI

//#define JIUAN
//#define JIUAN_2          	 	//9508
//#define JINSANJIA				//������
//#define JINGYANG				
//#define JUNMINGSHI
//#define JUNMINGSHI_SP2		//��ͬ��logo
//#define JUNMINGSHI_SP3

//#define KEAN
//#define KEKANGDA
//#define KEKANGDA_SP1
//#define KUANGSHIAN

//#define LONGANSHI				//
//#define LONGANSHI_Z			//9508
//#define LONGANSHI_SP1			//��Ӣ��
//#define LONGANSHI_SP2			//Ĭ������ΪӢ��
//#define LONGANSHI_SP3			//��·��Ƶ,Ĭ������ΪӢ��
//#define LONGANSHI_Z_SP1
//#define LONGANSHI_1A
//#define LIQIN					//����
//#define LIQIN_SP1				//�ͻ���ֻ����Ӣ������������

//#define MEIDIANBEIER_UI
//#define PANIKE

//#define SHENSHIYIN
//#define SHENSHIYIN_SP1		//IEĬ��Ԥ��ȫ��
//#define SHENSHIYIN_SP2		//SMTP�˿�����
//#define SHENGSHIYIN_VGA_600_480 //600_480�ֱ���
//#define TUMIN
//#define TAIJUN
//#define TUERKE

#define TIANMIN

//#define TIANMIN_A8008HP // 2011-05-17 add by spliang
                            // �°�:
                            // ��8·DVR���ͺ�ΪDVR-A8008HP,���������TW2868A/D���������Ҫ�������޸ģ�
							// 1������Ƶ����ӿڣ��ṩ8·��Ƶ����+8·��Ƶ���루��DVR8000HPһ�£�
							// 2��¼����Դ������8·��ʵʱD1ѹ����ÿ·���֡��Ϊ10F/S������DVR8000HPһ�£�
							// 3����������ӿڣ�ԭ8ͨ�����������Ϊ4ͨ���������롣
							// 4����尴�����壺ǰ��幦�ܰ�����Ϊ13���������������ܶ����������
							// 5�����������Լ�����������ϵͳ��
							// 6����Ʒ�ͺţ�DVR-A8008HP��


//#define TAIKANGWEIYE
//#define TAIKANGWEIYE_sp5  	//Խ����
//#define TIANWEISHIBAO
     			
//#define WEIDUOLIYA

//#define WANJIAAN

//#define XINWEITU
//#define XINQIGUOJI
//#define YINGSHITIANXIA
//#define ZHONGWEISHIJI


//yqluo 20101129
//#define PIP_PICTURE		//���л�
//#define HUMIDITY          //ʪ������
//#define ALARMSCH			//����

#if defined(TIANMIN)
#define TONGLI_DDNS
#endif

#if defined(TAIKANGWEIYE_sp5)
#define TAIKANGWEIYE
#endif

#if defined(TAIKANGWEIYE)
#define  HUMIDITY
#define PIP_PICTURE
#endif

//yqluo 20101104
#if defined(HUANYUWEISHI) 
#define CHN_ORDER
#endif

#if defined(LIQIN_SP1)
#define LIQIN
#endif

//yqluo 20101104
//#if defined(WANJIAAN)
#define CRUISE_INDEX_PRESETPOS //Ѳ��
//#endif

//yqluo 20101106
#if defined(HAOWEI_ZX)
#define HAOWEI
#endif


//pw 2010/10/25
#if defined(TL_9508) && defined(JUNMINGSHI_SP2)
#define JUNMINGSHI
#endif

//pw 2010/10/25
#if defined(TL_9504) && defined(JUNMINGSHI_SP3)
#define JUNMINGSHI
#endif

//pw 2010/10/25
#if defined(TUERKE)
#define CHN_ORDER
#endif
//pw 2010/10/12
#if defined(TUMIN)
#define USB_REC
#define D1_8
#ifdef TL_9508
#define PREVIEW_6WIN_
#define CHNS_6
#endif
#endif

#if defined(JINSANJIA) && defined (TL_9508_8M)
#define MANU_STANDARD
#endif

#if defined(TIANMIN)
#define SHENSHIYIN_SP2
//����ͼƬ
#define CUT_PNG
//VO VGA���ò���
#define Vo_Set_Param

//�չ��Լʱ�䡢ʱ��
#define _DAYTIME_

//pw 2010/9/10
#define USB_ONLY

//�ֶ���ʽ
#define MANU_STANDARD

//¼��OSD��������
#define REC_OSD

//����ץ��
#define VIDEO_SNAP

//u�̸�ʽ����ֻ������ʽ����������128m�ļ�
#define USB_FORMAT

//��·d1
#if defined(TIANMIN) && defined(TL_9508)
#define D1_8
#endif

//Ӳ��¼��ʱ��
#define REC_T
#endif

//pw 2010/7/30
#if defined(KEKANGDA_SP1)
#define KEKANGDA
#endif


//pw 2010/8/16
#if defined(HANKE_SP1) || defined(HANKE_SP2) || defined(HANKE_SP4) || defined(HANKE_SP3)  || defined(HANKE_SP5) || defined(HANKE_SP6)
#define HANKE
#endif

#ifdef HANKE_SP6
#define HANKE_SP5
#endif

#ifdef HANKE_SP1
#define VIDEO_SNAP
#define USB_REC
#endif

#ifdef HANKE_SP2
#define BACKGROUND_BLACK
#endif

//#define HOUR_24
//pw 2010/8/4
#if defined(HISHARP) || defined(TIANMIN)
#define REC_RETENTION_DAYS			//¼��������
	#ifdef HOUR_24
		#define DATA_NORMAL 100
	#endif
#endif



//PW 2010/6/10
//func module
#define MAC_STATIC  
#define FONT_BACKGROUND
#define EMAIL_OPT
#define DEFINITION_NAME
#define REC_D1
#define EFFECT_OPT

//pw 2010/9/29
#define USE_AUDIO_PCMU

//pw 2010/9/10
//#define SUB_STREAM_OPT		//�������Ż�
#ifndef SHENSHIYIN      //yqluo 20101213
#define SUB_STREAM_DANCING
#endif
#define SUB_GOP 25

//pw 2010/9/29
#if defined(HANKE) || defined(JUNMINGSHI) || defined(WANJIAAN) || defined(XINQIGUOJI) || defined(TAIKANGWEIYE) //yqluo 20101129
//�ֵı�������ģ��
#undef FONT_BACKGROUND
#endif

//videoloss flag
#ifdef HISHARP
//��ʾ��Ƶ��ʧ��Ϣ
#define SHOW_VL_FLAG

//Ӳ����Ϣ��ʾ
#define SHOW_HDD_INFO

//���뵼������ģ��
#define INPUT_OUTPUT_MODULE

//��ʾ�ͻ�������״̬
#define SHOW_CLIENT_CONN_STAT

#define NTP

#define DST	//daylight saving time

//#define BOUNCING_PICTURE
//#define BOUNCING_PICTURE_TEST
#define BOUNCING_PICTURE_MODULE
#endif

#define REC_BUG_FIX//zlb20100928

//pw 2010/7/6
#if defined(TL_9508) && defined(TIANMIN)
#define TIANMIN_VER
#define HALF_D1				1
#define ALARM_STATUS
#endif

//pw 2010/8/13
#ifdef HANKE
#define ALARM_STATUS
#endif

//pw 2010/6/5
#define FLAGEMAIL

//pw 2010/9/20
#if defined(SHENSHIYIN_SP1) //|| defined(SHENSHIYIN_SP2) || defined(SHENGSHIYIN_VGA_600_480)
#define SHENSHIYIN
#endif

#ifdef SHENGSHIYIN_VGA_600_480
#define SHENSHIYIN_SP2
#endif

//#if defined(SHENSHIYIN_SP2)
//#define SHENSHIYIN_SP1
//#endif

#ifdef SHENSHIYIN_SP1
#define REMOTE_PLAY_MULTI
#define VIDEO_SNAP
#endif

//����оƬ��Ȩ����֤                     
//#define NO_AUTH

#define HISI_3515
#ifdef HISI_3515
//#define HISI_3515_DEMO
#define HISI_3515_HD
#define FB_LESS_BUF_OPT
//#define HISI_3515_VGA_SCREEN
#define HISI_3515_HD_TDE_RESIZE

//pw 2010/7/5
#define HISI_3515_VGA_MOUSE//20100703 zlb_mouse

//2010/6/8
//#define HISI_3515_HD_TDE_RESIZE_STATE
//#define HISI_3515_VGA_SCREEN_SIMPLE
//#define HISI_3515_DEFLICKER
#endif

#ifdef MEIDIANBEIER_UI
#define MENUPIC_NUM	 6
#endif

//pw 2010/9/20
#if defined(LONGANSHI_SP3)
#define LONGANSHI_SP2
#define LONGANSHI_1A
#endif



//pw 2010/7/30
#if defined(LONGANSHI_SP1) || defined(LONGANSHI_1A) || defined(LONGANSHI_SP2)
#define LONGANSHI
#endif

#if defined(LONGANSHI_Z_SP1) 
#define LONGANSHI_Z
#endif

#if defined(LONGANSHI) ||defined(LONGANSHI_Z) 
#define LONGANSHI_DDNS
#define SHOW_DISK_PROGRSS
#define TIMED_REBOOT
#define LONGANSHI_M		//���ϵͳά��  yqluo 20101023
#endif

#if defined(LONGANSHI_DDNS) || defined(HAOWEI) // || defined(TIANMIN) debug lanston
#define CUSTOM_DDNS
#endif



//pw 2010/8/19
//#if defined(HANKE) ||defined(HAOWEI) || defined(SHENSHIYIN) || defined(JIUAN_2) || defined(XINQIGUOJI) || defined(KEAN) || defined(TIANMIN)//yqluo 20101112
#define BACKUP_AS_AVI
//#endif


#define NEW_HARDWARE
//#define WATERMARK
//#define __DEBUG
//#define DVR_TEST
//#define NO_AUDIO_TEST
#define SJJK_LANGTAO		//dong	100427

//#define MOBILE_SERVER
#define MOBILE_SERVER2  // 2011-05-10 spliang �ֻ����
#define GOP_KEY_SUPPORT // 2011-05-10 spliang �ֻ����

//pw:2010/6/12
#define SJJK_PORT
//#define NETSND_OPT//zlb20100802

#define ALARM_UPLOAD


//#define VL_VB_DEAL
#ifndef JUNMINGSHI
//#define SHOW_DISK_PROGRSS
#endif

// pw 2010/8/26 ��·�طŵ��Ź���
#define PLAY_BACKWARD_MULTI
//�Ż���ѡ��
//#define PLAYBACK_OPT

//pw 2010/8/6
#define CIF					0
#define CIF_DEFINITION_N	"352*240"
#define CIF_DEFINITION_P	"352*288"

//#define HALF_D1				1
#define HD1_DEFINITION_N	"704*240"
#define HD1_DEFINITION_P	"704*288"


#ifdef HALF_D1
#define D1					2
#else
#define D1					1
#endif
#define D1_DEFINITION_N	"704*480"
#define D1_DEFINITION_P		"704*576"


//pw 2010/06/11 ��״̬ͼ����Сͼ��
#define STATUS_PIC_HIGH 16
#define NEW_DDNS_UPDATE
#define NEW_PLAY_PROGRESS
#define SLIDER_DRAG_OPT
#define PLAY_BACKWARD_SUPPORT

#define VIDEO_BLOCK_LUMA_VALUE 0x00f00000
#define STREAM_TCP
#define AUDIO_TCP
//#define SUB_STREAM_TCP

#ifdef STREAM_TCP
#define STREAM_TCP_CHANNELS CHN_NUM_TOTAL
#define SUB_STREAM_TCP
#else
#define STREAM_TCP_CHANNELS CHN_NUM_SUB
#endif

#ifdef SUB_STREAM_TCP


#define STREAM_TCP_PORT 9008
#define EACH_STREAM_TCP_LINKS 5
#endif

#define NEW_REMOTE_PLAYBACK
#define NEW_REMOTE_UPDATE
#define NEW_REMOTE_DOWNLOAD


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#include "iflytype.h"

#ifndef WIN32
#include <unistd.h>
#include <pthread.h>
typedef pthread_t THREADHANDLE;
typedef void* (*LINUXFUNC)(void*);
#else
#include <wtypes.h>
typedef HANDLE THREADHANDLE;
#endif

#define PID_TIMER				0x10
#define PID_MAIN_CONTROL		0x20
#define PID_SYSLOG				0x30
#define PID_PANEL				0x40
#define PID_EVENT				0x50

#define THREAD_PRI_LOW			100
#define THREAD_PRI_MID			80
#define THREAD_PRI_HIGH			60

#define PRI_TIMER				THREAD_PRI_MID
#define PRI_UI					THREAD_PRI_HIGH
#define PRI_CP					THREAD_PRI_HIGH
#define PRI_CPCHECK				THREAD_PRI_LOW
#define PRI_CAPTURE				THREAD_PRI_HIGH
#define PRI_DISPLAY				THREAD_PRI_HIGH
#define PRI_ENCODE				THREAD_PRI_HIGH
#define PRI_DECODE				THREAD_PRI_HIGH
#define PRI_RECORD				THREAD_PRI_HIGH
#define PRI_MEDIASND			THREAD_PRI_HIGH
#define PRI_MEDIARCV			THREAD_PRI_HIGH
#define PRI_AUDIO				THREAD_PRI_MID
#define PRI_ALARM				THREAD_PRI_MID
#define NTP_CLIENT				THREAD_PRI_MID

#define THREAD_STKSIZE_DEFAULT	(8<<20)

#define STKSIZE_TIMER			(256<<10)
#define STKSIZE_UI				(4<<20)
#define STKSIZE_CP				(512<<10)
#define STKSIZE_CPCHECK			(256<<10)
#define STKSIZE_CAPTURE			(1<<20)
#define STKSIZE_DISPLAY			(1<<20)
#define STKSIZE_ENCODE			(1<<20)
#define STKSIZE_DECODE			(1<<20)
#define STKSIZE_RECORD			(2<<20)
#define STKSIZE_MEDIASND		(512<<10)
#define STKSIZE_MEDIARCV		(512<<10)
#define STKSIZE_AUDIO			(512<<10)
#define STKSIZE_ALARM			(256<<10)
#define STKSIZE_NTPCLIENT		(256<<10)

//#define UPDATE_TYPE_ALL 0
//#define UPDATE_TYPE_PROGRAM 1
//#define UPDATE_TYPE_FS 2
//#define UPDATE_TYPE_UIMAGE 3
//#define UPDATE_TYPE_DTB 4
//#define UPDATE_TYPE_PANEL 5
//find1
#define UPDATE_TYPE_MAINBOARD 0
#define UPDATE_TYPE_PANEL 1
#ifdef TONGLI_DDNS


typedef enum cmd_type
{
	DDNS_LOGIN = 0,
	DDNS_REGIST,
	DDNS_CANCEL,
	DDNS_HEART
}CMD_TYPE;
#endif

typedef struct _MSGQueue
{
	u32 dwReadID;
	u32 dwWriteID;
	u32 dwMsgNumber;
	u32 dwMsgLength;
}MSGQueue,* MSGQHANDLE;

typedef struct
{
	u16 sender;
	u16 event;
	u32 arg;
}ifly_msg_t;


//pw 2010/7/9
#if defined(TL_9508)
//yqluo 20101126
#ifdef CHNS_6
#define MAX_CHN_NUM			6//16//fpga:16 6446:4
#define MAX_AUDIO_CHN_NUM		4
#define SPLIT_NO					3//�ָ��ߵ�����
#else
#define MAX_CHN_NUM			8//16//fpga:16 6446:4
#define MAX_AUDIO_CHN_NUM		4
#define SPLIT_NO					3//�ָ��ߵ�����
#endif
#elif defined(TL_9504)
#define MAX_CHN_NUM		4
#define SPLIT_NO				2
#endif

#define MAX_REGION_NUM	4
#if 0
typedef struct _WndPos
{
	u32 x;/*���Ͻǵ�x����*/
	u32 y;/*���Ͻǵ�y����*/
}WndPos;

typedef struct _WndRect
{
	u32 x;/*���Ͻǵ�x����*/
	u32 y;/*���Ͻǵ�y����*/
	u32 w;/*���ο�*/
	u32 h;/*���θ�*/
}WndRect;
#endif
#define NO_WAIT			0
#define WAIT_FOREVER    -1

#ifndef WIN32
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

//pw 2010/6/10
#define PRINT(s) printf("FUNC:%s, LINE:%d, %s\n", __FUNCTION__, __LINE__, s);


#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define CONVERT_ENDIAN(x)  ((x)<<24 | (x)>>24 | ((x)&0x00FF0000)>>8 | ((x)&0x0000FF00)<<8)

#ifndef WIN32
/* Enables or disables debug output */
#ifdef __DEBUG
#define DBG(fmt, args...) fprintf(stderr, "Debug: " fmt, ## args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
#else
#define DBG
#define ERR
#endif

#define  THREADAFFMASK	1

//FTP��������
#define FTP_SUCCESS						0//�ɹ�
#define FTP_ERROR_PARAM					1//��������
#define FTP_ERROR_SERVER				2//������������
#define FTP_ERROR_FILE					3//�ļ�������
#define FTP_ERROR_USER					4//�û���������
#define FTP_ERROR_PASSWD				5//�������

#ifdef __cplusplus
extern "C" {
#endif

MSGQHANDLE IFly_CreateMsgQueue(u32 dwMsgNumber, u32 dwMsgLength);
void IFly_CloseMsgQueue(MSGQHANDLE hMsgQ);

int IFly_SndMsg(MSGQHANDLE hMsgQ, char *pchMsgBuf, u32 dwLen, int nTimeout);
int IFly_RcvMsg(MSGQHANDLE hMsgQ, char *pchMsgBuf, u32 dwLen, int nTimeout);

#ifdef WIN32
THREADHANDLE IFly_CreateThread(void* pvTaskEntry, char* szName, u8 byPriority, u32 dwStacksize, u32 dwParam, u16 wFlag, u32 *pdwTaskID);
#else
THREADHANDLE IFly_CreateThread(LINUXFUNC pvTaskEntry, char* szName, u8 byPriority, u32 dwStacksize, u32 dwParam, u16 wFlag, u32 *pdwTaskID);
#endif
BOOL IFly_ThreadExit();
BOOL IFly_ThreadTerminate(THREADHANDLE hTask);

void Dump_Thread_Info(char *name,int tid);

int OpenDev(char *Dev);
int set_Parity(int fd, int databits, int stopbits, int parity);
int set_speed(int fd, int speed);

time_t read_rtc(int utc);
void write_rtc(time_t t, int utc);
int show_clock(int utc);

u32 GetLocalIp();
u32 GetLocalIp2(char * name); //cyl added
int SetLocalIp(u32 dwIp);
int GetHWAddr(char *pBuf);
int SetHWAddr(char *pBuf);// wrchen 090403
u32 GetRemoteIP();// wrchen 091110
u32 GetNetMask();
u32 GetNetMask2(char * name);//cyl added
int SetNetMask(u32 dwIp);
u32 GetBroadcast();
int SetBroadcast(u32 dwIp);
u32 GetDefaultGateway();
int SetDefaultGateway(u32 dwIp);
u32 GetDNSServer();
int	SetDNSServer(u32 dwIp);
int AddDNSServer(u32 dwIp);

int ftpget(char *serverip,char *localfile,char *remotefile);
int ftpput(char *serverip,char *remotefile,char *localfile);

int mount_user(char *mounted_path,char *user_path);
int umount_user(char *user_path);

int wget_user(char *user_name,char *user_passwd,char *hostname,int typeflag);

BOOL CheckNetLink();

/* the arp packet to be sent*/
struct arp_struct{
	//��̫��ͷ
	unsigned char	dst_mac[6];
	unsigned char	src_mac[6];
	unsigned short	pkt_type;
	//arp����
	unsigned short	hw_type;
	unsigned short	pro_type;
	unsigned char	hw_len;
	unsigned char	pro_len;
	unsigned short	arp_op;
	unsigned char	sender_eth[6];
	unsigned char	sender_ip[4];
	unsigned char	target_eth[6];
	unsigned char	target_ip[4];
};

int open_arp_socket();
int sendarppacket(int s,unsigned int dstIp,unsigned int srcIp);

#ifdef __cplusplus
}
#endif

#ifdef WIN32
typedef struct
{
	__int64 curpos;
	__int64 totalpos;		
}ifly_format_progress_t;
#else
typedef struct
{
	long long curpos;
	long long totalpos;		
}ifly_format_progress_t;
#endif

ifly_format_progress_t tFormat;

extern int g_vga_width;
extern int g_vga_height;
extern THREADHANDLE NTP_pthread;
#endif
