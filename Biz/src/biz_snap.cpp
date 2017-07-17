#include <time.h>
#include "MultiTask/Timer.h"
#include "biz.h"
#include "biz_config.h"
#include "biz_encode.h"
#include "biz_record.h"
#include "mod_alarm.h"
#include "biz_snap.h"
#include "ipcamera.h"
#include "sg_platform.h"
#include "System/Locales.h"
#include "mod_encode.h"
#include "circlebuf.h"


#define g_SnapManager (*CSnapManager::instance())

#define TEXTLEN	(32)
#define nSECPERMIN	(60)
#define nSECPERHOUR	(60*60)
const int indextosec[] = {1, 2, 3, 4, 5, 10*nSECPERMIN, 30*nSECPERMIN, nSECPERHOUR, 12*nSECPERHOUR, 24*nSECPERHOUR};

#define SNAPBUFSIZE (64 << 10)


typedef struct {
	time_t StartTime;	//��������ʱ��
	time_t EndTime;	//�������ʱ��
} sTimeRange;

typedef struct
{
	u16 width;
	u16 height;
}SnapResolutionType;

const SnapResolutionType SnapRes[] = {
	{352, 288},	//AUTO
	{704, 576},	
	{352, 288},
	{176, 144},
};

typedef enum
{
	TypePause,
	TypeResume,
} EM_SNAPMSG_TYPE;


//ͨ���õ�ץͼ������������Ϣ����������
typedef struct
{
	
	u32 ChnReqTypeMask;	//����ץͼ��ǣ�ץͼ����
	
	//�Ƿ�Ҫ�ϴ���������ʱ�̵�ǰ��ʱ���ĸ�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 UploadPreRec_Sensor;//��Чλ��: m_MaxSensorNum
	u32 UploadPreRec_IPCMD;
	u32 UploadPreRec_IPCEXT;
	//�Ƿ�Ҫ�ϴ���ǰʱ�̸�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 UploadCur_Sensor;//��Чλ��: m_MaxSensorNum
	u32 UploadCur_IPCMD;
	u32 UploadCur_IPCEXT;

	CTimer *p_ChnRequestSnapTimer;	//ÿ��ͨ��һ��
	VD_BOOL TimerWorking;	//��ʾ��ʱ���߳��Ƿ����ڹ�����

	//ͼƬBUFFER
	u8 *pSnapData;
	//CMutex *pChnMutex;//ͨ����Ϣ��
}SnapChnProcessDataType;


class CSnapManager : public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(CSnapManager);
	
	~CSnapManager();
	VD_BOOL Start(u8 MaxSensorNum, u8 MaxIpcChn);
	void ThreadProc();
	//int RequestSnap(u8 chn, EM_SNAP_TYPE type);
	int AlarmStatusChange(u8 chn, EM_SNAP_TYPE type, int status);
	void Snapshot_Register(u8 chn);
	//�ϴ���������ʱ�̵�ǰ��ʱ����ڶ���Ԥ¼ͼƬ
	int UploadPreRecPic(EM_SNAP_TYPE type, u8 AlarmChn, u8 IPCChn);
	//ʱ�������Ϊ����������ָ������ΪҪ�ϴ�֮ǰ��Ԥ¼ͼƬ
	//һ�鱨����Ϣ������GUID������ͬ��
	int UploadPic(EM_SNAP_TYPE type, u8 AlarmChn, u8 IPCChn, time_t time, void *SnapData, u32 DataSize, int GUID);

	int requestSnap(u8 chn, EM_SNAP_TYPE type, const char *PoliceID, const char *pswd);
	int alarmStatusChange(u8 chn, EM_ALARM_TYPE type, int status);
	int SnapWriteMsgQueue(EM_SNAPMSG_TYPE msg);
	int SnapReadMsgQueue(EM_SNAPMSG_TYPE *pmsg);

protected:
	CSnapManager();
	
private:
	VD_BOOL  m_Started;	//��ʼ����ɣ�������
	CMutex *pChnMutex;//ͨ����Ϣ��
	u8 m_MaxSensorNum;	//������������Ŀ
	u8 m_MaxIpcChn;		//ͨ����Ŀ
	
	//CTimer **pp_ChnRequestSnapTimer;	//ÿ��ͨ��һ��
	//volatile u32 TimerWorkingMask;	//ÿ��ͨ��һλ����ʾ��ʱ���߳��Ƿ����ڹ�����
	volatile u32 *p_ChnReqTypeMask;	//����ץͼ��ǣ�ץͼ����
	
	//�Ƿ�Ҫ�ϴ���������ʱ�̵�ǰ��ʱ���ĸ�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 * p_UploadPreRec_Sensor;//��Чλ��: m_MaxSensorNum
	u32 * p_UploadPreRec_IPCMD;
	u32 * p_UploadPreRec_IPCEXT;
	//�Ƿ�Ҫ�ϴ���ǰʱ�̸�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 * p_UploadCur_Sensor;//��Чλ��: m_MaxSensorNum
	u32 * p_UploadCur_IPCMD;
	u32 * p_UploadCur_IPCEXT;

	//һ�鱨����Ϣ������GUID������ͬ���ϴ����ƽ̨��һ���ֶ�
	s32 *p_GUID_Sensor;//m_MaxSensorNum
	s32 *p_GUID_IPCMD;
	s32 *p_GUID_IPCEXT;
	//CMutex *p_ChnMutex;//ͨ����Ϣ��
	//
	time_t *p_ChnTimeLast;//ͨ����ʱ������ʱ��
	time_t *p_ChnPreRec;//ͨ��Ԥ¼������ʱ��
	
	//һά��IPCͨ����Ŀ����ά�Ǳ���Դ��Ŀ
	time_t **pp_ChnRecSensor;//¼������-- �¼�-- sensor������chn��󱣴�ʱ��[chn][sensor]
	time_t **pp_ChnRecIPCMD;//¼������-- �¼�-- IPCMD������chn��󱣴�ʱ��
	time_t **pp_ChnRecIPCEXT;//¼������-- �¼�-- IPCEXT������chn��󱣴�ʱ��
	time_t **pp_ChnSGSensor;//�ϴ�����-- ����-- sensor������chn����ϴ�ʱ��
	time_t **pp_ChnSGIPCMD;//�ϴ�����-- ����-- IPCMD������chn����ϴ�ʱ��
	time_t **pp_ChnSGIPCEXT;//�ϴ�����-- ����-- IPCEXT������chn����ϴ�ʱ��

	//ά��- ����Դ��Ŀ
	EM_ALARM_STATUS *p_SensorStatus;	//��������״̬
	EM_ALARM_STATUS *p_IPCMDStatus;	//IPC�ƶ����״̬
	EM_ALARM_STATUS *p_IPCEXTStatus;	//IPC�ⲿ����״̬

	//ͨ���õ�ץͼ������������Ϣ����������
	volatile SnapChnProcessDataType *pChnData;
	//���ڱ���
	
	//volatile u32 m_ChnSensorMask;//ÿһλ��ʾһ��������
	//volatile u32 m_ChnIPCMDMask;
	//volatile u32 m_ChnIPCEXTMask;
	sTimeRange *p_SensorRange;
	sTimeRange *p_IPCMDRange;
	sTimeRange *p_IPCEXTRange;

	char PoliceID[TEXTLEN];
	char PassWord[TEXTLEN];

	SCircleBufInfo SnapMsgQueue;
	
	int UploadAndRecDeal(EM_ALARM_TYPE type, time_t CurTime);
	int requestResource(void);
	void releaseResource(void);
	
};



/************************************************************/
PATTERN_SINGLETON_IMPLEMENT(CSnapManager);

CSnapManager::CSnapManager():CThread("SnapManager", TP_TIMER)
{
	
	m_Started 		= FALSE;	//��ʼ����ɣ�������
	m_MaxSensorNum	= 0;		//������������Ŀ
	m_MaxIpcChn		= 0;		//ͨ����Ŀ

	pChnMutex = NULL;
	p_ChnReqTypeMask = NULL;	//����ץͼ��ǣ�ץͼ����
	
	//�Ƿ�Ҫ�ϴ���������ʱ�̵�ǰ��ʱ���ĸ�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	p_UploadPreRec_Sensor	= NULL;//��Чλ��: m_MaxSensorNum
	p_UploadPreRec_IPCMD	= NULL;
	p_UploadPreRec_IPCEXT	= NULL;
	
	//�Ƿ�Ҫ�ϴ���ǰʱ�̸�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	p_UploadCur_Sensor	= NULL;//��Чλ��: m_MaxSensorNum
	p_UploadCur_IPCMD	= NULL;
	p_UploadCur_IPCEXT	= NULL;

	p_GUID_Sensor = NULL;
	p_GUID_IPCMD = NULL;
	p_GUID_IPCEXT = NULL;
	
	p_ChnTimeLast		= NULL;//ͨ����ʱ������ʱ��
	p_ChnPreRec		= NULL;//ͨ��Ԥ¼������ʱ��
	pp_ChnRecSensor	= NULL;//¼������-- �¼�-- sensor������chn��󱣴�ʱ��[chn][sensor]
	pp_ChnRecIPCMD	= NULL;//¼������-- �¼�-- IPCMD������chn��󱣴�ʱ��
	pp_ChnRecIPCEXT	= NULL;//¼������-- �¼�-- IPCEXT������chn��󱣴�ʱ��
	pp_ChnSGSensor	= NULL;//�ϴ�����-- ����-- sensor������chn����ϴ�ʱ��
	pp_ChnSGIPCMD	= NULL;//�ϴ�����-- ����-- IPCMD������chn����ϴ�ʱ��
	pp_ChnSGIPCEXT	= NULL;//�ϴ�����-- ����-- IPCEXT������chn����ϴ�ʱ��
	
	p_SensorStatus 	= NULL;	//��������״̬
	p_IPCMDStatus 	= NULL;	//IPC�ƶ����״̬
	p_IPCEXTStatus 	= NULL;	//IPC�ⲿ����״̬

	//ͨ���õ�ץͼ������������Ϣ����������
	pChnData 	= NULL;
	//���ڱ���
	
	p_SensorRange	= NULL;
	p_IPCMDRange	= NULL;
	p_IPCEXTRange	= NULL;

	memset(PoliceID, 0, sizeof(PoliceID));
	memset(PassWord, 0, sizeof(PassWord));	
		
	//printf("CSnapManager::CSnapManager()>>>>>>>>>\n");
}

