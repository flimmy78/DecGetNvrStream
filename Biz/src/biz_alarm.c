#include "biz_alarm.h"

void AlarmDeal(SAlarmCbData* psAlarmCbData);

static int TimeZoneOffset[] = 
{
	-12*3600,
	-11*3600,
	-10*3600,
	-9*3600,
	-8*3600,
	-7*3600,
	-6*3600,
	-5*3600,
	-4*3600-1800,
	-4*3600,
	-3*3600-1800,
	-3*3600,
	-2*3600,
	-1*3600,
	0,
	1*3600,
	2*3600,
	3*3600,
	3*3600+1800,
	4*3600,
	4*3600+1800,
	5*3600,
	5*3600+1800,
	5*3600+2700,
	6*3600,
	6*3600+1800,
	7*3600,
	8*3600,
	9*3600,
	9*3600+1800,
	10*3600,
	11*3600,
	12*3600,
	13*3600,
};

static int GetTZOffset(int index)
{
	if(index < 0 || index >= (int)(sizeof(TimeZoneOffset)/sizeof(TimeZoneOffset[0])))
	{
		return 0;
	}
	
	return TimeZoneOffset[index];
}

s32 AlarmInit(u8 nEnable, SBizAlarm* psInitPara)
{
	if(nEnable && psInitPara)
	{
		//csp modify 20131213
		SBizSystemPara sysPara;
		memset(&sysPara, 0, sizeof(SBizSystemPara));
		ConfigGetSystemPara(0, &sysPara);
		int nTimeZone = sysPara.nTimeZone;
		ModAlarmSetTimeZoneOffset(GetTZOffset(nTimeZone));
		
		SAlarmInitPara sAlarmInitPara;
		SBizManager* psBizManager = &g_sBizManager;
		
		sAlarmInitPara.nAlarmOutNum = psInitPara->nAlarmOutNum;
		sAlarmInitPara.nAlarmSensorNum = psInitPara->nAlarmSensorNum;
		sAlarmInitPara.nBuzzNum = psInitPara->nBuzzNum;
		sAlarmInitPara.nVBlindLuma = psInitPara->nVBlindLuma;
		sAlarmInitPara.nVideoChnNum = psInitPara->nVideoChnNum;
		sAlarmInitPara.nDiskNum = psInitPara->nDiskNum;
		sAlarmInitPara.pfnAlarmCb = AlarmDeal;
		sAlarmInitPara.nNVROrDecoder = psInitPara->nNVROrDecoder;//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
		
		if(0 != ModAlarmInit(&sAlarmInitPara, &psBizManager->hAla))
		{
			return -1;
		}
	}
	else
	{
		//�����ض໭��
		return 0;//csp modify
	}
	
	printf("Alarm init OK!!!\n");
	
	return 0;
}

//csp modify 20130326
#define	CMD_H8_LED8			(1 << 3)	//����CH8-16�ĵ�
#define	CMD_H8_LED0			(1 << 4)	//����CH0-7�ĵ�

static u16 chnstatus 		= 0x0;

void ChnLedInit(u32 status)
{
#if defined(CHIP_HISI3531)// || defined(CHIP_HISI3521)
	chnstatus = status & 0xffff;
	
	char buf[2]={0};
	buf[0] = CMD_H8_LED0;
	buf[1] = (chnstatus & 0xff);
	FrontboardWrite(buf,2);
	
	buf[0] = CMD_H8_LED8;
	buf[1] = (chnstatus >> 8) & 0xff;
	FrontboardWrite(buf,2);
#endif
}

void ChnLedCtrl(int flag,int chn)
{
#if defined(CHIP_HISI3531)// || defined(CHIP_HISI3521)
	if(chn >= 16)
	{
		return;
	}
	flag = flag?1:0;
	
	char buf[2]={0};
	u16 newstatus = (chnstatus & ~(1<<chn)) | (flag<<chn);
	if(chn < 8)
	{
		if((newstatus & 0xff) != (chnstatus & 0xff))
		{
			buf[0] = CMD_H8_LED0;
			buf[1] = (newstatus & 0xff);
			FrontboardWrite(buf,2);
		}
	}
	else
	{
		if((newstatus & 0xff00) != (chnstatus & 0xff00))
		{
			buf[0] = CMD_H8_LED8;
			buf[1] = (newstatus >> 8) & 0xff;
			FrontboardWrite(buf,2);
		}
	}
	
	chnstatus = newstatus;
#endif
}

