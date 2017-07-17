

#ifndef __SYSTEMM_API_H__
#define __SYSTEMM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup SystemAPI API System
/// ϵͳʱ�����ã��ػ���������ϵͳ��Դ״����������API��
/// @{

/// ϵͳʱ��ṹ
typedef struct SYSTEM_TIME{
    int  year;///< �ꡣ   
	int  month;///< �£�January = 1, February = 2, and so on.   
	int  day;///< �ա�   
	int  wday;///< ���ڣ�Sunday = 0, Monday = 1, and so on   
	int  hour;///< ʱ��   
	int  minute;///< �֡�   
	int  second;///< �롣   
	int  isdst;///< ����ʱ��ʶ��   
}SYSTEM_TIME;

/// ϵͳ���Խṹ
typedef struct SYSTEM_CAPS
{
	/// �ܹ��ڴ��С����Kilo-BytesΪ��λ��
	uint MemoryTotal;

	/// ʣ���ڴ��С����Kilo-BytesΪ��λ������ϵ��Ӧ�ó�����ڴ�ʹ�ò��ԡ�
	uint MemoryLeft;
}SYSTEM_CAPS;

/// �����汾����
/*
typedef struct SYSTEM_CAPS_EX
{
	uint ForNRT;			///< �Ƿ�Ϊ��ʵʱ
	BYTE ForATM;			///< �Ƿ�ΪATM��
	BYTE HasAudioBoard;		///< ����Ƶ��
	BYTE HasLoopBoard;		///< ����ͨ��
	BYTE HasMatrixBoard;	///< �������
}SYSTEM_CAPS_EX;
*/
typedef struct tagSYSTEM_CAPS_EX
{
 uint ForNRT;   //�Ƿ�Ϊ��ʵʱ, ��λ��ʾ����ͨ���Ƿ���Ϊ��ʵʱ.��0xffff��ʾǰ16·���Ƿ�ʵʱ.
 uchar ForATM;   //�Ƿ�ΪATM��, 1��ʾ����Һ��ATM,2��ʾ��Һ��ATM
 uchar HasAudioBoard;  //����Ƶ��
 uchar HasLoopBoard;  //����ͨ��
 uchar HasMatrixBoard; //�������
 uchar HasPtzBoard;           //����̨��
 uchar HasWlanBoard;        //�����߰� 
 uchar reserved[2];
}SYSTEM_CAPS_EX;

/// ϵͳ����״̬
enum system_upgrade_state
{
	SYSTEM_UPGRADE_OVER = 0,
	SYSTEM_UPGRADE_INIT = 1,
	SYSTEM_UPGRADE_RUNNING = 2
};

///ϵͳ�ָ���������
typedef enum CONFIG_RESUME_E
{
	CONFIG_RESUME_YES,
	CONFIG_RESUME_NO,
	CONFIG_RESUME_INVALID
}CONFIG_RESUME_OPR;

/// �õ���ǰϵͳʱ�䡣����������ܵ��õĺ�Ƶ��, ���е���ʱӦ���Ǵ��ڴ���ȡ��
/// ʱ��,������ֱ�Ӵ�Ӳ��, �ڶ�ʱ��������ڴ��е�ֵ.
/// 
/// \param [out] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemGetCurrentTime(SYSTEM_TIME * pTime);


/// ���õ�ǰϵͳʱ�䡣
/// 
/// \param [in] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemSetCurrentTime(SYSTEM_TIME * pTime);
//int SystemGetTimeSecond(int *pTime);


/// �õ�ϵͳ�ĺ�����������ε���֮��ļ��ʱ����ǵõ��ļ����Ĳ�ֵ��
/// 
/// \param ��
/// \return ��ǰ������
/// \note һ������£�49.7���õ������������ʹ�õ��ĵط�Ҫ���������
uint SystemGetMSCount(void);

/// �õ�ϵͳ�ĺ�����������ε���֮��ļ��ʱ����ǵõ��ļ����Ĳ�ֵ��
/// 
/// \param ��
/// \return ��ǰ������
/// \note һ������£�497���õ������������ʹ�õ��ĵط�Ҫ���������
uint64 SystemGetMSCount64(void);



/// �õ�ϵͳ��΢�����, �˺�����Ҫ����ģ�����ܵĲ��ԡ�����ĸ��졣
/// 
/// \param ��
/// \return ��ǰ������
uint SystemGetUSCount(void);


/// �õ�CPUռ���ʡ�
/// 
/// \param ��
/// \return CPUռ�ðٷֱȡ�
int SystemGetCPUUsage(void);


/// �õ�ϵͳ���ԡ�
/// 
/// \param [out] pCaps ָ��ϵͳ���ԽṹSYSTEM_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int SystemGetCaps(SYSTEM_CAPS * pCaps);



/// 
int SystemGetCapsEx(SYSTEM_CAPS_EX * pCaps);


/// ����ǰ�̣߳�ʱ�䵽���ٻ��ѡ�
/// 
/// \param [in] dwMilliSeconds �����ʱ�䡣
void SystemSleep(uint dwMilliSeconds);


/// 
void SystemUSleep(uint dwMicroSeconds);


/// ��ϵͳ��������������������������ͬ��ģʽ��ʱ�䵽�ŷ��ء�
/// 
/// \param [in] dwFrequence ����Ƶ�ʡ�
/// \param [in] dwDuration ������ʱ�䡣
void SystemBeep(uint dwFrequence, uint dwDuration );

