#ifndef _FHDEV_NET_DEFINE_H_
#define _FHDEV_NET_DEFINE_H_

#ifdef WIN32    // win32

#else           // linux

#ifndef BOOL
#   define BOOL         int
#endif

#ifndef DWORD
#   define DWORD        unsigned int
#endif

#ifndef LPDWORD
#   define LPDWORD      unsigned int*
#endif

#ifndef WORD
#   define WORD         unsigned short
#endif

#ifndef BYTE
#   define BYTE         unsigned char
#endif

#ifndef LPVOID
#   define LPVOID       void*
#endif

#ifndef DWORD64
#   define DWORD64      unsigned long long
#endif

#ifndef COLORREF
#   define COLORREF     unsigned int
#endif

#ifndef TRUE
#   define TRUE    1
#endif
#ifndef FALSE
#   define FALSE   0
#endif
#ifndef NULL
#   define NULL    0
#endif

#endif



#define FHNP_MACRO_NAMELEN_MAX              32      // �û�����󳤶�
#define FHNP_MACRO_COVER_MAX                4       // ��������������
#define FHNP_MACRO_IPLEN_MAX                32      // IP��ַ��󳤶�
#define FHNP_MACRO_DOMAINLEN_MAX            128     // ��ַ��󳤶�
#define FHNP_MACRO_LOGLEN_MAX               256     // ��־������󳤶�
#define FHNP_MACRO_CHANNEL_MAX              32      // ���֧��32��ͨ��
#define FHNP_MACRO_DAILY_SCHED_ITEM_MAX     3       // ��ʱ¼��ÿ��������õ�¼�����

//////////////////////////////////////////////////////////////////////////
/* ȫ�ִ����� */
#define FHNPEN_OK                           0       // û�д���
#define FHNPEN_ERR_SetConfig                1       // ����/����ʧ��
#define FHNPEN_ERR_GetConfig                2       // ��ȡʧ��
#define FHNPEN_ERR_GetData                  3       // ��ȡ����������
#define FHNPEN_ERR_Parameter                4       // ��������
#define FHNPEN_ERR_Authority                5       // ��Ȩ��
#define FHNPEN_ERR_UserInexistence          6       // �û�������
#define FHNPEN_ERR_Password                 7       // �������
#define FHNPEN_ERR_ReLogin                  8       // �ظ���¼
#define FHNPEN_ERR_Upgrading                10      // ������
#define FHNPEN_ERR_OutOfMemory              11      // �ڴ治��
#define FHNPEN_ERR_RecvOverTime             12      // ���ճ�ʱ
#define FHNPEN_ERR_Upgrade_DataError        13      // ���������ݴ���
#define FHNPEN_ERR_Upgrade_SameVersion      14      // ���������豸�汾һ��
#define FHNPEN_ERR_Upgrade_OldVersion       15      // ������Ϊ�ɰ汾
#define FHNPEN_ERR_UpgradeErr               16      // ����ʧ��
#define FHNPEN_ERR_Upgrade_WriteFlashFail   17      // ����������дflashʧ��
#define FHNPEN_ERR_NotSupport               20      // ���ܲ�֧��
/* ����SDK������ */
#define FHNPEN_ERR_SDK_Index                2000
#define FHNPEN_ERR_SDK_NoInit               (FHNPEN_ERR_SDK_Index + 1)  // û�г�ʼ��
#define FHNPEN_ERR_SDK_NoLogin              (FHNPEN_ERR_SDK_Index + 2)  // �û�û�е�¼
#define FHNPEN_ERR_SDK_NoHandle             (FHNPEN_ERR_SDK_Index + 3)  // ����ľ����Ч
#define FHNPEN_ERR_SDK_ParamIn              (FHNPEN_ERR_SDK_Index + 4)  // ����������Ϸ�
#define FHNPEN_ERR_SDK_RecvPacketSize       (FHNPEN_ERR_SDK_Index + 5)  // ���յ������ݰ���С����
#define FHNPEN_ERR_SDK_NetCreate            (FHNPEN_ERR_SDK_Index + 6)  // ����SOCKETʧ��
#define FHNPEN_ERR_SDK_NetConf              (FHNPEN_ERR_SDK_Index + 7)  // SOCKET��������ʧ��
#define FHNPEN_ERR_SDK_NetBind              (FHNPEN_ERR_SDK_Index + 8)  // SOCKET��ʧ��
#define FHNPEN_ERR_SDK_NetException         (FHNPEN_ERR_SDK_Index + 9)  // Ŀ���豸������SOCKET�쳣
#define FHNPEN_ERR_SDK_NetConn              (FHNPEN_ERR_SDK_Index + 10) // �޷�����
#define FHNPEN_ERR_SDK_NetSend              (FHNPEN_ERR_SDK_Index + 11) // ����ʧ��
#define FHNPEN_ERR_SDK_NetRecv              (FHNPEN_ERR_SDK_Index + 12) // ���ճ�ʱ
#define FHNPEN_ERR_SDK_OpenFile             (FHNPEN_ERR_SDK_Index + 13) // �ļ���ʧ��
#define FHNPEN_ERR_SDK_OutOfMemory          (FHNPEN_ERR_SDK_Index + 14) // �ڴ�����ʧ��
#define FHNPEN_ERR_SDK_NotSupport           (FHNPEN_ERR_SDK_Index + 15) // ���ܲ�֧��
#define FHNPEN_ERR_SDK_IllegalDevice        (FHNPEN_ERR_SDK_Index + 16) // �Ƿ��豸
#define FHNPEN_ERR_SDK_Unknow               (FHNPEN_ERR_SDK_Index + 40) // δ֪����


//////////////////////////////////////////////////////////////////////////
/* �������� */
#define FHNPEN_CFGType_EncodeOSD            1    // ����OSD
#define FHNPEN_CFGType_EncodeCover          2    // ������������
#define FHNPEN_CFGType_EncodeAudio          4    // ��Ƶ��������
#define FHNPEN_CFGType_SystemConfig         6    // ϵͳ����
#define FHNPEN_CFGType_DSTConfig            7    // ����ʱ����
#define FHNPEN_CFGType_NTPConfig            8    // NTP����
#define FHNPEN_CFGType_SystemTime           9    // �豸ʱ��
#define FHNPEN_CFGType_TimeZone             10   // �豸ʱ��
#define FHNPEN_CFGType_NetConfig            11   // ��������
#define FHNPEN_CFGType_SerialPortConfig     12   // ��������
#define FHNPEN_CFGType_AutoRebootConfig     14   // �Զ�ά������
#define FHNPEN_CFGType_StoragePolicy        15   // ����洢��������
#define FHNPEN_CFGType_RecordPlan           16   // ��ʱ¼��ƻ�
#define FHNPEN_CFGType_WifiConfig           33   // wifi����


//////////////////////////////////////////////////////////////////////////
/* �ϱ���Ϣ����(�豸��Ϣ) */
#define FHNPEN_SYS_Kick                     1   // ���߳�
#define FHNPEN_SYS_OffLine                  2   // ���豸�Ͽ�����(����ֹͣ)
#define FHNPEN_SYS_ShutDown                 4   // �豸�ػ�
#define FHNPEN_SYS_Reboot                   5   // �豸����
#define FHNPEN_SYS_Reset                    6   // �豸�ָ���������
#define FHNPEN_SYS_Upgrade                  7   // �豸����
#define FHNPEN_SYS_ReConnect                13  // �豸�����ɹ�
#define FHNPEN_SYS_ImportConfig             14  // ��������
/* �ϱ���Ϣ����(������Ϣ) */
#define FHNPEN_ALM_SDError                  80  // SD������
#define FHNPEN_ALM_SDFull                   81  // SD����
#define FHNPEN_ALM_VideoLost                86  // �źŶ�ʧ
#define FHNPEN_ALM_IPConflict               88  // IP��ͻ

//////////////////////////////////////////////////////////////////////////
/* ����ģʽ */
#define FHNPEN_TransMode_TCP                0   // TCP
#define FHNPEN_TransMode_UDP				1   // UDP



/* �û��� */
typedef enum
{
    FHNPEN_UG_Admin = 0x01,     // ����Ա
    FHNPEN_UG_User,             // �û�
}FHNPEN_UserGroup_e;


/* ���ڸ�ʽ */
typedef enum
{
    FHNPEN_TF_Y_M_D = 0,      // YYYY-MM-DD(��-��-��)
    FHNPEN_TF_M_D_Y,          // MM-DD-YYYY(��-��-��)
    FHNPEN_TF_D_M_Y,          // DD-MM-YYYY(��-��-��)
}FHNPEN_TimeFormat_e;


/* Сʱ��ʽ */
typedef enum
{
    FHNPEN_TS_24 = 0,         // 24Сʱ��
    FHNPEN_TS_12,             // 12Сʱ��
}FHNPEN_TimeStandard_e;


/* ������������ */
typedef enum
{
    FHNPEN_CDTYPE_COLOR = 0,  // ɫ��
    FHNPEN_CDTYPE_MOSAIC,     // ������
}FHNPEN_CoverType_e;


/* ����ֱ��� */
typedef enum
{
    FHNPEN_ER_QCIF = 0,     // QCIF
    FHNPEN_ER_CIF,          // CIF
    FHNPEN_ER_HALFD1,       // HALFD1
    FHNPEN_ER_4CIF,         // 4CIF
    FHNPEN_ER_D1,           // D1
}FHNPEN_EncResolution_e;


/* ��Ƶ�����ʽ */
typedef enum
{
    FHNPEN_VF_H264 = 0,     // H264
}FHNPEN_EncVideoFormat_e;


/* ֡���� */
typedef enum
{
    FHNPEN_FType_IFrame = 0,    // I FRAME
    FHNPEN_FType_PFrame,        // P FRAME
    FHNPEN_FType_BFrame,        // B FRAME
    FHNPEN_FType_AFrame,        // AUDIO FRAME
    FHNPEN_FType_JFrame,        // JPEG FRAME
} FHNPEN_FrameType_e;


/* ���ʿ������� */
typedef enum
{
    FHNPEN_BRCTRL_FIXQP = 0,    // FixQP
    FHNPEN_BRCTRL_CBR,          // CBR
    FHNPEN_BRCTRL_VBR,          // VBR
}FHNPEN_EncBRCtrl_e;


/* ȥ�뼶�� */
typedef enum
{
    FHNPEN_DENOISE_OFF = 0,      // �ر�
    FHNPEN_DENOISE_SUPER_WEAK,   // ��
    FHNPEN_DENOISE_WEAK,         // ����
    FHNPEN_DENOISE_NORMAL,       // һ��
    FHNPEN_DENOISE_STRONG,       // ��ǿ
    FHNPEN_DENOISE_SUPER_STRONG, // ǿ
}FHNPEN_EncDenoise_e;