//yaogang modify 20141118
/*
0		//Ӳ��δ��ʽ������
1		//Ӳ�̶�ʧ����
2		//Ӳ�̶�д������
3		//Ӳ��������
4		//�ź�������
5		//��Ƶ��ʧ����
6		//�ƶ���ⱨ��
7		//IPC�ڸǱ���
8		//��ʽ��ƥ�䱨��
9		//�Ƿ����ʱ���
10		//IPC�ⲿ����
11		//485��չ����

*/
void AlarmDeal(SAlarmCbData* psAlarmCbData)
{
	//printf("AlarmDeal......\n");
	SBizManager* psBizManager = &g_sBizManager;
	
	if(psAlarmCbData)
	{
		//printf("AlarmDeal... SAlarmCbData:event:%d chn:%d, nData:%d\n", psAlarmCbData->emAlarmEvent, psAlarmCbData->nChn, psAlarmCbData->nData);
		
		SBizEventPara sBizEventPara;
		
		sBizEventPara.emType = EM_BIZ_EVENT_UNKNOW;
		sBizEventPara.sBizAlaStatus.nChn = psAlarmCbData->nChn;
		sBizEventPara.sBizAlaStatus.nLastStatus = sBizEventPara.sBizAlaStatus.nCurStatus;//why???
		sBizEventPara.sBizAlaStatus.nCurStatus = psAlarmCbData->nData;
		sBizEventPara.sBizAlaStatus.nTime = psAlarmCbData->nTime;
		switch(psAlarmCbData->emAlarmEvent)
		{
			//yaogang modify 20160201
			//nvr.cpp ��Զ��NVR ����
		case EM_ALARM_EVENT_RemoteNVR_DISK_LOST:  //Ӳ�̶�ʧ����
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_RemoteNVR_DISK_ERR\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_RemoteNVR_DISK_LOST;
					sBizEventPara.sBizAlaStatus.nvrIP = psAlarmCbData->nvrIP;
					//BizSetUploadAlarmPara(5, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_RemoteNVR_DISK_ERR:  //Ӳ�̶�д����
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_RemoteNVR_DISK_ERR\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_RemoteNVR_DISK_ERR;
					sBizEventPara.sBizAlaStatus.nvrIP = psAlarmCbData->nvrIP;
					//BizSetUploadAlarmPara(5, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_RemoteNVR_VLOST:  //��Ƶ��ʧ����
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_RemoteNVR_VLOST\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_RemoteNVR_VLOST;
					sBizEventPara.sBizAlaStatus.nvrIP = psAlarmCbData->nvrIP;
					//BizSetUploadAlarmPara(5, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_SENSOR:  //�����������¼�
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_SENSOR\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_SENSOR;
					BizSetUploadAlarmPara(5, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_DISK_LOST:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_DISK_LOST\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_DISK_LOST;
					BizSetUploadAlarmPara(1, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_DISK_ERR:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_DISK_ERR\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_DISK_ERR;
					BizSetUploadAlarmPara(2, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_DISK_NONE:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_DISK_NONE\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_DISK_NONE;
					BizSetUploadAlarmPara(4, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_485EXT:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_485EXT\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_485EXT;
					BizSetUploadAlarmPara(12, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_IPCEXT:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_IPCEXT\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_IPCEXT;
					BizSetUploadAlarmPara(11, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_IPCCOVER:
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//printf("yg EM_ALARM_EVENT_IPCCOVER\n");
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_IPCCOVER;
					BizSetUploadAlarmPara(8, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_VLOST: //��Ƶ��ʧ�����¼�
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					//csp modify 20130326
					ChnLedCtrl(!GetAlarmCheckStaue(),sBizEventPara.sBizAlaStatus.nChn);
					
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_VLOSS;
					BizSetUploadAlarmPara(6, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_VBLIND:  //�ڵ������¼�
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_VBLIND;
					BizSetUploadAlarmPara(8, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
			}
		case EM_ALARM_EVENT_VMOTION:  //�ƶ���ⱨ���¼�
			{
				if(EM_BIZ_EVENT_UNKNOW == sBizEventPara.emType)
				{
					sBizEventPara.emType = EM_BIZ_EVENT_ALARM_VMOTION;
					BizSetUploadAlarmPara(7, sBizEventPara.sBizAlaStatus.nChn, GetAlarmCheckStaue());
				}
				
				//SendBizEvent(&sBizEventPara);
				//SendBizEvent2(&sBizEventPara);
				//�¼��ص�****************************************************************
				psBizManager->sBizPara.pfnBizEventCb(&sBizEventPara);
			}
			break;
		case EM_ALARM_EVENT_DISPATCH_ZOOMCHN: //����ͨ���Ŵ��¼�;
		case EM_ALARM_EVENT_DISPATCH_EMAIL: //����email�¼�;�ݲ�֧��
		case EM_ALARM_EVENT_DISPATCH_SNAP: //����ץͼ�¼�;�ݲ�֧��
		case EM_ALARM_EVENT_DISPATCH_REC_SENSOR: //����������¼���¼�
		case EM_ALARM_EVENT_DISPATCH_REC_IPCEXTSENSOR:
		case EM_ALARM_EVENT_DISPATCH_REC_IPCCOVER:
		case EM_ALARM_EVENT_DISPATCH_REC_VMOTION: //����������¼���¼�
		case EM_ALARM_EVENT_DISPATCH_REC_VBLIND: //����������¼���¼�
		case EM_ALARM_EVENT_DISPATCH_REC_VLOST: //����������¼���¼�
		case EM_ALARM_EVENT_DISPATCH_PTZ_PRESET: //������̨����Ԥ�õ��¼�
		case EM_ALARM_EVENT_DISPATCH_PTZ_PATROL: //������̨����Ѳ���¼�
		case EM_ALARM_EVENT_DISPATCH_PTZ_LOCUS: //������̨�����켣�¼�
			//SendBizDispatch(psAlarmCbData);
			AlarmDispatchEvent(psAlarmCbData);
			break;
		//csp modify 20130326
		case EM_ALARM_EVENT_CTRL_CHN_LED:
			ChnLedInit(psAlarmCbData->reserved[0]);
			break;
		default:
			break;
		}
	}
	
	//printf("AlarmDeal......Over!!!!\n");
}

s32 AlarmSetPara(u8 nChn, EMBIZPARATYPE emType, void* pData)
{
	//return 0;
	
	if(pData)
    {
        SAlarmPara sAlaPara;
        EMALARMPARATYPE emAlaType = EM_ALARM_PARA_NULL;
        SBizManager* psBizManager = &g_sBizManager;
        
        switch(emType)
        {
            case EM_BIZ_SENSORPARA:  //����������
                emAlaType = EM_ALARM_PARA_SENSOR;
                //memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
                SBizAlarmSensorPara* SensorPara = (SBizAlarmSensorPara*)pData;
				sAlaPara.sAlaSensorPara.emType = SensorPara->nType;
				sAlaPara.sAlaSensorPara.nEnable = SensorPara->nEnable;
				sAlaPara.sAlaSensorPara.nDelay = SensorPara->nDelay;
				strcpy(sAlaPara.sAlaSensorPara.name, SensorPara->name);
				//printf("chn:%d, type:%d, enable:%d\n", nChn, SensorPara->nType, SensorPara->nEnable);
	    case EM_BIZ_IPCEXTSENSORPARA:
		if (EM_ALARM_PARA_NULL == emAlaType)
		{
			emAlaType = EM_ALARM_PARA_IPCEXTSENSOR;
	                //memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
	                SBizAlarmIPCExtSensorPara* SensorPara = (SBizAlarmIPCExtSensorPara* )pData;
					sAlaPara.sAlaIPCExtSensorPara.nEnable = SensorPara->nEnable;
					sAlaPara.sAlaIPCExtSensorPara.nDelay = SensorPara->nDelay;
		}
	 case EM_BIZ_485EXTSENSORPARA:
		if (EM_ALARM_PARA_NULL == emAlaType)
		{
			emAlaType = EM_ALARM_PARA_485EXTSENSOR;
	                //memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
	                SBizAlarmIPCExtSensorPara* SensorPara = (SBizAlarmIPCExtSensorPara* )pData;
					sAlaPara.sAla485ExtSensorPara.nEnable = SensorPara->nEnable;
					sAlaPara.sAla485ExtSensorPara.nDelay = SensorPara->nDelay;
		}
	case EM_BIZ_IPCCOVERPARA:
		if (EM_ALARM_PARA_NULL == emAlaType)
		{
			emAlaType = EM_ALARM_PARA_IPCCOVER;
	                //memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
	                SBizAlarmIPCExtSensorPara* SensorPara = (SBizAlarmIPCExtSensorPara* )pData;
					sAlaPara.sAlaIPCCoverPara.nEnable = SensorPara->nEnable;
					sAlaPara.sAlaIPCCoverPara.nDelay = SensorPara->nDelay;
		}
	case EM_BIZ_HDDPARA:
		if (EM_ALARM_PARA_NULL == emAlaType)
		{
			emAlaType = EM_ALARM_PARA_HDD;
	                //memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
	                SBizAlarmHDDPara* SensorPara = (SBizAlarmHDDPara* )pData;
					sAlaPara.sAlaHDDPara.nEnable = SensorPara->nEnable;
					//sAlaPara.sAla485ExtSensorPara.nDelay = SensorPara->nDelay;
		}
            case EM_BIZ_VMOTIONPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VMOTION;
                    //memcpy(&sAlaPara.sAlaVMotionPara, pData, sizeof(SBizAlarmVMotionPara));
                    SEncodeParam sEncPara;
					SBizDvrInfo sDvr;
					EMCHNTYPE emChnType = EM_CHN_VIDEO_MAIN;
					ModEncodeGetParam(nChn, emChnType, &sEncPara);
					ConfigGetDvrInfo(0,&sDvr);
					
					sAlaPara.sAlaVMotionPara.emSetType = EM_ALMARM_VMOTION_AREA_SET;
					sAlaPara.sAlaVMotionPara.emResol = sEncPara.sVideoMainParam.emVideoResolution;
					sAlaPara.sAlaVMotionPara.nCols = sDvr.nMDAreaCols;
					sAlaPara.sAlaVMotionPara.nRows = sDvr.nMDAreaRows;
					
                    SBizAlarmVMotionPara* VMotionPara = (SBizAlarmVMotionPara*)pData;
					sAlaPara.sAlaVMotionPara.nEnable = VMotionPara->nEnable;
					sAlaPara.sAlaVMotionPara.nSensitivity = VMotionPara->nSensitivity;
					sAlaPara.sAlaVMotionPara.nDelay = VMotionPara->nDelay;
					memcpy(sAlaPara.sAlaVMotionPara.nBlockStatus, VMotionPara->nBlockStatus, sizeof(VMotionPara->nBlockStatus));
					
					if(VMotionPara->nEnable)
					{
						EncodeRequestStartWithoutRec(nChn, EM_CHN_VIDEO_MAIN);
					}	
					else
					{
						EncodeRequestStopWithoutRec(nChn, EM_CHN_VIDEO_MAIN);
					}
                }
            case EM_BIZ_VBLINDPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VBLIND;
                    //memcpy(&sAlaPara.sAlaVBlindPara, pData, sizeof(SBizAlarmVBlindPara));
                    SBizAlarmVBlindPara* VBlindPara = (SBizAlarmVBlindPara*)pData;
					sAlaPara.sAlaVBlindPara.nDelay = VBlindPara->nDelay;
					sAlaPara.sAlaVBlindPara.nEnable = VBlindPara->nEnable;
                }
            case EM_BIZ_VLOSTPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VLOST;
                    //memcpy(&sAlaPara.sAlaVLostPara, pData, sizeof(SBizAlarmVLostPara));
                    SBizAlarmVLostPara* VLostPara = (SBizAlarmVLostPara*)pData;
					sAlaPara.sAlaVLostPara.nDelay = VLostPara->nDelay;
					sAlaPara.sAlaVLostPara.nEnable = VLostPara->nEnable;
                }
            case EM_BIZ_ALARMOUTPARA: //�����������
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_ALARMOUT;
                    //memcpy(&sAlaPara.sAlaOutPara, pData, sizeof(SBizAlarmOutPara));
                    SBizAlarmOutPara* AlarmOutPara = (SBizAlarmOutPara*)pData;
					//sAlaPara.sAlaOutPara.emType = AlarmOutPara->nType;
					if(AlarmOutPara->nType == EM_BIZ_ALARM_NORMAL_CLOSE)
					{
						sAlaPara.sAlaOutPara.emType = EM_ALARM_SENSOR_NORMALCLOSE;
					}
					else
					{
						sAlaPara.sAlaOutPara.emType = EM_ALARM_SENSOR_NORMALOPEN;
					}
					sAlaPara.sAlaOutPara.nDelay = AlarmOutPara->nDelay;
					sAlaPara.sAlaOutPara.nEnable = AlarmOutPara->nEnable;
					strcpy(sAlaPara.sAlaOutPara.name, AlarmOutPara->name);
                }
            case EM_BIZ_ALARMBUZZPARA: //�������������
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_BUZZ;
                    //memcpy(&sAlaPara.sAlaBuzzPara, pData, sizeof(SBizAlarmBuzzPara));
                    SBizAlarmBuzzPara* BuzzPara = (SBizAlarmBuzzPara*)pData;
					sAlaPara.sAlaBuzzPara.nDelay = BuzzPara->nDelay;
					sAlaPara.sAlaBuzzPara.nEnable = BuzzPara->nEnable;
					//printf("buzz enable: %d\n", BuzzPara->nEnable);
					sAlaPara.sAlaBuzzPara.nDuration = 0;
					sAlaPara.sAlaBuzzPara.nInterval = 0;					
                }
             //һ�㱨���¼�������������
            case EM_BIZ_DISPATCH_SENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_SENSOR;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
	    case EM_BIZ_DISPATCH_IPCEXTSENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_IPCEXTSENSOR;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
	case EM_BIZ_DISPATCH_485EXTSENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_485EXTSENSOR;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
	case EM_BIZ_DISPATCH_IPCCOVER:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_IPCCOVER;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VMOTION:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VMOTION;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VBLIND:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VBLIND;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VLOST:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VLOST;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
	case EM_BIZ_DISPATCH_HDD:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_HDD;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            //һ�㱨���¼���������
            case EM_BIZ_SCHEDULE_SENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_SENSOR;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
	case EM_BIZ_SCHEDULE_IPCEXTSENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_IPCEXTSENSOR;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VMOTION:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VMOTION;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VBLIND:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VBLIND;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VLOST:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VLOST;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            //��������¼���������
            case EM_BIZ_SCHEDULE_ALARMOUT:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_ALARMOUT;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_BUZZ: //��ʱ��֧��
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_BUZZ;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
                
                return ModAlarmSetParam(psBizManager->hAla, emAlaType, nChn, &sAlaPara);
            default:
                break;
        }
    }
	
    return -1;
}

s32 AlarmGetPara(u8 nChn, EMBIZPARATYPE emType, void* pData)
{
    if (pData)
    {
       SAlarmPara sAlaPara;
       EMALARMPARATYPE emAlaType = EM_ALARM_PARA_NULL;
       SBizManager* psBizManager = &g_sBizManager;
   #if 0
        switch (emType)
        {
            case EM_BIZ_SENSORPARA:  //����������
                emAlaType = EM_ALARM_PARA_SENSOR;
                memcpy(&sAlaPara.sAlaSensorPara , pData, sizeof(SBizAlarmSensorPara));
            case EM_BIZ_VMOTIONPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VMOTION;
                    memcpy(&sAlaPara.sAlaVMotionPara, pData, sizeof(SBizAlarmVMotionPara));
                }
            case EM_BIZ_VBLINDPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VBLIND;
                    memcpy(&sAlaPara.sAlaVBlindPara, pData, sizeof(SBizAlarmVBlindPara));
                }
            case EM_BIZ_VLOSTPARA:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_VLOST;
                    memcpy(&sAlaPara.sAlaVLostPara, pData, sizeof(SBizAlarmVLostPara));
                }
            case EM_BIZ_ALARMOUTPARA: //�����������
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_ALARMOUT;
                    memcpy(&sAlaPara.sAlaOutPara, pData, sizeof(SBizAlarmOutPara));
                }
            case EM_BIZ_ALARMBUZZPARA: //�������������
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_PARA_BUZZ;
                    memcpy(&sAlaPara.sAlaBuzzPara, pData, sizeof(SBizAlarmBuzzPara));
                }
             //һ�㱨���¼�������������
            case EM_BIZ_DISPATCH_SENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_SENSOR;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VMOTION:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VMOTION;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VBLIND:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VBLIND;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            case EM_BIZ_DISPATCH_VLOST:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_DISPATCH_VLOST;
                    memcpy(&sAlaPara.sAlaDispatch, pData, sizeof(SBizAlarmDispatch));
                }
            //һ�㱨���¼���������
            case EM_BIZ_SCHEDULE_SENSOR:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_SENSOR;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VMOTION:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VMOTION;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VBLIND:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VBLIND;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_VLOST:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_VLOST;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            //��������¼���������
            case EM_BIZ_SCHEDULE_ALARMOUT:
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_ALARMOUT;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }
            case EM_BIZ_SCHEDULE_BUZZ: //��ʱ��֧��
                if (EM_ALARM_PARA_NULL == emAlaType)
                {
                    emAlaType = EM_ALARM_SCHEDULE_BUZZ;
                    memcpy(&sAlaPara.sAlaSchedule, pData, sizeof(SBizAlarmSchedule));
                }

                return ModAlarmGetParam(psBizManager->hAla, emAlaType, nChn, &sAlaPara);
            default:
                break;
        }
#endif
    }

    return -1;
}

s32 AlarmWorkingEnable(u8 nEnable)
{
	SBizManager* psBizManager = &g_sBizManager;
	return ModAlarmWorkingEnable(psBizManager->hAla, nEnable);
}

