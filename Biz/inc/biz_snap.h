#ifndef _BIZ_SNAP_H_
#define _BIZ_SNAP_H_

typedef enum
{
	TypeSensor = 0,	// 0
	TypeMD,			// 1
	TypeIPCEXT,		// 2
	TypeAlarmMax,
} EM_ALARM_TYPE;

typedef enum
{
	EM_ALARM_NONE = 0,	// 0 �ޱ�������ʼ״̬���������������״̬
	EM_ALARM_TIGGER,	// 1 ��������
	EM_ALARM_ING,		// 2 ������
	EM_ALARM_END,		// 3 �������
	EM_ALARM_STILL,	// 4	�����ӳ���¼
	EM_ALARM_MAX,
} EM_ALARM_STATUS;


/* recordģ����ͼƬ��������
ץͼ����	bit_mask	value
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

//��ģ����ͼƬ����
typedef enum
{
//upload
	TypeManualTEST,		// 0 ����ͼ���ϴ�--���ƽ̨--���Ա���
	TypeManualMAINTAIN,	// 1 ����ͼ���ϴ�
	TypeManualACCEPT,	// 2 ���չ����ϴ�
	TypeDaily,			// 3 �ճ�ͼƬ�ϴ�
	TypeSvrReq,			// 4 �����������ϴ�ͨ��ͼƬ
//record
	TypePreSnap,			// 5 Ԥ¼
	TypeTime,			// 6 ��ʱ-¼������--ץͼ����
	TypeManual,			// 7 �ֶ���ͼ
//upload & record
	TypeAlarmSensor,		// 8 ���ش���������
	TypeMotionDet,		// 9 �ƶ����
	TypeAlarmIPCEXT,		// 10 IPC�ⲿ����
	TypeSnapMax,
} EM_SNAP_TYPE;


#ifdef __cplusplus
extern "C" {
#endif

//�����ʼ��������
int SnapServerStart(u8 MaxSensorNum, u8 MaxIpcChn);

//�Ǳ�����������ץͼ
int RequestSnap(u8 chn, EM_SNAP_TYPE type, const char *PoliceID, const char *pswd);


//�ⲿ��֪����״̬(�����ͽ���)��ģ���ڲ�����ץͼ
//type : �ƶ���⡢����������IPC�ⲿ����
int AlarmStatusChange(u8 chn, EM_ALARM_TYPE type, int status);

int BizSnapPause(void);
int BizSnapResume(void);



#ifdef __cplusplus
}
#endif

#endif

