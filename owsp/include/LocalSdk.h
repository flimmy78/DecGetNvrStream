#ifndef _LOCAL_SDK_H_
#define _LOCAL_SDK_H_

// #ifndef MEDIA_TYPE
// #define MEDIA_TYPE
//ͼ���������
#define  MEDIA_TYPE_H264		(BYTE)98//H.264//������109?
#define  MEDIA_TYPE_MP4			(BYTE)97//MPEG-4
#define  MEDIA_TYPE_H261		(BYTE)31//H.261
#define  MEDIA_TYPE_H263		(BYTE)34//H.263
#define  MEDIA_TYPE_MJPEG		(BYTE)26//Motion JPEG
#define  MEDIA_TYPE_MP2			(BYTE)33//MPEG2 video

//������������
#define	 MEDIA_TYPE_MP3			(BYTE)96//mp3
#define  MEDIA_TYPE_PCMU		(BYTE)0//G.711 ulaw
#define  MEDIA_TYPE_PCMA		(BYTE)8//G.711 Alaw
#define	 MEDIA_TYPE_G7231		(BYTE)4//G.7231
#define	 MEDIA_TYPE_G722		(BYTE)9//G.722
#define	 MEDIA_TYPE_G728		(BYTE)15//G.728
#define	 MEDIA_TYPE_G729		(BYTE)18//G.729
#define	 MEDIA_TYPE_RAWAUDIO	(BYTE)19//raw audio
#define  MEDIA_TYPE_ADPCM		(BYTE)20//adpcm
#define  MEDIA_TYPE_ADPCM_HISI	(BYTE)21//adpcm-hisi
// #endif//MEDIA_TYPE

/* ========================== structure for each encoded frame ============================= */
typedef struct FrameHeadr
{
	unsigned char mediaType;			//encoded (video/audio) media type:
	unsigned char *pData;				//encoded data buffer
	unsigned char play_index;
	unsigned int preBufSize;			//pre buffer size, normally it is 12+4+12
	// (FIXED HEADER + Extence option + Extence bit)
	unsigned int dataSize;				//actual buffer size pointed by pData
	unsigned char frameRate;			//frame rate, used on receive part. 
	unsigned int frameID;				//fram id��used on receive part. 
	unsigned int timeStamp;				//time stamp, used on receive part. 
	union
	{
		struct{
			int keyFrame;				//I(1) or P(0)
			unsigned short videoWidth;	//video width
			unsigned short videoHeight;	//video height
		} videoParam;
		unsigned char audioMode;		//8, 16, or 32 bit
	};
} FrameHeadr;
typedef FrameHeadr* pFrameHeadr;

#pragma pack( push, 1 )//���ֽڶ���

enum NETDVR_RETURN_CODE	//
{
	NETDVR_SUCCESS	= 0,
	NETDVR_REC_STATUS_STARTED,
	NETDVR_REC_STATUS_STOPPED,
	NETDVR_ERROR = 2000,
	NETDVR_ERR_UNKNOWN,
	NETDVR_ERR_PARAM,
	NETDVR_ERR_RET_PARAM,						//???
	NETDVR_ERR_NOINIT,
	NETDVR_ERR_COMMAND,
	NETDVR_ERR_NEW_CONNECTION,					//???
	NETDVR_ERR_SEND,
	NETDVR_ERR_OUTOFMEMORY,
	NETDVR_ERR_RESOURCE,
	NETDVR_ERR_FILENOTEXIST,
	NETDVR_ERR_BAUDLIMIT,						//max = 5
	NETDVR_ERR_CREATESOCKET,					//create socket error
	NETDVR_ERR_SOCKET,							//socket error
	NETDVR_ERR_CONNECT,							//connect error
	NETDVR_ERR_BIND,							//bind error -1985
	NETDVR_ERR_LISTEN,		
	NETDVR_ERR_NETSND,							//send error
	NETDVR_ERR_NETRCV,							//recv error
	NETDVR_ERR_TIMEOUT,							//timeout 
	NETDVR_ERR_CHNERROR,						// > channel limit -1980
	NETDVR_ERR_DEVICEBUSY,						//device busy
	NETDVR_ERR_WRITEFLASH,						//erase error 
	NETDVR_ERR_VERIFY,							//verify error 
	NETDVR_ERR_CONFLICT,						//system resource conflict
	NETDVR_ERR_BUSY,							//system busy-1975
	NETDVR_ERR_USER_SAME,						//user name conflict
	NETDVR_ERR_LINKLIMIT,
	NETDVR_ERR_DATABASE,
	/* === error code for login === */
	NETDVR_ERR_NOUSER,
	NETDVR_ERR_PASSWD,
	NETDVR_ERR_MACADDR, 
	NETDVR_ERR_RELOGIN,
	NETDVR_ERR_NOLOGIN,
	/* === net player === */
	NETDVR_ERR_NETDVR_PLAYER_FULL,
	/* === updateing ==== */
	NETDVR_ERR_UPDATING,
	/* === remote file download error === */
	NETDVR_ERR_DOWNLOAD,
	NETDVR_ERR_FILEOPEN,
	NETDVR_ERR_USERSTOPPED,
	NETDVR_ERR_SERIAL_REOPEN,
	NETDVR_ERR_GET_LOCALMACADDR,
	NETDVR_ERR_SDK_CHECKFAILED,
	NETDVR_ERR_RERECIVE,
	NETDVR_ERR_SENDUPDATEFILE,
};

/* ========================== structure for the DVR information ============================= */
struct NETDVR_DeviceInfo_t
{
	unsigned int	deviceIP; 						//�豸IP  
	unsigned short	devicePort;						//�豸�˿� 
	char			device_name[32];				//�豸����
	char			device_mode[32];				//�豸�ͺ�
	unsigned char	maxChnNum;						//�����Ƶͨ����
	unsigned char	maxAudioNum;					//�����Ƶͨ����
	unsigned char	maxSubstreamNum;				//�����������
	unsigned char	maxPlaybackNum;					//���ط���
	unsigned char	maxAlarmInNum;					//��󱨾�������
	unsigned char	maxAlarmOutNum;					//��󱨾������
	unsigned char	maxHddNum;						//���Ӳ����(ʵ�ʲ�һ��������ô��Ӳ��)
	unsigned char	reserved[58];					//Ԥ��
}PACK_NO_PADDING;

/* ========================== structure for client's login information ============================= */
struct NETDVR_loginInfo_t
{
	char username[12];								//username for login
	char loginpass[12];								//password for login
	char macAddr[18];								//client mac address
	unsigned int ipAddr;							//client ip address
}PACK_NO_PADDING ;

struct NETDVR_VideoProperty_t 
{
	unsigned char	videoEncType;					//��Ƶ��������
	unsigned short	max_videowidth;
	unsigned short	max_videoheight;
	unsigned char	reserved[3];					//Ԥ��
}PACK_NO_PADDING ;

struct NETDVR_AudioProperty_t
{
	unsigned char	audioEnctype;					//Ԥ����Ƶ��������
	unsigned char	audioBitPerSample;				//Ԥ����Ƶλ��
	unsigned short	audioSamplePerSec;				//Ԥ����Ƶ������
	unsigned short	audioFrameSize;					//Ԥ����Ƶÿ֡��С
	unsigned short	audioFrameDurTime;				//Ԥ����Ƶÿ֡���
	unsigned char	reserved[4];					//Ԥ��
}PACK_NO_PADDING ;

struct NETDVR_VOIPProperty_t 
{
	unsigned char	VOIPBitPerSample;				//�����Խ�λ��
	unsigned short	VOIPSamplePerSec;				//�����Խ�������
	unsigned short	VOIPFrameSize;					//�����Խ�ÿ֡��С
	unsigned char	reserved[3];					//Ԥ��
}PACK_NO_PADDING ;

struct NETDVR_MDProperty_t 
{
	unsigned char	MDCol;							//�ƶ��������
	unsigned char	MDRow;							//�ƶ��������
	unsigned char	reserved[4];					//Ԥ��
}PACK_NO_PADDING ;

enum NETDVR_OVERLAP {
	NETDVR_OVERLAP_NO = 0,					//not overlap when disk(s) is(are) full
	NETDVR_OVERLAP_YES = 1,					//overlap when disk(s) is(are) full
};

enum NETDVR_DVRSTATUS {
	NETDVR_DVRSTATUS_HIDDEN = 0,			//don't display the DVR status 
	NETDVR_DVRSTATUS_DISPLAY = 1,			//display the DVR status
};

