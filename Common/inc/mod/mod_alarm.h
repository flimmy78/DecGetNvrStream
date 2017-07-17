#ifndef _MOD_ALARM_H_
#define _MOD_ALARM_H_

#include "common_basetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NAME_LEN	16

#define ALARM_PTZ_NUM	64

typedef void * AlaHdr;

typedef enum
{
	EM_ALARM_SENSOR_LOW = 0, //�������͵�ƽ��������
	EM_ALARM_SENSOR_HIGH, //�������ߵ�ƽ��������
} EMALARMSENSORTYPE;

typedef enum
{
	EM_ALARM_SENSOR_NORMALOPEN = 1, //�����������
	EM_ALARM_SENSOR_NORMALCLOSE, //���ձ������
} EMALARMOUTTYPE;

typedef enum
{
	EM_ALMARM_VMOTION_AREA_SELECTALL, //�ƶ��������ȫѡ
	EM_ALMARM_VMOTION_AREA_CLEAR, //�ƶ��������ȫ�����
	EM_ALMARM_VMOTION_AREA_SET, //�ƶ������������
} EMALARMVMOTIONAREASET;

typedef enum
{
	EM_ALARM_EVENT_SENSOR = 0, //�����������¼�
	EM_ALARM_EVENT_VLOST, //��Ƶ��ʧ�����¼�
	EM_ALARM_EVENT_VBLIND, //�ڵ������¼�
	EM_ALARM_EVENT_VMOTION, //�ƶ���ⱨ���¼�
	EM_ALARM_EVENT_IPCEXT,//IPC�ⲿ�����������¼�
	EM_ALARM_EVENT_IPCCOVER, //IPC �ڸ�
	EM_ALARM_EVENT_DISK_LOST,//Ӳ�̶�ʧ
	EM_ALARM_EVENT_DISK_ERR,//Ӳ�̶�д����
	EM_ALARM_EVENT_DISK_NONE,//���������Ӳ��
	EM_ALARM_EVENT_485EXT,
	//yaogang modify 20160201
	//nvr.cpp ��Զ��NVR ����
	EM_ALARM_EVENT_RemoteNVR_VLOST,
	EM_ALARM_EVENT_RemoteNVR_DISK_ERR,
	EM_ALARM_EVENT_RemoteNVR_DISK_LOST,
	
	EM_ALARM_EVENT_DISPATCH_ZOOMCHN = 100, //����ͨ���Ŵ��¼�;
	EM_ALARM_EVENT_DISPATCH_EMAIL, //����email�¼�;�ݲ�֧��
	EM_ALARM_EVENT_DISPATCH_SNAP, //����ץͼ�¼�;�ݲ�֧��
	EM_ALARM_EVENT_DISPATCH_REC_SENSOR,//����������¼���¼�
	EM_ALARM_EVENT_DISPATCH_REC_IPCEXTSENSOR,
	EM_ALARM_EVENT_DISPATCH_REC_IPCCOVER,
	EM_ALARM_EVENT_DISPATCH_REC_VMOTION,//�ƶ���ⴥ��¼���¼�
	EM_ALARM_EVENT_DISPATCH_REC_VBLIND,//��Ƶ�ڵ�����¼���¼�
	EM_ALARM_EVENT_DISPATCH_REC_VLOST,//��Ƶ��ʧ����¼���¼�
	EM_ALARM_EVENT_DISPATCH_PTZ_PRESET, //������̨����Ԥ�õ��¼�
	EM_ALARM_EVENT_DISPATCH_PTZ_PATROL, //������̨����Ѳ���¼�
	EM_ALARM_EVENT_DISPATCH_PTZ_LOCUS, //������̨�����켣�¼�
	
	//csp modify 20130326
	EM_ALARM_EVENT_CTRL_CHN_LED = 200,//����ͨ����
} EMALARMEVENT;

typedef struct
{
	EMALARMEVENT emAlarmEvent; //�ص��¼�
	u8 nChn; //��ʾ�����͵�ͨ����
	u8 nData; //������/�ƶ����/��Ƶ��ʧ/�ڵ���ʾ�����¼�״̬;��̨����ʱ��ʾԤ�õ�š�Ѳ��·����Ѳ��ֹͣ���켣ʱ��ʾ��ʼ/ֹͣ
	time_t nTime; //��ʾ���¼��������ʱ��(��ʱ����±�ʾ��󴥷���ʱ��,������ʱ���ʱ��)
	//yaogang modify 20160201
	//nvr.cpp ��Զ��NVR ����
	u32 nvrIP;
	//csp modify 20130326
	u32 reserved[1];
} SAlarmCbData;

