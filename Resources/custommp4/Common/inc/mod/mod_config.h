#ifndef _MOD_CONFIG_H_
#define _MOD_CONFIG_H_

typedef struct
{
	s8 nVersion[30];
	s8 nModel[20];
	s8 nproductnumber[32];//9624 ��Ʒ��
	u8 nPreviewNum;
	u8 nVidMainNum;
	u8 nVidSubNum;
	u8 nAudNum;
	u8 nSnapNum;
	u8 nVoipNum;
	u8 nRecNum;
	u8 nSensorNum;
	u8 nAlarmoutNum;
	u8 nBuzzNum;
	u8 nMaxHddNum;
	u32 nMaxMainFrameSize;
	u32 nMaxSubFrameSize;
	u32 nMaxAudFrameSize;
	u32 nMaxSnapFrameSize;
	u8 nOsdOperator;
	u8 nStrOsdNum;
	u8 nImgOsdNum;
	u8 nRectOsdNum;
	u8 nRectOsdRgb[3];
	u8 nLineOsdNum;
	u16 nRefWidth;
	u16 nRefHeight;
	u8 nAudioEncType;
	u16 nAudioSampleRate;
	u16 nAudioSampleDuration;
	u16 nAudioSampleSize;
	u16 nAudioBitWidth;
	u8 nVoipEncType;
	u16 nVoipSampleRate;
	u16 nVoipSampleDuration;
	u16 nVoipSampleSize;
	u16 nVoipBitWidth;
	u8 nMdAreaRows;
	u8 nMdAreaCols;
	u8 nMdSenseMin;
	u8 nMdSenseMax;
	u16 nSchSegments;
	u16 nOutputSync;
	u8 nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
}SModConfigDvrProperty;

typedef struct
{
	s8 nDevName[32];
	u32 nDevId;
	u8 nManual;
	u8 nVideoStandard;
	u8 nAuthCheck;
	u8 nOutput;
	u8 nLangId;
	u8 nShowGuide;
	u32 nLockTime;
	u8 nDateFormat;
	u8 nTimeStandard;
	u8 nCyclingRecord;
	u8 nShowFirstMenue;//��ʾ"�˵�����豸ѡ��"����
	//csp modify
	s8 sn[32];
	u8 reserved[256];
} SModConfigSystemParam;

//MainTain
typedef struct
{
	u8 nEnable;
	u8 nMainType;
	u8 nSubType;
	u8 ntime;
} SModConfigMainTainParam;

//Preview
typedef struct
{
	u8 nPreviewMode;
	u8 nModePara; 						//extend parameter for EMPREVIEWMODE�ο�SBizPreviewPara.nModePara
	u8 nAdioChn; 						//��Ƶ���ͨ��
	u8 nVolume; 						//����
	u8 nMute; 							//�Ƿ���
	u8 nIsPatrol; 						//Ĭ���Ƿ�����Ѳ
	u8 nPatrolChnNum;//��Ѳͨ����16 32 48 64
    u8 nInterval; 						//��Ѳ��� ��
	u8 nPatrolMode;
	u8 nStops; 							//һ���ֻ�ͣ������
	u8 pnStopModePara[256]; 				//ÿվԤ��ģʽ����;�����С���ϸ�����nStops�������ο�SBizPreviewPara.nModePara��nStops==0xffʱ��Ч	
	u8 nVideoSrcType[128];					//��Ƶ����Դ������
} SModConfigPreviewParam;

typedef struct
{
	u8 nHue;
	u8 nSaturation;
	u8 nContrast;
	u8 nBrightness;
} SModConfigImagePara;

//END


//osd
typedef struct
{
	u8 nShowTime;
	u16 nTimePosX;
	u16 nTimePosY;
	u8 nShowRecState;
	u8 nShowChnKbps;// �Ƿ���ʾͨ����ʵʱ��������
}SModConfigMainOsdParam;

typedef struct
{
	s8 nChnName[32];
	u8 nShowChnName;
	u16 nChnNamePosX;
	u16 nChnNamePosY;
	u8 nEncShowChnName;
	u16 nEncChnNamePosX;
	u16 nEncChnNamePosY;
	u8 nEncShowTime;
	u16 nEncTimePosX;
	u16 nEncTimePosY;	
}SModConfigStrOsdpara;

