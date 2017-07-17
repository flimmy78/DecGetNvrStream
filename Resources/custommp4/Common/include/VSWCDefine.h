#ifdef _VXWORKS_
#include "vxworks.h"
#include "semLib.h"
#include "taskLib.h"
#include "sysLib.h"
#include "usrLib.h"
#include "msgQLib.h"
#include "tickLib.h"
#include "string.h"
#include "sockLib.h"
#include "inetLib.h"
#include "routeLib.h"
#include "ioLib.h"
#include "stdio.h"
#define SEMHANDLE					 SEM_ID
#define TASKHANDLE					 u32
#elif defined WIN32
#include <wtypes.h>
#define SEMHANDLE					 HANDLE
#else
#include <semaphore.h>
#define SEMHANDLE					 sem_t *
#endif

#pragma pack(push, 1)

#define BANK1_OFFSET	8
#define BANK2_OFFSET	0
#define BANK3_OFFSET	0

#define AD_SIZE         20
#define VCODEC_SIZE		108
#define SERIAL_SIZE		16
#define SERIAL_IP_SIZE	40
#define NET_SIZE		16
#define MANAGE_SIZE		32

#define ControlIP1_Offset 		0
#define ControlIP2_Offset 		4
#define ControlIP3_Offset 		8
#define ControlIP4_Offset 		12
#define ContrlPort_Offset 		16
#define ControlHBeat_Offset		20
#define ControlAlarm_Offset     24
#define ControlAlarmPort_Offset 28

typedef struct _VSWC_MANAGE_CONTEXT
{
	unsigned int 	m_ControlCenterIP1;//��������1��IP
	unsigned int 	m_ControlCenterIP2;	
	unsigned int 	m_ControlCenterIP3;	
	unsigned int 	m_ControlCenterIP4;	
	
	unsigned int 	m_ControlPort;//�������ĵĿ��ƶ˿�----Ĭ�϶Եȶ˿�
	unsigned int 	m_HeartBeatFre;	//��������Ļ㱨�ķ���Ƶ�ʣ����룩
	
	unsigned int    m_AlarmCenter;
	unsigned int  	m_AlarmPort;
}VSWC_MANAGE_CONTEXT;////�����ֽڣ�

#define VCodecResolution_Offset 		0
#define VCodecRateControl_Offset 		4
#define VCodecBitRate_Offset			8
#define VCodecFrameRate_Offset			12
#define VCodecMinQuant_Offset           16
#define VCodecMaxQuant_Offset           20
#define VCodecIFrameInter_Offset		24
#define VCoddecBFrameInter_Offset       28

#define VCodecDestinationIP_Offset      52
#define VCodecDestinationPort_Offset    56
#define VCodecTransProtocal_Offset      60
#define VCodecGUID_Offset				64
#define VCodecPacketLen_Offset			80
#define VCodecInitStream_Offset			84
#define VCodecCurrentStream_Offset      88
#define VCodecIFrameNeed_Offset         92
#define VCodecFrameNum_Offset			96
#define VCodecSequenceNum_Offset		100
#define VCodecMotionD_Offset			104

/////////�ýṹ������Ƶ����ģ��Ĺ���������
struct VSWC_VCODEC_ENC_CONTEXT
{
//	//�ó�Ա����ǰ������ķֱ��ʣ� QCIF  CIF  2CIF 4CIF    ������ڷֱ��ʵ����� ����һ��
	unsigned int	m_VideoResolution;
	unsigned int   	m_RateControlMode;///////////////////////////////////
	unsigned int 	m_BitRate;///->��������ģ�͵Ľ������Ǹ�������Ĺ���
	unsigned int 	m_FrameRate;////���Լ���һ֡���-----���ǵ�ĳЩӦ�õ���Ҫ
	unsigned int  	m_MinQuantizer;
	unsigned int  	m_MaxQuantizer;
	unsigned int  	m_IFrameInterval;
	unsigned int  	m_BFrameEveryPFrame;////////Ŀǰ��ʱ����
	
	unsigned int	m_Reserved1;/////Ԥ��
	unsigned int	m_Reserved2;	
	unsigned int	m_Reserved3;	
	unsigned int	m_Reserved4;	
	unsigned int	m_Reserved5;	
	
	//////////����
	unsigned int   	m_DestinationIP;
	unsigned int 	m_DestinationPort;
	unsigned int	m_TransProtocal;
	GUID            m_ServerGUID;//////����TCP���ӵĽ���
	unsigned int	m_PacketLen;//////ָ��ÿ�����Ͱ��ĳ���
	
	/////////����״̬
	unsigned int	m_bInitialStream;////�Ƿ�һ�ϵ�ͷ�����Ƶ��
	unsigned int	m_bCurrentStream;////������ǰ�Ƿ��͸���Ƶ���������Ƿ񱻴�
	unsigned int	m_bNeedIFrame;
	unsigned int	m_EncodeFrameNum; //һ������֡����ţ����ֵNTSC(1~30),PAL(1~25)��
	unsigned int	m_VideoSequenceNum;
	unsigned int	m_bMotionDetection;
	
};

