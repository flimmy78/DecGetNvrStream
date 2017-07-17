#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CF_LE_W(v) (v)
#define CF_LE_L(v) (v)
#define CT_LE_W(v) (v)
#define CT_LE_L(v) (v)

//��Ƶ��ʽ(���� : 12-PAL; 10-NTSC)
typedef enum
{
	EM_NTSC = 10,
	EM_PAL = 12,
} EMVIDSTANDARD;

typedef struct
{
	u32 nConfSize;	//ѹ����Ĵ�С
	u32 nFileSize;	//ѹ��ǰ�Ĵ�С
	u32 nCheckSum;	//У���
}SConfigFlashHead;

typedef struct
{
	u8 nConfSaveType;			//0:flash 1;file
	u8 nConfFormat; 			//0=Struct;1=Ini;2=XML
	s8 nDevPath[50];			//flash�豸·��
	s8 nFilePath[100];			//�����ļ�·��
	s8 nDefaultConfPath[100];	//Ĭ�������ļ�·��
	u8 nConfZip;				//0:��ѹ�� 1:ѹ��
	u32 nFlashOffset;			//���������ƫ�Ƶ�ַ
	u32 nFlashSizeLimit;		//��������Ĵ�С
	s32 nDevFd;					//open���ú���ļ�������
	sem_t sem;
	u8 nReadInfoSuccess;		//��ȡ������Ϣ�Ƿ�ɹ�
}SConfigDevInfo;

typedef struct
{
	SModConfigSystemParam* psSystemPara;
	SModConfigPreviewParam* psPreviewPara;
	SModConfigMainTainParam* psMainTainPara;
	SModConfigImagePara* psImagePara;
	SModConfigImagePara* psVoImagePara;
	SModConfigMainOsdParam* psMainOsdPara;
	SModConfigStrOsdpara* psStrOsdPara;
	SModConfigRectOsdpara* psRectOsdPara;
	SModConfigVideoParam* psVideoMainPara;
	SModConfigVideoParam* psVideoSubPara;
	SModConfigVideoParam* psVideoMobPara;
	SModConfigRecordParam* psRecordPara;
	SModConfigRecTimerSch* psRecTimerSchPara;
	SModConfigRecVMSch* psRecVMSchPara;
	SModConfigRecAlarmInSch* psRecSensorSchPara;
	SModConfigSensorParam* psSensorPara;
	//yaogang modify 20141010
	SModConfigIPCExtSensorParam* psIPCCoverPara;
	SModConfigIPCExtSensorParam* psIPCExtSensorPara;
	SModConfigIPCExtSensorParam* ps485ExtSensorPara;
	SModConfigHDDParam* psHDDPara;
	
	SModConfigVMotionParam* psVMotionPara;
	SModConfigVLostParam* psVLostPara;
	SModConfigVBlindParam* psVBlindPara;
	SModConfigAlarmOutParam* psAlarmOutPara;
	SModConfigBuzzParam* psBuzzPara;
	SModConfigSensorSch* psSensorSchPara;
	SModConfigSensorSch* psIPCExtSensorSchPara;
	SModConfigVMotionSch* psVMotionSchPara;
	SModConfigAlarmOutSch* psAlarmOutSchPara;
	SModConfigPtzParam* psPtzPara;
	SModConfigUserPara* psUserPara;
	SModConfigNetParam* psNetPara;
	//NVR used
	SModConfigIPCameraParam* psIPCameraPara;
	SModConfigSnapChnPara *psSnapPara;//yaogang modify 20150105
	//yaogang modify 20141209 ���ƽ̨����
	SModConfigSGParam *psSGPara;
	SModDailyPicCFG *psSGDailyPara;
	SModAlarmPicCFG *psSGAlarmPara;
	//
	sem_t sem;
}SConfigAll;

typedef struct
{
	s32** pConfigValList;
	s32 nNum;
} SConfigAllList;

#define MAX_LIST_LEN 128
#define MAX_STR_LEN 64

typedef struct
{
	u8 **strList;
	u8 nItem;
} StrList;