/* ȥ�������� */
typedef enum
{
    FHNPEN_DEINT_NONE = 0,        // �ر�
    FHNPEN_DEINT_5TAP_WEAK,       // 5���˲�(��)
    FHNPEN_DEINT_5TAP_MEDIUM,     // 5���˲�(��)
    FHNPEN_DEINT_5TAP_STRONG,     // 5���˲�(ǿ)
    FHNPEN_DEINT_5TAP_SUPER_STRONG,// 5���˲�(��ǿ)
    FHNPEN_DEINT_ADAPT_AVG,       // Adapt AVG
    FHNPEN_DEINT_MEDIUM,          // Medium
    FHNPEN_DEINT_ADAPT_MEDIAN,    // Adapt Medium
    FHNPEN_DEINT_MA3,             // MA3
}FHNPEN_EncDeinterlace_e;


/* ��Ƶ�����ʽ */
typedef enum
{
    FHNPEN_AF_G711_ALAW = 0,    // G711 ALAW
    FHNPEN_AF_G711_ULAW,        // G711 ULAW
    FHNPEN_AF_PCM,              // PCM
    FHNPEN_AF_G726,             // G726
    FHNPEN_AF_AMR,              // AMR
    FHNPEN_AF_AMRDTX,           // AMRDTX
    FHNPEN_AF_AAC,              // AAC
}FHNPEN_EncAudioFormat_e;


/* ��Ƶ������ */
typedef enum
{
    FHNPEN_ASRATE_8000 = 8000,
    FHNPEN_ASRATE_11025 = 11025,
    FHNPEN_ASRATE_16000 = 16000,
    FHNPEN_ASRATE_22050 = 22050,
    FHNPEN_ASRATE_24000 = 24000,
    FHNPEN_ASRATE_32000 = 32000,
    FHNPEN_ASRATE_44100 = 44100,
    FHNPEN_ASRATE_48000 = 48000,
}FHNPEN_EncAudioSampleRate_e;


/* ��Ƶλ�� */
typedef enum
{
    FHNPEN_ABITWIDTH_8 = 0,         // 8bit
    FHNPEN_ABITWIDTH_16,            // 16bit
    FHNPEN_ABITWIDTH_32,            // 32bit
}FHNPEN_EncAudioBitWidth_e;


/* ��Ƶ���� */
typedef enum
{
    FHNPEN_ATRACK_MONO = 0,          // ������
    FHNPEN_ATRACK_STEREO,            // ������
}FHNPEN_EncAudioTrack_e;


/* ��Ƶ�����ʽ */
typedef enum
{
    FHNPEN_VideoInputMode_PAL = 0,   // PAL
    FHNPEN_VideoInputMode_NTSC,      // NTSC
}FHNPEN_VideoInputMode_e;


/* ����ʱģʽ */
typedef enum
{
    FHNPEN_DSTMode_Date = 0,         // ����ģʽ
    FHNPEN_DSTMode_Week,             // ����ģʽ
}FHNPEN_DSTMode_e;


/* ʱ�� */
typedef enum
{
    FHNPEN_TIMEZONE_GMT_1200 = 0,
    FHNPEN_TIMEZONE_GMT_1100,
    FHNPEN_TIMEZONE_GMT_1000,
    FHNPEN_TIMEZONE_GMT_0900,
    FHNPEN_TIMEZONE_GMT_0800,
    FHNPEN_TIMEZONE_GMT_0700,
    FHNPEN_TIMEZONE_GMT_0600,
    FHNPEN_TIMEZONE_GMT_0500,
    FHNPEN_TIMEZONE_GMT_0430,
    FHNPEN_TIMEZONE_GMT_0400,
    FHNPEN_TIMEZONE_GMT_0330,
    FHNPEN_TIMEZONE_GMT_0300,
    FHNPEN_TIMEZONE_GMT_0200,
    FHNPEN_TIMEZONE_GMT_0100,
    FHNPEN_TIMEZONE_GMT,
    FHNPEN_TIMEZONE_GMT0100,
    FHNPEN_TIMEZONE_GMT0200,
    FHNPEN_TIMEZONE_GMT0300,
    FHNPEN_TIMEZONE_GMT0330,
    FHNPEN_TIMEZONE_GMT0400,
    FHNPEN_TIMEZONE_GMT0430,
    FHNPEN_TIMEZONE_GMT0500,
    FHNPEN_TIMEZONE_GMT0530,
    FHNPEN_TIMEZONE_GMT0545,
    FHNPEN_TIMEZONE_GMT0600,
    FHNPEN_TIMEZONE_GMT0630,
    FHNPEN_TIMEZONE_GMT0700,
    FHNPEN_TIMEZONE_GMT0800,
    FHNPEN_TIMEZONE_GMT0900,
    FHNPEN_TIMEZONE_GMT0930,
    FHNPEN_TIMEZONE_GMT1000,
    FHNPEN_TIMEZONE_GMT1100,
    FHNPEN_TIMEZONE_GMT1200,
    FHNPEN_TIMEZONE_GMT1300,
}FHNPEN_TimeZone_e;


/* У�� */
typedef enum
{
    FHNPEN_PARITY_NONE = 0,     // ��У��
    FHNPEN_PARITY_ODD,          // ��У��
    FHNPEN_PARITY_EVEN,         // żУ��
}FHNPEN_Parity_e;