int CSnapManager::requestResource(void)
{
	u8 i;

	pChnMutex = (CMutex *)operator new(m_MaxIpcChn * sizeof(CMutex));
	for (i=0; i<m_MaxIpcChn; i++)
	{
		new(&pChnMutex[i]) CMutex(MUTEX_FAST); //MUTEX_RECURSIVE
	}
	
	p_ChnReqTypeMask = new (std::nothrow) u32[m_MaxIpcChn];
	if (p_ChnReqTypeMask == NULL)
	{
		printf("%s new p_ChnReqTypeMask failed\n", __func__);
		return 1;
	}
	memset((void *)p_ChnReqTypeMask, 0, sizeof(u32)*m_MaxIpcChn);

	p_UploadPreRec_Sensor = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadPreRec_Sensor == NULL)
	{
		printf("%s new p_UploadPreRec_Sensor failed\n", __func__);
		return 1;
	}
	memset(p_UploadPreRec_Sensor, 0, sizeof(u32) * m_MaxIpcChn);

	p_UploadPreRec_IPCMD = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadPreRec_IPCMD == NULL)
	{
		printf("%s new p_UploadPreRec_IPCMD failed\n", __func__);
		return 1;
	}
	memset(p_UploadPreRec_IPCMD, 0, sizeof(u32) * m_MaxIpcChn);
	
	p_UploadPreRec_IPCEXT = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadPreRec_IPCEXT == NULL)
	{
		printf("%s new p_UploadPreRec_IPCEXT failed\n", __func__);
		return 1;
	}
	memset(p_UploadPreRec_IPCEXT, 0, sizeof(u32) * m_MaxIpcChn);

	p_UploadCur_Sensor = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadCur_Sensor == NULL)
	{
		printf("%s new p_UploadCur_Sensor failed\n", __func__);
		return 1;
	}
	memset(p_UploadCur_Sensor, 0, sizeof(u32) * m_MaxIpcChn);

	p_UploadCur_IPCMD = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadCur_IPCMD == NULL)
	{
		printf("%s new p_UploadCur_IPCMD failed\n", __func__);
		return 1;
	}
	memset(p_UploadCur_IPCMD, 0, sizeof(u32) * m_MaxIpcChn);
	
	p_UploadCur_IPCEXT = new (std::nothrow) u32 [m_MaxIpcChn];
	if (p_UploadCur_IPCEXT == NULL)
	{
		printf("%s new p_UploadCur_IPCEXT failed\n", __func__);
		return 1;
	}
	memset(p_UploadCur_IPCEXT, 0, sizeof(u32) * m_MaxIpcChn);

	p_GUID_Sensor = new (std::nothrow) s32 [m_MaxSensorNum];
	if (p_GUID_Sensor == NULL)
	{
		printf("%s new p_GUID_Sensor failed\n", __func__);
		return 1;
	}
	memset(p_GUID_Sensor, 0, sizeof(s32) * m_MaxSensorNum);

	p_GUID_IPCMD = new (std::nothrow) s32 [m_MaxIpcChn];
	if (p_GUID_IPCMD == NULL)
	{
		printf("%s new p_GUID_IPCMD failed\n", __func__);
		return 1;
	}
	memset(p_GUID_IPCMD, 0, sizeof(s32) * m_MaxIpcChn);

	p_GUID_IPCEXT = new (std::nothrow) s32 [m_MaxIpcChn];
	if (p_GUID_IPCEXT == NULL)
	{
		printf("%s new p_GUID_IPCEXT failed\n", __func__);
		return 1;
	}
	memset(p_GUID_IPCEXT, 0, sizeof(s32) * m_MaxIpcChn);

	//time_t *p_ChnTimeLast;//ͨ����ʱ������ʱ��
	p_ChnTimeLast = new (std::nothrow) time_t [m_MaxIpcChn];
	if (p_ChnTimeLast == NULL)
	{
		printf("%s new p_ChnTimeLast failed\n", __func__);
		return 1;
	}
	memset(p_ChnTimeLast, 0, sizeof(time_t)*m_MaxIpcChn);

	//time_t *p_ChnPreRec;//ͨ��Ԥ¼������ʱ��
	p_ChnPreRec = new (std::nothrow) time_t [m_MaxIpcChn];
	if (p_ChnPreRec == NULL)
	{
		printf("%s new p_ChnPreRec failed\n", __func__);
		return 1;
	}
	memset(p_ChnPreRec, 0, sizeof(time_t)*m_MaxIpcChn);

	//һά��IPCͨ����Ŀ����ά�Ǳ���Դ��Ŀ
	//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
	pp_ChnRecSensor = new (std::nothrow) time_t*[m_MaxIpcChn];
	if (pp_ChnRecSensor == NULL)
	{
		printf("%s new pp_ChnRecSensor failed\n", __func__);
		return 1;
	}
	memset(pp_ChnRecSensor, 0, sizeof(time_t*) * m_MaxIpcChn);
	
	//time_t *p_ChnRecIPCMD;//¼������-- �¼�-- IPCMD������ʱ��
	pp_ChnRecIPCMD = new (std::nothrow) time_t* [m_MaxIpcChn];
	if (pp_ChnRecIPCMD == NULL)
	{
		printf("%s new pp_ChnRecIPCMD failed\n", __func__);
		return 1;
	}
	memset(pp_ChnRecIPCMD, 0, sizeof(time_t*)*m_MaxIpcChn);
	
	//time_t *p_ChnRecIPCEXT;//¼������-- �¼�-- IPCEXT������ʱ��
	pp_ChnRecIPCEXT = new (std::nothrow) time_t* [m_MaxIpcChn];
	if (pp_ChnRecIPCEXT == NULL)
	{
		printf("%s new pp_ChnRecIPCEXT failed\n", __func__);
		return 1;
	}
	memset(pp_ChnRecIPCEXT, 0, sizeof(time_t*)*m_MaxIpcChn);
	
	//time_t *p_SGSensor;//�ϴ�����-- ����-- sersor������ʱ��
	pp_ChnSGSensor = new (std::nothrow) time_t* [m_MaxIpcChn];
	if (pp_ChnSGSensor == NULL)
	{
		printf("%s new pp_ChnSGSensor failed\n", __func__);
		return 1;
	}
	memset(pp_ChnSGSensor, 0, sizeof(time_t*)*m_MaxIpcChn);

	//time_t *p_ChnSGIPCMD;//�ϴ�����-- ����-- IPCMD������ʱ��
	pp_ChnSGIPCMD = new (std::nothrow) time_t* [m_MaxIpcChn];
	if (pp_ChnSGIPCMD == NULL)
	{
		printf("%s new pp_ChnSGIPCMD failed\n", __func__);
		return 1;
	}
	memset(pp_ChnSGIPCMD, 0, sizeof(time_t*)*m_MaxIpcChn);
	
	//time_t *p_ChnSGIPCEXT;//�ϴ�����-- ����-- IPCEXT������ʱ��
	pp_ChnSGIPCEXT = new (std::nothrow) time_t* [m_MaxIpcChn];
	if (pp_ChnSGIPCEXT == NULL)
	{
		printf("%s new pp_ChnSGIPCEXT failed\n", __func__);
		return 1;
	}
	memset(pp_ChnSGIPCEXT, 0, sizeof(time_t*)*m_MaxIpcChn);

	for (i=0; i<m_MaxIpcChn; i++)
	{
		//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
		pp_ChnRecSensor[i] = new (std::nothrow) time_t [m_MaxSensorNum];
		if (pp_ChnRecSensor[i] == NULL)
		{
			printf("%s new pp_ChnRecSensor[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnRecSensor[i], 0, sizeof(time_t)*m_MaxSensorNum);
		
		//time_t *p_ChnRecIPCMD;//¼������-- �¼�-- IPCMD������ʱ��
		pp_ChnRecIPCMD[i] = new (std::nothrow) time_t [m_MaxIpcChn];
		if (pp_ChnRecIPCMD[i] == NULL)
		{
			printf("%s new pp_ChnRecIPCMD[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnRecIPCMD[i], 0, sizeof(time_t)*m_MaxIpcChn);
		
		//time_t *p_ChnRecIPCEXT;//¼������-- �¼�-- IPCEXT������ʱ��
		pp_ChnRecIPCEXT[i] = new (std::nothrow) time_t [m_MaxIpcChn];
		if (pp_ChnRecIPCEXT[i] == NULL)
		{
			printf("%s new pp_ChnRecIPCEXT[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnRecIPCEXT[i], 0, sizeof(time_t)*m_MaxIpcChn);
		
		//time_t *p_SGSensor;//�ϴ�����-- ����-- sersor������ʱ��
		pp_ChnSGSensor[i] = new (std::nothrow) time_t [m_MaxSensorNum];
		if (pp_ChnSGSensor[i] == NULL)
		{
			printf("%s new pp_ChnSGSensor[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnSGSensor[i], 0, sizeof(time_t)*m_MaxSensorNum);
		
		//time_t *p_ChnSGIPCMD;//�ϴ�����-- ����-- IPCMD������ʱ��
		pp_ChnSGIPCMD[i] = new (std::nothrow) time_t [m_MaxIpcChn];
		if (pp_ChnSGIPCMD[i] == NULL)
		{
			printf("%s new pp_ChnSGIPCMD[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnSGIPCMD[i], 0, sizeof(time_t)*m_MaxIpcChn);
		
		//time_t *p_ChnSGIPCEXT;//�ϴ�����-- ����-- IPCEXT������ʱ��
		pp_ChnSGIPCEXT[i] = new (std::nothrow) time_t [m_MaxIpcChn];
		if (pp_ChnSGIPCEXT[i] == NULL)
		{
			printf("%s new pp_ChnSGIPCEXT[i] failed\n", __func__);
			return 1;
		}
		memset(pp_ChnSGIPCEXT[i], 0, sizeof(time_t)*m_MaxIpcChn);
	}

	p_SensorStatus= new (std::nothrow) EM_ALARM_STATUS [m_MaxSensorNum];
	if (p_SensorStatus == NULL)
	{
		printf("%s new p_SensorStatus failed\n", __func__);
		return 1;
	}
	memset(p_SensorStatus, 0, sizeof(EM_ALARM_STATUS) * m_MaxSensorNum);

	p_IPCMDStatus= new (std::nothrow) EM_ALARM_STATUS [m_MaxIpcChn];
	if (p_IPCMDStatus == NULL)
	{
		printf("%s new p_IPCMDStatus failed\n", __func__);
		return 1;
	}
	memset(p_IPCMDStatus, 0, sizeof(EM_ALARM_STATUS) * m_MaxIpcChn);

	p_IPCEXTStatus= new (std::nothrow) EM_ALARM_STATUS [m_MaxIpcChn];
	if (p_IPCEXTStatus == NULL)
	{
		printf("%s new p_IPCEXTStatus failed\n", __func__);
		return 1;
	}
	memset(p_IPCEXTStatus, 0, sizeof(EM_ALARM_STATUS) * m_MaxIpcChn);
	
	//SnapChnProcessDataType *pChnData
	pChnData	= new  (std::nothrow) SnapChnProcessDataType[m_MaxIpcChn];
	if (pChnData == NULL)
	{
		printf("%s new pChnData failed\n", __func__);
		return 1;
	}
	memset((void *)pChnData, 0, sizeof(SnapChnProcessDataType) * m_MaxIpcChn);
	for (i=0; i<m_MaxIpcChn; i++)
	{
		pChnData[i].p_ChnRequestSnapTimer = new (std::nothrow) CTimer("Snap");
		if (pChnData[i].p_ChnRequestSnapTimer == NULL)
		{
			printf("%s new pChnData[%d].p_ChnRequestSnapTimer failed\n", __func__, i);
			return 1;
		}

		pChnData[i].pSnapData =  new (std::nothrow) u8[SNAPBUFSIZE];
		if (pChnData[i].pSnapData == NULL)
		{
			printf("%s malloc pSnapData failed\n", __func__);
			return 1;
		}
	}	

	p_SensorRange = new (std::nothrow) sTimeRange [m_MaxSensorNum];
	if (p_SensorRange == NULL)
	{
		printf("%s new p_SensorRange failed\n", __func__);
		return 1;
	}
	memset(p_SensorRange, 0, sizeof(sTimeRange) * m_MaxSensorNum);	

	p_IPCMDRange = new (std::nothrow) sTimeRange [m_MaxIpcChn];
	if (p_IPCMDRange == NULL)
	{
		printf("%s new p_IPCMDRange failed\n", __func__);
		return 1;
	}
	memset(p_IPCMDRange, 0, sizeof(sTimeRange) * m_MaxIpcChn);	

	p_IPCEXTRange = new (std::nothrow) sTimeRange [m_MaxIpcChn];
	if (p_IPCEXTRange == NULL)
	{
		printf("%s new p_IPCEXTRange failed\n", __func__);
		return 1;
	}
	memset(p_IPCEXTRange, 0, sizeof(sTimeRange) * m_MaxIpcChn);

	SnapMsgQueue.nBufId = 0;	
	SnapMsgQueue.nLength = 256;//int: 0 stop 1 resume
	
	if (CreateCircleBuf(&SnapMsgQueue))
	{
		printf("%s SnapMsgQueue init failed\n", __func__);
	}
	
	return 0;
}

void CSnapManager::releaseResource(void)
{
	u8 i;

	if (pChnMutex != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			pChnMutex[i].~CMutex();
		}
		operator delete(pChnMutex);
		pChnMutex = NULL;
	}

	if (p_ChnReqTypeMask != NULL)
	{
		delete []p_ChnReqTypeMask;
		p_ChnReqTypeMask = NULL;
	}

	if (p_UploadPreRec_Sensor != NULL)
	{
		delete []p_UploadPreRec_Sensor;
		p_UploadPreRec_Sensor = NULL;
	}

	if (p_UploadPreRec_IPCMD != NULL)
	{
		delete []p_UploadPreRec_IPCMD;
		p_UploadPreRec_IPCMD = NULL;
	}
	
	if (p_UploadPreRec_IPCEXT != NULL)
	{
		delete []p_UploadPreRec_IPCEXT;
		p_UploadPreRec_IPCEXT = NULL;
	}

	if (p_UploadCur_Sensor != NULL)
	{
		delete []p_UploadCur_Sensor;
		p_UploadCur_Sensor = NULL;
	}

	if (p_UploadCur_IPCMD != NULL)
	{
		delete []p_UploadCur_IPCMD;
		p_UploadCur_IPCMD = NULL;
	}
	
	if (p_UploadCur_IPCEXT != NULL)
	{
		delete []p_UploadCur_IPCEXT;
		p_UploadCur_IPCEXT = NULL;
	}

	if (p_GUID_Sensor != NULL)
	{
		delete []p_GUID_Sensor;
		p_GUID_Sensor = NULL;
	}
	if (p_GUID_IPCMD != NULL)
	{
		delete []p_GUID_IPCMD;
		p_GUID_IPCMD = NULL;
	}
	if (p_GUID_IPCEXT != NULL)
	{
		delete []p_GUID_IPCEXT;
		p_GUID_IPCEXT = NULL;
	}

	//time_t *p_ChnTimeLast;//ͨ����ʱ������ʱ��
	if (p_ChnTimeLast != NULL)
	{
		delete []p_ChnTimeLast;
		p_ChnTimeLast = NULL;
	}
	
	//time_t *p_ChnPreRec;//ͨ��Ԥ¼������ʱ��
	if (p_ChnPreRec != NULL)
	{
		delete []p_ChnPreRec;
		p_ChnPreRec = NULL;
	}

	//һά��IPCͨ����Ŀ����ά�Ǳ���Դ��Ŀ
	//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
	if (pp_ChnRecSensor != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnRecSensor[i] != NULL)
			{
				delete []pp_ChnRecSensor[i];
				pp_ChnRecSensor[i]  = NULL;
			}			
		}
		
		delete []pp_ChnRecSensor;
		pp_ChnRecSensor = NULL;
	}
	
	//time_t *p_ChnRecIPCMD;//¼������-- �¼�-- IPCMD������ʱ��
	if (pp_ChnRecIPCMD != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnRecIPCMD[i] != NULL)
			{
				delete []pp_ChnRecIPCMD[i];
				pp_ChnRecIPCMD[i]  = NULL;
			}			
		}
		
		delete []pp_ChnRecIPCMD;
		pp_ChnRecIPCMD = NULL;
	}
	
	//time_t *p_ChnRecIPCEXT;//¼������-- �¼�-- IPCEXT������ʱ��
	if (pp_ChnRecIPCEXT != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnRecIPCEXT[i] != NULL)
			{
				delete []pp_ChnRecIPCEXT[i];
				pp_ChnRecIPCEXT[i]  = NULL;
			}			
		}
		
		delete []pp_ChnRecIPCEXT;
		pp_ChnRecIPCEXT = NULL;
	}
	
	//time_t *p_SGSensor;//�ϴ�����-- ����-- sersor������ʱ��
	if (pp_ChnSGSensor != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnSGSensor[i] != NULL)
			{
				delete []pp_ChnSGSensor[i];
				pp_ChnSGSensor[i]  = NULL;
			}			
		}
		
		delete []pp_ChnSGSensor;
		pp_ChnSGSensor = NULL;
	}

	//time_t *p_ChnSGIPCMD;//�ϴ�����-- ����-- IPCMD������ʱ��
	if (pp_ChnSGIPCMD != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnSGIPCMD[i] != NULL)
			{
				delete []pp_ChnSGIPCMD[i];
				pp_ChnSGIPCMD[i]  = NULL;
			}			
		}
		
		delete []pp_ChnSGIPCMD;
		pp_ChnSGIPCMD = NULL;
	}
	
	//time_t *p_ChnSGIPCEXT;//�ϴ�����-- ����-- IPCEXT������ʱ��
	if (pp_ChnSGIPCEXT != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{
			//time_t *p_RecSensor;//¼������-- �¼�-- sersor������ʱ��
			if (pp_ChnSGIPCEXT[i] != NULL)
			{
				delete []pp_ChnSGIPCEXT[i];
				pp_ChnSGIPCEXT[i]  = NULL;
			}			
		}
		
		delete []pp_ChnSGIPCEXT;
		pp_ChnSGIPCEXT = NULL;
	}
	
	if (p_SensorStatus != NULL)
	{
		delete []p_SensorStatus;
		p_SensorStatus = NULL;
	}

	if (p_IPCMDStatus != NULL)
	{
		delete []p_IPCMDStatus;
		p_IPCMDStatus = NULL;
	}

	if (p_IPCEXTStatus != NULL)
	{
		delete []p_IPCEXTStatus;
		p_IPCEXTStatus = NULL;
	}
	
	//SnapChnProcessDataType *pChnData	
	if (pChnData != NULL)
	{
		for (i=0; i<m_MaxIpcChn; i++)
		{			
			if (pChnData[i].p_ChnRequestSnapTimer != NULL)
			{
				delete []pChnData[i].p_ChnRequestSnapTimer;
				pChnData[i].p_ChnRequestSnapTimer = NULL;
			}

			//u8 *pSnapData;
			if (pChnData[i].pSnapData != NULL)
			{
				delete []pChnData[i].pSnapData;
				pChnData[i].pSnapData = NULL;
			}			
		}
		
		delete []pChnData;
		pChnData = NULL;		
	}

	if (p_SensorRange != NULL)
	{
		delete []p_SensorRange;
		p_SensorRange = NULL;
	}

	if (p_IPCMDRange != NULL)
	{
		delete []p_IPCMDRange;
		p_IPCMDRange = NULL;
	}

	if (p_IPCEXTRange != NULL)
	{
		delete []p_IPCEXTRange;
		p_IPCEXTRange = NULL;
	}

	DestroyCircleBuf(&SnapMsgQueue);
}