typedef struct
{
	EMCONFIGPARATYPE emType;
	s32 nId;
	void* pData;
}SConfigParaInfoWriteFile;

typedef struct
{
	EMCONFIGPARATYPE emType;
	s32 nId;
	void* pData;
}SConfigMsgHeader;

#define ZIP_BUF_LEN ((64 << 10) - 100)

s32 ConfigCreateMsgQueue(void);
s32 ConfigDestroyMsgQueue(void);
s32 ConfigWriteMsgQueue(SConfigMsgHeader* pHeader);
s32 ConfigReadMsgQueue(SConfigMsgHeader* pHeader);

s32 IniGetString(s8* strFilePath, s8* strSection, s8* strItem, s8* strOutValue, s32 nMaxStrLen);
s32 IniGetValue(s8* strFilePath, s8* strSection, s8* strItem, s32* nOutValue);
s32 IniGetValueU64(s8* strFilePath, s8* strSection, s8* strItem, u64* nOutValue);
s32 IniGetArrayString(s8* strFilePath, s8* strSection, s8* strItem, u16 nIndex, s8* strOutValue, s32 nMaxStrLen);
s32 IniGetArrayValue(s8* strFilePath, s8* strSection, s8* strItem, u16 nIndex, s32* nOutValue);
s32 IniGetArrayValueU64(s8* strFilePath, s8* strSection, s8* strItem, u16 nIndex, u64* nOutValue);
s32 GetStringFromIni(s8* strFilePath, s8* strSection, s8* strItem, s8* strOutValue, s32 nMaxStrLen);
s32 GetArrayStringFromIni(s8* strFilePath, s8* strSection, s8* strItem, u16 nIndex, s8* strOutValue, s32 nMaxStrLen);
s32 SetStringToIni(s8* strFilePath, s8* strSection, s8* strItem, s8* strNewValue);
s32 SetArrayStringToIni(s8* strFilePath, s8* strSection, s8* strItem, u16 nIndex, s8* strNewtValue);

s32 InitConfigFlashInfo(void);
s32 InitDvrProperty(void);
s32 ReadFromEEPROM(int fd,unsigned int offset,char *buffer,unsigned int len);
s32 ReadZipConf(u32 offset, s8** pData, u32* length);
s32 ReadFlashToFile(s8* pFilePath);
s32 WriteToEEPROM(int fd,unsigned int offset,char *buffer,unsigned int len);
s32 SetZipConf(u32 offset, s8* pData, u32 len);
s32 WriteFileToFlash(s8* pFilePath);
void WriteConfigFileToFlashFxn(void);

s32 ConfigLoadAllParaToMem(SConfigAll* para, s8* pFilePath);
void ConfigWriteParamToFile(SConfigParaInfoWriteFile* para);
void ConfigManagerFxn(void);
//void ConfigLoadDefaultToMemFxn(void);//csp modify