/* ���������� */
typedef enum
{
    FHNPEN_FCTRL_NONE = 0,      // ��
    FHNPEN_FCTRL_XONXOFF,       // Xon / Xoff
    FHNPEN_FCTRL_HARD,          // Ӳ��
}FHNPEN_FlowControl_e;


/* �Զ�ά��ģʽ */
typedef enum
{
    FHNPEN_ARMode_Disable = 0,  // ����
    FHNPEN_ARMode_EveryDay,     // ÿ��ѭ��
    FHNPEN_ARMode_EveryWeek,    // ÿ��ѭ��
    FHNPEN_ARMode_EveryMonth,   // ÿ��ѭ��
}FHNPEN_AutoRebootMode_e;


/* ��־���� */
typedef enum
{
    FHNP_LogType_All = 0,       // ������־����
    FHNP_LogType_System,        // ϵͳ��־����
    FHNP_LogType_Operate,       // ������־����
    FHNP_LogType_Alarm,         // ������־����
}FHNP_LogType_e;


/* �ص������� */
typedef enum
{
    FHNPEN_ST_FRAME = 0,        // ֡����
    FHNPEN_ST_TS,               // TS����
}FHNPEN_StreamType_e;


/* ����״̬ */
typedef enum
{
    FHNPEN_UState_Finish = 0x01, // �������
    FHNPEN_UState_Sending,       // ������
    FHNPEN_UState_Waiting,       // �ȴ��豸����
    FHNPEN_UState_Fail,          // ����ʧ��
    FHNPEN_UState_Unknown,       // δ֪����
}FHNPEN_UpgradeState_e;


/* WIFI����ģʽ */
typedef enum
{
    FHNPEN_WLAN_MODE_INFRASTRUCTURE = 0,
    FHNPEN_WLAN_MODE_ADHOC,
    FHNPEN_WLAN_MODE_UAP,
    FHNPEN_WLAN_MODE_P2P,
    FHNPEN_WLAN_MODE_P2P_UAP,
}FHNPEN_WifiMode_e;


/* WIFI��ȫ���� */
typedef enum 
{
	FHNPEN_WLAN_SECURITY_NONE = 0,     // The network does not use security
	FHNPEN_WLAN_SECURITY_WEP_OPEN,     // The network uses WEP security with opn key
	FHNPEN_WLAN_SECURITY_WEP_SHARED,   // The network uses WEP security with shared key
	FHNPEN_WLAN_SECURITY_WPA,          // The network uses WPA security with PSK
	FHNPEN_WLAN_SECURITY_WPA2,         // The network uses WPA2 security with PSK
}FHNPEN_WifiSecurityType_e;



/* �طſ��� */
typedef enum
{
    FHNPEN_PBCtrl_Play = 1,      // ����
    FHNPEN_PBCtrl_Stop,          // ֹͣ
    FHNPEN_PBCtrl_Pause,         // ��ͣ
    FHNPEN_PBCtrl_Continue,      // ��ͣ����¼�������
    FHNPEN_PBCtrl_Slow,          // ����(��ǰ�ٶ�/2)
    FHNPEN_PBCtrl_Fast,          // ����(��ǰ�ٶ�*2)
    FHNPEN_PBCtrl_SetPlaySpeed,  // ���ò����ٶ�(FHNPEN_PlaySpeed_e)
    FHNPEN_PBCtrl_FramePrev,     // ��ת����һ��I֡
    FHNPEN_PBCtrl_FrameNext,     // ��ת����һ��I֡
    FHNPEN_PBCtrl_NOIFrameNext,  // ��ת����һ֡(��I֡)
}FHNPEN_PlayCtrl_e;


/* �ط����� */
typedef enum
{
    FHNPEN_PBSpeed_Normal = 0,    // ��������
    FHNPEN_PBSpeed_2,             // 2������
    FHNPEN_PBSpeed_4,             // 4������
    FHNPEN_PBSpeed_8,             // 8������
    FHNPEN_PBSpeed_16,            // 16������
    FHNPEN_PBSpeed_32,            // 32������(��֧��)
    FHNPEN_PBSpeed_64,            // 64������(��֧��)
    FHNPEN_PBSpeed_1_2,           // 1/2������
    FHNPEN_PBSpeed_1_4,           // 1/4������
    FHNPEN_PBSpeed_1_8,           // 1/8������
    FHNPEN_PBSpeed_1_16,          // 1/16������
    FHNPEN_PBSpeed_1_32,          // 1/32������(��֧��)
    FHNPEN_PBSpeed_1_64,          // 1/64������(��֧��)
}FHNPEN_PlaySpeed_e;

/* Ӳ��״̬ */
typedef enum
{
    FHNPEN_SDCardState_FOUND        = (1 << 0),     // �ҵ�SD��
    FHNPEN_SDCardState_LOADED       = (1 << 1),     // SD���Ѽ���
    FHNPEN_SDCardState_NORMAL       = (1 << 2),     // SD������
    FHNPEN_SDCardState_FORMATING    = (1 << 3),     // ��ʽ��������
}FHNPEN_SDCardState_e;


/* Ӳ�̸�ʽ������ */
typedef enum
{
    FHNPEN_FmtType_Fast = 0,  // ���ٸ�ʽ��
    FHNPEN_FmtType_Slow,      // ���ٸ�ʽ��
    FHNPEN_FmtType_Low,       // ���ٸ�ʽ��
}FHNPEN_FormatType_e;