struct VSWC_VCODEC_ENC_NOTIFY
{
	int   m_FrameRateNotify;//֡�ʸı�֪ͨ
	int   m_BitRateNofity;//���ʸı�֪ͨ
	int   m_ResolutionNotify;//�ֱ��ʸı�֪ͨ
	int   m_MaxQuanyNotify;//������������ı�֪ͨ
	int   m_MinQuantNotify;//��С���������ı�֪ͨ
	int   m_AnalogNotify;//������ʽ�ı�֪ͨ
	
};

#define VideoStandard_Offset 	0
#define Brightness_Offset       4
#define Contrast_Offset			8
#define Saturate_Offset			12
#define Hue_Offset				16
//////////�ýṹ����������ADת��ģ��Ĺ���������
struct VSWC_AD_CONTEXT
{
	//�ó�Ա����ǰ���������ʽ�� PAL    NTSC
	unsigned int		m_VideoStandard;
	//�ó�Ա����ǰ�����������
	unsigned int		m_Brightness;
	//�ó�Ա����ǰ������ĶԱȶ�
	unsigned int		m_Contrast;
	//�ó�Ա����ǰ������ı��Ͷ�	
	unsigned int		m_Saturate;
	//�ó�Ա����ǰ�������ɫ��	
	unsigned int		m_Hue;
	
};

#define BaudRate_Offset 0
#define StopBit_Offset  4
#define CheckNum_Offset	8
#define Databits_Offset 12

///////////�ýṹ������������
struct VSWC_SERIAL_CONTEXT
{
	//�ó�Ա�������ڲ�����
	unsigned int  m_BaudRate;
	//�ó�Ա��������ֹͣλ
	unsigned int   m_StopBit;
	//�ó�Ա��������У��λ
	unsigned int   m_CheckNum;
	//�ó�Ա������������λ
	unsigned int   m_DataBits;
	
};

#define UnitIP_Offset 	0
#define UnitMask_Offset	4
#define UnitGate_Offset 8
#define UnitDNS_Offset	12

///////////�ýṹ��������������������ӿ�
struct VSWC_NETINTERFACE_CONTEXT
{
	unsigned int m_UnitIP;
	unsigned int m_UnitMask;
	unsigned int m_UnitGate;
	unsigned int m_UnitDNSServerIP;
};

struct VSWC_VCODEC_CONTROL_DS
{
	SEMHANDLE    	m_CountMutex;
	SEMHANDLE    	m_QueueSem;
	// --------------------------------------------------------------------------
	// video buffer �߼��Ͽ��Կ���һ�����У�fifo��,����Ϊ�̶����ȣ�����codec��
	//���ԣ�ÿ��bufĿǰ���Ϊ64KB
	// --------------------------------------------------------------------------
	unsigned char   *m_pVideoBuf;
	unsigned char   m_FrameBufNum;
	unsigned int    m_PerBufLen;
	
	unsigned char	m_ReadyBufCount;
	
	unsigned char   m_CodecPointer;
	unsigned char   m_SendPointer;	
	
};

struct SYSYTEM_ALARM_MSG
{
	unsigned short	m_AlarmCapaID;
	unsigned short  m_AlarmAppID;
	unsigned short	m_EventType;
	unsigned int	m_EventArg;
};

struct SYSTEM_SERAIL_DATA_MSG
{
	unsigned int   m_MsgLen;
	void           *m_MsgPointer;
};

#define SerialRemoteIP_Offset 		0
#define SerialRemotePort_Offset		4
#define SerialLocalPort_Offset		8
#define SerialInitialStream_Offset	12
#define SerialCurrentStream_Offset	16
#define SerialGUID_Offset			20
#define SerialConn_Type				36

struct VSWC_SERIAL_IP_CONTROL
{
	unsigned int 	m_RemoteIP;
	unsigned int 	m_RemotePort;
	unsigned int	m_LocalPort;
	unsigned int  	m_InitialStream;
	unsigned int  	m_CurrentStream;
	GUID            m_TargetGUID;
	unsigned int    m_ConnType;
};

/////���µĴ������vswcdefines.h��
#define AUDIO_FORMAT_PCM                1
#define AUDIO_FORMAT_G723				4
#define AUDIO_LISTEN_PORT				59998

#define AENC_SIZE						20
#define ADEC_SIZE                       20

#define AEncInitStream_Offset 			0
#define AEncCurrentStream_Offset 		4 
#define AEncCodeType_Offset				8
#define AEncRemoteIP_Offset				12
#define AEncRemotePort_Offset			16

/////////////////////�������ı�����BANK3��,20�ֽ�,����pcm���룺Ĭ��8K  16λ   ������
struct VSWC_AUDIO_ENCODE_CONTEXT
{
	unsigned int m_bInitStream;	
	unsigned int m_bCurrentStream;
	