typedef void (*FNALARMCB)(SAlarmCbData* psAlarmCbData);

typedef struct
{
	u8 nVideoChnNum;//��Ƶͨ����
	u8 nAlarmSensorNum;//������ͨ����
	u8 nAlarmOutNum;//�������ͨ����
	u8 nBuzzNum;//����������;��ʱֻ֧��1��
	u32 nVBlindLuma;//�ڵ��������ȷ�ֵ
	u32 nDiskNum;//Ӳ����Ŀ
	
	FNALARMCB pfnAlarmCb; //�����¼������ص�����
	//yaogang modify 20150324 //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	u8 nNVROrDecoder;
}SAlarmInitPara;

typedef struct
{
	EMALARMSENSORTYPE emType;//����/����
	u8 nEnable;//�Ƿ�����
	u16 nDelay;//��ʱ	
	char name[MAX_NAME_LEN + 1];//��������
	char reserved[8];
} SAlarmSensorPara;
//yaogang modify 20141010
typedef struct
{
	u8 nEnable;//�Ƿ�����
	u16 nDelay;//��ʱ
} SAlarmIPCExtSensorPara;
typedef struct
{
	u8 nEnable;//�Ƿ�����
	//u16 nDelay;//��ʱ
} SAlarmHDDPara;


typedef enum
{
	EM_RESOL_CIF = 1, //ͨ���ֱ���
	EM_RESOL_D1 = 4, //ͨ���ֱ���
} EMCHNRESOL;

typedef struct
{
	EMALARMVMOTIONAREASET emSetType; //���÷�ʽ ȫѡ/���/��������
	EMCHNRESOL emResol;
	u8 nEnable; //�Ƿ�����
	u8 nSensitivity; //������
	u16 nDelay; //��ʱ	
	char reserved[6];
	u8 nRows; //���򻮷�������
	u8 nCols; //���򻮷�������
	u64 nBlockStatus[64]; //��Ӧ�����״̬
} SAlarmVMotionPara;

typedef struct
{
	u8 nEnable;
	u16 nDelay;
} SAlarmVLostPara;

typedef struct
{
	u8 nEnable;
	u16 nDelay;
} SAlarmVBlindPara;

typedef struct
{
	EMALARMOUTTYPE emType;
	u8 nEnable;	//�Ƿ�����
	u16 nDelay; //��ʱ	
	char name[MAX_NAME_LEN + 1];//�Լ���������
	char reserved[8];
} SAlarmOutPara;

typedef struct
{
	u8 nEnable; //�Ƿ�����
	u8 nDuration;//����ʱ��ʱ�����㽫�Ǽ�Ъʽ����(��λs) nDuration��nIntervalͬʱ�������Ч
	u8 nInterval; //����ʱ�������㽫�Ǽ�Ъʽ����(��λs)
	u16 nDelay; //��ʱ	
} SAlarmBuzzPara;

typedef enum
{
	EM_ALARM_PTZ_NULL = 0, //������
	EM_ALARM_PTZ_PRESET, //����Ԥ�õ�
	EM_ALARM_PTZ_PATROL, //Ѳ����
	EM_ALARM_PTZ_LOCUS, //�켣
} EMALARMPTZTYPE;

typedef struct
{
	u8 emALaPtzType; //EMALARMPTZTYPE
	u8 nChn; //ͨ����
	u8 nId; //��ʾԤ�õ�id/Ѳ����·id/�켣(��Ч)
} SAlarmPtz;

typedef struct
{
	u8 nFlagBuzz; //������
	u8 nZoomChn; //���汨��ͨ���� 0xff ��ʾ��Ч
	u8 nFlagEmail; //�ʼ�
	u8 nSnapChn[64]; //��Ŵ���ץͼ��ͨ���� ���ͬʱ64��;0xff��ʾ������
	u8 nAlarmOut[64]; //�������� ���ͬʱ����64������id;0xff��ʾ������
	u8 nRecordChn[64]; //����¼��ͨ��;0xff��ʾ������
	SAlarmPtz sAlarmPtz[ALARM_PTZ_NUM]; //���ͬʱ����64������
} SAlarmDispatch;

typedef enum//������������ö��
{
	EM_ALARM_SCH_WEEK_DAY = 0,//ÿ�ܵ��ļ���
	EM_ALARM_SCH_MONTH_DAY,//ÿ�µ��ļ���
	EM_ALARM_SCH_EVERY_DAY,//ÿ��
	EM_ALARM_SCH_ONCE_DAY,//ֻһ����Ч / ����
} EMALARMSCHTYPE;