/* ¼������ */
typedef enum
{
    FHNPEN_RecType_All    = 0,            // ����¼��
    FHNPEN_RecType_Manual = (1 << 0),     // �ֶ�¼��
    FHNPEN_RecType_Time   = (1 << 1),     // ��ʱ¼��
    FHNPEN_RecType_Alarm  = (1 << 2),     // ����¼��
}FHNPEN_RecordType_e;


/* ��ͼ���� */
typedef enum
{
    FHNPEN_PicType_All    = 0,            // ���н�ͼ
    FHNPEN_PicType_Manual = (1 << 0),     // �ֶ���ͼ
    FHNPEN_PicType_Time   = (1 << 1),     // ��ʱ��ͼ
    FHNPEN_PicType_Alarm  = (1 << 2),     // ������ͼ
}FHNPEN_PictureType_e;



typedef enum
{
    FHNPEN_EState_Finish = 0x01, // �����ɹ�
    FHNPEN_EState_Ing,           // ִ�й�����
    FHNPEN_EState_Fail,          // ʧ��
    FHNPEN_EState_Error,         // δ֪����
}FHNPEN_DurationState_e;






/* IP��ַ�ṹ�� */
typedef struct
{
    char sIPV4[16];     // IPv4��ַ
    BYTE sIPV6[128];    // ����
} FHNP_IPAddr_t, *LPFHNP_IPAddr_t;


/* ���γߴ� */
typedef struct 
{
    int x;          // x����
    int y;          // y����
    int w;          // ��
    int h;          // ��
} FHNP_Rect_t, *LPFHNP_Rect_t;


/* ϵͳʱ�� */
typedef struct 
{
    WORD  year;     // ��(1970-2038)
    BYTE  month;    // ��(1-12)
    BYTE  day;      // ��(1-31)
    BYTE  wday;     // ����(0-6)(0-������, 1-����һ...)
    BYTE  hour;     // ʱ(0-23)
    BYTE  minute;   // ��(0-59)
    BYTE  second;   // ��(0-59)
    DWORD msecond;  // ����(0-999)
} FHNP_Time_t, *LPFHNP_Time_t;


/* �û���Ϣ */
typedef struct 
{
    char chUser[FHNP_MACRO_NAMELEN_MAX];    // �û���
    char chPassword[FHNP_MACRO_NAMELEN_MAX];// �û�����
    BYTE btUserGroup;                       // �û���(FHNPEN_UserGroup_e)
    BYTE btUserOnline;                      // �Ƿ�����
    BYTE btRes[2];                          // ����
} FHNP_User_t, *LPFHNP_User_t;


/* ������������ */
typedef struct  
{
    BYTE btEnable;                              // ʹ��
    BYTE btCoverType;                           // ������������(FHNPEN_CoverType_e)
    BYTE btRes[2];                              // ����
    FHNP_Rect_t stRect[FHNP_MACRO_COVER_MAX];   // ������������(����720x576)
} FHNP_EncodeCover_t, *LPFHNP_EncodeCover_t;


/* ��Ƶ�������� */
typedef struct 
{
    BYTE btResolution;          // ����ֱ���(FHNPEN_EncResolution_e)
    BYTE btBRCtrl;              // ���ʿ���(FHNPEN_EncBRCtrl_e)
    BYTE btDenoise;             // ȥ��(FHNPEN_EncDenoise_e)
    BYTE btDeinter;             // ȥ����(FHNPEN_EncDeinterlace_e)
    int  iBitRate;              // ����
    int  iFrameRate;            // ֡��
    int  iGOP;                  // I֡���
} FHNP_EncodeVideo_t, *LPFHNP_EncodeVideo_t;


/* ����OSD���� */
typedef struct 
{
    BYTE btShowTimeOSD;                     // �Ƿ���ʾTimeOSD
    BYTE btTimeFmt;                         // ���ڸ�ʽ(FHNPEN_TimeFormat_e)
    BYTE btTimeStd;                         // Сʱ��ʽ(FHNPEN_TimeStandard_e)
    BYTE btRes;                             // ����
    int  iTimeOSDX, iTimeOSDY;              // TimeOSD����(����720x576)
    COLORREF colTimeOSD;                    // TimeOSD��ɫ

    BYTE btShowCustomTOSD;                  // �Ƿ���ʾ�Զ���TOSD
    BYTE btRes1[3];                         // ����
    int  iCustomTOSDX, iCustomTOSDY;        // �Զ���TOSD����(����720x576)
    char chCustomTOSD[FHNP_MACRO_NAMELEN_MAX]; // �Զ���TOSD�ַ���
    COLORREF colCustomOSD;                  // �Զ���TOSD��ɫ   
} FHNP_EncodeOSD_t, *LPFHNP_EncodeOSD_t;


/* ����ˮӡ���� */
typedef struct
{
    BYTE btEnable;              // ʹ��
    BYTE btRes[3];              // ����
    char chText[256];           // ˮӡ����
} FHNP_EncodeWaterMark_t, *LPFHNP_EncodeWaterMark_t;


/* ��Ƶ�������� */
typedef struct 
{
    BYTE  btAudioFmt;           // ��Ƶ�����ʽ(FHNPEN_EncAudioFormat_e)
    BYTE  btBitWidth;           // λ��(FHNPEN_EncAudioBitWidth_e)
    BYTE  btAudioTrack;         // ����(FHNPEN_EncAudioTrack_e)
    BYTE  btRes;                // ����
    DWORD dwSampleRate;         // ������(FHNPEN_EncAudioSampleRate_e)    
    DWORD dwDataLen;            // ���ݳ���
} FHNP_EncodeAudio_t, *LPFHNP_EncodeAudio_t;