CSnapManager::~CSnapManager()
{
	DestroyThread();
	releaseResource();
}

int CSnapManager::Start(u8 MaxSensorNum, u8 MaxIpcChn)
{
	//printf("CSnapManager %s 1\n", __func__);
	m_MaxSensorNum = MaxSensorNum;
	m_MaxIpcChn = MaxIpcChn;
	m_Started = FALSE;
	memset(&SnapMsgQueue, 0, sizeof(SnapMsgQueue));
	
	//printf("CSnapManager %s 2\n", __func__);
	if (0 != requestResource())
	{
		releaseResource();
		return 1;
	}
	
	if (FALSE == CreateThread())
	{
		releaseResource();
		return 1;
	}
	
	m_Started = TRUE;
	
	return 0;
}

typedef s32 (* pfuncGetAlarmDispatch)(u32 bDefault, SBizAlarmDispatch *psPara, u8 nId);

int CSnapManager::UploadAndRecDeal(EM_ALARM_TYPE type, time_t CurTime)
{
	u32 i, j;
	SBizSGParam SGParam;	//���ƽ̨����
	SBizAlarmPicCFG AlarmPicCFG;//�����ϴ�ͨ��
	SBizSnapChnPara SnapChnParam;	//ͨ��ץͼ����
	SBizAlarmDispatch AlarmDispatch;	//��������--����ͨ��¼��
	
	int Interval;
	u8 *ipcchn = NULL;

	if (ConfigGetSGPara(0, &SGParam) != 0)
	{
		printf("%s ConfigGetSGPara failed\n", __func__);
		return 1;
	}

	u8 MaxAlarmNum = 0;
	u8 offset = 0;
	
	/*
	EM_ALARM_STATUS *p_SensorStatus;	//��������״̬
	EM_ALARM_STATUS *p_IPCMDStatus;	//IPC�ƶ����״̬
	EM_ALARM_STATUS *p_IPCEXTStatus;	//IPC�ⲿ����״̬
	*/
	EM_ALARM_STATUS *p_AlarmStatus = NULL;
	
	/*
	//�Ƿ�Ҫ�ϴ���������ʱ�̵�ǰ��ʱ���ĸ�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 * p_UploadPreRec_Sensor;//��Чλ��: m_MaxSensorNum
	u32 * p_UploadPreRec_IPCMD;
	u32 * p_UploadPreRec_IPCEXT;
	*/
	u32 *p_UploadPreRec_Alarm = NULL;
	/*
	//�Ƿ�Ҫ�ϴ���ǰʱ�̸�ͨ������ͼƬ
	//����m_MaxIpcChn��������ÿһλָʾ����Դ
	u32 * p_UploadCur_Sensor;//��Чλ��: m_MaxSensorNum
	u32 * p_UploadCur_IPCMD;
	u32 * p_UploadCur_IPCEXT;
	*/
	u32 *p_UploadCur_Alarm = NULL;
	/*
	sTimeRange *p_SensorRange;
	sTimeRange *p_IPCMDRange;
	sTimeRange *p_IPCEXTRange;
	*/
	sTimeRange *p_AlarmRange = NULL;
	
	/*
	s32 ConfigGetAlarmSensorDispatch(u32 bDefault,SBizAlarmDispatch *psPara, u8 nId);
	s32 ConfigGetAlarmIPCExtSensorDispatch(u32 bDefault,SBizAlarmDispatch *psPara, u8 nId);
	s32 ConfigGetAlarmVMotionDispatch(u32 bDefault,SBizAlarmDispatch *psPara, u8 nId);
	*/
	pfuncGetAlarmDispatch p_GetAlarmDispatch = NULL;

	EM_SNAP_TYPE BitSnapType;
	/*
	time_t **pp_ChnRecSensor;//¼������-- �¼�-- sersor������chn��󱣴�ʱ��
	time_t **pp_ChnRecIPCMD;//¼������-- �¼�-- IPCMD������chn��󱣴�ʱ��
	time_t **pp_ChnRecIPCEXT;//¼������-- �¼�-- IPCEXT������chn��󱣴�ʱ��
	time_t **pp_ChnSGSensor;//�ϴ�����-- ����-- sersor������chn����ϴ�ʱ��
	time_t **pp_ChnSGIPCMD;//�ϴ�����-- ����-- IPCMD������chn����ϴ�ʱ��
	time_t **pp_ChnSGIPCEXT;//�ϴ�����-- ����-- IPCEXT������chn����ϴ�ʱ��
	*/
	time_t **pp_ChnRecAlarm = NULL;
	time_t **pp_ChnSGAlarm = NULL;
	switch (type)
	{
		case TypeSensor:	// 0
		{
			MaxAlarmNum = m_MaxSensorNum;
			offset = 0;
			p_AlarmStatus = p_SensorStatus;
			p_UploadPreRec_Alarm = p_UploadPreRec_Sensor;
			p_UploadCur_Alarm = p_UploadCur_Sensor;
			p_AlarmRange = p_SensorRange;
			p_GetAlarmDispatch = ConfigGetAlarmSensorDispatch;
			BitSnapType = TypeAlarmSensor;
			pp_ChnRecAlarm = pp_ChnRecSensor;
			pp_ChnSGAlarm = pp_ChnSGSensor;
		} break;
		case TypeMD:		// 1
		{
			MaxAlarmNum = m_MaxIpcChn;
			offset = m_MaxSensorNum;
			p_AlarmStatus = p_IPCMDStatus;
			p_UploadPreRec_Alarm = p_UploadPreRec_IPCMD;
			p_UploadCur_Alarm = p_UploadCur_IPCMD;
			p_AlarmRange = p_IPCMDRange;
			p_GetAlarmDispatch = ConfigGetAlarmVMotionDispatch;
			BitSnapType = TypeMotionDet;
			pp_ChnRecAlarm = pp_ChnRecIPCMD;
			pp_ChnSGAlarm = pp_ChnSGIPCMD;
		} break;
		case TypeIPCEXT:	// 2
		{
			MaxAlarmNum = m_MaxIpcChn;
			offset = m_MaxSensorNum;
			p_AlarmStatus = p_IPCEXTStatus;
			p_UploadPreRec_Alarm = p_UploadPreRec_IPCEXT;
			p_UploadCur_Alarm = p_UploadCur_IPCEXT;
			p_AlarmRange = p_IPCEXTRange;
			p_GetAlarmDispatch = ConfigGetAlarmIPCExtSensorDispatch;
			BitSnapType = TypeAlarmIPCEXT;
			pp_ChnRecAlarm = pp_ChnRecIPCEXT;
			pp_ChnSGAlarm = pp_ChnSGIPCEXT;
		} break;
		default:
		{
			printf("%s EM_ALARM_TYPE, type: %d invalid\n", __func__, type);
			return 1;
		}
	}

	//���Ԥ¼�ϴ�
	//�������״̬�����ұ�����������
	if (SGParam.RunEnable && SGParam.AlarmEnable)	
	{
		for (i=0; i<MaxAlarmNum; i++)
		{
		//����Դ����ͨ��
			if (ConfigGetSGAlarmPara(0, &AlarmPicCFG, i + offset) == 0)
			{
				if (AlarmPicCFG.IpcChn)
				{					
					for (j=0; j<m_MaxIpcChn; j++)
					{
						pChnMutex[j].Enter();
						
						if (AlarmPicCFG.IpcChn & (1<<j))//�뱨��Դ������IPC
						{	
							//��������IPCͨ����Ԥ¼��ǣ��Ա�ȡ��ͼƬ�󱣴�
							//��ͨ�������Ƕ������������������Ҫ��¼���Ԥ¼ʱ��
							//��ǰʱ�����������Ԥ¼ʱ���1S
							//printf("%s alarmchn%d, IPCchn%d, curtime: %d, p_ChnPreRec: %d\n", 
								//	__func__, i, j, CurTime, p_ChnPreRec[j]);
								
							if ((CurTime > p_ChnPreRec[j]) && (CurTime -  p_ChnPreRec[j]  >= 1))
							{
								p_ChnPreRec[j] = CurTime;

								p_ChnReqTypeMask[j] |= 1<<TypePreSnap;
							}
							
							//�Ƿ�Ҫ�ϴ���������ʱ�̵�ǰ��ʱ��������ͼƬ
							if ( p_AlarmStatus[i] == EM_ALARM_TIGGER )
							{
								//�ɴ�����i ����
								//�ϴ���ǰ��������j ͨ����Ԥ¼ͼƬ
								struct tm now;
								localtime_r(&CurTime,&now);
								printf("%s Alarmchn%d EM_ALARM_TIGGER time: %d - %2d:%2d:%2d\n", \
									__func__, i, CurTime, now.tm_hour+8, now.tm_min, now.tm_sec);
		
								p_UploadPreRec_Alarm[j] = 1<<i;
							}
							
							//�Ƿ��ϴ�����������ͨ��ͼƬ
							if ( p_AlarmStatus[i] != EM_ALARM_NONE )
							{
								Interval = AlarmPicCFG.Interval;
								if ((CurTime > pp_ChnSGAlarm[j][i]) && (CurTime - pp_ChnSGAlarm[j][i]  >= Interval))
								{
									pp_ChnSGAlarm[j][i] = CurTime;

									//�ɴ�����i ����
									//�ϴ���ǰ��������j ͨ����ͼƬ
									p_UploadCur_Alarm[j] = 1<<i;
								}
							}							
						}

						pChnMutex[j].Leave();
					}
				}
			}
		}
	}

	//��ⱨ�����Ƿ񱣴�ͼƬ
	int processflag = 0;
	for (i=0; i<MaxAlarmNum; i++)
	{
	//��������-����¼�����Ƿ���ͨ����������
		//״̬�����Ǳ��������������С��������
		processflag = 0;
		switch (p_AlarmStatus[i])
		{
			case EM_ALARM_NONE:	//=0,	// 0 �ޱ�������ʼ״̬���������������״̬
			{
				processflag = 0;
			}break;
			case EM_ALARM_TIGGER:		// 1 ��������
			{
				p_AlarmStatus[i] = EM_ALARM_ING;
				p_AlarmRange[i].StartTime = CurTime;
				p_AlarmRange[i].EndTime = 0;
				
				processflag = 1;
			}break;
			case EM_ALARM_ING:		// 2 ������
			{
				processflag = 1;
			}break;
			case EM_ALARM_END:		// 3 �������
			{
				p_AlarmStatus[i] = EM_ALARM_STILL;
				p_AlarmRange[i].EndTime = CurTime;
				
				struct tm now;
				localtime_r(&CurTime,&now);
				printf("%s Alarmchn%d EM_ALARM_END time: %d - %02d:%02d:%02d\n", \
					__func__, i, CurTime, now.tm_hour+8, now.tm_min, now.tm_sec);
				
				processflag = 1;
			}break;
			case EM_ALARM_STILL:	// 4	�����ӳ���¼�ϴ�
			{
				ConfigGetSGAlarmPara(0, &AlarmPicCFG, i + offset);
				
				if (CurTime >= p_AlarmRange[i].EndTime+ AlarmPicCFG.StillTimes)
				{
					p_AlarmStatus[i] = EM_ALARM_NONE;
					p_AlarmRange[i].StartTime = 0;
					p_AlarmRange[i].EndTime = 0;

					struct tm now;
					localtime_r(&CurTime,&now);
					printf("%s Alarmchn%d EM_ALARM_STILL time: %d - %2d:%2d:%2d\n", \
						__func__, i, CurTime, now.tm_hour+8, now.tm_min, now.tm_sec);
				}

				processflag = 0;
			}break;
			default:
			{
				printf("%s EM_ALARM_STATUS, type: %d invalid\n", __func__, p_AlarmStatus[i]);
				return 1;
			}
		}

		//���������ͽ���ڼ�Ŵ���
		if (processflag)//�Ƿ��¼����ͼƬ
		{
			if ((*p_GetAlarmDispatch)(0, &AlarmDispatch, i) == 0)
			{
				ipcchn = AlarmDispatch.nRecordChn;
				//printf("%s alarm ipcchn: 0x%x\n", __func__, ipcchn);
				
				for (j=0; j<m_MaxIpcChn; j++)
				{
					//�Ƿ񴥷��˸�¼��ͨ��
					if (ipcchn[j] != 0xff)
					{	//��ͨ���Ƿ�ʹ�����¼�ץͼ
						if ((ConfigGetSnapChnPara(0, &SnapChnParam, j) == 0) 
							&& (SnapChnParam.EventSnap.Enable))
						{							
							Interval = indextosec[SnapChnParam.EventSnap.Interval];
							//�մ������ǵ�������
							if ((CurTime > pp_ChnRecAlarm[j][i]) && (CurTime - pp_ChnRecAlarm[j][i]  >= Interval))
							{
								pp_ChnRecAlarm[j][i] = CurTime;

								pChnMutex[j].Enter();
								p_ChnReqTypeMask[j] |= 1<<BitSnapType;
								pChnMutex[j].Leave();
							}							
						}
					}
				}
			}
		}
	}

	return 0;
}