typedef struct 
{
	u32 nStartTime;//ÿ��ʱ��ε���ʼʱ��
	u32 nStopTime;//����ʱ��
} SAlarmSchTime;

#define MAX_ALARM_TIME_SEGMENTS	12//ÿ������õ�ʱ��ε������Ŀ

typedef struct 
{
	u8 nSchType;//����������ʱ������EMALARMSCHTYPE	
	SAlarmSchTime nSchTime[31][MAX_ALARM_TIME_SEGMENTS];//���β���ʱ��
} SAlarmSchedule;

typedef struct
{
	union
	{
		SAlarmSensorPara sAlaSensorPara;
		//yaogang modify 20141010
		SAlarmIPCExtSensorPara sAlaIPCExtSensorPara;
		SAlarmIPCExtSensorPara sAlaIPCCoverPara;
		SAlarmIPCExtSensorPara sAla485ExtSensorPara;
		SAlarmHDDPara sAlaHDDPara;
		
		SAlarmVMotionPara sAlaVMotionPara;
		SAlarmVLostPara sAlaVLostPara;
		SAlarmVBlindPara sAlaVBlindPara;
		SAlarmOutPara sAlaOutPara;
		SAlarmBuzzPara sAlaBuzzPara;
		SAlarmDispatch sAlaDispatch;
		SAlarmSchedule sAlaSchedule;
	};	
} SAlarmPara;

//����ģ���������
typedef enum
{
	//һ�㱨���¼���������
	EM_ALARM_PARA_SENSOR = 0,
	//yaogang modify 20141010
	EM_ALARM_PARA_IPCCOVER,
	EM_ALARM_PARA_IPCEXTSENSOR,
	EM_ALARM_PARA_485EXTSENSOR,
	EM_ALARM_PARA_HDD,
	EM_ALARM_PARA_VMOTION,
	EM_ALARM_PARA_VBLIND,
	EM_ALARM_PARA_VLOST,
	
	//���������¼���������
	EM_ALARM_PARA_DISKFULL = 50,
	EM_ALARM_PARA_NETBROKEN,
	EM_ALARM_PARA_NETADDRCONFLICT,
	
	//���������������
	EM_ALARM_PARA_ALARMOUT = 100,
	EM_ALARM_PARA_BUZZ,
	
	//һ�㱨���¼���������
	EM_ALARM_DISPATCH_SENSOR = 200,
	EM_ALARM_DISPATCH_IPCEXTSENSOR,//yaogang modify 20141010
	EM_ALARM_DISPATCH_IPCCOVER,
	EM_ALARM_DISPATCH_485EXTSENSOR,
	EM_ALARM_DISPATCH_HDD,
	
	EM_ALARM_DISPATCH_VMOTION,
	EM_ALARM_DISPATCH_VBLIND,
	EM_ALARM_DISPATCH_VLOST,
	
	//һ�㱨���¼���������
	EM_ALARM_SCHEDULE_SENSOR = 300,
	EM_ALARM_SCHEDULE_IPCEXTSENSOR,
	EM_ALARM_SCHEDULE_IPCCOVER,
	EM_ALARM_SCHEDULE_VMOTION,
	EM_ALARM_SCHEDULE_VBLIND,
	EM_ALARM_SCHEDULE_VLOST,
	
	//���������������
	EM_ALARM_SCHEDULE_ALARMOUT = 400,
	EM_ALARM_SCHEDULE_BUZZ,//��ʱ��֧��
	
	EM_ALARM_PARA_NULL = 0xffffffff,
}EMALARMPARATYPE;

s32 ModAlarmInit(SAlarmInitPara* psAlarmInitPara, PARAOUT AlaHdr* pAlaHdr);
s32 ModAlarmDeinit(AlaHdr AlaHdr);
s32 ModAlarmSetParam(AlaHdr AlaHdr, EMALARMPARATYPE emAlarmParaType, u8 id, const SAlarmPara* psAlarmPara);
s32 ModAlarmGetParam(AlaHdr AlaHdr, EMALARMPARATYPE emAlarmParaType, u8 id, PARAOUT SAlarmPara* psAlarmPara);
s32 ModAlarmWorkingEnable(AlaHdr AlaHdr, u8 nEnable);//nEnable: 0,ģ��ֹͣ��������0,ģ�鿪ʼ����
u8 GetAlarmCheckStaue();

s32 ModAlarmSetTimeZoneOffset(int nOffset);
s32 ModAlarmGetTimeZoneOffset();

#ifdef __cplusplus
}
#endif

#endif //_MOD_ALARM_H_

