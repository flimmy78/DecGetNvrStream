#ifndef _MOD_RECORD_H_
#define _MOD_RECORD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/time.h>


/*************�ṩ��ҵ����*****************/

typedef enum			//�ֱ���ö��
{
	EM_REC_ENC_D1 = 0,		//D1
	EM_REC_ENC_4CIF,		//4CIF
	EM_REC_ENC_HALF_D1,		//HD1
	EM_REC_ENC_CIF,			//Cif
	EM_REC_ENC_QCIF,		//Qcif
	EM_REC_ENC_N_D1 = 10,	//N��D1
	EM_REC_ENC_N_4CIF,		//N��4CIF
	EM_REC_ENC_N_HALF_D1,	//N��HD1
	EM_REC_ENC_N_CIF,		//N��Cif
	EM_REC_ENC_N_QCIF,		//N��Qcif
	EM_REC_ENC_720P = 20,	//720P
	EM_REC_ENC_1080P,		//1080P
	EM_REC_ENC_960H,		//960H
	EM_REC_ENC_N_960H,		//N��960H//csp modify
}EMRECENCRESOL;

typedef struct mr_recordpara
{
	//u8 nChn;						//ͨ����
	u32 nBitRate;					//���������(��λΪk)
	EMRECENCRESOL emResolution;		//����ֱ���
	u8 nFrameRate;					//����֡��
} SModRecordRecPara;

typedef enum				//ҵ��㷢����Ϣ����ö��
{
	EM_RECALARM_V_MOTION = 0,			//�ƶ����
	EM_RECALARM_V_ALARM,				//��������
	EM_RECALARM_V_IPCEXTALARM,				//��������	
	EM_RECALARM_V_BIND,					//��Ƶ�ڵ�
	EM_RECALARM_V_LOST,					//��Ƶ��ʧ
	EM_RECALARM_NETLINK_LOST,			//�������
	EM_RECALARM_ABNORMAL,				//�쳣
} EMRECALARMTYPE;

typedef enum					//¼�񲼷�����ö��
{
	EM_REC_SCH_WEEK_DAY = 0,
	EM_REC_SCH_MONTH_DAY,
	EM_REC_SCH_EVERY_DAY,
	EM_REC_SCH_ONCE_DAY,
}EMRECSCHTYPE;

typedef struct mr_schtime
{
	u32 nStartTime;	//ÿ��ʱ��ε���ʼʱ��
	u32 nStopTime;	//����ʱ��
}SModRecSchTime;

typedef enum
{
    EM_REC_MASK_MANUAL = (1 << 0),
    EM_REC_MASK_TIMING = (1 << 1),
    EM_REC_MASK_MOTION = (1 << 2),
    EM_REC_MASK_SENSORALARM = (1 << 3),
} EMRECTYPEMASK;

#define MAX_TIME_SEGMENTS 12	//ÿ������õ�ʱ��ε������Ŀ

typedef enum					//录像布防类型枚举
{
	EM_REC_SCH_REC_TIMER = 0,
	EM_REC_SCH_REC_VMOTION,
	EM_REC_SCH_REC_ALARM,
}EMRECSCHRECTYPE;

typedef struct mr_schpara
{
	u8 nChn;										//通道号
	EMRECSCHTYPE nTimeType;							//录像布防的时间类型
	u32 nValue;										//对应类型下的取值，需要按位操作，（可以实现表示31天）
	EMRECSCHRECTYPE nRecType;					
	SModRecSchTime nSchTime[31][MAX_TIME_SEGMENTS];	//各段布防时间
}SModRecSchPara;

/*
typedef struct mr_schpara
{
	u8 nChn;										//ͨ����
	EMRECSCHTYPE nTimeType;							//¼�񲼷���ʱ������
	u32 nValue;										//��Ӧ�����µ�ȡֵ����Ҫ��λ������������ʵ�ֱ�ʾ31�죩
	u8 nRecType[31][MAX_TIME_SEGMENTS];				//¼������(λ���㣬�ɵ͵���λ�ֱ����
													//��ʱ���ƶ���⡢�������롢�ڵ�����ʧ��.....������)
	SModRecSchTime nSchTime[31][MAX_TIME_SEGMENTS];	//���β���ʱ��
}SModRecSchPara;
*/