typedef struct
{
	u8 nShowRectOsd;
	u8 nRectOsdNum;
	u16 nRectOsdX[256];
	u16 nRectOsdY[256];
	u16 nRectOsdW[256];
	u16 nRectOsdH[256];	
}SModConfigRectOsdpara;
//end


//Encode
typedef struct 
{
	u32 nBitRate;
	u32 nFrameRate;
	u32 nGop;
	u32 nMinQP;
	u32 nMaxQP;
	u8 nResolution;
	u8 nEncodeType;
	u8 nBitRateType;
	u8 nPicLevel;
} SModConfigVideoParam;

typedef struct
{
	u8 nEncodeType;
	u32 nSampleRate;
	u32 nSampleDuration;
	u32 nSampleSize;
	u8 nBitWidth;
} SModConfigAudioParam;
//END


//Record
typedef struct
{
	u8 bRecording;
	u8 nEnable;
	u8 nStreamType;
	u8 nEncChn;
	u8 nChnEncType;
	u8 nPreTime;
	u16 nDelayTime;
	u8 nExpireDays;
	u8 nTimerRecSchType;
	u8 nMDRecSchType;
	u8 nSensorRecSchType;
} SModConfigRecordParam;
//END


//Alarm
typedef struct
{
	u8 nPtzType;
	u8 nChn; //ͨ����
	u8 nId; //��ʾԤ�õ�id/Ѳ����·id/�켣id(��Ч)
} SModConfigAlarmPtz;
	
typedef struct
{
	u8 nFlagBuzz; //������
	u8 nZoomChn; //���汨��ͨ���� 0xff ��ʾ��Ч
	u8 nFlagEmail; //�ʼ�
	u64 nSnapChn; //��Ŵ���ץͼ��ͨ���� ���ͬʱ64��
	u64 nAlarmOut; //�������� ���ͬʱ����64������id;
	u64 nRecordChn; //����¼��ͨ��;
	SModConfigAlarmPtz sAlarmPtz[256]; //���ͬʱ����256������
} SModConfigAlarmDispatch;

typedef struct
{
	u8 nEnalbe;
	u8 nSensorType;
	u16 nDelay;
	s8 name[20];
	u8 nSchType;
	SModConfigAlarmDispatch psDispatch;
} SModConfigSensorParam;
//yaogang modify 20141010
typedef struct
{
	u8 nEnalbe;
	u16 nDelay;
	SModConfigAlarmDispatch psDispatch;
} SModConfigIPCExtSensorParam;

typedef struct
{
	u8 nEnalbe;
	SModConfigAlarmDispatch psDispatch;
} SModConfigHDDParam;


typedef struct
{
	u8 nEnalbe;
	u8 nSensitivity;
    u16 nDelay;
	u8 nSchType;
	SModConfigAlarmDispatch psDispatch;
	u64 nBlockStatus[64]; //��Ӧ�����״̬
} SModConfigVMotionParam;

typedef struct
{
	u8 nEnalbe;
	u16 nDelay;
	u8 nSchType;
	SModConfigAlarmDispatch psDispatch;
} SModConfigVLostParam;

typedef struct
{
	u8 nEnalbe;
	u16 nDelay;
	u8 nSchType;
	SModConfigAlarmDispatch psDispatch;
} SModConfigVBlindParam;

typedef struct
{
	 u8 nEnalbe;
	 u8 nAlarmOutType;
	 u16 nDelay;
	 s8 name[20];
	 u8 nSchType;
} SModConfigAlarmOutParam;

typedef struct
{
	u8 nEnalbe; 
    u16 nDelay;
	u8 nDuration;
    u8 nInterval;
} SModConfigBuzzParam;
//END


//Schedule
typedef struct 
{
	u32 nStartTime;	//ÿ��ʱ��ε���ʼʱ��
	u32 nStopTime;	//����ʱ��
} SModConfigSchTime;