/* ========================== keybord lock time============================= */
enum NETDVR_LOCKTIME {
	NETDVR_LOCKTIME_NEVER = 0,				//never lock
	NETDVR_LOCKTIME_MIN_1 = 60,				//lock time equals 1 minute
	NETDVR_LOCKTIME_MIN_2 = 120,			//lock time equals 2 minutes
	NETDVR_LOCKTIME_MIN_5 = 300,			//lock time equals 5 minutes
	NETDVR_LOCKTIME_MIN_10 = 600,			//lock time equals 10 minutes
	NETDVR_LOCKTIME_MIN_20 = 1200,			//lock time equals 20 minutes
	NETDVR_LOCKTIME_MIN_30 = 1800,			//lock time equals 30 minutes
};

enum NETDVR_SWITCHTIME {
	NETDVR_SWITCHTIME_NEVER = 0,			//never switch
	NETDVR_SWITCHTIME_SEC_5 = 5,			//switch time equals 5 seconds
	NETDVR_SWITCHTIME_SEC_10 = 10,			//switch time equals 10 seconds
	NETDVR_SWITCHTIME_SEC_20 = 20,			//switch time equals 20 seconds
	NETDVR_SWITCHTIME_SEC_30 = 30,			//switch time equals 30 seconds
	NETDVR_SWITCHTIME_MIN_1 = 60,			//switch time equals 1 minute
	NETDVR_SWITCHTIME_MIN_2 = 120,			//switch time equals 2 minute
	NETDVR_SWITCHTIME_MIN_5 = 300,			//switch time equals 5 minute
};

enum NETDVR_VIDEOFORMAT {
	NETDVR_VIDEOFORMAT_PAL = 0,				//PAL video format
	NETDVR_VIDEOFORMAT_NTSC = 1,			//NTSC video format
};
//typedef enum NETDVR_VIDEOFORMAT video_format_t;



/* ========================== VGASOLUTION param structure =========================== */

struct NETDVR_VGAPROPERTY
{
	unsigned short		width;						//�ֱ��ʿ�
	unsigned short		height;						//�ֱ��ʸ�
	unsigned char		flashrate;					//�ֱ���ˢ����
}PACK_NO_PADDING ;

struct NETDVR_VGARESOLLIST
{
	struct NETDVR_VGAPROPERTY vgapro[16];
};

enum NETDVR_TRANSPARENCY {
	NETDVR_TRANSPARENCY_NO = 0,				//no transparency
	NETDVR_TRANSPARENCY_LOW = 1,			//low transparency
	NETDVR_TRANSPARENCY_MID = 2,			//middle transparency
	NETDVR_TRANSPARENCY_HIGH = 3,			//high transparency
};

enum NETDVR_LANGUAGE {
	NETDVR_LANGUAGE_SC = 0,					//simplified chinese
	NETDVR_LANGUAGE_EN = 1,					//english
	NETDVR_LANGUAGE_TC = 2,					//Traditional Chinese
};

/* ========================== sys param structure =========================== */
struct NETDVR_systemParam_t
{
	unsigned short	device_id;					//the dvr device id for ң����
	char			device_name[32];			//device name of dvr
	unsigned char	disable_pw;					//��������
	enum NETDVR_OVERLAP flag_overlap;			//flag_overlap=1, then overlap the disk when the disk is full. or if flag_overlap=0, then not overlap
	enum NETDVR_DVRSTATUS flag_statusdisp;		//decide whether to dispay system status or not
	enum NETDVR_LOCKTIME lock_time;				//lock time for keybord
	enum NETDVR_SWITCHTIME switch_time;			//switch time for preview mode
	enum NETDVR_VIDEOFORMAT video_format;		//PAL or NTSC ������Ч
	unsigned char vga_solution;					//vga�ֱ���
	enum NETDVR_TRANSPARENCY transparency;		//�˵�͸����
	enum NETDVR_LANGUAGE language;				//����
	
	char reserved[32];
}PACK_NO_PADDING ;

enum NETDVR_BAUDRATE {
	NETDVR_BAUDRATE_115200 = 0,	
	NETDVR_BAUDRATE_57600 = 1,
	NETDVR_BAUDRATE_38400 = 2,
	NETDVR_BAUDRATE_19200 = 3,
	NETDVR_BAUDRATE_9600 = 4,
	NETDVR_BAUDRATE_4800 = 5,
	NETDVR_BAUDRATE_2400 = 6,
	NETDVR_BAUDRATE_1200 = 7,
	NETDVR_BAUDRATE_300 = 8,
};

enum NETDVR_DATABITSEL {
	NETDVR_DATABITSEL_8 = 0,				//data bit: 8 bits
	NETDVR_DATABITSEL_7 = 1,				//data bit: 7 bits
	NETDVR_DATABITSEL_6 = 2,				//data bit: 6 bits
};

enum NETDVR_STOPBITSEL {
	NETDVR_STOPBITSEL_1 = 0,				//stop bit: 1 bit
	NETDVR_STOPBITSEL_2 = 1,				//stop bit: 2 bits
};

enum NETDVR_CHECK_TYPE {
	NETDVR_CHECK_NONE = 0,					//no check
	NETDVR_CHECK_ODD = 1,					//odd check
	NETDVR_CHECK_EVEN = 2,					//even check
};

enum NETDVR_FLOWCONTROL {
	NETDVR_FLOWCONTROL_NO = 0,				//no flow control
	NETDVR_FLOWCONTROL_HARDWARE = 1,		//flow control by hardware
	NETDVR_FLOWCONTROL_XON_XOFF = 2,		//flow control by Xon/Xoff
};

enum NETDVR_PROTOCOLTYPE {
	NETDVR_PROTOCOLTYPE_PELCO_D = 0,		//protocol type : Pelco-d
	NETDVR_PROTOCOLTYPE_PELCO_P = 1,		//protocol type : Pelco-p
	NETDVR_PROTOCOLTYPE_B01 = 2,			//protocol type : B01
	NETDVR_PROTOCOLTYPE_SAMSUNG = 3,		//protocol type : Samsung
};

#define MAX_PRESET_NUM 128					//preset limit 0-MAX_PRESET_NUM
#define MAX_CRUISE_PATH_NUM 16				//max cruise paths of each PTZ
#define MAX_CRUISE_POS_NUM 16				//max cruise positions of each cruise path
#define MAX_CRUISE_SPEED 9					//max cruise speed
#define MAX_DWELL_TIME 255					//max dwell time

enum NETDVR_YTTRACKCONTROL {
	NETDVR_YT_COM_STARTRECORDTRACK = 0,
	NETDVR_YT_COM_STOPRECORDTRACK = 1,
	NETDVR_YT_COM_STARTTRACK = 2,
	NETDVR_YT_COM_STOPTRACK = 3,	
};

enum NETDVR_YTPRESETPOINTCONTROL
{
	NETDVR_YT_COM_ADDPRESETPOINT = 0,
	NETDVR_YT_COM_DELPRESETPOINT = 1,
	NETDVR_YT_COM_TOPRESETPOINT = 2,
};

struct NETDVR_cruisePos_t
{
	unsigned char preset_no;				//1 ~ MAX_PRESET_NUM
	unsigned char dwell_time;				//1 ~ 255 secondes
	unsigned char cruise_speed;				//1 ~ MAX_CRUISE_SPEED(1 : the slowest speed)
	unsigned char cruise_flag;				//1-add 2-del
}PACK_NO_PADDING ;

struct NETDVR_cruisePath_t
{
	unsigned char chn;
	unsigned char path_no;										//1 ~ MAX_CRUISE_PATH_NUM	
	struct NETDVR_cruisePos_t cruise_pos[MAX_CRUISE_POS_NUM];	//cruise poisitions of cruise path
}PACK_NO_PADDING ;

struct NETDVR_ptzParam_t 
{
	unsigned char		chn;						//����ͷ������Ƶͨ��
	unsigned short		address;					//��̨��������ַ
	enum NETDVR_BAUDRATE baud_ratesel;
	enum NETDVR_DATABITSEL data_bitsel;
	enum NETDVR_STOPBITSEL stop_bitsel;
	enum NETDVR_CHECK_TYPE check_type;
	enum NETDVR_FLOWCONTROL flow_control;
	enum NETDVR_PROTOCOLTYPE protocol;
	unsigned int copy2Chnmask;						//��ǰ�������Ƶ�����ͨ�� ��λ
	char reserved[32];
}PACK_NO_PADDING ;

