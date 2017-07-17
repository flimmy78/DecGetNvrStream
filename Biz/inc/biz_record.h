#ifndef _BIZ_RECORD_H_
#define _BIZ_RECORD_H_

#include "biz.h"
#include "mod_record.h"
#include "biz_manager.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    u8 nChnNum;
    SModRecordRecPara* sModRecPara;
    RecHddInfo hddinfo;
} SBizRecord;

s32 RecordInit(u8 nEnable, SBizRecord* psInitPara);
s32 RecordDeInit();
s32 RecordStartManual(u8 nChn);
s32 RecordStopManual(u8 nChn);
s32 RecordStop(u8 nChn);
s32 RecordStopAll(void);
//yaogang modify 20150314
s32 RecordSnapPause(u8 cause);
s32 RecordSnapResume(u8 cause);

s32 RecordPause(u8 nChn);
s32 RecordResume(u8 nChn);
s32 RecordSetPreTime(u8 nChn, u16 nTime);
s32 RecordSetDelayTime(u8 nChn, u16 nTime);
s32 RecordSetTrigger(u8 nChn, EMRECALARMTYPE emType, u8 nKey);
s32 RecordSetSchedulePara(u8 nChn, SModRecSchPara* psPara);
s32 RecordWriteOneFrame(u8 nChn, SModRecRecordHeader* psHeader);
void RecordSetDealHardDiskFull(u8 nCover); //Ӳ����ʱ�Ƿ񸲸�:0�� 1��
void RecordSetWorkingEnable(u8 nEnable);
s32 RecordBufRelease(void);
s32 RecordBufResume(void);
void RecordExceptionCB(u8 nType); //�쳣�ص�����,nType:0,Ӳ���쳣; 1,�����쳣; 2,¼���ļ��쳣; 3,��������

//yaogang modify 20141225
/*ץͼ����	bit_mask	value
��ʱ			0		
�ƶ����	1
����			2 //����ֻ���ⲿ������IPC�ⲿ������ץͼ
����򱨾�			(1<<1) |(1<<2)			���ڱ���
�����ұ���			(1<<1) |(1<<2) |(1<<7)	ͬ��
�ֶ�			3
/////////////////�������	4
�ֶ���ͼ	4
////////////////////////////////�طŽ�ͼ	6
ȫ������			0xff
Ԥ¼			5		��Ϊ������Ҫ�ϴ���������ǰn���ͼƬ
�ճ�			6		(ÿ������ʱ����ϴ����)
ȫ������	0xff

//biz_snap ģ��ʹ�õ�����ץͼ����
typedef enum
{
	//upload
	TypeManualTEST = 0,	// 0 ����ͼ���ϴ�--���ƽ̨--���Ա���
	TypeManualMAINTAIN,	// 1 ����ͼ���ϴ�
	TypeManualACCEPT,	// 2 ���չ����ϴ�
	TypeDaily,	// 3 �ճ�
	//record
	TypePreSnap,		// 4 Ԥ¼
	TypeTime,		// 5 ��ʱ-¼������--ץͼ����
	TypeManual;		// 6 �ֶ���ͼ
	//upload & record
	TypeAlarmSensor,	// 7 ���ش���������
	TypeMotionDet,	// 8 �ƶ����
	TypeAlarmIPCEXT,	// 9 IPC�ⲿ����
	TypeSnapMax,
} EM_SNAP_TYPE;

typedef struct rec_header
{
	u8	nChn;					//֡��ͨ����
	u8	nStreamType;				//�������ͣ�0:��Ƶ����1:����Ƶ����2: snap��
	u8	nMediaType; 				//���뷽ʽ��0:H264��10:PCMU��11:ADPCM��
	u8	nFrameType; 				//�Ƿ�ؼ�֡(0:��1:��) FRAME_TYPE_P = 0,FRAME_TYPE_I = 1/3/5
								//nFrameType ��Snapģ������ץͼ����
	u32 nBitRate;					//������
	EMRECENCRESOL emResolution; 	//����ֱ���
	u64 nPts;						//ʱ�����΢�
								//nPts ��Snapģ������struct timeval {time_t tv_sec; suseconds_t tv_usec}; gettimeofday(); )
	u64 nTimeStamp; 				//֡ʱ��������룩
	u8* nDate;					//ָ��֡���ݵ�ָ��
	u32 nDataLength;				//֡���ݵ���ʵ����
	u16 width;
	u16 height;
}SModRecRecordHeader;
*/

//int RecordSnapshotToFile(u8 chn, u8 pic_type, void *pdata, u32 data_size, u32 width, u32 height);
s32 RecordWriteOneSnap(u8 nChn, SModRecRecordHeader* psHeader);
s32 RecordReadOneSnap(u8 nChn, SModRecRecordHeader* psHeader);
s32 RecordReadOnePreSnap(u8 nChn, SModRecRecordHeader* psHeader);


#ifdef __cplusplus
}
#endif


#endif