/* ϵͳ���� */
typedef struct 
{
    char chDeviceName[FHNP_MACRO_DOMAINLEN_MAX];    // �豸����
    int  iDeviceIndex;                              // �豸���
    BYTE btVideoInputMode;                          // ��Ƶ������ʽ(FHNPEN_VideoInputMode_e)
    BYTE btRes[3];                                  // ����
} FHNP_DeviceConfig_t, *LPFHNP_DeviceConfig_t;


/* ����ʱ����ģʽ�µ�ʱ���ʽ */
typedef struct 
{
    BYTE month;                 // �·�[1-12]
    BYTE wdayIndex;             // �ڼ�������[1-5]
    BYTE wDay;                  // ���ڼ�[0-6]
    BYTE hour;                  // Сʱ[0-23]
    BYTE minute;                // ����[0-59]
    BYTE btRes[3];              // ����
} FHNP_DSTWeekModeTime_t, *LPFHNP_DSTWeekModeTime_t;


/* ����ʱ���ڸ�ʽ */
typedef struct 
{
    FHNP_Time_t stStartTime;    // ��ʼʱ��
    FHNP_Time_t stStopTime;     // ����ʱ��
} FHNP_DSTDateMode_t, *LPFHNP_DSTDateMode_t;


/* ����ʱ����ģʽ */
typedef struct 
{
    FHNP_DSTWeekModeTime_t stStartTime; // ��ʼʱ��
    FHNP_DSTWeekModeTime_t stStopTime;  // ����ʱ��
} FHNP_DSTWeekMode_t, *LPFHNP_DSTWeekMode_t;


/* ����ʱ */
typedef struct 
{
    BYTE btDST;                         // ʹ��
    BYTE btDSTMode;                     // ����ʱ����(FHNPEN_DSTMode_e)
    BYTE btRes[2];                      // ����
    FHNP_DSTDateMode_t stDSTDateMode;   // ����ģʽ
    FHNP_DSTWeekMode_t stDSTWeekMode;   // ����ģʽ
} FHNP_DST_t, *LPFHNP_DST_t;


/* �豸���ܲ��� */
typedef struct 
{
    char chSerialNum[FHNP_MACRO_NAMELEN_MAX];// ���к�
    BYTE btDevType;         // �豸����(FHNPEN_DevType_e)
    BYTE btRes[3];          // ����
    
    DWORD dwHWVersion;      // Ӳ���汾

    DWORD dwFWVersion;      // �̼��汾
    DWORD dwFWBuildDate;    // �̼��汾ʱ��

    DWORD dwSWVersion;      // ����汾
    DWORD dwSWBuildDate;    // ����汾ʱ��

    BYTE btVideoInNum;      // ��Ƶ������
    BYTE btAudioInNum;      // ��Ƶ������
    BYTE btAlarmInNum;      // �����������
    BYTE btAlarmOutNum;     // ����������� 

    BYTE btTalkPortNum;     // �����ڸ���
    BYTE btRS232Num;        // 232���ڸ���
    BYTE btRS485Num;        // 485���ڸ���
    BYTE btNetWorkPortNum;  // ���ڸ���

    BYTE btStorageNum;      // �洢�豸����
    BYTE btChannelNum;      // ģ��ͨ������
    BYTE btDecodeChannelNum;// �豸����·��
    BYTE btVGANum;          // VGA����
    BYTE btHDMINum;         // HDMI����
    BYTE btUSBNum;          // USB����
    BYTE btSpotNum;         // ���ڸ���
    BYTE btIPCanNum;        // �������ͨ��
    BYTE btRes2[4];         // ����
} FHNP_Capacity_t, *LPFHNP_Capacity_t;


/* NTP���� */
typedef struct 
{
    BYTE btNTP;             // �Ƿ�����NTP
    BYTE btIsSuccess;       // �Ƿ����ӳɹ�
    BYTE btRes[2];          // ����
    char chNTPServer[FHNP_MACRO_DOMAINLEN_MAX];// NTP��������ַ    
    FHNP_Time_t stPrevTime; // ��һ�γɹ����µ�ʱ��
    FHNP_Time_t stNextTime; // ��һ�μ���Ҫ���µ�ʱ��
} FHNP_NTP_t, *LPFHNP_NTP_t;


/* �������� */
typedef struct 
{
    BYTE  btDynamicIP;                  // �Ƿ��Զ���ȡIP
    BYTE  btRes1[3];                    // ����
    FHNP_IPAddr_t stIP;                 // IP��ַ
    FHNP_IPAddr_t stNetMask;            // ��������
    FHNP_IPAddr_t stGateWay;            // ����
    char  chMAC[FHNP_MACRO_IPLEN_MAX];  // MAC��ַ
    DWORD dwPort;                       // �˿�

    BYTE btDynamicDNS;                  // �Ƿ��Զ���ȡDNS
    BYTE btRes2[3];                     // ����
    FHNP_IPAddr_t stHostDNS;            // ��ѡDNS������
    FHNP_IPAddr_t stSecondDNS;          // ����DNS������
} FHNP_NetConfig_t, *LPFHNP_NetConfig_t;


/* �������� */
typedef struct 
{
    int  iBaudRate;                     // ������
    int  iDataBit;                      // ����λ
    int  iStopBit;                      // ֹͣλ
    BYTE btParity;                      // ��żУ��(FHNPEN_Parity_e)
    BYTE btFlowCtrl;                    // ����������(FHNPEN_FlowControl_e)
    BYTE btRes[2];                      // ����
} FHNP_SerialPort_t, *LPFHNP_SerialPort_t;


