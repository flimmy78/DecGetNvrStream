
#ifndef _TOOL_UPGRADE_H_
#define _TOOL_UPGRADE_H_

//create by lbzhu, 2011-11-23
#include "common_basetypes.h"

/*
*����� flash��С���ڴ�ʱ��
*for UpgradeCheckFile ������strModel���� 

*/
#define SIMPLE_MODEL_CHK "SIMPLE_MODEL_CHK"

typedef enum{//�����ֵ���������޸�
	EM_UPGRADE_FT_BYFNAME = 0,
	EM_UPGRADE_FT_BOOTLOAD = 1,
	EM_UPGRADE_FT_KERNEL = 2,
	EM_UPGRADE_FT_FS_ROOT = 3,
	EM_UPGRADE_FT_FS_NORMAL = 4,
	EM_UPGRADE_FT_OTHER = 5,
	EM_UPGRADE_FT_CONTAINER = 6 //�Ÿ��������ļ�
} EMUPGRADEFILETYPE;

typedef s32 (*pFnUpgradeProgressCb)(u64 lTotalSize, u64 lCurSize);

s32 UpgradeFileGenerate(int argc, char** argv);
s32 UpgradeFileDestruct(const char* strUpgradeFile);
s32 UpgradeFileOut(const char* strUpgradeFile, EMUPGRADEFILETYPE emType, char* strFileOutName, u32 nMaxNameLen);
s32 UpgradeFileRenew(const char* strUpgradeFile, EMUPGRADEFILETYPE emType, char* strFileInName);
s32 UpgradeConvertToFlashFile(const char* strUpgradeFile);

void UpgradeSetModel(char* strModel);
s32 UpgradeCheckFile(char* strModel, char* strFileName);
s32 UpgradeToFlash(char* strFileName, pFnUpgradeProgressCb pFnCb);
s32 UpgradeStartLogo(char* strLogoFile, pFnUpgradeProgressCb pFnCb, char* strFlashDev, int nOft, int nSize);
s32 UpgradeAppLogo(char* strLogoFile, pFnUpgradeProgressCb pFnCb, char* strFlashDev, int nOft, int nSize);
#endif

