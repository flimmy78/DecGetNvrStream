#ifndef __SG_PLATFORM_H_
#define __SG_PLATFORM_H_
//yaogang modify 20141209 ���ƽ̨

#include "common_basetypes.h"
#include "hddcmd.h"
#include "diskmanage.h"
#include "Cmdcallback.h"
#include "netcomm.h"

#ifdef __cplusplus
extern "C" {
#endif

//��Ϣ����
typedef enum
{
	EM_TYPE_MIN = 0,
	EM_PIC_ALARM_LINK_UPLOAD = 1,	// 1 ��������ͼ��
	EM_PIC_MAINTAIN_UPLOAD,	// 2 �����ϴ�ͼ��
	EM_PIC_DAILY_UPLOAD,	// 3 �ճ��ϴ�ͼ��
	EM_PIC_TEST_UPLOAD,	// 4 �����ϴ�ͼ��
	EM_PIC_SERVER_GET,		// 5 ��������ͼ�� 1
	EM_SYS_Other = 11,		// 11 ϵͳ��������
	EM_Perimeter_Fault,		// 12 �ܽ籨������
	EM_Networking_Fault,		// 13 ������������
	EM_LocalAlarm_Fault,		// 14 ���ر�������
	EM_VideoMonitor_Fault,		// 15 ��Ƶ��ع���
	EM_BuildingIntercom_Fault,	// 16 ¥��Խ�����
	EM_AccessControl_Fault,	// 17 ������ƹ���
	EM_ElectronicPatrol_Fault,	// 18 ����Ѳ������
	EM_DVR_STARTUP,		// 19 DVRϵͳ����
	EM_DVR_EXIT_NORMOAL,	// 20 DVRϵͳ�˳�
	EM_DVR_EXIT_ABNORMAL,	// 21 DVR�쳣�˳�
	EM_DVR_PARAM_SET,		// 22 DVR��������
	EM_DVR_PARAM_SAVE,	// 23 DVR��������
	EM_DVR_VLOST,			// 24 DVR��Ƶ��ʧ
	EM_DVR_VMOTION,		// 25 DVR�ƶ����
	EM_DVR_ALARM_EXT,		// 26 DVR�ⲿ����
	EM_DVR_ALARM_RESUME,	// 27 ϵͳ�������
	EM_DVR_EXIT_ILLEGALITY,	// 28 DVR�Ƿ��˳�
	EM_REPAIR_CHECK,		// 29 ϵͳά��ǩ��
	EM_MAINTAIN_CHECK,		// 30 ϵͳά��ǩ��
	EM_DVR_PLAYBACK,		// 31 DVR���ػط�
	EM_REMOTE_PLAYBACK,	// 32 DVRԶ�̻ط�
	EM_PIC_ACCEPT_UPLOAD,	// 33 �����ϴ�ͼ��
	EM_DVR_MOTION,			// 34 ƫλ���� 1
	EM_PIC_ALARM_UPLOAD_UNREC = 36,	// 36 �����ϴ�ͼ��δ¼����ƶ���⣩
	EM_PIC_MAINTAIN_UPLOAD_UNREC,// 37 �����ϴ�ͼ��δ¼����ƶ���⣩
	EM_PIC_DAILY_UPLOAD_UNREC,	// 38 �ճ��ϴ�ͼ�񣨵���δ¼��
	EM_PIC_TEST_UPLOAD_UNREC,		// 39 �����ϴ�ͼ��δ¼����ƶ���⣩
	EM_PIC_ACCEPT_UPLOAD_UNREC,	// 40 �����ϴ�ͼ��δ¼����ƶ���⣩
	EM_DVR_HDD_ERR,	// 41 DVR���̴���
	EM_HEART_TIMEOUT,	// 42 ϵͳ������ʱ
	EM_HEART_RESUME,	// 43 ϵͳ�����ָ�
	EM_VEDIO_OTHER,	// 44��Ƶ�����¼�	
	EM_HEART,			// ��������SGƽ̨��Ҫ���ͣ����ڱ�ϵͳ
	EM_TYPE_MAX,
}EM_MSG_TYPE;

typedef enum
{
	EM_SG_ALARM_SENSOR, 		//����������
	EM_SG_ALARM_VMOTION, 		//�ƶ����
	EM_SG_ALARM_VLOSS, 		//��Ƶ��ʧ
	EM_SG_ALARM_IPCCOVER,		//IPC�ڵ�
	EM_SG_ALARM_IPCEXT,		//IPC�ⲿ������
	EM_SG_ALARM_DISK_LOST,	//Ӳ�̶�ʧ
	EM_SG_ALARM_DISK_WRERR,		//Ӳ�̶�д����
	EM_SG_ALARM_DISK_NONE,	//������Ӳ��
	EM_SG_ALARM_485EXT,		//485����
} EM_ALM_TYPE;
/*
typedef struct
{
	EM_ALM_TYPE type;
	int status;
} ALM_INFO_TYPE;
//����
typedef union
{
	ALM_INFO_TYPE alm_info;//����ͨ��
	char note[64];		//��Ӧ��SG�ϱ���Ϣ�е�Note
} DetailsType;
*/
typedef struct
{
	EM_MSG_TYPE	type;
	u32 chn;		//�ϴ�ͼ��ͨ��
	//ALM_INFO_TYPE alm_info;//����ͨ��
	char note[64];		//��Ӧ��SG�ϱ���Ϣ�е�Note
	//DetailsType details;
} SSG_MSG_TYPE;

s32 net_sg_init(PSNetCommCfg pCfg);
void upload_sg(SSG_MSG_TYPE *pmsg);
char *base64_encode_v1(const char* data, int data_len);
char *base64_decode_v1(const char *data, int data_len) ;
//ʱ�������Ϊ����������ָ������ΪҪ�ϴ�֮ǰ��Ԥ¼ͼƬ
s32 upload_sg_proc(SSG_MSG_TYPE *pmsg, time_t time, void *SnapData, unsigned int DataSize, int GUID);



#ifdef __cplusplus
}
#endif

#endif // __NETCOMM_H_