void CSnapManager::ThreadProc()
{
	u32 i, j, curSecs, speSecs;//��ǰʱ�任��ɴ���㿪ʼ������
	SBizSGParam SGParam;	//���ƽ̨����
	SBizDailyPicCFG para1;
	SBizSnapChnPara SnapChnParam;	//ץͼ����
	EM_SNAPMSG_TYPE msg;
	
	time_t CurTime;
	int Interval, ret, running;
	//u32 ReqChnMask;
	
	printf("CSnapManager::ThreadProc: m_MaxSensorNum: %d, m_MaxIpcChn: %d\n", m_MaxSensorNum, m_MaxIpcChn);
	
	running = 1;
	
	while (1)
	{
		//usleep(1000*1000);// 200msɨһ��
		sleep(1);
		
		ret = SnapReadMsgQueue(&msg);
		if (0 == ret)
		{
			printf("%s get msg type: %d, cur statue: %d\n", __func__, msg, running);
			switch(msg)
			{
				case TypePause:
				{
					running = 0;
				} break;
				case TypeResume:
				{
					running = 1;
				} break;
				default:
					printf("%s invalid msg type: %d\n", __func__, msg);
			}
		}

		if (0 == running)//��ʽ�� �����Ȳ��ʱ��ͣ
		{
			continue;
		}

		//yaogang modify 20150724 //��Ԥ��״̬��
		if(!ModPreviewIsOpened())
		{
			printf("ModPreviewIsOpened == 0\n");
			continue;
		}
		
		if (ConfigGetSGPara(0, &SGParam) != 0)
		{
			printf("%s ConfigGetSGPara failed\n", __func__);
			continue;
		}
		
		time(&CurTime);
		
		//1������Դ��IPCץͼͨ�������Ӱ��
		for (i=0; i<TypeAlarmMax; i++)
		{
			UploadAndRecDeal((EM_ALARM_TYPE)i, CurTime);
		}
	
		//2���ճ�ͼ���Ƿ�����(ÿ������ʱ����ϴ����)
		SBizDateTime DateTime;
		BizSysComplexDTGet_TZ(&DateTime);
		static int flagtime1 = 0;
		static int flagtime2 = 0;
		//printf("RunEnable: %d, DailyPicEnable: %d\n", SGParam.RunEnable, SGParam.DailyPicEnable);
		if (SGParam.RunEnable && SGParam.DailyPicEnable)	//����״̬�������ճ�ͼ������
		{
			if (ConfigGetSGDailyPara(0, &para1) == 0)
			{
				speSecs = nSECPERHOUR*para1.Time1.hour + nSECPERMIN*para1.Time1.min + para1.Time1.sec;
				curSecs = nSECPERHOUR*DateTime.nHour + nSECPERMIN*DateTime.nMinute + DateTime.nSecond;

				//printf("IpcChn1: 0x%x, Time1Enable: %d\n", para1.IpcChn1, para1.Time1.TimeEnable);
				//printf("IpcChn2: 0x%x, Time2Enable: %d\n", para1.IpcChn2, para1.Time2.TimeEnable);				
				/*
				printf("hour: %d, min: %d, sec: %d\n", 
					DateTime.nHour, 
					DateTime.nMinute, 
					DateTime.nSecond);
				printf("speSecs: %d, curSecs: %d\n", speSecs, curSecs);
				*/
				/*
				printf("hour: %d, min: %d, sec: %d\n", 
					para1.Time2.hour, 
					para1.Time2.min, 
					para1.Time2.sec);
				*/
				if (para1.Time1.TimeEnable && (curSecs >= speSecs) && (curSecs - speSecs < 10))
				{			
					if (flagtime1 == 0)
					{
						flagtime1 = 1;
						//printf("curSecs >= para1.Time1\n");
						//printf("para1.IpcChn1: 0x%x, m_MaxIpcChn: %d\n", para1.IpcChn1, m_MaxIpcChn);
						for (j=0; j<m_MaxIpcChn; j++)
						{
							if (para1.IpcChn1 & (1<<j))//����
							{
								pChnMutex[j].Enter();
								p_ChnReqTypeMask[j] |= 1<<TypeDaily;
								pChnMutex[j].Leave();
							}
						}
					}					
				}
				else
				{
					flagtime1 = 0;
				}
				
				speSecs = nSECPERHOUR*para1.Time2.hour + nSECPERMIN*para1.Time2.min + para1.Time2.sec;				
				if (para1.Time2.TimeEnable && (curSecs >= speSecs) && (curSecs - speSecs < 10))
				{		
					//printf("curSecs >= para1.Time2\n");
					if (flagtime2 == 0)
					{
						flagtime2 = 1;
						//printf("curSecs >= para1.Time2\n");
						//printf("para1.IpcChn2: 0x%x, m_MaxIpcChn: %d\n", para1.IpcChn2, m_MaxIpcChn);
						for (j=0; j<m_MaxIpcChn; j++)
						{
							if (para1.IpcChn2 & (1<<j))//����
							{
								pChnMutex[j].Enter();
								p_ChnReqTypeMask[j] |= 1<<TypeDaily;
								pChnMutex[j].Leave();
							}
						}
					}					
				}
				else
				{
					flagtime2 = 0;
				}
			}
		}
		
		for (i=0; i<m_MaxIpcChn; i++) 
		{				
			//3����ʱץͼ�Ƿ�����
			//		¼������--> ץͼ����
			//	�����ڴ����ã��յ�ͼƬ��������
			//if ((ConfigGetSnapChnPara(0, &SnapChnParam, i) == 0)
				//&& (SnapChnParam.TimeSnap.Enable))
			if (ConfigGetSnapChnPara(0, &SnapChnParam, i) == 0)
			{	/*
				//debug
				if (i == 0)
				{
					printf("TimeSnap: enable: %d, Res: %d, Quality: %d, Interval: %d\n", 
						SnapChnParam.TimeSnap.Enable, SnapChnParam.TimeSnap.Res,
						SnapChnParam.TimeSnap.Quality, SnapChnParam.TimeSnap.Interval);

					printf("EventSnap: enable: %d, Res: %d, Quality: %d, Interval: %d\n", 
						SnapChnParam.EventSnap.Enable, SnapChnParam.EventSnap.Res,
						SnapChnParam.EventSnap.Quality, SnapChnParam.EventSnap.Interval);

					printf("CurTime: %u, p_ChnTimeLast[0]: %u, Interval: %u\n", CurTime, p_ChnTimeLast[0], indextosec[SnapChnParam.TimeSnap.Interval]);
				}
				*/
				if (SnapChnParam.TimeSnap.Enable)
				{
					Interval = indextosec[SnapChnParam.TimeSnap.Interval];
					
					if ((CurTime > p_ChnTimeLast[i]) && (CurTime - p_ChnTimeLast[i]  >= Interval))
					{
						pChnMutex[i].Enter();
						p_ChnReqTypeMask[i] |= 1<<TypeTime;
						pChnMutex[i].Leave();
						p_ChnTimeLast[i] = CurTime;
					}
				}
			}

			/*
			if (i==0 && p_ChnReqTypeMask[i])
			{
				printf("%s chn%d p_ChnReqTypeMask: 0x%x\n", __func__, i, p_ChnReqTypeMask[i]);
			}
			*/
			
			//printf("%s chn%d, 1\n", __func__, i);
			
			//���ͨ��û�н�IPC����ô���ͨ�����
			if (IPC_GetLinkStatus(i) == 0)
			{
				pChnMutex[i].Enter();
				
				p_ChnReqTypeMask[i] = 0;				
				p_UploadPreRec_Sensor[i] = 0;
				p_UploadPreRec_IPCMD[i] = 0;
				p_UploadPreRec_IPCEXT[i] = 0;
				p_UploadCur_Sensor[i] = 0;
				p_UploadCur_IPCMD[i] = 0;
				p_UploadCur_IPCEXT[i] = 0;

				pChnMutex[i].Leave();
			}

			//printf("%s chn%d, 2\n", __func__, i);
			
			if (p_ChnReqTypeMask[i])
			{
				//printf("%s chn%d, 2\n", __func__, i);
				//if (pp_ChnRequestSnapTimer[i]->IsStarted() == FALSE)
				
				
				
				if (pChnData[i].TimerWorking) //��ͨ����ʱ���߳����ڴ���֮ǰ�����񣬺��Դ˴�����
				{
					printf("CSnapManager::ThreadProc chn%d is working, ignore once snap\n", i);
				}
				else
				{
					
					pChnMutex[i].Enter();
					
					pChnData[i].TimerWorking = TRUE;
					pChnData[i].ChnReqTypeMask = p_ChnReqTypeMask[i];				
					pChnData[i].UploadPreRec_Sensor = p_UploadPreRec_Sensor[i];
					pChnData[i].UploadPreRec_IPCMD = p_UploadPreRec_IPCMD[i];
					pChnData[i].UploadPreRec_IPCEXT = p_UploadPreRec_IPCEXT[i];
					pChnData[i].UploadCur_Sensor = p_UploadCur_Sensor[i];
					pChnData[i].UploadCur_IPCMD = p_UploadCur_IPCMD[i];
					pChnData[i].UploadCur_IPCEXT = p_UploadCur_IPCEXT[i];

					//�����ǣ�ֻ����TimerWorking == FALSE�����
					p_ChnReqTypeMask[i] = 0;				
					p_UploadPreRec_Sensor[i] = 0;
					p_UploadPreRec_IPCMD[i] = 0;
					p_UploadPreRec_IPCEXT[i] = 0;
					p_UploadCur_Sensor[i] = 0;
					p_UploadCur_IPCMD[i] = 0;
					p_UploadCur_IPCEXT[i] = 0;

					pChnMutex[i].Leave();
					//printf("%s timer start %d\n", __func__, i);
					pChnData[i].p_ChnRequestSnapTimer->Start(this, (VD_TIMERPROC)&CSnapManager::Snapshot_Register, 0, 0, i);
					
				}
				
			}
		}
		//printf("%s chn%d, 3\n", __func__, i);
		
	}
	
}

