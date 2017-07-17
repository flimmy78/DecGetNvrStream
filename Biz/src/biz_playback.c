#include "biz_playback.h"

void PlaybackDeal(SPBCallbackInfo* pbCBInfo);//declare

//��ʱ��ط�
s32 PlayBackByTime(SBizSearchPara* pSearchParam);
//���ļ��ط�
s32 PlayBackByFile(SBizRecfileInfo* pFileInfo);

s32 PlaybackInit(u8 nEnable, SBizPlayback* psInitPara)
{
    SBizManager* psBizManager = &g_sBizManager;
    
    if (nEnable && psInitPara)
    {
        psBizManager->hPb = ModPlayBackInit((u32)psInitPara->nMaxChn, psInitPara->hDisk, psInitPara->nChnMaskOfD1);
        if (NULL == psBizManager->hPb)
        {
            return -1;
        }

		ModPlayBackProgress(PlaybackDeal);
    }

    return 0;
}

s32 PlayBackRegistFunCB(u8 type, PBREGFUNCCB func)
{
	return ModPlayBackRegistFunCB(type, func);
}

void PlaybackDeal(SPBCallbackInfo* pbCBInfo)
{
    SBizEventPara sBizEventPara;
	memset(&sBizEventPara,0,sizeof(SBizEventPara));
	
	u32 nTotal = pbCBInfo->nTotalTime;
	u64 nCurTime = pbCBInfo->nCurTime;
	u32 nProg = pbCBInfo->nProg;
	BOOL bStop = pbCBInfo->bStop;
	
	//printf("nProg = %d\n",nProg);
	if(bStop)
	{
		sBizEventPara.emType = EM_BIZ_EVENT_PLAYBACK_DONE;
	    sBizEventPara.sBizProgress.lCurSize = nCurTime;
	    sBizEventPara.sBizProgress.lTotalSize = nTotal;
	    sBizEventPara.sBizProgress.nProgress = nProg;
	}
	else
	{
		sBizEventPara.emType = EM_BIZ_EVENT_PLAYBACK_RUN;
	   	sBizEventPara.sBizProgress.lCurSize = nCurTime;
	    sBizEventPara.sBizProgress.lTotalSize = nTotal;
	    sBizEventPara.sBizProgress.nProgress = nProg;
	}
	
	//sBizEventPara.emType = 100000;
    
    SendBizEvent(&sBizEventPara);
}

//��ʼ�ط�
s32 PlaybackStart(SBizPbStart *psPbStart)
{
    if (psPbStart)
    {
        switch (psPbStart->emBizPlayType)
        {
            case EM_BIZPLAY_TYPE_TIME:
                return PlayBackByTime((SBizSearchPara *)psPbStart->para);
            case EM_BIZPLAY_TYPE_FILE:
                return PlayBackByFile((SBizRecfileInfo *)psPbStart->para);
            default:
                break;
        }
    }

    return -1;
}

s32 PlayBackByTime(SBizSearchPara* pSearchParam)
{
    SBizManager* psBizManager = &g_sBizManager;
    
    return ModPlayBackByTime(psBizManager->hPb, (SPBSearchPara*)pSearchParam);
}

//���ļ��ط�
s32 PlayBackByFile(SBizRecfileInfo* pFileInfo)
{
    SBizManager* psBizManager = &g_sBizManager;
    
    return ModPlayBackByFile(psBizManager->hPb, (SPBRecfileInfo *)pFileInfo);
}

//�طſ��� 
s32 PlayBackControl(SBizPbControl* psBizPbCtl)
{
    EmPBCtlCmd emPBCtlcmd;
    u32 nContext;

    if (psBizPbCtl)
    {
        SBizManager* psBizManager = &g_sBizManager;
        
        emPBCtlcmd = psBizPbCtl->emPBCtlcmd;
        nContext = psBizPbCtl->nContext;

		//printf("PlayBackControl emPBCtlcmd = %d\n ",emPBCtlcmd);
        return ModPlayBackControl(psBizManager->hPb, emPBCtlcmd, nContext);
    }
    
    return -1;
}

s32 PlayBackZoom(s32 nKey)
{
	return ModPlayBackZoom(g_sBizManager.hPb, nKey);
}

s32 PlayBackGetRealPlayChn(u64* pChnMask)
{
	return ModPlayBackGetRealPlayChn(g_sBizManager.hPb, pChnMask);
}

s32 PlayBackGetVideoFormat(u8 nChn)
{
	return ModPlayBackGetVideoFormat(g_sBizManager.hPb,nChn);
}

s32 PlayBackSetModeVOIP(u8 nVoip)
{
	return ModPlayBackSetModeVOIP(g_sBizManager.hPb, nVoip);
}

//yaogang modify 20150112
s32 SnapDisplay(SBizRecSnapInfo *pSnapInfo)
{
	//SBizManager* psBizManager = &g_sBizManager;

	//return ModSnapDisplay(psBizManager->hPb, (SPBRecSnapInfo *)pSnapInfo);
	return ModSnapDisplay((SPBRecSnapInfo *)pSnapInfo);
}