enum NETDVR_PTZCONTROL {
	NETDVR_PTZ_COM_STOP = 0,
	NETDVR_PTZ_COM_MOVEUP = 1,
	NETDVR_PTZ_COM_MOVEDOWN = 2,
	NETDVR_PTZ_COM_MOVELEFT = 3,
	NETDVR_PTZ_COM_MOVERIGHT = 4,
	NETDVR_PTZ_COM_ROTATION = 5,
	NETDVR_PTZ_COM_ZOOMADD = 6,
	NETDVR_PTZ_COM_ZOOMSUBTRACT = 7,
	NETDVR_PTZ_COM_FOCUSADD = 8,
	NETDVR_PTZ_COM_FOCUSSUBTRACT = 9,
	NETDVR_PTZ_COM_APERTUREADD = 10,
	NETDVR_PTZ_COM_APERTURESUBTRACT = 11,
	NETDVR_PTZ_COM_LIGHTINGOPEN = 12,
	NETDVR_PTZ_COM_LIGHTINGCLOSE = 13,
	NETDVR_PTZ_COM_WIPERSOPEN = 14,
	NETDVR_PTZ_COM_WIPERSCLOSE = 15,
	NETDVR_PTZ_COM_FAST = 16,
	NETDVR_PTZ_COM_NORMAL = 17,
	NETDVR_PTZ_COM_SLOW = 18,
	NETDVR_PTZ_COM_AUXILIARYOPEN = 19,
	NETDVR_PTZ_COM_AUXILIARYCLOSE = 20,
};

struct NETDVR_PtzCtrl_t
{
	unsigned char	chn;						//ͨ��
	enum NETDVR_PTZCONTROL  cmd;				//����
	unsigned char	aux;						//��������
}PACK_NO_PADDING ;

#define NETDVR_VIDEO_LOST_DECT 0			//dect video lost
#define NETDVR_VIDEO_LOST_UNDECT 1			//not dect video lost

#define NETDVR_VIDEO_MD_CLOSE 0				//not dect video motion
#define NETDVR_VIDEO_MD_SEN_LOWEST 1		//dect video motion in a lowest sensitivity
#define NETDVR_VIDEO_MD_SEN_LOWER 2			//dect video motion in a lower sensitivity
#define NETDVR_VIDEO_MD_SEN_LOW 3			//dect video motion in a low sensitivity
#define NETDVR_VIDEO_MD_SEN_HIGH 4			//dect video motion in a high sensitivity
#define NETDVR_VIDEO_MD_SEN_HIGHER 5		//dect video motion in a higher sensitivity
#define NETDVR_VIDEO_MD_SEN_HIGHEST 6		//dect video motion in a highest sensitivity



struct NETDVR_videoParam_t
{
	unsigned char		channel_no;					//ͨ����
	char				channelname[32];			//ͨ����
	unsigned char		flag_name;					//����λ����ʾ
	unsigned short		chnpos_x;					//����x����
	unsigned short		chnpos_y;					//����y����
	unsigned char		flag_time;					//ʱ��λ����ʾ
	unsigned short		timepos_x;					//ʱ��x����
	unsigned short		timepos_y;					//ʱ��y����
	unsigned char		flag_mask;					//�ڸ�
	
	//�ڸ�����;R8016ÿһ����Ƶͨ��֧��2���ڸ�����;��˼ϵ��֧��4��
	struct net_maskAREA_t
	{
		unsigned short	 	x;
		unsigned short		y;
		unsigned short		width;
		unsigned short		height;
	}maskinfo[4];
	
	//handler
	unsigned char		flag_safechn;				//��ȫͨ����ǣ���������
	unsigned int		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

typedef enum NETDVR_FLOW_TYPE {
	NETDVR_FLOW_VIDEO = 0,						//video flow
	NETDVR_FLOW_MUTI,							//multiple flow(both video and audio)
} flow_type_t;

typedef enum NETDVR_BITRATE_TYPE {
	NETDVR_BITRATE_FIXED = 0,					//fixed bit rate
	NETDVR_BITRATE_VARIABLE,					//variable bit rate
} bitrate_type_t;

typedef enum NETDVR_BITRATE {
	NETDVR_BITRATE_64 = 64,					//64kbps
	NETDVR_BITRATE_128 = 128,					//128kbps
	NETDVR_BITRATE_256 = 256,					//256kbps
	NETDVR_BITRATE_384 = 384,					//384kbps
	NETDVR_BITRATE_512 = 512,					//512kbps
	NETDVR_BITRATE_768 = 768,					//768kbps
	NETDVR_BITRATE_1024 = 1024,				//1Mbps
	NETDVR_BITRATE_1536 = 1536,				//1.5Mbps
	NETDVR_BITRATE_2048 = 2048,				//2Mbps
} bitrate_t;

typedef enum NETDVR_VIDEO_QUALITY {
	NETDVR_VIDEO_QUALITY_BEST = 0,				//best video quality
	NETDVR_VIDEO_QUALITY_BETTER,				//better video quality
	NETDVR_VIDEO_QUALITY_GOOD,					//good video quality
	NETDVR_VIDEO_QUALITY_NORMAL,				//normal video quality
	NETDVR_VIDEO_QUALITY_BAD,					//bad video quality
	NETDVR_VIDEO_QUALITY_WORSE,				//worse video quality
} video_quality_t;

typedef enum NETDVR_FRAMERATE {
	NETDVR_FRAMERATE_25 = 25,					//25f/s
	NETDVR_FRAMERATE_20 = 20,					//20f/s
	NETDVR_FRAMERATE_15 = 15,					//15f/s
	NETDVR_FRAMERATE_10 = 10,					//10f/s
	NETDVR_FRAMERATE_5 = 5,					//5f/s
	NETDVR_FRAMERATE_2 = 2,					//2f/s
	NETDVR_FRAMERATE_1 = 1,					//1f/s
} framerate_t;

typedef enum NETDVR_PRERECORD_TIME {
	NETDVR_PRERECORD_TIME_0 = 0,				//do not pre-record
	NETDVR_PRERECORD_TIME_5 = 5,					//pre-record time: 5s
	NETDVR_PRERECORD_TIME_10 = 10,					//pre-record time: 10s
	NETDVR_PRERECORD_TIME_15 = 15,					//pre-record time: 15s
	NETDVR_PRERECORD_TIME_20 = 20,					//pre-record time: 20s
	NETDVR_PRERECORD_TIME_25 = 25,					//pre-record time: 25s
	NETDVR_PRERECORD_TIME_30 = 30,					//pre-record time: 30s
} prerecord_time_t;

typedef enum NETDVR_POSTRECORD_TIME {
	NETDVR_POSTRECORD_TIME_5 = 5,					//post-record time: 5s
	NETDVR_POSTRECORD_TIME_10 = 10,					//post-record time: 10s
	NETDVR_POSTRECORD_TIME_30 = 30,					//post-record time: 30s
	NETDVR_POSTRECORD_TIME_60 = 60,					//post-record time: 60s
	NETDVR_POSTRECORD_TIME_120 = 120,				//post-record time: 120s
	NETDVR_POSTRECORD_TIME_300 = 300,				//post-record time: 300s
	NETDVR_POSTRECORD_TIME_600 = 600,				//post-record time: 600s
} postrecord_time_t;

struct NETDVR_recordParam_t
{
	unsigned char		channelno;					//ͨ����
	flow_type_t			code_type;					//see flow_type_t
	bitrate_type_t		bit_type;					//see bitrate_type_t
	bitrate_t			bit_max;					//see bitrate_t
	unsigned short		intraRate;					//�ؼ�֡���
	unsigned char		qi;							//�ؼ�֡��������
	unsigned char		minQ;						//��С��������
	unsigned char		maxQ;						//�����������
	video_quality_t		quality;					//see video_quality_t
	framerate_t			frame_rate;					//see framerate_t
	prerecord_time_t	pre_record;					//see prerecord_time_t
	postrecord_time_t	post_record;				//see postrecord_time_t
	