//�ϴ���������ʱ�̵�ǰ��ʱ����ڶ���Ԥ¼ͼƬ
/*
TypeAlarmSensor,	// 7 ���ش���������
TypeMotionDet,		// 8 �ƶ����
TypeAlarmIPCEXT,	// 9 IPC�ⲿ����
*/
int CSnapManager::UploadPreRecPic(EM_SNAP_TYPE type, u8 AlarmChn, u8 IPCChn)
{
	time_t TriggerTime, FirstSnapTime, UploadTime;
	u8 PreTimes;		//ǰ��ʱ��
	u8 Interval;		//���ʱ��
	SBizAlarmPicCFG AlarmPicCFG;//�����ϴ�ͨ��
	SModRecRecordHeader head;
	u8 *pSnapData = NULL;
	struct timeval tv;
	int ret;
	int GUID = 0;//һ�鱨����Ϣ������GUID������ͬ

	pSnapData = (u8 *)malloc(SNAPBUFSIZE);
	if (pSnapData == NULL)
	{
		printf("%s malloc failed\n", __func__);
		return 1;
	}
	
	if (IPCChn >= m_MaxIpcChn)
	{
		printf("%s IPCChn: %d invalid\n", __func__, IPCChn);
		return 1;
	}

	switch (type)
	{
		case TypeAlarmSensor:
		{			
			if (AlarmChn >= m_MaxSensorNum)
			{
				printf("%s TypeAlarmSensor AlarmChn: %d invalid\n", __func__, AlarmChn);
				return 1;
			}

			GUID = p_GUID_Sensor[AlarmChn];
			TriggerTime = p_SensorRange[AlarmChn].StartTime;
			
			if ( ConfigGetSGAlarmPara(0, &AlarmPicCFG, AlarmChn) )
			{
				printf("%s TypeAlarmSensor AlarmChn%d ConfigGetSGAlarmPara() failed\n", __func__, AlarmChn);
				return 1;
			}
		} break;
		case TypeMotionDet:
		{			
			if (AlarmChn >= m_MaxIpcChn)
			{
				printf("%s TypeMotionDet AlarmChn: %d invalid\n", __func__, AlarmChn);
				return 1;
			}

			GUID = p_GUID_IPCMD[AlarmChn];
			TriggerTime = p_IPCMDRange[AlarmChn].StartTime;
			
			if ( ConfigGetSGAlarmPara(0, &AlarmPicCFG, AlarmChn + m_MaxSensorNum) )
			{
				printf("%s TypeMotionDet AlarmChn%d ConfigGetSGAlarmPara() failed\n", __func__, AlarmChn);
				return 1;
			}
		} break;
		case TypeAlarmIPCEXT:
		{			
			if (AlarmChn >= m_MaxIpcChn)
			{
				printf("%s TypeAlarmIPCEXT AlarmChn: %d invalid\n", __func__, AlarmChn);
				return 1;
			}

			GUID = p_GUID_IPCEXT[AlarmChn];
			TriggerTime = p_IPCEXTRange[AlarmChn].StartTime;
			
			if ( ConfigGetSGAlarmPara(0, &AlarmPicCFG, AlarmChn + m_MaxSensorNum) )
			{
				printf("%s TypeAlarmIPCEXT AlarmChn%d ConfigGetSGAlarmPara() failed\n", __func__, AlarmChn);
				return 1;
			}
		} break;
		default:
		{
			printf("%s snap type: %d invalid\n", __func__, type);
			return 1;
		}
	}

	PreTimes = AlarmPicCFG.PreTimes;
	Interval = AlarmPicCFG.Interval;
	FirstSnapTime = TriggerTime - PreTimes;

	
	//�ϴ�ǰ��ʱ��㵽��������ʱ���ڵĶ���Ԥ¼ͼƬ
	for (UploadTime = FirstSnapTime; UploadTime < TriggerTime; UploadTime += Interval)
	{
		printf("%s FirstSnapTime: %d, Interval: %d, UploadTime: %d\n", \
			__func__, FirstSnapTime, Interval, UploadTime);
		memset(&head, 0, sizeof(head));
		
		head.nChn = IPCChn;
		head.nStreamType = 2;//Snap
		head.nBitRate = 1<<TypePreSnap;
		head.nDate = pSnapData;
		head.nDataLength = SNAPBUFSIZE;
		tv.tv_sec = UploadTime;
		tv.tv_usec = 0;
		memcpy(&head.nPts, &tv, sizeof(tv));
		
		printf("%s read PreRec time: %d\n", __func__, UploadTime);
		ret = RecordReadOnePreSnap(IPCChn, &head);
		if (ret)
		{
			printf("%s chn%d RecordReadOneSnap failed, ret: %d\n", __func__, IPCChn, ret);
			continue;
		}

		ret = UploadPic(type, AlarmChn, IPCChn, tv.tv_sec, (char *)head.nDate, head.nDataLength, GUID);
		if (ret)
		{
			printf("%s type: %d, AlarmChn: %d, chn%d UploadPic failed, ret: %d\n", \
				__func__, type, AlarmChn, IPCChn, ret);
			continue;
		}
	
	}

/*����ʱ�̵�ͼƬ��Snapshot_Register() �ϴ�
	//�ϴ���������ʱ�̵�ͼ��
	memset(&head, 0, sizeof(head));
	head.nChn = IPCChn;
	head.nStreamType = 2;//Snap
	head.nBitRate = 1<<TypePreSnap;
	head.nDate = pSnapData;
	head.nDataLength = SNAPBUFSIZE;
	
	tv.tv_sec = TriggerTime;
	tv.tv_usec = 0;
	memcpy(&head.nPts, &tv, sizeof(tv));

	ret = RecordReadOnePreSnap(IPCChn, &head);
	if (ret)
	{
		printf("%s chn%d RecordReadOneSnap failed, ret: %d\n", __func__, IPCChn, ret);
		return 1;
	}

	ret = UploadPic(type, AlarmChn, IPCChn, tv.tv_sec, head.nDate, head.nDataLength, GUID);
	if (ret)
	{
		printf("%s type: %d, AlarmChn: %d, chn%d UploadPic failed, ret: %d\n", \
			__func__, type, AlarmChn, IPCChn, ret);
		return 1;
	}
*/
	return 0;
}

