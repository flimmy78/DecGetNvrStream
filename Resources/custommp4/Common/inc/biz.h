#ifndef _BIZ_H_
#define _BIZ_H_

#include "common_basetypes.h"
#include "common_geometric.h"
#include "common_mutex.h"
#include "common_semaphore.h"
#include "common_thread.h"
#include "common_debug.h"
#include "public.h"

#include "biz_types.h"
#include "biz_errcode.h"
#include "diskmanage.h"
//#include "../../Biz/inc/biz_playback.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////ҵ����ʼ��///////////////
//ҵ����ʼ��������ʼ��
s32 BizInit(SBizInitPara* psBizPara);
s32 BizDeinit(void);

//csp modify
//У�����к�
s32 BizVerifySN();

//////////////////Ԥ��ģ�����///////////////
//�ֶ���ʼ��ֹͣԤ��
s32 BizStartPreview(SBizPreviewPara* psBizPreviewPara);
s32 BizStopPreview(void);

//��ͨ��Ԥ��
s32 BizChnPreview(u8 nChn);
//���ӷŴ�Ԥ��
s32 BizElecZoomPreview(int flag, u8 nChn, SBizPreviewElecZoom* stCapRect);
s32 BizPbElecZoomPreview(int flag, u8 nChn, SBizPreviewElecZoom* stCapRect);

//ͼƬԤ��
s32 BizPreviewSnap(int flag, SBizPreviewElecZoom* stMenuRect);

s32 BizPlayBackRegistFunCB(u8 type, BIZPBREGFUNCCB func);

//��ͨ��Ԥ������Ԥ��ֹͣ��ָ��໭��
s32 BizResumePreview(u8 nForce);
//Ԥ����һҳ
s32 BizNextPreview(void);
s32 BizLastPreview(void);
s32 BizNextPreviewMod(void);
s32 BizLastPreviewMod(void);
//��ͨ���Ŵ�
s32 BizPreviewZoomChn(u8 chn);

//�ֶ���������ֹͣԤ������
s32 BizPreviewMute(u8 nEnable);
//������ƵԤ�����ͨ��
s32 BizPreviewAudioOutChn(u8 nChn);//0xff��ʾ�Զ�
//������ƵԤ������
s32 BizPreviewVolume(u8 nVolume);
//ֱ������Ԥ��ͼ�����������������
s32 BizPreviewSetImage(u8 nChn, SBizPreviewImagePara* psImagePara);
s32 BizPlayBackSetImage(SBizVoImagePara* psVoImagePara);
//��ȡԤ��ͼ�����
s32 BizPreviewGetImage(u8 nChn, SBizPreviewImagePara* psImagePara);

//�ֶ�����ֹͣ��Ѳ
s32 BizStartPatrol(void);
s32 BizStopPatrol(void);

//////////////////¼��ģ�����///////////////
s32 BizStartManualRec(u8 nChn);
s32 BizStopManualRec(u8 nChn);
s32 BizRecordStop(u8 nChn);
s32 BizRecordStopAll(void);
s32 BizRecordPause(u8 nChn);
s32 BizRecordResume(u8 nChn);
void BizRecordSetWorkingEnable(u8 nEnable);

/////////////////����ģ��//////////////
s32 BizAlarmWorkingEnable(u8 nEnable); //nEnable: 0,ģ��ֹͣ��������0,ģ�鿪ʼ����


/////////////////�ط�ģ�����///////////////
s32 BizStartPlayback(EMBIZPLAYTYPE emBizPlayType, void* para);
s32 BizPlaybackControl(EMBIZPLAYCTRLCMD emBizPlayCtrlCmd, s32 nContext);
s32 BizPlayBackZoom(s32 nKey);
s32 BizPlayBackGetRealPlayChn(u64* pChnMask);
s32 BizPlayBackGetVideoFormat(u8 nChn);
s32 BizPlayBackSetModeVOIP(u8 nVoip);
void SetCurPlayRate(int rate);
void BizSetPlayType(int type);
int GetCurPlayRate();

//yaogang modify 20150112
s32 BizSnapDisplay(SBizRecSnapInfo *pSnapInfo);


/////////////////��̨ģ�����///////////////
s32 BizPtzCtrl(u8 nChn, EMBIZPTZCMD emCmd, s16 nId); 