/* ʵʱ������� */
typedef struct 
{
    BYTE btChannel;                     // ͨ��(����Ч, ��0)
    BYTE btEncID;                       // ����ID(����Ч, ��0)
    BYTE btTransMode;                   // ����Э��(FHNPEN_TransMode_TCP/FHNPEN_TransMode_UDP)
    BYTE btBlocked;                     // 0-�첽, 1-ͬ��
    char chMultiCastIP[FHNP_MACRO_IPLEN_MAX];   // �ಥIP��ַ(����Ч)
} FHNP_Preview_t, *LPFHNP_Preview_t;


/* �Զ�ά������ */
typedef struct 
{
    BYTE btARebootMode;         // �Զ�ά��ģʽ(FHNPEN_AutoRebootMode_e)
    BYTE btDay;                 // ��
    BYTE btHour;                // ʱ
    BYTE btRes;                 // ����
} FHNP_AutoReboot_t, *LPFHNP_AutoReboot_t;


/* ��־��ѯ */
typedef struct 
{
    BYTE btType;                // ��־��ѯ����(FHNP_LogType_e)
    BYTE btRes[3];              // ����
    FHNP_Time_t stStartTime;    // ��ʼʱ��
    FHNP_Time_t stStopTime;     // ����ʱ��
} FHNP_LogSearch_t, *LPFHNP_LogSearch_t;


/* ��־��Ϣ */
typedef struct 
{
    FHNP_Time_t stTime;                 // ��־ʱ��
    BYTE btType;                        // ��־����(FHNP_LogType_e)
    BYTE btChannel;                     // ͨ����
    BYTE btRes[2];                      // ����
    char chUser[FHNP_MACRO_NAMELEN_MAX];// �û���
    char chIP[FHNP_MACRO_IPLEN_MAX];    // IP��ַ
    char chLog[FHNP_MACRO_LOGLEN_MAX];  // ��־����
} FHNP_Log_t, *LPFHNP_Log_t;


/* wifi���� */
typedef struct
{
    char sSSID[33];
    char sDummy[3];
    DWORD dwChannel;
    int iMode;                // FHNPEN_WifiMode_e
    int iType;                // FHNPEN_WifiSecurityType_e
    char sPSK[128];
    BYTE btPSKLen;
    BYTE btRes[3];
} FHNP_WifiConfig_t, *LPFHNP_WifiConfig_t;


/* ��Ƶ֡ͷ��Ϣ */
typedef struct
{
    BYTE    btFrameType;        // ֡����(FHNPEN_FrameType_e)
    BYTE    btVideoFormat;      // ��Ƶ�����ʽ(FHNPEN_EncVideoFormat_e)
    BYTE    btRestartFlag;      // ͼ���������÷�������
    BYTE    btRes;              // ����
    WORD    wWidth;             // ͼ���
    WORD    wHeight;            // ͼ���
    DWORD64 ullTimeStamp;       // ʱ���
    BYTE    btRes2[4];          // ����
} FHNP_VFrameHead_t, *LPFHNP_VFrameHead_t;


/* ��Ƶ֡ͷ��Ϣ */
typedef struct
{
    BYTE    btFrameType;        // ֡����(FHNPEN_FrameType_e)
    BYTE    btAudioFormat;      // ��Ƶ�����ʽ(FHNPEN_EncAudioFormat_e)
    BYTE    btBitWidth;         // ��Ƶλ��(FHNPEN_EncAudioBitWidth_e)
    BYTE    btTrace;            // ����(FHNPEN_EncAudioTrack_e)
    DWORD64 ullTimeStamp;       // ʱ���
    DWORD   dwSampleRate;       // ������(FHNPEN_EncAudioSampleRate_e)
    DWORD   dwDataLen;          // ���ݳ���
    BYTE    btRes[4];           // ����
} FHNP_AFrameHead_t, *LPFHNP_AFrameHead_t;


/* */
typedef struct 
{
    BYTE    btFrameType;        // ֡����(FHNPEN_FrameType_e)
    DWORD64 ullTimeStamp;       // ʱ���
    BYTE    btRes[4];           // ����
} FHNP_JFrameHead_t, *LPFHNP_JFrameHead_t;


/* ֡ͷ��Ϣ(������) */
typedef union
{
    FHNP_VFrameHead_t stVFrameHead; // ��Ƶ֡ͷ
    FHNP_AFrameHead_t stAFrameHead; // ��Ƶ֡ͷ
    FHNP_JFrameHead_t stJFrameHead; // ��ͼ֡ͷ
} FHNP_FrameHead_t, *LPFHNP_FrameHead_t;


/* SDK״̬ */
typedef struct
{
    DWORD dwTotalLoginNum;		// ��ǰlogin�û���
    DWORD dwTotalRealPlayNum;	// ��ǰrealplay·��
    DWORD dwTotalUpgradeNum;	// ��ǰ����·��
    DWORD dwTotalSerialNum;     // ��ǰ����͸��·��
    DWORD dwRes[10];            // ����
} FHNP_SDKState_t, *LPFHNP_SDKState_t;