//ʱ�������Ϊ����������ָ������ΪҪ�ϴ�֮ǰ��Ԥ¼ͼƬ
//һ�鱨����Ϣ������GUID������ͬ��
int CSnapManager::UploadPic(EM_SNAP_TYPE type, u8 AlarmChn, u8 IPCChn, time_t time, void *SnapData, u32 DataSize, int GUID)
{
	SSG_MSG_TYPE msg;
	
	memset(&msg, 0, sizeof(SSG_MSG_TYPE));
	AlarmChn += 1;//0-15-->1-16
	msg.chn = IPCChn+1;

	switch (type)
	{
		case TypeManualTEST:	//����
		{
			msg.type = EM_PIC_TEST_UPLOAD;
			strcpy(msg.note, GetParsedString("&CfgPtn.TestImageUpload"));
							
		} break;
		case TypeManualMAINTAIN:	//����
		{
			msg.type = EM_PIC_MAINTAIN_UPLOAD;
			strcpy(msg.note, GetParsedString("&CfgPtn.MaintenanceImageUpload"));
		} break;
		case TypeManualACCEPT:		//����
		{
			msg.type = EM_PIC_ACCEPT_UPLOAD;
			sprintf(msg.note, "%s: %s %s: %s",
							GetParsedString("&CfgPtn.PoliceID"), PoliceID, \
							GetParsedString("&CfgPtn.KeyWord"), PassWord);
		} break;
		case TypeDaily:	//�ճ�
		{
			//if (IPCChn == 0)
				//printf("%s: chn%d TypeDaily upload\n", __func__, IPCChn);
			
			msg.type = EM_PIC_DAILY_UPLOAD;
			strcpy(msg.note, GetParsedString("&CfgPtn.DayImageUpload"));
		} break;
		case TypeSvrReq:	//���ƽ̨�����ϴ�
		{
			msg.type = EM_PIC_SERVER_GET;
			//strcpy(msg.note, GetParsedString("&CfgPtn.DayImageUpload"));
		} break;
		/*
		����N����	ͨ��M����ͼ���ϴ�
		IPC N����
		IPC N�ƶ����
		*/
	#if 0
		case TypeAlarmSensor:
		case TypeMotionDet:
		case TypeAlarmIPCEXT:
		{
			msg.type = EM_PIC_ALARM_LINK_UPLOAD;
			sprintf(msg.note, "%s[%s%02d]", \
				GetParsedString("&CfgPtn.AlarmLinkPic"),
				GetParsedString("&CfgPtn.Channel"), 
				msg.chn);
			
		} break;
	#else
		case TypeAlarmSensor:
		{
			msg.type = EM_PIC_ALARM_LINK_UPLOAD;
			sprintf(msg.note, "%s%02d%s, %s%02d%s", \
				GetParsedString("&CfgPtn.Local"),
				AlarmChn,
				GetParsedString("&CfgPtn.Alarm1"),
				GetParsedString("&CfgPtn.Channel"), 
				msg.chn,
				GetParsedString("&CfgPtn.LinkPicUpload")
				);
			
		} break;
		case TypeMotionDet:
		{
			msg.type = EM_PIC_ALARM_LINK_UPLOAD;
			sprintf(msg.note, "IPC%02d%s, %s%02d%s", \
				AlarmChn,
				GetParsedString("&CfgPtn.Motion"),
				GetParsedString("&CfgPtn.Channel"), 
				msg.chn,
				GetParsedString("&CfgPtn.LinkPicUpload")
				);
		} break;
		case TypeAlarmIPCEXT:
		{
			msg.type = EM_PIC_ALARM_LINK_UPLOAD;
			sprintf(msg.note, "IPC%02d%s, %s%02d%s", \
				AlarmChn,
				GetParsedString("&CfgPtn.Alarm1"),
				GetParsedString("&CfgPtn.Channel"), 
				msg.chn,
				GetParsedString("&CfgPtn.LinkPicUpload")
				);
		} break;
	#endif
		default:
		{
			printf("%s type %d invalid\n", __func__, type);
			return 1;
		} 
	}

	return upload_sg_proc(&msg, time, SnapData, DataSize, GUID);	
}

