#ifndef _BIZ_SYSCOMPLEX_H_
#define _BIZ_SYSCOMPLEX_H_

#include "biz.h"
#include "mod_syscomplex.h"

//��������
typedef struct {
    EMBIZUPGRADETARGET emUpdateTarget;
    char *pcUpdateFName;
} SBizUpgrade;

typedef struct {
	EMBIZUPGRADETARGET emUpdateTarget;
	SBizUpdateFileInfoList* spFileList;
} SBizUpgradeFileInfo;

//���ݲ���
typedef struct {
    EMBIZFILETYPE emFileType;
    PSBizBackTgtAtt	psBackTgtAtt;
    SBizSearchCondition* psSearchCondition;
} SBizBackup;

//���̹�����Ϣ
typedef struct {
    SBizStoreDevManage *psStoreDevManage;
    u8 nMaxDiskNum;
} SBizDiskManage;

//���̸�ʽ��
typedef struct {
    char *pstrDevPath;
    EMBIZFORMATWAY emFormatWay;
} SBizDiskFormat;

//�ļ�����
typedef struct {
    EMBIZFILETYPE emFileType;
    SBizSearchPara* psSearchPara;
    SBizSearchResult* psSearchResult;
    u16 nMaxFileNum;
} SBizDiskSearch;

typedef struct
{
    HDDHDR hddHdr;
} SBizSysComplex;

s32 SysComplexInit(u8 nEnable, SBizSysComplex* psInitPara);
s32 SysComplexExit(EMBIZSYSEXITTYPE emExitType);
s32 SysComplexUpgrade(SBizUpgrade* psBizUpgrade);
s32 SysComplexBackup(SBizBackup* pSPara);
s32 SysComplexBreakBackup(int type);
s32 SysComplexDiskManage(SBizDiskManage* pSPara);
s32 SysComplexDiskFormat(SBizDiskFormat* pSPara);
s32 SysComplexDiskSearch(SBizDiskSearch* pSPara);
s32 SysComplexDTSet(const SBizDateTime* pSPara);
s32 SysComplexSetTimeZone(int nTimeZone);
s32 SysComplexDTGet(SBizDateTime* pSPara);
s32 SysComplexDTGet_TZ(SBizDateTime* pSPara);
s32 SysComplexDTGetStr(char* pSPara);

u8 SysGetInsertDiskStatue();
void SysComplexSetProductModel(char* updatemodel);

#endif

