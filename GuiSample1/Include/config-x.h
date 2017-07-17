


#ifndef __config_x_h__
#define __config_x_h__

#ifdef WIN32
	#define PRTSC_DIR "Win32/PrtSc"		//ץͼ�ļ�Ŀ¼
	#define CONFIG_DIR "Win32/Config"		//�����ļ�Ŀ¼
	#define LOG_DIR "Win32/Log"			//��־�ļ�Ŀ¼
	#define PROFILE_DIR "Common/Lua"		//Lua�ļ�Ŀ¼
	#define FONT_BIN "Common/FontBin"	//��������Ĺ���
	#define DATA_DIR "Win32/Pics"  //ͼƬĿ¼
	#define FONT_DIR "Win32/Log" //����
	#define	FONT_DEFAULT_DIR "Win32/Strings" //����
	#define LOGO_DIR "Win32/Logo"	//LogoĿ¼
	#define WEB_DIR	"Win32/Web"		//WebĿ¼
//#elif defined(DVR_HI)
#else
// 	#define PRTSC_DIR ""
// 	#define CONFIG_DIR "/mnt/mtd/Config"
// 	#define LOG_DIR "/mnt/mtd/Log"
// 	#define PROFILE_DIR "/usr/bin/lua"
// 	#define DATA_DIR "/usr/data/Data"
// 	#define FONT_BIN "/usr/data/Data"
// 	#define	FONT_DIR "/mnt/custom" //����
// 	#define FONT_DEFAULT_DIR "/usr/data/Data"	//����
// 	#define LOGO_DIR "/mnt/logo"	//LogoĿ¼
// 	#define WEB_DIR "/mnt/web"
	#define PRTSC_DIR ""
	#define CONFIG_DIR "/tmp/ui/mtd/Config"
	#define LOG_DIR "/tmp/ui/mtd/Log"
	#define PROFILE_DIR "/tmp/ui/bin/lua"
	#define DATA_DIR "/tmp/ui/data/Data"
	#define FONT_BIN "/tmp/ui/data/Data"
	#define	FONT_DIR "/tmp/ui/data/Data" //����
	#define FONT_DEFAULT_DIR "/tmp/ui/data/Data"	//����
	#define LOGO_DIR "/tmp/ui/logo"	//LogoĿ¼
	#define WEB_DIR "/tmp/ui/web"
#endif

#ifdef _lint
#define SHBELL
#endif

/*!< ģ��� */

/*! <GUIģ�� */
//#define GRAPHICS_SUPPORT_SETRESOLUTION 1	// �������÷ֱ���

/*!< ����ģ�� */
//#define BAK_EJECT_DISK			1 // ������Ƭ����
//#define BAK_BURN_DISK				1 //ʵʱ��¼����
//#define BAK_SUPPORT_CHANNEL		2 //֧�ֿ�¼��¼��ͨ����
//#define BAK_SUPPORT_DIRECTORY		1 //֧�ֶ༶Ŀ¼����

/*!< ��̨��� */
//#define PTZ_SUPPORT_BANKNOTE	//����㳮���ĺ� //#define PTZ_BANKNOTE					//ͨ��485ʵ��һ̨�㳮�����ܣ�֮ǰ�����COM_SUPPORT_GUAO��
//#define PTZ_RW_SEMIDUPLEX  //485��˫��ͨ�� //#define PTZ_SEMIDUPLEX				//485��˫��ͨ��
//#define PTZ_CONTROL_IRIS_DEEPLY   //�������Ȧ�޷���������  //#define IRISNOTCTRL					//�������Ȧ�޷���������
//#define PTZ_SUPPORT_ANNUNCIATOR	//������������������ //#define PTZ_ALARM_SUPPORTED			//������������������
//#define PTZ_WATCH_NIGHT //ҹ����Ȩ�޿��� //#define USE_NIGHTVISION        	 //ҹ����Ȩ�޿���

/*!< ALARM */
//#define ALM_SUPPORT_SNAPSHOT			//ʹ�ñ�������ץͼ��ȡ����

/*!< ����ģ�� */
//#define ENC_SHOW_SMALL_FONT 1 //����С����

//#define ENC_CHANGE_PARAM 		//֧�ֶ���ͱ����Բ�ͬ�ķֱ��ʻ�֡��¼�� 1:���ý��� 2�Զ�����

//#define ENC_SUPPORT_SNAPSHOT //֧��ץͼ����
//#define ENC_ENCODE_MULTI 1

//#define ENC_SUB_COMPRESS // 128:-H264 1:-MPEG4
//#define ENC_ADD_MP4_COMPRESS //128:MPEG4+H264

//#define ENC_SUB_IMG //1:-D1 2:-HD1 3:-D1-HD1 4:-BCIF 5:-D1-BCIF 6:-HD1-BCIF 7:-D1-HD1-BCIF 8:-CIF
//#define ENC_ADD_CIF_IMG  //1:CIF+D1 2:CIF+HD1 3:CIF+D1+HD1 4:CIF+BCIF 5:CIF+D1+BCIF 6:CIF+HD1+BCIF 7:CIF+D1+HD1+BCIF
//#define ENC_ADD_BCIF_IMG //1:BCIF+D1 2:BCIF+HD1 3:BCIF+D1+HD1
//#define ENC_ADD_HD1_IMG //1:HD1+D1

//! ץͼģ��
//#define SNAP_REQUIRE_ISTC //ISTCץͼ����
//#define SNAP_REQUIRE_JQBAR //��������ץͼ
//#define	SNAP_REQUIRE_SHCL	//�Ϻ�����ץͼ

/// ¼��ģ��
//#define	REC_SUPPORT_HEAD  //¼���ļ��ﺬ64K��ͷ��Ϣ,����¼����Ҫ������꣬��Ҳû������


//#define MON_PROTECT_SCREEN //��Ļ������
//#define  MON_SUPPORT_SPLIT6 //�������֧�֣�1����С
//����������̵���ѵ��
//#define MON_NETKEY_TOUR
//#define MON_OPEN_TVMONITORBYPOWER //Һ���ص���ֻ�а���Դ�����ܻ���
/*!< �ط�ģ�� */
//#define FUNC_SYN4_PLAY 1

/*��Ҫ���ڴ����򿪺꣬
��Ҫ�ڱ�������ӣ�
linux����makefile�win32 ������������*/

//#define FTP_TRAN_SNAPSHOT //FTP����ͼƬ,�򿪴˺�ʱ,�����ENC_SUPPORT_SNAPSHOT ��

//#define FUNC_AUTOTEST_TVVGA //1500�Զ�����л�TV/VGA�л�

//#define FUNC_MONITOR_RECORD

//#define  LOG_SUPPORT_PLAYBACK
//#define FUNC_SUPPORT_ALM_BACKTO_SPLIT16 //�����ӳٽ����󵯻�16����
//#define AUTO_MAINTAIN_TUS_TWO //Ĭ��ÿ�ܶ��賿�����Զ�ά������
//#define FUNC_SHOW_TZTIP 1  //��ʾNTP������ʱ��ѡ�����ʾ
//#define FUNCTION_SUPPORT_BLIND_LATCHDELAY	//֧���ڵ����ȥ��������,Ŀǰ��ATM��ʹ��
//#define MOTION_LATCHDELAY_NOSHOW //֧����Ƶ���ȥ���������Ƿ���,Ŀǰ��ATM��ʹ��
//#define MON_ADDLOGOUT_FOR_LOGINUSR 1//�û���½���Ҽ��˵�������ע��ѡ�����Ĭ���û����Ҽ��˵�����ʾ��ѡ��
#endif //__config_x_h__