void CSnapManager::Snapshot_Register(u8 chn)
{
	int i, cnt, ret, TimeResIdx, EventResIdx;//�ֱ�������
	u32 SnapSize;
	SBizSnapChnPara SnapChnParam;	//ץͼ����
	SModRecRecordHeader head;
	u16 width, height;
	u8 *pSnapData = pChnData[chn].pSnapData;
	struct timeval tv;
	struct tm curtm;

	#if 0//����Ӳ�̣���ʱ�ر�Ԥ¼20150527
	pChnData[chn].TimerWorking = FALSE;
		
	return ;
	#endif

	gettimeofday(&tv, NULL);
	gmtime_r(&tv.tv_sec, &curtm);
	//printf("%s chn%d, time: %d:%d:%d, size: %d, width: %d, height: %d\n", 
		//__func__, chn, curtm.tm_hour+8, curtm.tm_min, curtm.tm_sec, DataSize, width, height);

	//dbg
	//if (chn == 0)
		//printf("chn%d ChnReqTypeMask: 0x%x\n", chn, pChnData[chn].ChnReqTypeMask);
	


#if 1
	//����ץͼ�������ֱ��ʺ�ͼƬ����(������û�мӽ�ȥ)
	if (ConfigGetSnapChnPara(0, &SnapChnParam, chn) == 0)
	{
		TimeResIdx = SnapChnParam.TimeSnap.Res;//��ʱץͼ�ֱ���
		EventResIdx = SnapChnParam.EventSnap.Res;//�¼�ץͼ�ֱ���
	}
	else
	{
		printf("%s ConfigGetSnapChnPara failed\n", __func__);
		pChnData[chn].TimerWorking = FALSE;
		
		return ;
	}

	//��ʱ���¼�ͼƬ�ֱ��ʲ�ͬʱ�������ȴ���
	if ( (TimeResIdx != EventResIdx) &&
		((pChnData[chn].ChnReqTypeMask & (1<<TypeTime)) || (pChnData[chn].ChnReqTypeMask & (1<<TypeDaily))) )
	{

		SnapSize = SNAPBUFSIZE;
		width = SnapRes[TimeResIdx].width;
		height = SnapRes[TimeResIdx].height;
		
		//��ȡ��ʱ����ʱ��ץͼ
		ret = ModEncGetSnapshot(chn, &width, &height, pSnapData, &SnapSize);
		if (ret)
		{
			if (SnapSize == 0)
			{
				printf("%s chn%d ModEncGetSnapshot 1 space too small\n", __func__, chn);					
			}
			printf("%s chn%d ModEncGetSnapshot 1 failed\n", __func__, chn);
			pChnData[chn].TimerWorking = FALSE;
			
			return ;
		}

		// 1����ʱץͼ����
		if (pChnData[chn].ChnReqTypeMask & (1<<TypeTime))
		{
			//�����ȥ����ʱ����
			pChnData[chn].ChnReqTypeMask &= ~(1<<TypeTime);
			
			memset(&head, 0, sizeof(head));
			head.nChn = chn;
			head.nStreamType = 2;//Snap
			//head.nBitRate = ChnReqTypeMask & (0x3f<<TypePreSnap);// 6λ
			head.nBitRate =  (1<<TypeTime);// 6λ
			memcpy(&head.nPts, &tv, sizeof(tv));
			head.nDate = (u8*)pSnapData;
			head.nDataLength = SnapSize;
			head.width = width;
			head.height = height;

			//printf("%s RecordWriteOneSnap chn%d\n", __func__, chn);
			ret = RecordWriteOneSnap(chn, &head);
			if (ret)
			{
				printf("%s chn%d RecordWriteOneSnap 1 failed, ret: %d\n", __func__, chn, ret);
				pChnData[chn].TimerWorking = FALSE;
				return ;
			}
		}

		//2���ճ�ͼƬ�ϴ�(��ʱ���¼�ץͼ��������ͬʱ)
		if (pChnData[chn].ChnReqTypeMask & (1<<TypeDaily))
		{
			//if (chn == 0)
			//	printf("%s: chn%d TypeDaily upload 1\n", __func__, chn);
			pChnData[chn].ChnReqTypeMask &= ~(1<<TypeDaily);
			g_SnapManager.UploadPic(TypeDaily, 0, chn, tv.tv_sec, pSnapData, SnapSize, 0);
		}
	}

	//��ȡ�¼�����ʱ��ץͼ
	SnapSize = SNAPBUFSIZE;
	width = SnapRes[EventResIdx].width;
	height = SnapRes[EventResIdx].height;

	//printf("%s ModEncGetSnapshot chn%d, width: %d, height: %d\n", __func__, chn, width, height);
	ret = ModEncGetSnapshot(chn, &width, &height, pSnapData, &SnapSize);
	if (ret)
	{
		if (SnapSize == 0)
		{
			printf("%s chn%d ModEncGetSnapshot 2 space too small\n", __func__, chn);
		}
		printf("%s chn%d ModEncGetSnapshot 2 failed\n", __func__, chn);
		pChnData[chn].TimerWorking = FALSE;
		
		return ;
	}
	
//һ������ϴ�����ͼƬ
	for (i=0; i<g_SnapManager.m_MaxSensorNum; i++)
	{
		//1. �ϴ���������ʱ�̵�ǰ��ʱ����ڶ���Ԥ¼ͼƬ
		if (pChnData[chn].UploadPreRec_Sensor & (1<<i))
		{
			printf("%s Alarmchn%d, ipcchn%d, UploadPreRec_Sensor\n", __func__, i, chn);
			g_SnapManager.UploadPreRecPic(TypeAlarmSensor, i, chn);
		}
		//2. ����ϴ�һ�ű���ͼƬ�����ڱ�����������¼֮��
		if (pChnData[chn].UploadCur_Sensor & (1<<i))
		{
			g_SnapManager.UploadPic(TypeAlarmSensor, i, chn, tv.tv_sec, \
				pSnapData, SnapSize, p_GUID_Sensor[i]);
		}
	}
	
	for (i=0; i<g_SnapManager.m_MaxIpcChn; i++)
	{
		if (pChnData[chn].UploadPreRec_IPCMD & (1<<i))
		{
			g_SnapManager.UploadPreRecPic(TypeMotionDet, i, chn);
		}
		
		if (pChnData[chn].UploadCur_IPCMD & (1<<i))
		{
			g_SnapManager.UploadPic(TypeMotionDet, i, chn, tv.tv_sec, \
				pSnapData, SnapSize, p_GUID_IPCMD[i]);
		}

		
		if (pChnData[chn].UploadPreRec_IPCEXT & (1<<i))
		{
			g_SnapManager.UploadPreRecPic(TypeAlarmIPCEXT, i, chn);
		}
		
		if (pChnData[chn].UploadCur_IPCEXT & (1<<i))
		{
			g_SnapManager.UploadPic(TypeAlarmIPCEXT, i, chn, tv.tv_sec, \
				pSnapData, SnapSize, p_GUID_IPCEXT[i]);
		}
	}

	//3.����ͼ���ϴ�
	cnt = ret = 0;
	if (pChnData[chn].ChnReqTypeMask & (1<<TypeManualTEST))
	{
		do {
			printf("%s TypeManualTEST UploadPic cnt: %d\n", __func__, cnt);
			cnt++;
			ret = g_SnapManager.UploadPic(TypeManualTEST, 0, chn, tv.tv_sec, \
						pSnapData, SnapSize, 0);
		} while ((ret !=0) && (cnt < 2));
	}
	
	//4.����ͼ���ϴ�
	cnt = ret = 0;
	if (pChnData[chn].ChnReqTypeMask & (1<<TypeManualMAINTAIN))
	{
		do {
			printf("%s TypeManualMAINTAIN UploadPic cnt: %d\n", __func__, cnt);
			cnt++;
			ret = g_SnapManager.UploadPic(TypeManualMAINTAIN, 0, chn, tv.tv_sec, \
						pSnapData, SnapSize, 0);
		} while ((ret !=0) && (cnt < 2));
	}

	//5.���չ����ϴ�
	cnt = ret = 0;
	if (pChnData[chn].ChnReqTypeMask & (1<<TypeManualACCEPT))
	{
		do {
			printf("%s TypeManualACCEPT UploadPic cnt: %d\n", __func__, cnt);
			cnt++;
			ret = g_SnapManager.UploadPic(TypeManualACCEPT, 0, chn, tv.tv_sec, \
						pSnapData, SnapSize, 0);
		} while ((ret !=0) && (cnt < 2));
	}

	//6.���ƽ̨�����ϴ�
	cnt = ret = 0;
	if (pChnData[chn].ChnReqTypeMask & (1<<TypeSvrReq))
	{
		do {
			printf("%s TypeSvrReq UploadPic cnt: %d\n", __func__, cnt);
			cnt++;
			ret = g_SnapManager.UploadPic(TypeSvrReq, 0, chn, tv.tv_sec, \
						pSnapData, SnapSize, 0);
		} while ((ret !=0) && (cnt < 2));
		
	}

	//7���ճ�ͼƬ�ϴ�(��ʱ���¼�ץͼ������ͬʱ)
	if (pChnData[chn].ChnReqTypeMask & (1<<TypeDaily))
	{
		//if (chn == 0)
			//printf("%s: chn%d TypeDaily upload 2\n", __func__, chn);
		pChnData[chn].ChnReqTypeMask &= ~(1<<TypeDaily);
		g_SnapManager.UploadPic(TypeDaily, 0, chn, tv.tv_sec, pSnapData, SnapSize, 0);
	}
	
//��������ͼƬ
	// 1.Ԥ¼	
	// 2.��ʱ 
	// 3.�ֶ���ͼ	
	// 4.��������	
	/*
	TypePreSnap,		// 4 Ԥ¼
	TypeTime,			// 5 ��ʱ-¼������--ץͼ����
	TypeManual,		// 6 �ֶ���ͼ
	//upload & record
	TypeAlarmSensor,	// 7 ���ش���������
	TypeMotionDet,		// 8 �ƶ����
	TypeAlarmIPCEXT,	// 9 IPC�ⲿ����
	*/
//	int ret;
#if 1
	memset(&head, 0, sizeof(head));
	head.nChn = chn;
	head.nStreamType = 2;//Snap
	head.nBitRate = pChnData[chn].ChnReqTypeMask & (0x3f<<TypePreSnap);//�����и� 6λ�����Ͳ���Ҫ����
	memcpy(&head.nPts, &tv, sizeof(tv));
	head.nDate = pSnapData;
	head.nDataLength = SnapSize;
	head.width = width;
	head.height = height;

	//printf("%s write snap, chn%d, type: 0x%x, size: %d, time: %d\n", 
	//		__func__, chn, head.nBitRate, SnapSize, tv.tv_sec);
	
	if (head.nBitRate)
	{
		ret = RecordWriteOneSnap(chn, &head);
		if (ret)
		{
			printf("%s chn%d RecordWriteOneSnap 2 failed, ret: %d\n", __func__, chn, ret);
			//return 1;
		}
	}
#endif
	pChnData[chn].TimerWorking = FALSE;
	
#else
	//sleep(10);
#endif
	return ;
}

