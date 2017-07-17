#ifndef _BIZ_ALARM_H_
#define _BIZ_ALARM_H_

#include "biz.h"
#include "biz_config.h"
#include "biz_encode.h"
#include "mod_alarm.h"
#include "biz_manager.h"

typedef struct
{
    u8 nVideoChnNum; //��Ƶͨ����
    u8 nAlarmSensorNum; //������ͨ����
    u8 nAlarmOutNum; //�������ͨ����
    u8 nBuzzNum; //����������;��ʱֻ֧��1��
    u32 nVBlindLuma; //�ڵ��������ȷ�ֵ
    u32 nDiskNum;//Ӳ����Ŀ
    //yaogang modify 20150324 //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
    u8 nNVROrDecoder;
} SBizAlarm;

s32 AlarmInit(u8 nEnable, SBizAlarm* psInitPara);
s32 AlarmSetPara(u8 nChn, EMBIZPARATYPE emType, void* pData);
s32 AlarmGetPara(u8 nChn, EMBIZPARATYPE emType, PARAOUT void* pData);
s32 AlarmWorkingEnable(u8 nEnable); //nEnable: 0,ģ��ֹͣ��������0,ģ�鿪ʼ����
void AlarmDeal(SAlarmCbData* psAlarmCbData);

#endif