typedef struct rec_header
{
	u8	nChn;					//֡��ͨ����
	u8	nStreamType;				//�������ͣ�0:��Ƶ����1:����Ƶ����2: snap��
	u8	nMediaType; 				//���뷽ʽ��0:H264��10:PCMU��11:ADPCM��
	u8	nFrameType; 				//�Ƿ�ؼ�֡(0:��1:��) FRAME_TYPE_P = 0,FRAME_TYPE_I = 1/3/5
	u32 nBitRate;					//������
								//nFrameType ��Snapģ������ץͼ����EM_SNAP_TYPE
	EMRECENCRESOL emResolution; 	//����ֱ���
	u64 nPts;						//ʱ�����΢�
								//nPts ��Snapģ������struct timeval {time_t tv_sec; suseconds_t tv_usec}; gettimeofday(); )
	u64 nTimeStamp; 				//֡ʱ��������룩
	u8* nDate;					//ָ��֡���ݵ�ָ��
	u32 nDataLength;				//֡���ݵ���ʵ����
	u16 width;
	u16 height;
}SModRecRecordHeader;



typedef void (*FPMODRECSTATUS)(void* status);
typedef void* RecHddInfo;
typedef void (*FPMODRECEXCEPTION)(u8 nType);

s32 ModRecordInit(u8 nChnNum, SModRecordRecPara* para, FPMODRECSTATUS getrecstatus, RecHddInfo hddinfo, FPMODRECEXCEPTION RecExceptCB);
s32 ModRecordDeInit();
s32 ModRecordStartManual(u8 nChn);
s32 ModRecordStopManual(u8 nChn);
s32 ModRecordStop(u8 nChn);
s32 ModRecordStopAll();
s32 ModRecordPause(u8 nChn);
s32 ModRecordResume(u8 nChn);
s32 ModRecordSetPreTime(u8 nChn, u32 time);
s32 ModRecordSetDelayTime(u8 nChn, u32 time);
//s32 ModRecordSetRecChnTriggered(u8 chn, EMRECALARMTYPE emType, u64 data);
s32 ModRecordSetTrigger(u8 chn, EMRECALARMTYPE emType, u8 key);
s32 ModRecordSetSchedulePara(u8 nChn, SModRecSchPara* para);
s32 ModRecordWriteOneFrame(u8 nChn, SModRecRecordHeader* header);
void ModRecordSetDealHardDiskFull(u8 nCover); //Ӳ����ʱ�Ƿ񸲸�:0�� 1��
void ModRecordSetWorkingEnable(u8 nEnable); //����ģ���Ƿ���:0�� 1��
s32 ModRecordBufRelease(void);//�ͷ�¼�񻺳���������ڴ�
s32 ModRecordBufResume(void); //�ָ����ϸ��ӿ��ͷŵ��Ļ�����

s32 ModRecordSetTimeZoneOffset(int nOffset);
s32 ModRecordGetTimeZoneOffset();

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
*/

typedef struct
{
	u8 chn;
	u8 pic_type;
	u32 data_size;
	u32 width;
	u32 height;
	time_t tv_sec;
	suseconds_t tv_usec;
}SModRecSnapHeader;

int ModRecordSnapshotToFile(u8 chn, u8 pic_type, time_t tv_sec, suseconds_t tv_usec, void *pdata, u32 data_size, u32 width, u32 height);
int ModRecordSnapshotToBuf(u8 chn, u8 pic_type, time_t tv_sec, suseconds_t tv_usec, void *pdata, u32 data_size, u32 width, u32 height);

//Ԥ¼
int ModRecordPreSnapToFile(u8 chn, time_t tv_sec, void *pdata, u32 data_size, u32 width, u32 height);
int ModRecordPreSnapFromFile(u8 chn, time_t tv_sec, void *pdata, u32 * pdata_size);


/********************ҵ����ṩ����***************************/

#ifdef __cplusplus
}
#endif

#endif //_MOD_RECORD_H_