#define CONFIG_MAX_SCH_TIME_SEGMENTS 12	//ÿ������õ�ʱ��ε������Ŀ

typedef struct 
{
	u8 nSchType; //����������ʱ������	
	SModConfigSchTime nSchTime[31][CONFIG_MAX_SCH_TIME_SEGMENTS];	//���β���ʱ��
} SModConfigSchedule;

typedef struct
{
	SModConfigSchedule psSensorSch;
} SModConfigSensorSch;

typedef struct
{
	SModConfigSchedule psVMSch;
} SModConfigVMotionSch;

typedef struct
{
	SModConfigSchedule psVLostSch;
} SModConfigVLostSch;

typedef struct
{
	SModConfigSchedule psVBlindSch;
} SModConfigVBlindSch;

typedef struct
{
	SModConfigSchedule psAlarmOutSch;
} SModConfigAlarmOutSch;

typedef struct
{
	SModConfigSchedule psRecSch;
} SModConfigRecTimerSch;

typedef struct
{
	SModConfigSchedule psRecSch;
} SModConfigRecAlarmInSch;

typedef struct
{
	SModConfigSchedule psRecSch;
} SModConfigRecVMSch;

typedef struct
{
	SModConfigSchedule psRecSch;
} SModConfigRecVLostSch;

typedef struct
{
	SModConfigSchedule psRecSch;
} SModConfigRecVBlindSch;
//END


//Ptz
typedef struct
{
	u8 nPresetId;
	u8 nSpeed;
	u32 nStayTime;
}SModConfigPresetPara;

typedef struct
{
	u64 nPresetId[4];
	u64 nTourId[4];
	SModConfigPresetPara nTourPresetId[32][128];//nTourPresetId[256][256];
}SModConfigPtzTourPresetPara;

typedef struct
{
	u8 nEnable;
	u8 nCamAddr;	
	u8 nDataBit;
	u8 nStopBit;
	u32 nBaudRate;
	u8 nCheckType;
	u8 nFlowCtrlType;
	s8 nProtocolName[20];
	SModConfigPtzTourPresetPara sTourPresetPara;
}SModConfigPtzParam;
//END


//USER
#define MOD_CONFIG_MAX_GROUP_NUM 16			//��֧�ֵ������û�����Ŀ
#define MOD_CONFIG_MAX_USER_NUM 128			//��֧�ֵ������û���Ŀ

typedef struct
{
	u8 nAuthor[64];
	u64 nLiveView[4];
	u64 nRemoteView[4];
	u64 nRecord[4];
	u64 nPlayback[4];
	u64 nBackUp[4];
	u64 nPtzCtrl[4];
}SModConfigUserAuthority;

typedef struct
{
	u8 GroupName[15];
	u8 GroupDescript[30];
	u8 nDoWithUser;	//��������޸ĺ�ɾ��ʱ���Ƿ�ʹ����Ӧ�õ������µ������û���[0]:��[��0]:��
	SModConfigUserAuthority GroupAuthor;
}SModConfigUserGroupPara;

typedef struct
{
	u8 UserName[15];
	u8 Password[15];
	u8 GroupBelong[15];
	u8 UserDescript[30];
	u8 emLoginTwince;
	u8 emIsBindPcMac;
	u64 PcMacAddress;
	SModConfigUserAuthority UserAuthor;
}SModConfigUserUserPara;

typedef struct
{
	u64 nFlagGroupSetted;
	u64 nFlagUserSetted[2];
	SModConfigUserUserPara sUserPara[MOD_CONFIG_MAX_USER_NUM];
	SModConfigUserGroupPara sGroupPara[MOD_CONFIG_MAX_GROUP_NUM];
}SModConfigUserPara;
//END
/*
 ���ýӿڴ�ȡ��������ṹ
*/
typedef struct
{
	char	szSmtpServer[64];
	
	// ����������
	char	szSendMailBox[64];
	char	szSMailPasswd[64];

    u8		nSSLFlag;

	// �ռ�������
	char	szReceiveMailBox[64];
	char	szReceiveMailBox2[64];
	char	szReceiveMailBox3[64];

	// ����ͼƬ
	char	szSnapPic[64];
	
	// ץͼ���
	u32		nSnapIntvl;
	
} SModConfigNetAdvancePara;