// �����ӿ�
// ��isSilenceΪ1ʱ����ʾϵͳ��Ҫ�������ر�������IO�ڣ�
// ��isSilenceΪ0ʱ����ʾϵͳ��Ҫȡ����������������IO�ڣ�
void SystemSilence(int isSilence);

/// ϵͳ��λ
//void SystemReboot(void);

/// ϵͳ�ر�
void SystemShutdown(void);

/// Ӧ�ó����յ�����������ϵͳ��ѹ����ͬ������ģʽ��ֱ����ѹ��ɲŷ��ء�
/// 
/// \param [in] pData ���ݰ����ڴ��е�ָ�롣
/// \param [in] dwLength ���ݳ��ȡ�
/// \param [out] pProgress ��ǰ���Ȱٷֱȣ��������ù���ʱ�ᱻ��ʱ���£�Ӧ�ó���
///        ��Ӧ���޸����ֵ��
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
/// \note ��ͬ��ϵͳ���ݴ���ĸ�ʽ��ͬ����Ӧ��������ݴ���ͽ�ѹ�ĵĳ���Ҳ��ͬ��
int SystemUpgrade(uchar* pData, uint dwLength, uint *pProgress);

//������Ƭ����IPC��Ƭ�������ӿ�
int SystemUpgradeC8051(char *pData, unsigned int dwLength, unsigned int *pProgress);

/*Begin: Add by chenjianqun 20061024 for New Upgrade method*/
int SystemUpgradeEx(void *iov, uint *pProgress);
//int SystemUpgradeEx(struct iovec *iov, uint *pProgress);

/*End: Add by chenjianqun 20061024 for New Upgrade method*/

/// ϵͳ��־��ʼ����ϵͳ��־������¼Ӧ�ó�������ʱ���еı�׼�����
/// 
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemLogInit(void);


/// ��¼ϵͳ��־�� ͬʱ������д������׼������塣
/// 
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemLogWrite(char * format, ...);


/// ��ȡϵͳ���к�
int SystemGetSerialNumber(char *pData, int len);


/// ��ȡϵͳӲ���汾
uint SystemGetHWVersion(void);


/// ��ȡ�豸�����豸������һ���ַ���������С��8
///�ַ��������豸��ͬ�ֱ�Ϊ��"IPCF4x5"��"IPCA6"��"IPCA8"��"IPCA7"��"IPVM"��"IPSD"
int SystemGetDeviceType(char* type); 

/// ����u-boot��������
/// ��: ����N��P��, SystemSetUbootParam("videomode", "NTSC"),SystemSetUbootParam("videomode", "PAL")
//int SystemSetUbootParam(const char *para, const char *value);

/// ��ȡ�汾��, ��ʽ����: r9508:2.15:12:13:14:16, ��󳤶�128�ֽ�
int SystemGetVersion(char *version);

/// ��ȡUBOOT�����м�⵽��config_resume ״̬�������ϲ�����Ƿ���Ҫ�ָ�Ĭ������
CONFIG_RESUME_OPR SystemGetConfigResume(void);

// �������ԣ���ȡ����
enum ui_language_t {
UI_LANGUAGE_ENGLISH = 0,                // Ӣ�� 
UI_LANGUAGE_CHINESE_SIMPLE =1,          // ���� 
UI_LANGUAGE_CHINESE_TRADITIONAL = 2 , //��������   
UI_LANGUAGE_ITALIAN  = 3 ,             //�������    
UI_LANGUAGE_SPANISH  = 4 ,             //��������    
UI_LANGUAGE_JAPANESE  = 5 ,             //����   
UI_LANGUAGE_RUSSIAN  = 6 ,             //����  
UI_LANGUAGE_FRENCH = 7 ,             //���� 
UI_LANGUAGE_GERMAN = 8 ,             //���� 
UI_LANGUAGE_PORTUGUESE = 9 ,         //������ 
UI_LANGUAGE_TURKISH  = 10,             //������   
UI_LANGUAGE_POLISH = 11,             //������ 
UI_LANGUAGE_ROMANIAN  = 12,             //��������    
UI_LANGUAGE_HUNGARIAN  = 13,            //�������� 
UI_LANGUAGE_FINNISH  = 14,             //������ 
UI_LANGUAGE_ESTONIAN  = 15,             //��ɳ������  
UI_LANGUAGE_KOREAN = 16,             //���� 
UI_LANGUAGE_FARSI  = 17,                //��˹�� 
UI_LANGUAGE_DANISH = 18,             //������ 
UI_LANGUAGE_BULGARIAN  = 19,            //�������� 
UI_LANGUAGE_ARABIC = 20,             //�������� 
UI_LANGUAGE_CZECH  = 21,                //�ݿ� 
UI_LANGUAGE_SLOVAK = 22,             //˹�工�� 
UI_LANGUAGE_INVALID = 255

};
enum ui_language_t SystemGetUILanguage(void);
int SystemSetUILanguage(enum ui_language_t lang);
		     

/// wyt: Ч��������ڵײ������ʵ�֣����ϲ���ȫ͸�����ϲ㲻���κ����顣2009-7-15
/// �û���������Ч������, �ɹ���0
//int SystemCheckAT88_1(void);
//int SystemCheckAT88_2(void);

/// @} end of group



#define	SYS_VGAPIXEL_ERR				-1
#define	SYS_VGAPIXEL_800x600			0		// 800 x 600
#define	SYS_VGAPIXEL_1024x768			1		// 1024 x 768



int SystemGetVGAPixelCaps(int *pCaps);
int SystemGetVGAPixel();
int SystemSetVGAPixel(int vgapixel);


#ifdef __cplusplus
}
#endif

#endif //__SYSTEMM_API_H__