	unsigned int m_CodecType;///֧������ģʽ��PCM��G.723
	unsigned int m_DestinationIP;
	unsigned int m_DestinationPort;
};

#define ADecInitStream_Offset 		0
#define ADecCurrentStream_Offset 	4 
#define ADecCodeType_Offset			8
#define ADecRemoteIP_Offset			12
#define ADecRemotePort_Offset		16
/////////////////////�������ı�����BANK3��,20�ֽ�,����PCM����,Ҫ��ԭ��44K  16 ˫��������
struct VSWC_AUDIO_DECODE_CONTEXT
{
	unsigned int m_bInitStream;	
	unsigned int m_bCurrentStream;
	
	unsigned int m_CodecType;///֧������ģʽ��PCM��G.723
	unsigned int m_RemoteIP;
	unsigned int m_LocalPort;///////LocalPort :���Թ̶�������
};

#define MDETECT_CONTEXT_SIZE		254
#define MD_ENABLE_Offset			0
#define MD_MASKSTRLEN_OFFSET		4
#define MD_CONSECUTIVEFRAME_Offset 	8
#define MD_UPPERTHRESHHOLD_Offset	12
#define MD_LOWERTHRESHHOLD_Offset	16
#define MD_VECTORTHRESHHOLD_Offset	20
#define MD_FROMR2L_Offset			24
#define MD_FROML2R_Offset			28
#define MD_FROMT2B_Offset			32
#define MD_FROMB2T_Offset			36
#define MD_MASKENABLE_Offset		40
#define MD_MASKSTR_Offset			44

struct MOTION_DETECTION_CONTEXT
{
	unsigned int m_bMotionDetection;
	unsigned int m_MaskStrLen;

	unsigned int m_ConsecutiveFrame;	
	unsigned int m_UpperThreshhold;
	unsigned int m_LowerThreshhold;

	unsigned int m_VectorThreshhold;
	unsigned int m_FromR2L;
	unsigned int m_FromL2R;
	unsigned int m_FromT2B;
	unsigned int m_FromB2T;

	unsigned int m_bMAlert;
	unsigned int m_SFrameCount;
	unsigned int m_MFrameCount;

	unsigned int m_bMaskEnable;///////////////////////////////////////40�ֽ�
	char         m_MotionMaskStr[210];
};

struct VSWC_VCODEC_CONTROL_DS  		m_VCodecControlDS;

/*
struct VSWC_VCODEC_ENC_CONTEXT 		m_VCodecEncContext;
struct VSWC_VCODEC_ENC_CONTEXT 		m_VCodecEncContext2;
struct VSWC_VCODEC_ENC_CONTEXT 		m_VCodecEncContext3;
struct VSWC_VCODEC_ENC_CONTEXT 		m_VCodecEncContext4;
*/

struct VSWC_VCODEC_ENC_NOTIFY		m_EncNotify;
struct VSWC_VCODEC_ENC_NOTIFY		m_EncNotify2;
struct VSWC_VCODEC_ENC_NOTIFY		m_EncNotify3;
struct VSWC_VCODEC_ENC_NOTIFY		m_EncNotify4;

struct VSWC_AD_CONTEXT              m_ADContext1;
struct VSWC_AD_CONTEXT              m_ADContext2;
//////////////��������ad�����ı�����bank3��
struct VSWC_AD_CONTEXT              m_ADContext3;
struct VSWC_AD_CONTEXT              m_ADContext4;

VSWC_MANAGE_CONTEXT            		m_CenterContext;

struct VSWC_SERIAL_CONTEXT          m_485Context;
struct VSWC_SERIAL_CONTEXT          m_232Context;
struct VSWC_NETINTERFACE_CONTEXT    m_NetContext;
struct VSWC_SERIAL_IP_CONTROL		m_485IPControl;
struct VSWC_SERIAL_IP_CONTROL		m_232IPControl;

struct VSWC_AUDIO_ENCODE_CONTEXT 	m_ACodecEncContext;
struct VSWC_AUDIO_DECODE_CONTEXT 	m_ACodecDecContext;

struct MOTION_DETECTION_CONTEXT  	m_MotionContext;//Ĭ��motion detection ֻ�Ե�һ·��Ƶ����ĵ�һ·����Ч
struct MOTION_DETECTION_CONTEXT  	m_MotionContext3;//2
struct MOTION_DETECTION_CONTEXT  	m_MotionContext4;//3
struct MOTION_DETECTION_CONTEXT  	m_MotionContext5;//4

#ifdef _VXWORKS_
MSG_Q_ID							m_AMsgID;
MSG_Q_ID							m_485MsgID;
MSG_Q_ID							m_232MsgID;

unsigned int                        m_bInterrupt;
unsigned char 						readromcontence;

///////IIC MUTEX;
SEMHANDLE    	m_IICMutex;
SEMHANDLE    	m_MaskStrMutex;
int 			Advideo1;
int				Advideo2;
int 			Advideo3;
int				Advideo4;
int				m_bConfigMotionD;
#endif

#pragma pack( pop )