	unsigned int		copy2chnmask;
	unsigned char		supportdeinter;				//ֻ�����Ƿ�֧��deinter���� 1�� 0�� (����)
	unsigned char		deinterval;					//deinterǿ�� 0-4 ���ã������У�ǿ����ǿ
	unsigned char		supportResolu;				//ֻ�����Ƿ�֧������¼��ֱ���
	unsigned char		resolutionpos;				//�ֱ���ѡ��ֵ, ȡֵ��NETDVR_VideoResolu_list_t
	unsigned char		reserved1[12];				//�����ֶ�	
}PACK_NO_PADDING ;

//��/�������ֱ����б� CTRL_GET_RESOLUTION_LIST
#define NETDVR_VIDEORESOLU_BEGIN	1 //��1��ʼ
#define NETDVR_VIDEORESOLU_QCIF		1 //QCIF
#define NETDVR_VIDEORESOLU_CIF		2 //CIF
#define NETDVR_VIDEORESOLU_HD1		3 //HD1
#define NETDVR_VIDEORESOLU_D1		4 //D1
#define NETDVR_VIDEORESOLU_END		NETDVR_VIDEORESOLU_D1 //��NETDVR_VIDEORESOLU_D1�������������ͨ���ж��Ƿ���BEGIN��END֮����ȷ���Ƿ���ȷ
struct  NETDVR_VideoResolu_list_t
{
	unsigned char chn;	//ͨ��
	unsigned char type;	// 0 ������ 1 ������
	unsigned char videoresolu[8];	//�ֱ����б� δʹ�õ���0 VIDEORESOLU_BEGIN ~ VIDEORESOLU_END
}PACK_NO_PADDING ;

/* ========================== ���������� ============================= */
typedef enum NETDVR_SUBFLAG 
{
	NETDVR_SUBFLAG_CIF = 0,
	NETDVR_SUBFLAG_QCIF = 1,	
}subflag_t;

typedef enum NETDVR_SUBBITTYPE {
	NETDVR_SUBBITRATE_FIXED = 0,					//fixed bit rate
	NETDVR_SUBBITRATE_VARIABLE,					//variable bit rate
} subbittype_t;

typedef enum NETDVR_SUBVIDEOQUALITY {
	NETDVR_SUBVIDEOQUALITY_BEST = 0,				//best video quality
	NETDVR_SUBVIDEOQUALITY_BETTER,				//better video quality
	NETDVR_SUBVIDEOQUALITY_GOOD,					//good video quality
	NETDVR_SUBVIDEOQUALITY_NORMAL,				//normal video quality
	NETDVR_SUBVIDEOQUALITY_BAD,					//bad video quality
	NETDVR_SUBVIDEOQUALITY_WORSE,				//worse video quality
} subvideoquality_t;

typedef enum NETDVR_SUBFRAMERATE {
	NETDVR_SUBFRAMERATE_25 = 25,					//25f/s
	NETDVR_SUBFRAMERATE_20 = 20,					//20f/s
	NETDVR_SUBFRAMERATE_15 = 15,					//15f/s
	NETDVR_SUBFRAMERATE_10 = 10,					//10f/s
	NETDVR_SUBFRAMERATE_5 = 5,						//5f/s
	NETDVR_SUBFRAMERATE_2 = 2,						//2f/s
	NETDVR_SUBFRAMERATE_1 = 1,						//1f/s
} subframerate_t;

typedef enum NETDVR_SUBBITRATE {
	NETDVR_SUBBITRATE_64 = 64,					//64kbps
	NETDVR_SUBBITRATE_128 = 128,					//128kbps
	NETDVR_SUBBITRATE_256 = 256,					//256kbps
	NETDVR_SUBBITRATE_384 = 384,					//384kbps
	NETDVR_SUBBITRATE_512 = 512,					//512kbps
	NETDVR_SUBBITRATE_768 = 768,					//768kbps
	NETDVR_SUBBITRATE_1024 = 1024,				//1Mbps
	NETDVR_SUBBITRATE_1536 = 1536,				//1.5Mbps
	NETDVR_SUBBITRATE_2048 = 2048,				//2Mbps
} subbitrate_t;

struct NETDVR_SubStreamParam_t								
{
	unsigned char		chn;
	subflag_t			sub_flag;					//���ֶ������������� 0 cif 1 qcif
	subbittype_t		sub_bit_type;				//������λ������
	unsigned short		sub_intraRate;				//�ؼ�֡���
	unsigned char		sub_qi;						//�ؼ�֡��������
	unsigned char		sub_minQ;					//��С��������
	unsigned char		sub_maxQ;					//�����������
	subvideoquality_t	sub_quality;				//������ͼ������
	subframerate_t 		sub_framerate;				//��������֡��
	subbitrate_t 		sub_bitrate;				//��������λ��
	unsigned int		copy2chnmask;				//���Ƶ�����ͨ����ÿһλһ��ͨ��
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

typedef enum NETDVR_ALARMINTYPE {
	NETDVR_ALARMIN_HIGH = 1,				//high level alarm input
	NETDVR_ALARMIN_LOW,						//low level alarm input
} alarmintype_t;

typedef enum NETDVR_ALARMOUTYPE {
	NETDVR_ALARMOUT_NO = 1,					//alarm output type:normal open
	NETDVR_ALARMOUT_NC,						//alarm output type:normal close
} alarmouttype_t;

typedef enum NETDVR_DELAY_TIME {
	NETDVR_DELAY_5 = 5,					//5s
	NETDVR_DELAY_10=10,					//10s
	NETDVR_DELAY_30=30,					//30s
	NETDVR_DELAY_60=60,					//60s
	NETDVR_DELAY_120=120,				//120s
	NETDVR_DELAY_300=300,				//300s
	NETDVR_DELAY_MANUAL=0xffff, 		//manual
} delay_t;

struct AlarmInPtz								//PTZ����
{
	unsigned char		flag_preset;			//Ԥ�õ�
	unsigned char		preset;
	unsigned char		flag_cruise;			//Ѳ����
	unsigned char		cruise;
	unsigned char		flag_track;				//�켣
}PACK_NO_PADDING ;

struct NETDVR_alarmInParam_t
{
	unsigned char		inid;						//����������
	unsigned char		flag_deal;					//1: deal with input alarm ; 0: for no
	alarmintype_t		typein;						//input alarm type:�͵�ƽor�ߵ�ƽ���� see alarmintype_t
	unsigned int		triRecChn;					//����ͨ��¼��ÿһλһͨ��
	unsigned int		triAlarmoutid;				//���������������λ
	unsigned char		flag_buzz;					//����������
	unsigned char		flag_email;					//����emaill����
	unsigned char		flag_mobile;				//�����ֻ�����
	delay_t				delay;						//���������ʱ
	unsigned int		copy2AlarmInmask;			//��ǰ�������Ƶ�������������ͨ������λ
	struct AlarmInPtz	alarmptz[32];				//����PTZ����
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

struct NETDVR_alarmOutParam_t
{
	unsigned char		outid;						//�������ͨ�� 
	alarmouttype_t		typeout;					//�����������:������or������ see alarmouttype_t
	unsigned int		copy2AlarmOutmask;			//���Ƶ����������������λ
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

struct NETDVR_AlarmNoticeParam_t									
{
	char			alarm_email[32];				//����email��ַ
	char			alarm_mobile[32];				//�����ֻ�����
	unsigned char	reserved[32];					//�����ֶ�
}PACK_NO_PADDING ;

struct NETDVR_networkParam_t
{
	char				mac_address[18];			//mac��ַ
	unsigned int		ip_address;					//ip��ַ
	unsigned short		server_port;				//�豸�˿�
	unsigned int		net_mask;					//����
	unsigned int		net_gateway;				//����
	unsigned int		dns;						//dns
	unsigned char		flag_multicast;				//�鲥���ñ��
	unsigned int		multicast_address;			//�鲥��ַ
	unsigned short		http_port;					//http�˿�
	unsigned char		flag_pppoe;					//pppoe���ñ��
	char				pppoe_user_name[64];		//pppoe�û���
	char				pppoe_passwd[64];			//pppoe����
	unsigned char		flag_dhcp;					//dhcp���ñ�־
	unsigned char		ddnsserver;					//ddns������
	unsigned char		flag_ddns;					//ddns���ñ�־
	char				ddns_domain[64];			//ddns����
	char				ddns_user[64];				//ddns�û���
	char				ddns_passwd[64];			//ddns����
	unsigned int		centerhost_address;			//���ķ�������ַ
	unsigned short		centerhost_port;			//���ķ������˿�
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

typedef enum NETDVR_TimeFormat_T
{
	NETDVR_TF_YYYYMMDD = 0,
	NETDVR_TF_MMDDYYYY = 1,
}timeFormat_t;

//ϵͳʱ��
struct NETDVR_SysTime_t
{
	unsigned int		systemtime;					//ϵͳʱ��
	timeFormat_t		format;						//ʱ���ʽ ѡ��ֵ
	unsigned char		flag_time;					//Ԥ��ʱ��λ����ʾ
	unsigned short		timepos_x;					//Ԥ��ʱ��x����
	unsigned short		timepos_y;					//Ԥ��ʱ��y����
}PACK_NO_PADDING ;

/* ========================== Motion Detect structures =========================== */

#define NETDVR_MD_MIN_SENSE	0						
#define NETDVR_MD_MAX_SENSE	5

//�ƶ����
struct NETDVR_motionDetection_t 
{
	unsigned char	chn;
	unsigned int 	trigRecChn;						//����ͨ��¼�� ��λ
	unsigned int 	trigAlarmOut;					//����������� ��λ
	unsigned char	flag_buzz;						//����������
	unsigned char	flag_email;						//����emaill����
	unsigned char	flag_mobile;					//�����ֻ�����
	unsigned char	sense;							//�ƶ����������:0-�����;1-5������,ֵԽ��Խ����
	delay_t			delay;							//��ʱ
	unsigned char	block[44*36];					//�ƶ��������:ÿһ���ֽڴ���һ������,1����ѡ��,0����ѡ��
	unsigned char	reserved[16];					//Ԥ��
}PACK_NO_PADDING ;

//��Ƶ��ʧ
struct NETDVR_VideoLostParam_t
{
	unsigned char		chn;
	unsigned int 		trigRecChn;					//����ͨ��¼�� ��λ
	unsigned int 		trigAlarmOut;				//����������� ��λ
	unsigned char 		flag_buzz;					//����������
	unsigned char		flag_email;					//����emaill����
	unsigned char		flag_mobile;				//�����ֻ�����
	delay_t 			delay;						//��ʱ
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

//��Ƶ�ڵ�
struct NETDVR_VideoBlockParam_t
{
	unsigned char		chn;
	unsigned int 		trigRecChn;					//����ͨ��¼�� ��λ
	unsigned int 		trigAlarmOut;				//����������� ��λ
	unsigned char 		flag_buzz;					//����������
	unsigned char		flag_email;					//����emaill����
	unsigned char		flag_mobile;				//�����ֻ�����
	delay_t				delay;						//��ʱ
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

/* ========================== remote HDD INFO structures=========================== */
struct NETDVR_hddInfo_t 
{
	unsigned char		hdd_index;					//Ӳ�����
	unsigned char		hdd_exist;  				//1:����Ӳ��; 0:Ӳ��û�н���
	unsigned int		capability;					//MB
	unsigned int		freesize;					//MB
	unsigned char		reserved[2];				//Ԥ��
}PACK_NO_PADDING ;

/* ========================== remote System Version info structures=========================== */
struct NETDVR_SysVerInfo_t
{
	char devicename[32];
	char devicemodel[32];
	char deviceser[32];
	char version[64];
}PACK_NO_PADDING ;

/*=========================remote pic adjust============================*/
enum NETDVR_PICADJUST {
	NETDVR_PIC_BRIGHTNESS = 0,					
	NETDVR_PIC_CONTRAST,
	NETDVR_PIC_HUE,
	NETDVR_PIC_SATURATION,
};

struct NETDVR_PICADJUST_T
{
	unsigned char			channel_no;				//ͨ����
	enum NETDVR_PICADJUST	flag;					//���ڱ�־:0-3
	unsigned char			val;					//����ֵ
	unsigned int			copy2chnmask;			//���Ƶ�����ͨ����ÿһλһ��ͨ��
}PACK_NO_PADDING ;

typedef enum NETDVR_WEEKDAY {
	NETDVR_WEEKDAY_1 = 0,							//Monday
	NETDVR_WEEKDAY_2,							//Tuesday
	NETDVR_WEEKDAY_3,							//Wednesday
	NETDVR_WEEKDAY_4,							//Thursday
	NETDVR_WEEKDAY_5,							//Friday
	NETDVR_WEEKDAY_6,							//Saturday
	NETDVR_WEEKDAY_7,							//Sunday
} weekday_t;


/* ======================================================= */
struct RecTimeField_t
{
	unsigned int	starttime;						//��ʼʱ��
	unsigned int	endtime;						//��ֹʱ��
	unsigned char	flag_sch;						//��ʱ¼��
	unsigned char 	flag_md;						//�ƶ����¼��
	unsigned char	flag_alarm;						//����¼��
	unsigned char	reserved[4];					//�����ֶ�
}PACK_NO_PADDING ;

//¼�񲼷�
struct NETDVR_RecordSCH_t
{
	unsigned char		chn;						//ͨ��
	enum NETDVR_WEEKDAY	weekday;					//����
	