s32 ConfigGetSystemParam(SModConfigSystemParam* para, s8* pFilepath);
s32 ConfigGetMainTainParam(SModConfigMainTainParam* para, s8* pFilepath);
s32 ConfigGetPreviewParam(SModConfigPreviewParam* para, s8* pFilepath);
s32 ConfigGetImageParam(SModConfigImagePara* para, s32 nId, s8* pFilepath);
s32 ConfigGetVoImageParam(SModConfigImagePara* para, s32 nId, s8* pFilepath);
s32 ConfigGetMainOsdParam(SModConfigMainOsdParam* para, s8* pFilepath);
s32 ConfigGetStrOsdParam(SModConfigStrOsdpara* para, s32 nId, s8* pFilepath);
s32 ConfigGetRectOsdParam(SModConfigRectOsdpara* para, s32 nId, s8* pFilepath);
s32 ConfigGetMainEncodeParam(SModConfigVideoParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetSubEncodeParam(SModConfigVideoParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetRecordParam(SModConfigRecordParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetTimerRecSchParam(SModConfigRecTimerSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetVMotionRecSchParam(SModConfigRecVMSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetSensorRecSchParam(SModConfigRecAlarmInSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetSensorParam(SModConfigSensorParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetVMotionParam(SModConfigVMotionParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetVLostParam(SModConfigVLostParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetVBlindParam(SModConfigVBlindParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetAlarmOutParam(SModConfigAlarmOutParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetBuzzParam(SModConfigBuzzParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetSensorSchParam(SModConfigSensorSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetVMotionSchParam(SModConfigVMotionSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetAlarmOutSchParam(SModConfigAlarmOutSch* para, s32 nId, s8* pFilepath);
s32 ConfigGetPtzParam(SModConfigPtzParam* para, s32 nId, s8* pFilepath);
s32 ConfigGetUserParam(SModConfigUserPara* para, s8* pFilepath);
s32 ConfigGetNetParam(SModConfigNetParam* para, s32 nId, s8* pFilepath);

s32 ConfigSetSystemParam(SModConfigSystemParam* para, s8* pFilepath);
s32 ConfigSetMainTainParam(SModConfigMainTainParam* para, s8* pFilepath);
s32 ConfigSetPreviewParam(SModConfigPreviewParam* para, s8* pFilepath);
s32 ConfigSetImageParam(SModConfigImagePara* para, s32 nId, s8* pFilepath);
s32 ConfigSetVoImageParam(SModConfigImagePara* para, s32 nId, s8* pFilepath);
s32 ConfigSetMainOsdParam(SModConfigMainOsdParam* para, s8* pFilepath);
s32 ConfigSetStrOsdParam(SModConfigStrOsdpara* para, s32 nId, s8* pFilepath);
s32 ConfigSetRectOsdParam(SModConfigRectOsdpara* para, s32 nId, s8* pFilepath);
s32 ConfigSetMainEncodeParam(SModConfigVideoParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetSubEncodeParam(SModConfigVideoParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetRecordParam(SModConfigRecordParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetTimerRecSchParam(SModConfigRecTimerSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetVMotionRecSchParam(SModConfigRecVMSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetSensorRecSchParam(SModConfigRecAlarmInSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetSensorParam(SModConfigSensorParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetVMotionParam(SModConfigVMotionParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetVLostParam(SModConfigVLostParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetVBlindParam(SModConfigVBlindParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetAlarmOutParam(SModConfigAlarmOutParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetBuzzParam(SModConfigBuzzParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetSensorSchParam(SModConfigSensorSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetVMotionSchParam(SModConfigVMotionSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetAlarmOutSchParam(SModConfigAlarmOutSch* para, s32 nId, s8* pFilepath);
s32 ConfigSetPtzParam(SModConfigPtzParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetUserParam(SModConfigUserPara* para, s8* pFilepath);
s32 ConfigSetNetParam(SModConfigNetParam* para, s32 nId, s8* pFilepath);

//NVR used
s32 ConfigGetIPCameraParam(SModConfigIPCameraParam* para, s32 nId, s8* pFilepath);
s32 ConfigSetIPCameraParam(SModConfigIPCameraParam* para, s32 nId, s8* pFilepath);

//Snap
s32 ConfigGetSnapParam(SModConfigSnapChnPara* para, s32 nId, s8* pFilepath);
s32 ConfigSetSnapParam(SModConfigSnapChnPara* para, s32 nId, s8* pFilepath);

//���ƽ̨
s32 ConfigGetSGParam(SModConfigSGParam* para, s8* pFilepath);
s32 ConfigSetSGParam(SModConfigSGParam* para, s8* pFilepath);
s32 ConfigGetSGDailyParam(SModDailyPicCFG* para, s8* pFilepath);
s32 ConfigSetSGDailyParam(SModDailyPicCFG* para, s8* pFilepath);
s32 ConfigGetSGAlarmParam(SModAlarmPicCFG* para, s32 nId, s8* pFilepath);
s32 ConfigSetSGAlarmParam(SModAlarmPicCFG* para, s32 nId, s8* pFilepath);



s32 ConfigGetParamFromMem(EMCONFIGPARATYPE emType, void* para, s32 nId, SConfigAll* pAllPara);




#endif //_CONFIG_H_