//�����̨�Ƿ���Ѳ��
s32 BizPtzCheckTouring(u8 nChn);

//���������ģ��δ��ʼ��֮ǰ��Ҫ�����͵��øýӿ�
void BizSystemRestart();

/////////////////����ģ�����///////////////
//	��������:ϵͳ�˳�
s32 BizSysComplexExit(EMBIZSYSEXITTYPE emSysExitType);

//��������:ϵͳ����
s32 BizSysComplexUpgrade(EMBIZUPGRADETARGET emUpdateTarget, char *pcUpdateFName);

//��������:ϵͳ�����ļ��б�
s32 BizSysComplexUpgradeFilelist(EMBIZUPGRADETARGET emUpdateTarget, SBizUpdateFileInfoList* spFileList);

//��������:�����ļ��б�
s32 BizSysComplexConfigFilelist(char* szDir, SBizUpdateFileInfoList* spFileList);
//��������:usb�豸����·���б�
s32 BizSysComplexUsbDevMountDirlist(SBizUpdateFileInfoList* spFileList);

//��������:�ж��ļ�����
s32 BizSysCmplexBreakBackup(int type);

//���������ļ���Ʒ��֤�ͺ�
void BizSysComplexSetProductModel(char* updatemodel);

//��ȡ�´��̲���ʱϵͳ���״̬
u8 BizSysGetInsertDiskStatue();

//���ô���ϵͳ���״̬
void BizSysSetRefreshDiskStatue(u8 flag);

//��ȡ�����ϴ����̼��״̬
u8 BizSysGetCheckDiskStatue();
	
//��������:�ļ�����
s32 BizSysComplexBackup(EMBIZFILETYPE emFileType, 
						 PSBizBackTgtAtt	psBackTgtAtt, 
						 SBizSearchCondition* psSearchCondition);

//��������:��ô洢�豸������Ϣ
s32 BizSysComplexDMGetInfo(SBizStoreDevManage *psStoreDevManage, u8 nMaxDiskNum);

//��������:��ʽ��
s32 BizSysComplexDMFormat(char *pstrDevPath, EMBIZFORMATWAY emFormatWay);

//��������:�ļ�����
s32 BizSysComplexDMSearch(EMBIZFILETYPE emFileType, SBizSearchPara* psSearchPara, SBizSearchResult* psSearchResult, u16 nMaxFileNum);

//��������:����ϵͳ����ʱ��
s32 BizSysComplexDTSet(const SBizDateTime* psDateTime);

//��������:���ϵͳ����ʱ��
s32 BizSysComplexDTGet(SBizDateTime* psDateTime);

//��������:��ø�ʽ��ϵͳ����ʱ���ַ���
s32 BizSysComplexDTGetString(char* pstrDateTime);

//��ñ������
s32 BizGetVMainEncodePara(u32 bDefault,SBizEncodePara *psPara, u8 nId);
s32 BizSetVMainEncodePara(SBizEncodePara *psPara, u8 nId);
s32 BizGetVSubEncodePara(u32 bDefault,SBizEncodePara *psPara, u8 nId);
s32 BizSetVSubEncodePara(SBizEncodePara *psPara, u8 nId);

//��ȡ/������ѯ����
s32 BizGetIsPatrolPara(u32 bDefault,SBizCfgPatrol *psPara, u8 nId);
s32 BizSetIsPatrolPara(SBizCfgPatrol *psPara, u8 nId);

s32 BizSendBizEvent(SBizEventPara* psBizEventPara);

//��ȡ����汾��
s32 BizConfigGetDvrInfo(u32 flag, char* sDvr_info, char* sDvr_Model);
//////////////////����ģ�����///////////////
s32 BizConfigDefault(void);//�ָ�Ĭ������
s32 BizConfigBackup(char* pszFileName); //���������ļ�
s32 BizConfigResume(EMBIZCONFIGPARATYPE emType, s32 nId, s8* pFilePath); //�����ñ����ļ��лָ�����
s32 BizConfigResumeDefault(EMBIZCONFIGPARATYPE emType, s32 nId); // ��Ĭ�������ļ��лָ�����

//��ò����ַ���ȡֵ�б�
s32 BizConfigGetParaStr(EMBIZCFGPARALIST emParaType, PARAOUT s8* strList[], PARAOUT u8* nRealListLen, u8 nMaxListLen, u8 nMaxStrLen);
//���ݲ���ȡֵ����ַ����б��е�index
s32 BizConfigGetParaListIndex(EMBIZCFGPARALIST emParaType, s32 nValue, PARAOUT u8* pnIndex);
//����index���ʵ�ʲ���ȡֵ
s32 BizConfigGetParaListValue(EMBIZCFGPARALIST emParaType, u8 nIndex, PARAOUT s32* pnValue);

//////////////////���ü���������ģ���������///////////////
//��ü����ò���
//(����ʱ���ֻ�ı�ṹ�岿�ֲ�����get��set)
s32 BizSetPara(SBizParaTarget* psBizParaTarget, void* pData);
s32 BizGetPara(SBizParaTarget* psBizParaTarget, PARAOUT void* pData);
s32 BizGetDefaultPara(SBizParaTarget* psBizParaTarget, PARAOUT void* pData);

int BizGetVideoFormatReal(void);

//�û�
s32 BizUserLogin(SBizUserLoginPara* para, u32* nUserId);
s32 BizUserLogout(u32 nUserId);
EMBIZUSERCHECKAUTHOR BizUserCheckAuthority(EMBIZUSEROPERATION emOperation,u32 * para,u32 nUserId);
s32 BizUserAddUser(SBizUserUserPara* para);
s32 BizUserDeleteUser(s8* pUserName);

//��־
s32 BizWriteLog(SBizLogInfo* psLogInfo);
s32 BizSetUserNameLog(char* pName);//cw_log
s32 BizSearchLog(SBizLogSearchPara* psLogSearch, SBizLogResult* psLogResult);
s32 BizExportLog(SBizLogResult* psLogResult, u8* pPath);
// ����ģ��ӿ�

// ��ȡ����״̬ ����/�Ͽ�
EM_BIZNET_STATE BizNetGetState();
s32 BizNetReadCfg( SBizNetPara* pCfg );
s32 BizNetReadCfgEx( SBizNetPara* pCfg );
s32 BizNetWriteCfg( SBizNetPara* pCfg );
int BizSetHWAddr(char *pBuf);
void BizNetStartDHCP();
void BizNetStopDHCP();

s32 BizNetStartPPPoE(char *pszUser, char* pszPasswd);
s32 BizNetStopPPPoE();

s32 BizNetSendMail(SBizMailContent* pMail);

s32 BizNetDdnsCtrl( EM_BIZ_DDNS_ACT eAct, sBizNetDdnsPara* pPara );

s32 BizGetDvrInfo( SBizDvrInfo* pInfo );

s32 BizGetSysInfo(SBizSystemPara* sysPara);

s32 BizNetReqConnStatus(SBizNetConnStatus* psNCS);

void BizSetZoomMode(u8 flag);//cw_zoom

void BizSetDwellStartFlag(u8 flag);
u8 BizGetDwellStartFlag();
void BizSetUploadAlarmPara(u8 type, u8 id, u8 state);

// return:  0 closed, 1 opened
u8 BizNetGetVoipState( void );

BOOL DiskIsExist(); //add by Lirl on Nov/11/2011
u8 GetHardDiskNum();
s32 BizGetMaxChnNum(void);

void BizSetCurRecStatus(u8 nChn,u8 flag);
s32 BizGetCurRecStatus(u8 nChn);

int GetChnNameMax(void);

typedef void (*CHECKNOAUTH)(void);//cw_auth
	
void BizSetNoAuthFuncCB(CHECKNOAUTH func);
u8 CheckAllHDDFormated();

typedef void (*PREVIEWMODECB)(EMBIZPREVIEWMODE);//cw_preview

typedef void (*LEDDTRLFN)(int flag,int type);//cw_led

void RegLedCtrlFUN(LEDDTRLFN fun);

void SetPrevicwModeFuncCB(PREVIEWMODECB func);

#if 0//csp modify
u64 GetTimeStamp();//cw_time
#endif

void SetBizResolHNum(int num);//cw_9508S
int GetBizResolHNum();

void BizNetUploadAlarm(u32 key);
void BizSetUploadAlarmPara(u8 type, u8 id, u8 state);
u8 GetUpgradeStatue();

#ifdef __cplusplus
}
#endif

#endif