typedef struct
{
	// ��������
	u8     	Version[8];			    // 8�ֽڵİ汾��Ϣ
    char   	HostName[16];           // ������
    u32  	HostIP;                 // IP ��ַ
    u32  	Submask;                // ��������
    u32  	GateWayIP;              // ���� IP
    u32  	DNSIP;                  // DNS IP
    u32  	DNSIPAlt;                 // DNS IP
    u8 		MAC[18];				// MAC Address
	
    // �ⲿ�ӿ�
    u32   	AlarmServerIP;          // ��������IP
    u16 	AlarmServerPort;        // �������Ķ˿�
    u32   	SMTPServerIP;           // SMTP server IP
    u16 	SMTPServerPort;         // SMTP server port
	
    u32   	LogServerIP;            // Log server IP
    u16 	LogServerPort;          // Log server port
	
    // ��������˿�
    u16 	HttpPort;               	// HTTP����˿ں�
    u16 	MobilePort;              	// �ֻ���ض˿ں�
    
    u16 	TCPMaxConn;             	// TCP ���������
    u16 	TCPPort;                	// TCP �����˿�
	
	u8		nSubStreamMax;				// ������������
	u8		nSendBufNumPerChn;			// ÿͨ������buff��
	u32		nFrameSizeMax;				// ֡buff��С����
	
	// ��·������
	u8		nVideoMediaType;			// ��Ƶ��������
	u8		nAudioMediaType;			// Ԥ����Ƶ��������
	u8		nVoipMediaType;				// �Խ���Ƶ��������
	u8		nAudioSampleMode;			// ��Ƶ����ģʽ
	
    u16 	UDPPort;                	// UDP �����˿�
	
    u8		McastFlag;
    u16 	McastPort;              	// �鲥�˿�
    u32 	McastIP;                	// �鲥IP
    
	u8		PPPOEFlag;					// pppoe ʹ�ܿ���
	u8		PPPOEUser[64];
	u8		PPPOEPasswd[64];
	
	u8		DhcpFlag;					// Dhcp ʹ�ܿ���
	
	u8		DDNSFlag;					// DDNS ����λ
	u8		DDNSDomain[64];
	u8		DDNSUser[64];
	u8		DDNSPasswd[64];
	u16		UpdateIntvl;				// DDNS	IP���¼��
	
    // ����
    u8    	MonMode;                	// ����Э�� TCP|UDP|MCAST
    u8    	PlayMode;               	// �ط�Э�� TCP|UDP|MCAST
    u8    	AlmSvrStat;             	// ��������״̬ <��/��>
	
	// �߼�����
    SModConfigNetAdvancePara	sAdancePara;
	
	//csp modify 20130321
	u8		UPNPFlag;
} SModConfigNetParam;