/* �ϱ���Ϣ */
typedef struct 
{
    BYTE  btNotify;         // �ϱ���ϢID
    BYTE  btFlag;           // ����/�ر�
    BYTE  btChannel;        // ͨ��
    BYTE  btRes;            // ����
    DWORD dwUserID;         // UserID
    DWORD dwBufLen;         // ���ݳ���
    char  chBuffer[4096];   // ��������
} FHNP_Notify_t, *LPFHNP_Notify_t;




/* Զ�̻ط����� */
typedef struct 
{
    BYTE  btChannel;                // ͨ��
    BYTE  btEncID;                  // ����ID
    BYTE  btTransMode;              // ����Э��
    BYTE  btRes;                    
    DWORD dwRecTypeMask;            // ¼������(FHNPEN_RecordType_e��λ��)
    FHNP_Time_t stStartTime;        // ��ʼʱ��
    FHNP_Time_t stStopTime;         // ����ʱ��    
} FHNP_Playback_t, *LPFHNP_Playback_t;


/* Զ��¼���ѯ */
typedef struct
{
    BYTE  btLockFlag;            // 0ȫ�� 1���� 2δ����
    BYTE  btChanNum;             // ͨ������
    BYTE  btRes[2];
    BYTE  btChannel[FHNP_MACRO_CHANNEL_MAX];    // ͨ���б�
    DWORD dwRecTypeMask;        // ¼������(FHNPEN_RecordType_e��λ��)
    FHNP_Time_t stStartTime;    // ��ʼʱ��
    FHNP_Time_t stStopTime;     // ����ʱ��
} FHNP_RecSearch_t, *LPFHNP_RecSearch_t;


/* Զ�̽�ͼ��ѯ */
typedef struct 
{
    BYTE  btLockFlag;            // 0ȫ�� 1���� 2δ����
    BYTE  btChanNum;             // ͨ������
    BYTE  btRes[2];
    BYTE  btChannel[FHNP_MACRO_CHANNEL_MAX];    // ͨ���б�    
    DWORD dwPicTypeMask;        // ��ͼ����(FHNPEN_PictureType_e��λ��)
    FHNP_Time_t stStartTime;    // ��ʼʱ��
    FHNP_Time_t stStopTime;     // ����ʱ��
} FHNP_PicSearch_t, *LPFHNP_PicSearch_t;


/* Զ��¼����Ϣ */
typedef struct 
{
    BYTE btChannel;             // ͨ��
    BYTE btRecType;             // ¼������(FHNPEN_RecordType_e)
    BYTE btLockFlag;            // ����״̬(0δ��, 1����)
    BYTE btRes;
    DWORD64 ullStartTime;       // ��ʼʱ��(΢��)
    DWORD64 ullStopTime;        // ����ʱ��(΢��)    
    DWORD64 ullDataSize;
} FHNP_Record_t, *LPFHNP_Record_t;


/* Զ�̽�ͼ��Ϣ */
typedef struct 
{
    BYTE btChannel;             // ͨ��
    BYTE btPicType;             // ��ͼ����(FHNPEN_PictureType_e)
    BYTE btLockFlag;            // ����״̬(0δ��, 1����)
    BYTE btRes;
    DWORD64 ullStartTime;       // ��ʼʱ��(΢��)
    DWORD64 ullStopTime;        // ����ʱ��(΢��)    
    DWORD64 ullDataSize;
    DWORD64 ullFrameCount;      // ����
} FHNP_Picture_t, *LPFHNP_Picture_t;


/* ����洢���� */
typedef struct 
{
    DWORD dwMaxHour;            // ���������д����Сʱ������(0����)
    DWORD dwFullThreshold;      // С�ڶ���MBʱ��Ϊ��
    BYTE  btRecycleFlag;        // ������ʱ�Ƿ�ѭ������(0-������, 1-����)
    BYTE  btCleanData;          // �Ƿ񳹵��������(0-���������, 1-���)
    BYTE  btRes[2];    
} FHNP_StoragePolicy_t, *LPFHNP_StoragePolicy_t;


typedef struct 
{
    BYTE btState;               // SD��״̬(FHNPEN_SDCardState_e)
    BYTE btRes[3];
    DWORD64 ullTotalSize;       // SD��������
    DWORD64 ullUsedSize;        // SD�����ÿռ�
} FHNP_SDCardInfo_t, *LPFHNP_SDCardInfo_t;


/* ÿ��¼��ʱ��� */
typedef struct 
{
    BYTE btEnable;         // ʹ��
    BYTE btRecAudio;       // �Ƿ�¼����Ƶ
    BYTE btStartHour;      // ��ʼʱ��(ʱ)
    BYTE btStartMinute;    // ��ʼʱ��(��)
    BYTE btStopHour;       // ����ʱ��(ʱ)
    BYTE btStopMinute;     // ����ʱ��(��)
    BYTE btReserve[2];
} FHNP_RecScheduleSegment_t, *LPFHNP_RecScheduleSegment_t;

/* ÿ��¼��ʱ��� */
typedef struct 
{
    FHNP_RecScheduleSegment_t stSegment[FHNP_MACRO_DAILY_SCHED_ITEM_MAX];
    BYTE btEnable;
    BYTE btReserve[3];
} FHNP_RecScheduleDaily_t, *LPFHNP_RecScheduleDaily_t;

/* ��ʱ¼��ʱ��� */
typedef struct 
{
    FHNP_RecScheduleDaily_t stDaily[7];     // 0-����
    BYTE btEnable;
    BYTE btReserve[3];
} FHNP_RecScheduleTiming_t, *LPFHNP_RecScheduleTiming_t;


#endif