	struct RecTimeField_t recTimeFieldt[4];
	/*struct RecTimeField_t
	{
		unsigned int	starttime;					//��ʼʱ��
		unsigned int	endtime;					//��ֹʱ��
		unsigned char	flag_sch;					//��ʱ¼��
		unsigned char 	flag_md;					//�ƶ����¼��
		unsigned char	flag_alarm;					//����¼��
		unsigned char	reserved[4];				//�����ֶ�
	}recTimeFieldt[4];*/
	
	unsigned char		copy2Weekday;				//���Ƶ�һ�ܵ������� ��λ 
	unsigned int		copy2Chnmask;				//���Ƶ�����ͨ�� ��λ
	unsigned char		reserved[16];				//�����ֶ�
}PACK_NO_PADDING ;

//�ֶ�¼��
struct NETDVR_ManualRecord_t
{
	unsigned int  chnRecState;					//ͨ���ֶ�¼��״̬ ��λ	
}PACK_NO_PADDING ;

/* ==================== user control structure ======================== */
struct NETDVR_userInfo_t 
{
	char	name[12];
	char	password[12];
	
	char	mac_address[18];
	
	/* 1:open�� 0:close */
	unsigned char		rcamer;						//remote yuntai
	unsigned char		rrec;						//remote record
	unsigned char		rplay;						//remote playback
	unsigned char		rsetpara;					//remote set param
	unsigned char		rlog;						//remote get log
	unsigned char		rtool;						//remote use tool
	unsigned char		rpreview;					//remote preview
	unsigned char		ralarm;						//remote alarm
	unsigned char		rvoip;						//voip
	unsigned char		lcamer;						//local yuntai
	unsigned char		lrec;						//local record
	unsigned char		lplay;						//local palyback
	unsigned char		lsetpara;					//local set param
	unsigned char		llog;						//local log
	unsigned char		ltool;						//local tool
}PACK_NO_PADDING ;

struct NETDVR_UserNumber_t 
{
	struct NETDVR_userInfo_t userinfo[8];
}PACK_NO_PADDING ;

struct NETDVR_AlarmUploadState_t
{
	//0-�ź�������,1-Ӳ����,2-�źŶ�ʧ,3���ƶ����,4��Ӳ��δ��ʽ��,
	//5-��дӲ�̳���,6-�ڵ�����,7-��ʽ��ƥ��, 8-�Ƿ�����
	unsigned char	type;		
	unsigned char	state;			//1���� 2�ָ�
	unsigned char	id;				//ͨ��,Ӳ�̺�,���������,ȡ����type 
	unsigned short	reserved1;		//Ԥ��
	unsigned int	reserved2;		//Ԥ��	
}PACK_NO_PADDING ;

enum NETDVR_REC_INDEX_MASK {
	NETDVR_REC_INDEX_TIMER = 0x1,
	NETDVR_REC_INDEX_MD = 0x2,
	NETDVR_REC_INDEX_ALARM = 0x4,
	NETDVR_REC_INDEX_HAND = 0x8,
	/*��ʱ���ƶ���⡢�������ֶ�¼����Խ��л����*/
	NETDVR_REC_INDEX_ALL = 0x10,//ȫ������¼��
};

//¼���ļ���������
struct NETDVR_recfileSearchCond_t
{
	unsigned short chn_mask;				//¼��ͨ������λ:ĳһΪ1��Ӧ��ͨ��ѡ��,��Ҫ������ͨ����¼������
	unsigned short type;					//NETDVR_REC_INDEX_MASK
	unsigned int start_time;				//��"1970-01-01 00:00:00"��ʼ������
	unsigned int end_time;					//��"1970-01-01 00:00:00"��ʼ������
	//unsigned short startID;
	//unsigned short max_return;
	unsigned char reserved[4];
}PACK_NO_PADDING ;

#if 0
#define MAX_FRAME_NUM_PER_SEQ  (30)
typedef struct{
	unsigned int timestamp;
	unsigned int video_frame_num:8;
	unsigned int audio_frame_num:8;
	unsigned int reserved:16;
	unsigned short video_frame_len[MAX_FRAME_NUM_PER_SEQ];
}key_info_t;
#define MAX_KEY_NUM_PER_SEG  (5000)
typedef struct{
	/*ý����Ϣ1*/
	unsigned int video_type:8;
	unsigned int video_resolution:8;
	unsigned int is_cbr:1;
	unsigned int bitrate:15; /*kbps, VBR��ʾ�������, CBR��ʾƽ������*/
	/*ý����Ϣ2*/
	unsigned int is_pal:1;
	unsigned int frame_rate:7; /*0 --��ʾȫ֡��*/
	unsigned int audio_type:8;
	unsigned int audio_frame_len:16; /*��Ƶ�������ݣ�bytes*/
	/*��Ƶ֡������Ϣ*/
	unsigned int key_num:16;
	unsigned int reserved:16;
	key_info_t  key_info[MAX_KEY_NUM_PER_SEG];
}replay_desc_t;
#else
typedef struct{
	/*ý����Ϣ1*/
	unsigned int video_type:8;
	unsigned int video_resolution:8;
	unsigned int is_cbr:1;
	unsigned int bitrate:15; /*kbps, VBR��ʾ�������, CBR��ʾƽ������*/
	
	/*ý����Ϣ2*/
	unsigned int is_pal:1;
	unsigned int frame_rate:7; /*0 --��ʾȫ֡��*/
	unsigned int audio_type:8;
	unsigned int audio_frame_len:16; /*��Ƶ�������ݣ�bytes*/
	
	/*��Ƶ֡������Ϣ*/
	//unsigned int key_num:16;
	//unsigned int reserved:16;
	//key_info_t  key_info[MAX_KEY_NUM_PER_SEG];
	
	unsigned int video_frames;//��Ƶ��֡��
	unsigned int audio_frames;//��Ƶ��֡��
}PACK_NO_PADDING replay_desc_t;

/*typedef struct{
	unsigned int timestamp;//ʱ���
	unsigned int key:8;//1:I֡;0:P֡
	unsigned int frame_len:24;//֡����
}video_desc_t;

typedef struct{
	unsigned int timestamp;//ʱ���
}audio_desc_t;*/

typedef struct{
	unsigned int timestamp;//ʱ���
	unsigned int media:1;//0:��Ƶ;1:��Ƶ
	unsigned int key:7;//0:P֡;��0:�ؼ�֡
	unsigned int frame_len:24;//֡����
}PACK_NO_PADDING media_desc_t;
#endif

struct NETDVR_recFileInfo_t
{
	unsigned char chn;						//¼��ͨ��:0-15
	unsigned char type;						//NETDVR_REC_INDEX_MASK
	unsigned int start_time;				//start time of the record file:��"1970-01-01 00:00:00"��ʼ������
	unsigned int end_time;					//end time of the record file:��"1970-01-01 00:00:00"��ʼ������
	unsigned char image_format;				//frame type:3(Pal-cif) ; 4(Pal-D1); 8(NTSC-cif); 9(NTSC-D1)
	unsigned char stream_flag;				//stream flag:0 for video stream ; 1 for video and audio stream
	unsigned int size;						//¼���ļ���С
	unsigned int offset;					//¼���ļ���128M�ļ������е�ƫ��(�ط�ʱҪ����ò���)
	char filename[64];						//¼���ļ���
	//struct NETDVR_recFileInfo_t *pnext;	//poiter reference to the next record file information
}PACK_NO_PADDING ;

//¼���ļ��������
struct NETDVR_recFileSearchResult_t
{
	//unsigned short sum;					//totals of remote indexed files.
	//unsigned short startID;				//if no file is indexed, startID will be 0, or it'll be a value based on index condition's startID(struct NETDVR_fileSearchCondition_t)
	//unsigned short endID;					//if startID isn't 0, then (endID - startID + 1) files is indexed.
	//struct NETDVR_recFileInfo_t *precInfo;//if the first file of all indexed files.
	struct NETDVR_recFileInfo_t *precArray;	//����¼���ļ���Ϣ������
	unsigned short arraySize;				//����¼���ļ���Ϣ������Ĵ�С
	unsigned short realSize;				//ʵ�����������ļ���¼������
}PACK_NO_PADDING ;

struct NETDVR_progressParam_t
{
	unsigned int curr_pos;
	unsigned int total_size;
}PACK_NO_PADDING ;

//���������
struct NETDVR_NRServer_t
{
	unsigned int nrserverip; //��������ַ
	unsigned short serverport; //�������˿�
	unsigned short databindport; //���ݰ󶨶˿�
	unsigned char reserved[16]; //Ԥ��
}PACK_NO_PADDING ;

//����
struct NETDVR_AlarmVal_t
{
	unsigned char alarmid;		//alarmid 0��ʼ
	unsigned char  val;			//ȡֵ 0δ���� 1����
	unsigned char reserved[2];	//Ԥ��
}PACK_NO_PADDING ;

struct NETDVR_TimePlayCond_t
{
	unsigned char chn;						//¼��ͨ��:0-15
	unsigned short type;					//NETDVR_REC_INDEX_MASK
	unsigned int start_time;				//��"1970-01-01 00:00:00"��ʼ������
	unsigned int end_time;					//��"1970-01-01 00:00:00"��ʼ������
}PACK_NO_PADDING ;

//������Ƶ��ʽ
typedef enum NETDVR_FMT_TYPE {
	NETDVR_FMT_RGB24 = 2,					//rgb24 format
		NETDVR_FMT_RGB32 = 4,					//rgb32 format
		NETDVR_FMT_YV12 = 6,					//yv12 format
		NETDVR_FMT_I420 = 8,					//i420 format
		NETDVR_FMT_YUY2 = 10,					//yuy2 format(snapshot is not supported currently)
} fmt_type_t;

//��������֡�ṹ
typedef struct FrameHeadrDec
{
	unsigned char mediaType;			//original encoded (video/audio) media type:
	char reserved1[3];					//reserved
	void *data;							//decoded data buf
	unsigned int data_size;				//decoded data length
	char reserved2[32];					//reserved for extensible development
	union
	{
		struct{
			fmt_type_t fmt;				//decoded format
			unsigned short width;		//video width
			unsigned short height;		//video height
		} video_param;
		unsigned char audio_mode;		//8, 16, or 32 bit
	};
} FrameHeadrDec;

struct NETDVR_xwServer_t
{
	u8 		flag_server;
	u32 	ip_server;
	u16 	port_server;
	u16 	port_download;
	char	device_serial[32];
	char	device_passwd[32];
	u8		flag_verifytime;
}PACK_NO_PADDING ;
#pragma pack( pop )

/* =============== callback function type for user register  ============= */
typedef void (*PFUN_MSGHASAUDIO_T)(unsigned char b_has_audio, unsigned int dwContent);
typedef void (*PFUN_PROGRESS_T)(struct NETDVR_progressParam_t progress, unsigned int dwContent);
typedef void (*pFrameCallBack)(pFrameHeadr pFrmHdr, unsigned int dwContext);
typedef void (*PFUN_MSG_T)(unsigned int dwContent);
typedef void (*PFUN_ALARMSTATE_T)(struct NETDVR_AlarmUploadState_t alarmstate, unsigned int dwContent);
typedef FrameHeadrDec* pFrameHeadrDec;
typedef void (* pDecFrameCallBack)(pFrameHeadrDec pFrmHdrDec, unsigned int dwContext);

#ifdef __cplusplus
extern "C"{
#endif

/* =============== NETDVR user function definetion  ============= */

int DVRSDK_startup(int *pHandle);
int DVRSDK_cleanup(int Handle);

int DVRSDK_openVideoChannel(int Handle, unsigned char chn, pFrameCallBack pCBFun, unsigned int dwContent);
int DVRSDK_closeVideoChannel(int Handle, unsigned char chn);

int DVRSDK_openSubVideoChannel(int Handle, unsigned char chn, pFrameCallBack pCBFun, unsigned int dwContent);
int DVRSDK_closeSubVideoChannel(int Handle, unsigned char chn);

int DVRSDK_openAudioChannel(int Handle, unsigned char chn, pFrameCallBack pCBFun, unsigned int dwContent);
int DVRSDK_closeAudioChannel(int Handle, unsigned char chn);

//�طŻ��������������ļ�
//flag: 0 - �ط� ; 1 - ���� �ط�������Ψһ���������ڻط�ʱDVR�˵�Ӧ�ó�������¼���ļ���ʱ�������ʱ���ƶ����ز���Ҫ��ʱ
//p_player_index:�������,����طž��
//�طŽ���ʱSDKĬ�ϴ���Ϊ����DVRSDK_stopFilePlay����,��Ҳ��������;����DVRSDK_stopFilePlayֹͣ�ط�
int DVRSDK_startFilePlay(int Handle, int *p_player_index, const struct NETDVR_recFileInfo_t *pFileInfo, unsigned char flag, pFrameCallBack pCBFun, unsigned int dwContent, PFUN_MSG_T p_over_func, unsigned int dwOverContent);
//ֹͣ�ļ��ط�
int DVRSDK_stopFilePlay(int Handle, int player_index);

//��ʱ��طŻ�����
//�طŽ���ʱSDKĬ�ϴ���Ϊ����DVRSDK_stopTimePlay����,��Ҳ��������;����DVRSDK_stopTimePlayֹͣ�ط�
int DVRSDK_startTimePlay(int Handle, int *p_player_index, const struct NETDVR_TimePlayCond_t *prfs, unsigned char flag, pFrameCallBack pCBFun, unsigned int dwContent, PFUN_MSG_T p_over_func, unsigned int dwOverContent);
//ֹͣʱ��ط�
int DVRSDK_stopTimePlay(int Handle, int player_index);

typedef void (*pDowloadDataCBFunc)(int Handle, unsigned char* databuf, unsigned int datalen, unsigned int content);
int NETDVR_startFileDownload(int Handle, const struct NETDVR_recFileInfo_t *pFileInfo, pDowloadDataCBFunc pcbfUNC, unsigned int content);
int NETDVR_stopFileDownload(int Handle);

//¼���ļ�����
int NETDVR_recFilesSearch(int Handle, const struct NETDVR_recfileSearchCond_t *prfs, struct NETDVR_recFileSearchResult_t *pdesc);

//ע�ᱨ���ϴ��ص�����
int NETDVR_regCBAlarmState(int Handle, PFUN_ALARMSTATE_T p_cb_func, unsigned int dwContent);

//������ֹͣ�����ϴ�
//uploadflag:1-����;0-ֹͣ
int NETDVR_SetAlarmUpload(int Handle, const unsigned char uploadflag);

//���á���ȡ���ķ���������
//�ض��ͻ�����
int NETDVR_setxwServerParams(int Handle, const struct NETDVR_xwServer_t *pxwServ);
int NETDVR_getxwServerParams(int Handle, struct NETDVR_xwServer_t *pxwServ);

//���á���ȡϵͳͨ�ò���
int NETDVR_setSystemParams(int Handle, const struct NETDVR_systemParam_t *pSysPara);
int NETDVR_getSystemParams(int Handle, struct NETDVR_systemParam_t *pSysPara);

//�õ�VGA�ֱ����б�,Ŀǰ���ǵ�DVR�豸һ��֧��800*600/1024*768/1280*1024���ֱַ���
int NETDVR_GetVGAResolutonList(int Handle, struct NETDVR_VGARESOLLIST *pvgasol);

//���á���ȡ�ƾ����Ʋ���
int NETDVR_setPtzParams(int Handle, const struct NETDVR_ptzParam_t *ptzParam);
int NETDVR_getPtzParams(int Handle, unsigned char chn, struct NETDVR_ptzParam_t *p_ptz_param);

//���á���ȡѲ������
int NETDVR_SetCruiseParam(int Handle, const struct NETDVR_cruisePath_t *p_cruise_path);
int NETDVR_GetCruiseParam(int Handle, unsigned char chn, unsigned char pathnum, struct NETDVR_cruisePath_t *p_cruise_path);

//������̨
int NETDVR_PtzControl(int Handle, const struct NETDVR_PtzCtrl_t *p_para);

//��ʼ��ֹͣ��̨Ѳ��
int NETDVR_startYuntaiCruise(int Handle, unsigned char chn, unsigned char path_no);
int NETDVR_stopYuntaiCruise(int Handle, unsigned char chn, unsigned char path_no);

//��ʼ��¼��������¼����ʼ���С�ֹͣ������̨�켣
int NETDVR_SetYTTrack(int Handle, unsigned char chn, enum NETDVR_YTTRACKCONTROL yt_cmd);

//���á�ɾ����������̨Ԥ�õ�
int NETDVR_SetYTPresetPoint(int Handle, unsigned char chn, unsigned char preset_pos, enum NETDVR_YTPRESETPOINTCONTROL yt_com);

//���á���ȡͼ�����
int NETDVR_setVideoParams(int Handle, const struct NETDVR_videoParam_t *p_para);
int NETDVR_getVideoParams(int Handle, unsigned char chn, struct NETDVR_videoParam_t *p_para);

//��ȡ������ͼ�����ȡ��Աȶȡ�ɫ�������Ͷ�
int NETDVR_setPicAdjust(int Handle, const struct NETDVR_PICADJUST_T *p_para);
int NETDVR_getPicAdjust(int Handle, unsigned char chn, enum NETDVR_PICADJUST type, struct NETDVR_PICADJUST_T *p_para);

//���á���ȡ�ƶ�������
int NETDVR_getMotionDection(int Handle, unsigned char chn, struct NETDVR_motionDetection_t *p_para);
int NETDVR_setMotionDection(int Handle, const struct NETDVR_motionDetection_t *p_para);

//���á���ȡ��Ƶ��ʧ����
int NETDVR_getVideoLost(int Handle, unsigned char chn, struct NETDVR_VideoLostParam_t *p_para);
int NETDVR_setVideoLost(int Handle, const struct NETDVR_VideoLostParam_t *p_para);

//���á���ȡ��Ƶ�ڵ���������
int NETDVR_GetVideoBlockParam(int Handle, unsigned char chn, struct NETDVR_VideoBlockParam_t *p_para);
int NETDVR_SetVideoBlockParam(int Handle, const struct NETDVR_VideoBlockParam_t *p_para);

//���á���ȡ¼�����(������):λ�ʡ�֡�ʡ�ͼ��������
int NETDVR_setRecordParams(int Handle, const struct NETDVR_recordParam_t *p_para);
int NETDVR_getRecordParams(int Handle, unsigned char chn, struct NETDVR_recordParam_t *p_para);

//��ȡ¼�񲼷�����(¼�񲼷�������ʱ¼���ڵ�����¼����ƶ����¼������¼��ʽ�Ĳ���)
int NETDVR_GetRecordSCH(int Handle, unsigned char chn, enum NETDVR_WEEKDAY day, struct NETDVR_RecordSCH_t *p_para);
//����¼�񲼷�����(������һ��ͨ�������ڼ���p_para��ָ��Ľṹ����ָ��)
int NETDVR_SetRecordSCH(int Handle, const struct NETDVR_RecordSCH_t *p_para);

//���á���ȡ�������������(������):λ�ʡ�֡�ʡ�ͼ��������
int NETDVR_GetSubStreamParam(int Handle, unsigned char chn, struct NETDVR_SubStreamParam_t *p_para);
int NETDVR_SetSubStreamParam(int Handle, const struct NETDVR_SubStreamParam_t *p_para);

//���á���ȡ�����������
int NETDVR_setAlarmInParams(int Handle, const struct NETDVR_alarmInParam_t *p_para);
int NETDVR_getAlarmInParams(int Handle, unsigned char in_id, struct NETDVR_alarmInParam_t *p_para);

//���á���ȡ�����������
int NETDVR_setAlarmOutParams(int Handle, const struct NETDVR_alarmOutParam_t *p_para);
int NETDVR_getAlarmOutParams(int Handle, unsigned char out_id, struct NETDVR_alarmOutParam_t *p_para);
//�������(���������������б������)
int NETDVR_clearAlarms(int Handle);

//���á���ȡ����email��ַ�Ͷ��ű����ֻ���(����ֻ֧���ƶ����ŷ���,Ŀǰ���δ˹���)
int NETDVR_GetAlarmNoticeParam(int Handle, struct NETDVR_AlarmNoticeParam_t *p_para);
int NETDVR_SetAlarmNoticeParam(int Handle, const struct NETDVR_AlarmNoticeParam_t *p_para);

//���á���ȡ�������
int NETDVR_setNetworkParams(int Handle, const struct NETDVR_networkParam_t *p_para);
int NETDVR_getNetworkParams(int Handle, struct NETDVR_networkParam_t *p_para);

//����MAC��ַ
int NETDVR_setmacaddress(int Handle, const struct NETDVR_networkParam_t *p_para);

//��ȡ�����û���Ϣ(һ��֧��8���û�)
int NETDVR_GetUserInfo(int Handle, struct NETDVR_UserNumber_t *p_para);
//���һ���û�(ֻ��admin�û������Ȩ��)
int NETDVR_AddUserInfo(int Handle, const struct NETDVR_userInfo_t *p_para);
//�޸��û�����(�����û��������Ȩ��)
int NETDVR_EditUserInfo(int Handle, const struct NETDVR_userInfo_t *p_para);
//ɾ���û�(ֻ��admin�û������Ȩ��/admin�û�����ɾ���Լ�)
int NETDVR_DelUserInfo(int Handle, const char *username);

//�õ�DVR�豸�ĳ�����Ϣ
int NETDVR_GetDeviceInfo(int Handle, struct NETDVR_DeviceInfo_t *pDeviceInfo);

//�õ�DVR�豸����Ƶ����:���ߡ����뷽ʽ(h264)
int NETDVR_GetVideoProperty(int Handle, struct NETDVR_VideoProperty_t *pVideoPro);
//�õ�DVR�豸����Ƶ����:�����ʡ�bit�����뷽ʽ��
int NETDVR_GetAudioProperty(int Handle, struct NETDVR_AudioProperty_t *pVideoPro);
//�õ�DVR�豸�������Խ�����:�����ʡ�bit�����뷽ʽ��
int NETDVR_GetVoipProperty(int Handle, struct NETDVR_VOIPProperty_t *pVoipPro);
//�õ�DVR�豸���ƶ��������:�ƶ��������ֶ����С�������
int NETDVR_GetMDProperty(int Handle, struct NETDVR_MDProperty_t *pMDPro);

//��ȡϵͳ�汾��Ϣ
int NETDVR_remoteGetSysVerInfo(int Handle, struct NETDVR_SysVerInfo_t *p_para);

//���á���ȡϵͳʱ��
int NETDVR_getSystemTime(int Handle, struct NETDVR_SysTime_t *p_para);
int NETDVR_setSystemTime(int Handle, const struct NETDVR_SysTime_t *p_para);

//��ȡ�ֶ�¼��״̬
int NETDVR_GetRecordState(int Handle, struct NETDVR_ManualRecord_t *p_para);
//�����ֶ�¼��(ÿһλ����һ��ͨ��:1������,0����ֹͣ)
int NETDVR_SetRecordState(int Handle, const struct NETDVR_ManualRecord_t *p_para);

//��ȡϵͳ��hddindex��SATA�ڵ�Ӳ����Ϣ,hddindex��0��ʼ����
int NETDVR_remoteGetHddInfo(int Handle, unsigned char hddindex, struct NETDVR_hddInfo_t *p_hddinfo);

//�����Խ�
//����voipindexֻ֧�� 0
int NETDVR_startVOIP(int Handle, int voipindex);
int NETDVR_stopVOIP(int Handle, int voipindex);
int NETDVR_VOIPRegRcvCB(int Handle, int voipindex, pDecFrameCallBack rcvfunc, unsigned int dwContext);
int NETDVR_VOIPSetSendMode(int Handle, int voipindex, unsigned char flagmode);
//flagmode��0: win32��ȡ�������ݷ���  1:�Զ������ݷ���
int NETDVR_VOIPSendData(int Handle, int voipindex, FrameHeadrDec voipdata);
//NETDVR_VOIPSetSendMode ��1��ʱ����Ч

//���������
int NETDVR_setNRServer(int Handle, const struct NETDVR_NRServer_t *p_para);
int NETDVR_getNRServer( int Handle, struct NETDVR_NRServer_t * p_para);

//����
int NETDVR_setAlarmOutVal(int Handle, const struct NETDVR_AlarmVal_t *p_para);//��ȡ��������ֵ
int NETDVR_getAlarmInVal( int Handle, unsigned char alarm_in_id,struct NETDVR_AlarmVal_t * p_para);//���ñ������ֵ

//͸������
typedef void(*pSerialDataCallBack)(int lSerialPort, unsigned char bySerialChannel, char *pRecvDataBuffer, unsigned int dwBufSize, unsigned int dwContent);
//lSerialPort 1��232���ڣ�2��485���� 
int NETDVR_SerialStart(int Handle, int lSerialPort,	pSerialDataCallBack cbSerialDataCallBack, unsigned int dwContent);
//byChannel, ʹ��485����ʱ��Ч����1��ʼ��232������Ϊ͸��ͨ��ʱ��ֵ����Ϊ0 
//dwBufSize ���ֵ4096
int NETDVR_SerialSend(int Handle, int lSerialPort, unsigned char byChannel, char* pSendBuf, unsigned int dwBufSize);
int NETDVR_SerialStop(int Handle, int lSerialPort);

/*
NETDVR_regCBMsgHasAudio: register a callback function to deal with no audio(file has no audio) notification message for one dvr's play reciever
NETDVR_regCBMsgProgress: register a callback function to deal with progress notification message for one dvr's play reciever
*/
int NETDVR_regCBMsgHasAudio(int Handle, int player_index, PFUN_MSGHASAUDIO_T p_cb_func, unsigned int dwContent);
int NETDVR_regCBMsgProgress(int Handle, int player_index, PFUN_PROGRESS_T p_cb_func, unsigned int dwContent);
/*
NETDVR_pausePlay: pause a play
NETDVR_resumePlay: resume a paused play
NETDVR_singleFramePlay: single frame play
NETDVR_fastPlay: play faster(when playrate = 8,  reset playrate to 1)
NETDVR_slowPlay: play slower(when playrate = -8,  reset playrate to 1)
NETDVR_setPlayRate: set play rate to a new value(play_rate [-8, 8])(-8 means 1/8)
*/
//��ͣ����
int NETDVR_pausePlay(int Handle, int player_index);
//�ָ�����
int NETDVR_resumePlay(int Handle, int player_index);
//֡��
int NETDVR_singleFramePlay(int Handle, int player_index);
//���
int NETDVR_fastPlay(int Handle, int player_index);
//����
int NETDVR_slowPlay(int Handle, int player_index);
//��������
int NETDVR_setPlayRate(int Handle, int player_index, int play_rate);

/*
NETDVR_playPrevious: play previous segments when play by file; play previous file when play by time
NETDVR_playNext: play next segments when play by file; play next file when play by time
NETDVR_playSeek: play to a new position
NETDVR_playMute: play in mute or not depending on b_mute
NETDVR_playProgress: let DVR send play progress depending on b_send_progress
*/
//��һ��
int NETDVR_playPrevious(int Handle, int player_index);
//��һ��
int NETDVR_playNext(int Handle, int player_index);
//��λ����λ��
int NETDVR_playSeek(int Handle, int player_index, unsigned int new_time);
//�طž���
int NETDVR_playMute(int Handle, int player_index, int b_mute);
//�������Ȼص�
int NETDVR_playProgress(int Handle, int player_index, int b_send_progress);

//�ָ���������
int NETDVR_restoreFactorySettings(int Handle);
//�����豸
int NETDVR_reboot(int Handle);
//�ػ�(ͨ�������ػ�)
int NETDVR_shutdown(int Handle);

//�����Ƶ�ֱ���
int NETDVR_getVideoResoluList( int Handle, unsigned char chn, unsigned char vidoetype, struct NETDVR_VideoResolu_list_t* pList );

int NETDVR_sendExtendCmd(int Handle, unsigned short wCommand, const void *pInData, int nInDataLen, void* pOutData, int nMaxOutDatalen);

#ifdef __cplusplus
}
#endif

#endif