typedef enum
{
    EM_CONFIG_PARALIST_BASE = 0,
    EM_CONFIG_PARALIST_LANGSUP, // ֧�ֵ������б�
    EM_CONFIG_PARALIST_LANGSRC, // ֧�ֵ�������Դ�ļ��б� ;��֧���ַ����б�
    EM_CONFIG_PARALIST_PREVIEWMODE, // ֧�ֵ�Ԥ��ģʽ
    EM_CONFIG_PARALIST_BITRATE, // ֧�ֵ�λ���б�
    EM_CONFIG_PARALIST_BITTYPE, // λ�������б�
    EM_CONFIG_PARALIST_PICLEVEL, // ͼ�������б�
    EM_CONFIG_PARALIST_VMAINRESOLH_NUM, // ������ͨ��֧�ֵķֱ����б�1��ͨ����Ŀ//cw_9508S
    EM_CONFIG_PARALIST_VMAINRESOL, // ������ͨ��֧�ֵķֱ����б�
    EM_CONFIG_PARALIST_VMAINRESOL2, // ������ͨ��֧�ֵķֱ����б�2
    EM_CONFIG_PARALIST_VSUBRESOL, // ������ͨ��֧�ֵķֱ����б�
    EM_CONFIG_PARALIST_VMOBRESOL, // �ֻ�����ͨ��֧�ֵķֱ����б�
    EM_CONFIG_PARALIST_FPSPAL, // P�Ƴ�����֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_FPSNTSC, // N�Ƴ�����֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_FPSPALD1, // P��D1��֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_FPSNTSCD1, // N��D1��֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_FPSPALCIF, // P��Cif��֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_FPSNTSCCIF, // N��Cif��֧�ֵ�֡���б�
    EM_CONFIG_PARALIST_STREAMTYPE, // ¼�����������б�
    EM_CONFIG_PARALIST_VIDEOSTANDARD, // ��Ƶ��ʽ�б�
    EM_CONFIG_PARALIST_OUTPUT, // ��Ƶ����б�
    
    EM_CONFIG_PARALIST_SWITCHPICTURE, //��Ѳ����ѡ�� 
    EM_CONFIG_PARALIST_LOCKTIME, // ������ʱ��ѡ���б�
    EM_CONFIG_PARALIST_DATEFORMAT, // ���ڸ�ʽ�б�
    EM_CONFIG_PARALIST_TIMESTANDARD, // ʱ����ʽ�б�
    EM_CONFIG_PARALIST_SITEMAINOUT, // �ֳ�������б�
    EM_CONFIG_PARALIST_INTERVAL, // ʱ�����б�
    EM_CONFIG_PARALIST_SENSORTYPE, // �����������б�
    EM_CONFIG_PARALIST_ALARMOUTTYPE, // ������������б�
    EM_CONFIG_PARALIST_DELAY, // ��ʱʱ��ѡ���б�
    EM_CONFIG_PARALIST_PRETIME, // Ԥ¼ʱ��ѡ���б�
    EM_CONFIG_PARALIST_RECDELAY, // ¼����ʱʱ��ѡ���б�
    EM_CONFIG_PARALIST_RECEXPIRE, // ¼���������ѡ���б�
    EM_CONFIG_PARALIST_SCHEDULETYP, // ��������ѡ���б�
    EM_CONFIG_PARALIST_PTZLINKTYPE, // ��̨��������ѡ���б�
    EM_CONFIG_PARALIST_BAUDRATE, // ������ѡ���б�
    EM_CONFIG_PARALIST_DATABIT, // ����λ����ѡ���б�
    EM_CONFIG_PARALIST_STOPBIT, // ֹͣλ����ѡ���б�
    EM_CONFIG_PARALIST_CHECKTYPE, // У����������ѡ���б�
    EM_CONFIG_PARALIST_FLOWCTRLTYPE, // ���ط�ʽ����ѡ���б�
    EM_CONFIG_PARALIST_DDNSDOMAIN, // DDNSDomain
    EM_CONFIG_PARALIST_DDNSIPUPDATEITVL, // DDNS IP update    
    EM_CONFIG_PARALIST_VIDEOSRCTYPE, // ��ƵԴ����ѡ���б�
    //
    EM_CONFIG_PARALIST_PTZPROTOCOL, // ��̨Э��ѡ���б�
    //yaogang modify 20141210 ShenGuang
    EM_CONFIG_PARALIST_RECORD_SNAP_RES,//ץͼ�ֱ���
    EM_CONFIG_PARALIST_RECORD_SNAP_QUALITY,//ץͼ����
    EM_CONFIG_PARALIST_RECORD_SNAP_INTERVAL,//ץͼʱ����
    EM_CONFIG_PARALIST_RepairName,//����ϵͳ����
    EM_CONFIG_PARALIST_RepairType,//����ϵͳ����
    EM_CONFIG_PARALIST_Maintain,//ά��ά��
    EM_CONFIG_PARALIST_Test,//����
    EM_CONFIG_PARALIST_AlarmInput,//����ͼƬ����: �������

    EM_CONFIG_PARALIST_NUM,
}EMCONFIGPARALISTTYPE;