//�Ǳ�����������ץͼ
int CSnapManager::requestSnap(u8 chn, EM_SNAP_TYPE type, const char *PoliceID, const char *pswd)
{
	if (!m_Started)
	{
		return 0;
	}

	//printf("%s yg 2 chn%d type: %d\n", __func__, chn, type);
	
	if (type >= TypeSnapMax)
	{
		printf("%s chn%d type: %d invalid\n", __func__, chn, type);
		return 1;
	}
	
	if (chn < 0 || chn > m_MaxIpcChn)
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return 1;
	}
	
	CGuard guard(pChnMutex[chn]);	

	if (TypeManualACCEPT == type)
	{
		if (strlen(PoliceID) +1 > TEXTLEN)
		{
			strncpy(this->PoliceID, PoliceID, TEXTLEN-1);
			this->PoliceID[TEXTLEN-1] = '\0';
		}
		else
		{
			strcpy(this->PoliceID, PoliceID);
		}

		if (strlen(pswd) +1 > TEXTLEN)
		{
			strncpy(PassWord, pswd, TEXTLEN-1);
			PassWord[TEXTLEN-1] = '\0';
		}
		else
		{
			strcpy(PassWord, pswd);
		}
	}

	p_ChnReqTypeMask[chn] |= 1<<type;
	
	return 0;
}

//chn: ָʾ��һ·����Դ(sensor/IPCMD/IPCEXT)
int CSnapManager::alarmStatusChange(u8 chn, EM_ALARM_TYPE type, int status)
{
	SSG_MSG_TYPE msg;

	if (!m_Started)
	{
		return 0;
	}

	//printf("%s yg 2 chn%d type: %d\n", __func__, chn, type);
	
	if (type >= TypeAlarmMax)
	{
		printf("%s chn%d type: %d invalid\n", __func__, chn, type);
		return 1;
	}
	
	if (chn < 0 || chn > m_MaxIpcChn)
	{
		printf("%s chn%d invalid\n", __func__, chn);
		return 1;
	}	
	
	switch (type)
	{
		case TypeSensor:
		{			
			if (status)
			{
				if (p_SensorStatus[chn] == EM_ALARM_NONE)
				{
					printf("%s sensor%d trigger\n", __func__, chn);
					p_SensorStatus[chn] = EM_ALARM_TIGGER;	//��������	

					
					
				//�����������ȷ��Ͳ�����ͼƬ�ı�����Ϣ
					//���ɱ���GUID��һ�鱨����Ϣ������GUID������ͬ
					memset(&msg, 0, sizeof(SSG_MSG_TYPE));
					msg.type = EM_DVR_ALARM_EXT;// 26 DVR�ⲿ����
					msg.chn = chn+1;
					strcpy(msg.note, GetParsedString("&CfgPtn.LocalAlarm"));
					upload_sg_proc(&msg, 0, NULL, 0, 0);
				
					srand(time(NULL));
					do {
						p_GUID_Sensor[chn] = rand();
					} while(p_GUID_Sensor[chn] == 0);

					memset(&msg, 0, sizeof(SSG_MSG_TYPE));
					msg.type = EM_PIC_ALARM_LINK_UPLOAD;// 1 ��������ͼ��
					msg.chn = chn+1;
					//����chn����������ͼ���ϴ�
					sprintf(msg.note, "%s%02d%s, %s", \
						GetParsedString("&CfgPtn.Local"),
						chn+1,
						GetParsedString("&CfgPtn.Alarm1"),
						GetParsedString("&CfgPtn.LinkPicUpload")
						);
					upload_sg_proc(&msg, 0, NULL, 0, p_GUID_Sensor[chn]);
					
				}
			}
			else
			{
				if ((p_SensorStatus[chn] == EM_ALARM_TIGGER) \
					|| (p_SensorStatus[chn] == EM_ALARM_ING))
				{
					printf("%s sensor%d over\n", __func__, chn);
					p_SensorStatus[chn] = EM_ALARM_END;	//�������
				}
			}

		} break;
		case TypeMD:
		{
			if (status)
			{
				if (p_IPCMDStatus[chn] == EM_ALARM_NONE)
				{
					p_IPCMDStatus[chn] = EM_ALARM_TIGGER;	//��������

				//�����������ȷ��Ͳ�����ͼƬ�ı�����Ϣ
					//���ɱ���GUID��һ�鱨����Ϣ������GUID������ͬ
					srand(time(NULL));
					do {
						p_GUID_IPCMD[chn] = rand();
					} while(p_GUID_IPCMD[chn] == 0);

					memset(&msg, 0, sizeof(SSG_MSG_TYPE));
					msg.type = EM_PIC_ALARM_LINK_UPLOAD;// 1 ��������ͼ��
					msg.chn = chn+1;
					//IPC chn�ƶ���⣬����ͼ���ϴ�
					sprintf(msg.note, "IPC%02d%s, %s", \
						chn+1,
						GetParsedString("&CfgPtn.Motion"),
						GetParsedString("&CfgPtn.LinkPicUpload")
						);
					upload_sg_proc(&msg, 0, NULL, 0, p_GUID_IPCMD[chn]);
				}
			}
			else
			{
				if ((p_IPCMDStatus[chn] == EM_ALARM_TIGGER) \
					|| (p_IPCMDStatus[chn] == EM_ALARM_ING))
				{
					p_IPCMDStatus[chn] = EM_ALARM_END;	//�������
				}
			}

		} break;
		case TypeIPCEXT:
		{
			if (status)
			{
				if (p_IPCEXTStatus[chn] == EM_ALARM_NONE)
				{
					p_IPCEXTStatus[chn] = EM_ALARM_TIGGER;	//��������
					
				//�����������ȷ��Ͳ�����ͼƬ�ı�����Ϣ
					//���ɱ���GUID��һ�鱨����Ϣ������GUID������ͬ
					srand(time(NULL));
					do {
						p_GUID_IPCEXT[chn] = rand();
					} while(p_GUID_IPCEXT[chn] == 0);

					memset(&msg, 0, sizeof(SSG_MSG_TYPE));
					msg.type = EM_PIC_ALARM_LINK_UPLOAD;// 1 ��������ͼ��
					msg.chn = chn+1;
					//IPC chn����������ͼ���ϴ�
					sprintf(msg.note, "IPC%02d%s, %s", \
						chn+1,
						GetParsedString("&CfgPtn.Alarm1"),
						GetParsedString("&CfgPtn.LinkPicUpload")
						);
					upload_sg_proc(&msg, 0, NULL, 0, p_GUID_IPCEXT[chn]);
				}
			}
			else
			{
				if ((p_IPCEXTStatus[chn] == EM_ALARM_TIGGER) \
					|| (p_IPCEXTStatus[chn] == EM_ALARM_ING))
				{
					p_IPCEXTStatus[chn] = EM_ALARM_END;	//�������
				}
			}

		} break;
		default:
		{
			printf("%s type: %d invalid\n", __func__, type);
			return 1;
		}
	}
	
	return 0;
}

int CSnapManager::SnapWriteMsgQueue(EM_SNAPMSG_TYPE msg)
{
	int ret = 0;
	
	ret = WriteDataToBuf(&SnapMsgQueue, (u8 *)&msg, sizeof(EM_SNAPMSG_TYPE));
	if (ret)
	{
		printf("%s WriteDataToBuf failed\n", __func__);
	}
	
	return ret;
}

int CSnapManager::SnapReadMsgQueue(EM_SNAPMSG_TYPE *pmsg)
{
	return ReadDataFromBuf(&SnapMsgQueue, (u8 *)pmsg, sizeof(EM_SNAPMSG_TYPE));
}


/*************  Export API  *********************/

//�����ʼ��������
int SnapServerStart(u8 MaxSensorNum, u8 MaxIpcChn)
{	
	//����Ӳ�̣���ʱ�ر�Ԥ¼20150527
	//return 0;
	
	return g_SnapManager.Start(MaxSensorNum, MaxIpcChn);
}

//�Ǳ�����������ץͼ
int RequestSnap(u8 chn, EM_SNAP_TYPE type, const char *PoliceID, const char *pswd)
{
	return g_SnapManager.requestSnap(chn, type, PoliceID, pswd);		
}

//�ⲿ��֪����״̬(�����ͽ���)��ģ���ڲ�����ץͼ
//type : �ƶ���⡢����������IPC�ⲿ����
//ÿ�����Ͷ�Ҫ�������ʾ�����߼����������ĸ�IPCͨ��ȡͼ
int AlarmStatusChange(u8 chn, EM_ALARM_TYPE type, int status)
{	
	return g_SnapManager.alarmStatusChange(chn, type, status);	
}


//yaogang modify 2015715
int BizSnapPause(void)
{
	return g_SnapManager.SnapWriteMsgQueue(TypePause);
}

int BizSnapResume(void)
{
	return g_SnapManager.SnapWriteMsgQueue(TypeResume);
}