typedef enum
{
	EM_CONFIG_PARA_SYSTEM = 0,
	EM_CONFIG_PARA_PREVIEW = 100,
	EM_CONFIG_PARA_IMAGE = 200,
	EM_CONFIG_PARA_MAIN_OSD = 300,
	EM_CONFIG_PARA_STR_OSD = 330,
	EM_CONFIG_PARA_RECT_OSD = 360,	
	EM_CONFIG_PARA_SENSOR = 400,
	//yaogang modify 20141010
	EM_CONFIG_PARA_IPCCOVER,
	EM_CONFIG_PARA_IPCEXTSENSOR,
	EM_CONFIG_PARA_485EXTSENSOR,
	EM_CONFIG_PARA_HDD,
	
	EM_CONFIG_PARA_VMOTION = 500,
	EM_CONFIG_PARA_VLOST = 600,
	EM_CONFIG_PARA_VBLIND = 700,
	EM_CONFIG_PARA_ALARMOUT = 800,
	EM_CONFIG_PARA_BUZZ = 900,
	EM_CONFIG_PARA_SENSOR_SCH = 1000,
	EM_CONFIG_PARA_IPCEXTSENSOR_SCH,
	EM_CONFIG_PARA_IPCCOVER_SCH,
	EM_CONFIG_PARA_VMOTION_SCH = 1100,
	EM_CONFIG_PARA_VLOST_SCH = 1200,
	EM_CONFIG_PARA_VBLIND_SCH = 1300,
	EM_CONFIG_PARA_ALARMOUT_SCH = 1400,
	EM_CONFIG_PARA_BUZZ_SCH = 1500,	
	EM_CONFIG_PARA_VO_IMAGE = 1600,
	EM_CONFIG_PARA_PATROL = 1700,
	EM_CONFIG_PARA_MAINTAIN = 1800,
	//NVR used
	//IPCamera��������
	EM_CONFIG_PARA_IPCAMERA = 1900,//
	//
	EM_CONFIG_PARA_ENC_MAIN = 4000,
	EM_CONFIG_PARA_ENC_SUB = 4100,
	EM_CONFIG_PARA_ENC_MOB,
	//
	EM_CONFIG_PARA_RECORD = 6000,
	EM_CONFIG_PARA_TIMER_REC_SCH = 6100,
	EM_CONFIG_PARA_SENSOR_REC_SCH = 6200,
	EM_CONFIG_PARA_VMOTION_REC_SCH = 6300,
	EM_CONFIG_PARA_VLOST_REC_SCH = 6400,
	EM_CONFIG_PARA_VBLIND_REC_SCH = 6500,	
	//
	EM_CONFIG_PARA_USER = 8000,
	EM_CONFIG_PARA_PTZ = 8100,
	EM_CONFIG_PARA_NETWORK = 8200,	
	//
	EM_CONFIG_PARA_DVR_PROPERTY = 9000,
	//yaogang modify 20141209
	//���ƽ̨����
	EM_CONFIG_PARA_SG,
	EM_CONFIG_PARA_SG_DAILY,
	EM_CONFIG_PARA_SG_ALARM,
	EM_CONFIG_PARA_SNAP_CHN,
	
	EM_CONFIG_PARA_ALL = 10000,
	//
}EMCONFIGPARATYPE;

s32 ModConfigInit(void);
s32 ModDefaultConfigRelease(void);
s32 ModDefaultConfigResume(void);
s32 ModConfigGetParam(EMCONFIGPARATYPE emType, void* para, s32 nId);//nId��ͨ���ŵȣ� -1 ��ʾ��Ч
s32 ModConfigSetParam(EMCONFIGPARATYPE emType, void* para, s32 nId);
s32 ModConfigGetDefaultParam(EMCONFIGPARATYPE emType, void* para, s32 nId);
s32 ModConfigDefault(void);
s32 ModConfigBackup(s8* pFilePath);//��Ҫ�����ļ�������/tmp/usb/config_backup.ini������/tmp/usb��Ч
s32 ModConfigResume(EMCONFIGPARATYPE emType, s32 nId, s8* pFilePath);//ͬ��//���������ļ�Ҫ�õ�

s32 ModConfigResumeDefault(EMCONFIGPARATYPE emType, s32 nId);
s32 ModConfigGetConfigFilePath(s8* pFilePath, u32 nMaxPathLen);
s32 ModConfigSyncFileToFlash(u8 nMode);//nMode˵�� (0 ���� ��0)
									   //[0:ϵͳ�ػ�������ǰ����, ��0:ƽʱ��Ҫͬ���ļ���flashʱʹ��]

//����ַ���ȡֵ�б�
s32 ModConfigGetParaStr(EMCONFIGPARALISTTYPE emParaType, PARAOUT s8* strList[], PARAOUT u8* nRealListLen, u8 nMaxListLen, u8 nMaxStrLen);
//��������ȡֵ����ַ����б��е�index
s32 ModConfigGetParaListIndex(EMCONFIGPARALISTTYPE emParaType, s32 nValue, PARAOUT u8* pnIndex);
//����index���ʵ�ʲ���ȡֵ
s32 ModConfigGetParaListValue(EMCONFIGPARALISTTYPE emParaType, u8 nIndex, PARAOUT s32* pnValue);

//yaogang modify 20141209
//���ƽ̨����
typedef struct
{
	char AgentID[100];
	char CenterIP[100];
	u8 RunEnable;//����״̬����
	u8 AlarmEnable;//������������
	u8 DailyPicEnable;//�ճ�ͼ������
	u8 normal_exit;// 1: ϵͳ�����ػ�0: �쳣�˳�
} SModConfigSGParam;

//����ͼƬ���ã�ÿ������ͨ��һ���ṹ
//m·���أ�n·IPC�ⲿ����(m 16--n 32)
typedef struct
{
	u8 PreTimes;		//ǰ��ʱ��
	u8 Interval;		//���ʱ��
	u8 StillTimes;		//��¼ʱ��
	u8 AlarmInput;	//�������
	u32 IpcChn;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
} SModAlarmPicCFG;

//�ճ�ͼƬ����
typedef struct
{
	u8 TimeEnable;	//��ʱ�ϴ�ʱ��ʹ��
	u8 hour;
	u8 min;
	u8 sec;
} SModSGTime;

typedef struct
{
	SModSGTime Time1;		//��ʱ�ϴ�ʱ��1 
	SModSGTime Time2;		//��ʱ�ϴ�ʱ��2
	u32 IpcChn1;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
	u32 IpcChn2;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
} SModDailyPicCFG;

#if 0
typedef struct
{
	u8 TimeEnable;	//��ʱ�ϴ�ʱ��ʹ��
	u8 hour;
	u8 min;
	u8 sec;
} SModSGTime;
typedef struct
{
	u8 PreTimes;		//ǰ��ʱ��
	u8 Interval;		//���ʱ��
	u8 StillTimes;		//��¼ʱ��
	u8 AlarmInput;	//�������
	u32 IpcChn;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
} SModAlarmPicCFG;
typedef struct
{
	SModSGTime Time1;		//��ʱ�ϴ�ʱ��1 
	SModSGTime Time2;		//��ʱ�ϴ�ʱ��2
	u32 IpcChn1;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
	u32 IpcChn2;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
} SModDailyPicCFG;
typedef struct
{
	char AgentID[100];
	char CenterIP[100];
	u8 RunEnable;//����״̬����
	u8 AlarmEnable;//������������
	u8 DailyPicEnable;//�ճ�ͼ������
	SModAlarmPicCFG SAlarmPicParam;//����ͼƬ���ò���
	SModDailyPicCFG SDailyPicParam;//�ճ�ͼƬ���ò���
} SModConfigSGParam;
#endif


//yaogang modify 20150105
typedef struct 
{
	u8 Enable;	//����
	u8 Res;		//�ֱ���
	u8 Quality;	//ͼƬ����
	u8 Interval;	//ʱ����
} SModConfigSnapPara;
typedef struct 
{
	SModConfigSnapPara TimeSnap;		//��ʱץͼ
	SModConfigSnapPara EventSnap;	//�¼�ץͼ
} SModConfigSnapChnPara;

#endif //_MOD_CONFIG_H_

