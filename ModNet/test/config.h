#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "common.h"
#include "camera.h"
#include "ctrlprotocol.h"
#ifdef CSVISION
#include "vsip.h"
#endif

//#define CAP_BUF_LEN (256 << 10)
#define CAP_BUF_LEN (512 << 10)

//#define CAPTURE_DEMO

#ifdef CAPTURE_DEMO

#define CAP_SRC_FILE "video_src.264"
#define CIF_SRC_FILE "video_cif.264"

#define DEMO_MAX_CHN_NUM MAX_CHN_NUM

#define NALU_TYPE_SLICE 1
#define NALU_TYPE_DPA 2
#define NALU_TYPE_DPB 3
#define NALU_TYPE_DPC 4
#define NALU_TYPE_IDR 5
#define NALU_TYPE_SEI 6
#define NALU_TYPE_SPS 7
#define NALU_TYPE_PPS 8
#define NALU_TYPE_PD 9
#define NALU_TYPE_EOSEQ 10
#define NALU_TYPE_EOSTREAM 11
#define NALU_TYPE_FILL 12

FILE *open_cap_demo_device(void);
FILE *open_cif_demo_device(void);
int h264_check_head(const u8 *, u32);
int h264_next_frame(const u8 *, int, u32, int *);
int get_frame_type(u8);

#endif

//#define APP_VERSION			"V_0_6_1"	//3511

#define MAX_RECCHN_NUM		MAX_CHN_NUM
#define MAX_ENCODER_NUM		5				//06-12   �����ı�������
#define MAX_NATCHN_NUM		6				//km627

#define SEGMENTS_OF_DAY		4
#define DAYS_OF_WEEK		7

#define MAX_STRING_LENGTH	32

#define STREAM_VIDEO		0
#define STREAM_MULTI		1

#define IMAGE_Q_LEVEL_1		0
#define IMAGE_Q_LEVEL_2		1
#define IMAGE_Q_LEVEL_3		2
#define IMAGE_Q_LEVEL_4		3
#define IMAGE_Q_LEVEL_5		4
#define IMAGE_Q_LEVEL_6		5

//#define MAX_ALARM_IN_NUM	8 // 16			//fgpa:16 6446:4
//#define MAX_ALARM_OUT_NUM	2 // 4			//fpga:4 6446:4

//2007-09-28
#define MAX_EXCEPTION_NUM	6			//8

#define MAX_LOG_NUM			3
#define MAX_USER_NUM		8

#define MAX_LOG_SEARCH_NUM 	1024		//add on 07-09-22

/* added by kangmao macro definition */
#define LOCAL_PRIVILIGE_NUM				6
#define REMOTE_PRIVILIGE_NUM			9

//����������
#define MASTER_ALARM					0						//�����ͱ���
#define MASTER_LOCAL_OP					0x01					//�����ͱ��ز���
#define MASTER_REMOTE_OP				0x02					//������Զ�̲���							
#define MASTER_EXCEPTION				0x03					//�������쳣
#define MASTER_ALL                      0x04					//������ȫ��

//����������
#define SLAVE_ALARMIN_START				0						//�����ͱ������뿪ʼ
#define SLAVE_ALARMIN_END				0x01					//�����ͱ����������
#define SLAVE_MOTION_START				0x02					//�������ƶ���⿪ʼ
#define SLAVE_MOTION_OVER				0x03					//�������ƶ�������
#define SLAVE_ALARM_ALL					0x04					//������ȫ��

//�û�����������
//���ز���
#define SLAVE_LOCAL_POWERON				0						//������Ϊ����
#define SLAVE_LOCAL_POWEROFF			0x01					//������Ϊ�ػ�
#define SLAVE_LOCAL_LOGIN				0x02					//������Ϊ���ص�½
#define SLAVE_LOCAL_LOGOFF				0x03					//������Ϊ����ע��
#define SLAVE_LOCAL_CONFIG				0x04					//������Ϊ��������
#define SLAVE_LOCAL_REC_START			0x05					//������Ϊ��������¼��
#define SLAVE_LOCAL_REC_END				0x06					//������Ϊ����ֹͣ¼��
#define SLAVE_LOCAL_UPDATE				0x07					//������Ϊ��������
#define SLAVE_LOCAL_FORMATE				0x08					//������Ϊ���ظ�ʽ��
#define SLAVE_LOCAL_REBOOT				0x09					//����������
#define SLAVE_LOCAL_OP_ALL				0x0a					//������ȫ��

//�û�����������
//Զ�̲���
#define SLAVE_REMOTE_LOGIN				0						//������ΪԶ�̵�½
#define SLAVE_REMOTE_LOGOFF				0x01					//������ΪԶ��ע��
#define SLAVE_REMOTE_REC_START			0x02					//������ΪԶ������¼��
#define SLAVE_REMOTE_REC_END			0x03					//������ΪԶ��ֹͣ¼��
#define SLAVE_REMOTE_CONFIG				0x04					//������ΪԶ������
#define SLAVE_REMOTE_REBOOT				0x05					//������ΪԶ������
#define SLAVE_REMOTE_VOIP_START			0x06					//������ΪԶ�������Խ���ʼ
#define SLAVE_REMOTE_VOIP_END			0x07					//������ΪԶ�������Խ�����
#define SLAVE_REMOTE_UPDATE				0x08					//������ΪԶ������
#define SLAVE_REMOTE_POWEROFF				0x09					//�����͹ػ�
#define SLAVE_REMOTE_OP_ALL				0x0a					//������ȫ��

//�쳣������
#define SLAVE_SIGNAL_LOSS      			0						//�����������źŶ�ʧ
#define SLAVE_SHELTER_ALARM				0x01					//�������ڵ�����
#define SLAVE_HARDDISK_ERROR			0x02					//������Ӳ�̴���
#define SLAVE_HARDDISK_FLUSH			0x03					//������Ӳ����
//#define SLAVE_IP_CLASH                  0x04					//������IP��ͻ
#define SLAVE_ILLAGE_OP					0x04					//�����ͷǷ�����
#define SLAVE_EXCEPTION_ALL				0x05					//������ȫ��
/* 07-10-08 end */

//fpga��Ŀ��ʹ��eeprom���������ļ�,ʹ��flash
//#define DEV_EEPROM				"/dev/eeprom"
//#define DEV_EEPROM					"data/fpga.db"
#ifdef VERSION_OF_9508
#define DEV_EEPROM					"/mnt/flash/R9508.db"
#else
#define DEV_EEPROM					"/mnt/flash/R9008.db"
#endif

extern u8 bySuccFlag;//0:update;1:layout;2:resume

#pragma pack( push, 1 )

/*************************/
typedef struct
{
	int curpos;
	int totalpos;
}PACK_NO_PADDING ifly_update_progress_t;

extern ifly_update_progress_t tUpdate;

typedef struct
{
	u64 curpos;
	u64 totalpos;
	int cancel;
}PACK_NO_PADDING ifly_copy_progress_t;
/*************************/

typedef struct
{
	u8 channel_no;
	u8 path_no;
	u8 cruise_no;
	u8 preset_no;
	u8 dwell_time;
	u8 cruise_speed;
}PACK_NO_PADDING ifly_cruise_pos_t;

typedef struct
{
	ifly_cruise_pos_t cruise_pos[MAX_CRUISE_POS_NUM];
}PACK_NO_PADDING ifly_cruise_path_t;

typedef struct
{
	u8   channel_no;
	char name[32];/*ͨ����*/
	u8   code_type;/*������or��Ƶ��*/
	u8   video_resolution;/*�ֱ���*/
	u8   bit_type;/*λ������*/
	int  bit_max;/*λ������*/
	u8   quality;/*ͼ������*/
	u8   frame_rate;/*֡��*/
	u16  pre_record;/*Ԥ¼ʱ��*/
	u16  post_record;/*��ʱʱ��*/
	u8   flag_record;/*¼������־*/

	//wrchen 090225
	u8 	sub_bit_type;					//������������
	u8   sub_quality;
	u8 	sub_framerate;				//��������֡��
	u8   sub_video_resolution;/*�ֱ���*///wrchen 091105
	int 	sub_bitrate;//��������λ��
	/*
	#ifdef IECTRL_NEW
	u8	 supportdeinter;				//�Ƿ�֧��deinter���� 1�� 0�� (����)
	u8	 deinterval;	 				//deinterǿ�� 0-4 ���ã������У�ǿ����ǿ
	u8	 supportResolu;   				//�Ƿ�֧������¼��ֱ���
	#endif */

}PACK_NO_PADDING ifly_record_t;


typedef struct
{
	u8	substream_ctrl;
	u8   substream_frame_rate;/*������֡��*/ // wrchen 080624
	u8   substream_quality;
	int  substream_bit_max;/*������λ��*/ // wrchen 080624
}PACK_NO_PADDING ifly_substream_t;

typedef struct
{
	u8   channel_no;
	u8   flag_hand;
}PACK_NO_PADDING ifly_hand_t;

typedef struct
{
	u8   channel_no;
	u8   type;
	u8   week;
	u8   flag_alltime;
	u32  start_time;
	u32  end_time;
	u8   reserved;
}PACK_NO_PADDING ifly_valid_time_t;

typedef struct
{
	u8	 channel_no;
	u16  address;
	u32  baud_rate;
	u8   data_bit;
	u8   stop_bit;
	u8   crccheck;
	u8   flow_control;
	u8   protocol;
//	u8   preset[16];
//	u16  cruise[16];
//	u8   track[256];
}PACK_NO_PADDING ifly_yuntai_t;

typedef struct
{
	u8	 id;
	u8   type;// 1���ߵ�ƽ��2���͵�ƽ
	u8   flag_deal;
	//����ʽ
	char record_channel[8];
	u8	 flag_buzz;
	u8   flag_send;
	char alarm_out[4];
	u8  delay_time;//cyl added
	//PTZ����
//	u8	 flag_preset;
//	u8   preset;
//	u8   flag_cruise;
//	u8   cruise_no;
//	u8   flag_track;
}PACK_NO_PADDING ifly_alarm_in_t;

typedef struct
{
	u8	 id;
	u8   type;// 1�������ͣ�2��������
	u16  delay;
}PACK_NO_PADDING ifly_alarm_out_t;

typedef struct
{
	u8	 id;
	u8   channel_no;
	//PTZ����
	u8	 flag_preset;
	u8   preset;
	u8   flag_cruise;
	u8   cruise_no;
	u8   flag_track;
}PACK_NO_PADDING ifly_alarm_ptz_t;

typedef struct
{
	u8	 channel_no;
	//��ͼ��������ص�
	char channel_name[32];
	u8   flag_name;
	u16  x_name;
	u16  y_name;
	u8   brightness;
	u8   contrast;
	u8   saturation;
	u8   hue;
	//ʱ����ص�
	u8   flag_time;
	u16  x_time;
	u16  y_time;
	u8   time_format;
	u8   flag_mask;//�ڸ�
	u8   flag_overlay;//�ڵ�����
	u8   flag_lost;//��Ƶ��ʧ
	u8   flag_motion;//�ƶ����
	//����ʽ
	char record_channel[8];
	u8   flag_buzz;
	u8   flag_send;
	u16 flag_delay;//��ʱ
	char alarm_out[4];
}PACK_NO_PADDING ifly_image_t;

typedef struct
{
	char record_channel[8];
	u8   flag_display;
	u8   flag_buzz;	//����
	u8   flag_send;	//send email
	u8   flag_alarm;	
	char alarm_out[4]; //��·�������
}PACK_NO_PADDING ifly_dispose_t;

typedef struct
{
	u8   channel_no;
	u8   type;
	u16  x;
	u16  y;
	u16  w;
	u16  h;
}PACK_NO_PADDING ifly_area_t;

typedef struct
{
	u8   channel_no;
	//char flag_block[192];
	char flag_block[396];//wrchen 090223
}PACK_NO_PADDING ifly_md_area_t;

/* added by kangmao exception */
typedef struct
{
	u8 exceptiontype;
	u8 flag_display;
	u8 flag_buzz;
	u8 flag_send;
	u8 flag_alarm;
	char alarm_out[4];
}PACK_NO_PADDING ifly_exception_t;
/* added by kangmao exception */

/* added by kangmao structure definition */
/* 07-10-08 begin */
typedef struct
{
	u8  mode;										//��ѯ��ʽ
	u8  type;										//��ѯ����
	u32 start;										//��ʼʱ��
	u32 end;										//����ʱ��
}PACK_NO_PADDING ifly_log_t;

typedef struct
{
	u8  type;										//��������
	u32 start;										//������ʼʱ��
	char loginfo[32];								//���ؾ�����Ϣ
}PACK_NO_PADDING ifly_loginfo_t;

typedef struct
{
	u16 lognum;										//���ص���־��
	ifly_loginfo_t LogResult[MAX_LOG_SEARCH_NUM];	//������־��ѯ�Ľ��
}PACK_NO_PADDING ifly_logresult_t;

typedef struct
{
	sem_t rsem;
	sem_t wsem;
	pthread_mutex_t Mutex;
	u16 Head;
	u16 Tail;
	u16 Length;
	ifly_loginfo_t LogResult[MAX_LOG_SEARCH_NUM+1];//2007-12-04
}PACK_NO_PADDING ifly_logmng_t;
/* 07-10-08 end */
/* added by kangmao structure definition */

/* added by kangmao structure definition */
typedef struct
{
	char	name[12];
	char	password[12];
	char	local_privilege[16];
	char	remote_privilege[16];
	char	mac_address[18];
	u16		startID;
}PACK_NO_PADDING ifly_usermanage_t;
/* added by kangmao structure definition */

//�û���¼�Ľṹ
typedef struct
{
	char	username[12];
	char	pass[12];
	char	local_privilege[16];		      //��ǰ�����û��ı���Ȩ�� 
	char	remote_privilege[16];		      //��ǰ�����û���Զ��Ȩ��
}PACK_NO_PADDING ifly_userlogin_t;

typedef struct
{
	u16		device_id;
	char	device_name[32];
	u8		flag_overlay;
	u8      fb_show_type;
	u8		kb_lock_time;
	u8      split_image;
	u8		switch_time;
	u8		flag_status;
	u8		video_format;
	u8		vga_resolution;
	u8      transparency;
	u8		language;
	char	device_model[32];
	char	device_serial[32];
	char	version[64];

}PACK_NO_PADDING ifly_sysparam_t;
//zlb20090915 begin
typedef struct
{
	u8 preview_mode;
	u16 buzz_delay;
	u16		port2;		//pw:26/05/2010
	//pw 2010/6/8
	u32		flag_timeoverwrt;
	u32		rec_del_t;				//pw 2010/9/27
	u8 reserved1[8-MAX_CHN_NUM];
	u8 enc_osd_chn[MAX_CHN_NUM];	//pw 2010/9/14
	u8 enc_osd_time;	//pw 2010/9/14
	//pw 2010/6/30
	u8 bnc_pic_flag;
	//pw 2010/6/23
	u8 ntp_flag;
	u8 ntp_server[25];
	u8 time_zone;
	u8 dst_flag;
	u8 dst_start_month;
	u8 dst_start_day;
	u8 dst_end_month;
	u8 dst_end_day;
	u8 reserved2[1];
	char alarm_email[MAX_EMAIL_LENGTH];
	u16 md_emailalarm;
	u16 in_emailalarm;
	u16 vl_emailalarm;
	u16 vb_emailalarm;
#ifdef _DAYTIME_
	u8 dst_enable;
	u8 reserved[64 - MAX_EMAIL_LENGTH - 1];
#else
	u8 reserved[64 - MAX_EMAIL_LENGTH];
#endif

}PACK_NO_PADDING ifly_reserved_t;
//zlb20090915 end
typedef struct
{
	u16 version;
	u8  valid;	
	u8  reserved1;
	u32 magic;
	u32 op_nums;
	u8  reserved2[44];
	u32 conf_size;
	u32 verify;
}PACK_NO_PADDING ifly_config_head_t;

typedef struct //wrchen 20080521
{
	u32 size;
	u8 outputtype;//0=CVBS,1=VGA
	//	u8	reserved[28];
	//total 256 bytes
}PACK_NO_PADDING ifly_config_zip_head_t;

//pw 2010/7/14
typedef struct
{
	char alarm_send_email[MAX_EMAIL_LENGTH];			//EMAIL_OPT
	char send_email_pwd[16];								//EMAIL_OPT
	char send_email_smtp[MAX_EMAIL_LENGTH];				//EMAIL_OPT
#ifdef IECTRL_NEW
	u8	 supportdeinter;				//�Ƿ�֧��deinter���� 1�� 0�� (����)
	u8	 deinterval;	 				//deinterǿ�� 0-4 ���ã������У�ǿ����ǿ
	u8	 supportResolu;   				//�Ƿ�֧������¼��ֱ���
#endif
	
#ifdef LONGANSHI_M
	BOOL start_flag;		//yqluo 20101023
	int day_flag;			//yqluo 20101023
	BOOL type_flag;			//yqluo 20101023
	BOOL week_flag;			//yqluo 20101023
	int hour;				//yqluo 20101023
	int minite;				//yqluo 20101023
	int second;				//yqluo 20101023
	char add[89];		//yqluo 20101023
#elif defined(Vo_Set_Param) && defined(SHENSHIYIN_SP2)
	u8 		vga_brightness;
	u8 		vga_contrast;
	u8 		vga_hue;
	u8 		vga_saturation;
	short 	smtp_port;
	char 	add[111];
#elif defined(SHENSHIYIN_SP2)
	short smtp_port;
	char add[115];
#elif defined(HUMIDITY) && defined(PIP_PICTURE)       //yqluo 20101217
	char	humidity_num[10];
	u8		humidity_status;
	int 	main_chn_num;
	int 	minor_chn_num_1;
	int 	minor_chn_num_2;
	BOOL 	pip_use_flag;
	char 	add[90];
#elif defined(HUMIDITY)
	char	humidity_num[10];
	u8		humidity_status;
	char 	add[106];
#elif defined(PIP_PICTURE)
	int 	main_chn_num;
	int 	minor_chn_num_1;
	int 	minor_chn_num_2;
	BOOL 	pip_use_flag;
	char 	add[101];
#else
	char 	add[117];//add[120];//yqluo 20100121
#endif
}PACK_NO_PADDING ifly_add_t;

typedef struct
{
	ifly_config_head_t		head;
	ifly_image_t			image[MAX_CHN_NUM];
	ifly_area_t				envelop_area[MAX_CHN_NUM][MAX_COVER_REGION_NUM];
	ifly_md_area_t			md_area[MAX_CHN_NUM];
	ifly_valid_time_t		md_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		sh_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		vl_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	//ifly_dispose_t		tmp_md_dispose[MAX_CHN_NUM];
	//ifly_md_area_t		tmp_md_area[MAX_CHN_NUM];
	//ifly_valid_time_t		tmp_md_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	//ifly_valid_time_t		tmp_sh_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	//ifly_valid_time_t		tmp_vl_time[MAX_CHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	
	ifly_record_t			recset[MAX_RECCHN_NUM];
	ifly_valid_time_t		recvalidtime[MAX_RECCHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_hand_t				hand[MAX_RECCHN_NUM];
	
	ifly_yuntai_t			yuntai[MAX_CHN_NUM];
	ifly_cruise_path_t		cruise_path[MAX_CHN_NUM][MAX_CRUISE_PATH_NUM];
	ifly_alarm_in_t			alarm_in[MAX_ALARM_IN_NUM];
	ifly_alarm_out_t		alarm_out[MAX_ALARM_OUT_NUM];
	ifly_alarm_ptz_t		alarm_ptz[MAX_ALARM_IN_NUM][MAX_RECCHN_NUM];
	ifly_valid_time_t		alarm_in_time[MAX_ALARM_IN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		alarm_out_time[MAX_ALARM_OUT_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_exception_t		excep[MAX_EXCEPTION_NUM];
	unsigned int			user_num;
	ifly_usermanage_t		user[MAX_USER_NUM];
	ifly_sysparam_t			sysparam;
	ifly_reserved_t			reserved_param;//zlb20090915
	u8 					datefmt;
	ifly_dispose_t			VideoLose[MAX_CHN_NUM];//cyl added 
	ifly_dispose_t			Videocover[MAX_CHN_NUM];//cyl added
	ifly_add_t			add_param;
#ifdef ALARMSCH
	ifly_valid_time_t		alarm_sch_time[MAX_RECCHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		md_sch_time[MAX_RECCHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		vlose_sch_time[MAX_RECCHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
	ifly_valid_time_t		vcover_sch_time[MAX_RECCHN_NUM][DAYS_OF_WEEK][SEGMENTS_OF_DAY];
#endif
}PACK_NO_PADDING ifly_config_t;

#pragma pack( pop )

typedef struct
{
	int fd;
	u8  byInitCode;
	ifly_config_t config;
	pthread_mutex_t lock;
}ifly_config_manager_t;


//��ʼ�������ļ�
BOOL InitConf();
BOOL ReadConfig(ifly_config_manager_t *);
//pw 2010/9/6
int ErareConfig(ifly_config_manager_t *pManager);
BOOL WriteConfig(ifly_config_manager_t *);
//pw 2010/9/6
void* config_update(void* param);

BOOL GetDateformatParam(u8 *dtfmt);//wrchen 20080716
BOOL SetDateformatParam(u8 *dtfmt);//wrchen 20080716


inline BOOL GetYunTaiParam(ifly_yuntai_t *pyt,u8 chn);
BOOL SetYunTaiParam(ifly_yuntai_t *pyt,u8 chn);
BOOL SetTmpYunTaiParam(ifly_yuntai_t *pyt,u8 chn); 
BOOL GetTmpYunTaiParam(ifly_yuntai_t *pyt,u8 chn); 

BOOL GetCruisePath(ifly_cruise_path_t *ppath,u8 chn);
BOOL SetCruisePath(ifly_cruise_path_t *ppath,u8 chn);

inline BOOL GetRecordSet(ifly_record_t *prs,u8 chn);
BOOL SetRecordSet(ifly_record_t *prs,u8 chn);

BOOL GetTmpRecordSet(ifly_record_t *prs,u8 chn);//  wrchen 080611
BOOL SetTmpRecordSet(ifly_record_t *prs,u8 chn);//  wrchen 080611

inline BOOL GetRecordTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetRecordTime(ifly_valid_time_t *pvt,u8 chn);

BOOL GetTmpRecordTime(ifly_valid_time_t *pvt,u8 chn); // wrchen 080611
BOOL SetTmpRecordTime(ifly_valid_time_t *pvt,u8 chn); // wrchen 080611
#ifdef ALARMSCH
inline BOOL GetAlarmSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetAlarmSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL GetTmpAlarmSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetTmpAlarmSchTime(ifly_valid_time_t *pvt, u8 chn);
inline BOOL GetMDSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetMDSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL GetTmpMDSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetTmpMDSchTime(ifly_valid_time_t *pvt, u8 chn);
inline BOOL GetVLoseSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetVLoseSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL GetTmpVLoseSchTime(ifly_valid_time_t *pvt, u8 chn);
BOOL SetTmpVLoseSchTime(ifly_valid_time_t *pvt, u8 chn);
#endif

inline BOOL GetHandParam(ifly_hand_t *phand,u8 chn);
BOOL SetHandParam(ifly_hand_t *phand,u8 chn);

inline BOOL GetNetWorkParam(ifly_NetWork_t *pnw);
BOOL SetNetWorkParam(ifly_NetWork_t *pnw);

inline BOOL GetAlarmInParam(ifly_alarm_in_t *pain,u8 id);
BOOL SetAlarmInParam(ifly_alarm_in_t *pain,u8 id);

BOOL GetTmpAlarmInParam(ifly_alarm_in_t *pain,u8 id);
BOOL SetTmpAlarmInParam(ifly_alarm_in_t *pain,u8 id);

inline BOOL GetAlarmOutParam(ifly_alarm_out_t *paout,u8 id);
BOOL SetAlarmOutParam(ifly_alarm_out_t *paout,u8 id);

BOOL GetTmpAlarmOutParam(ifly_alarm_out_t *paout,u8 id);
BOOL SetTmpAlarmOutParam(ifly_alarm_out_t *paout,u8 id);

inline BOOL GetAlarmPtzParam(ifly_alarm_ptz_t *paptz,u8 id);
BOOL SetAlarmPtzParam(ifly_alarm_ptz_t *paptz,u8 id);

BOOL GetTmpAlarmPtzParam(ifly_alarm_ptz_t *paptz,u8 id);
BOOL SetTmpAlarmPtzParam(ifly_alarm_ptz_t *paptz,u8 id);

inline BOOL GetAlarmInTime(ifly_valid_time_t *pvt,u8 id);
BOOL SetAlarmInTime(ifly_valid_time_t *pvt,u8 id);

BOOL GetTmpAlarmInTime(ifly_valid_time_t *pvt,u8 id);
BOOL SetTmpAlarmInTime(ifly_valid_time_t *pvt,u8 id);

inline BOOL GetAlarmOutTime(ifly_valid_time_t *pvt,u8 id);
BOOL SetAlarmOutTime(ifly_valid_time_t *pvt,u8 id);

BOOL GetTmpAlarmOutTime(ifly_valid_time_t *pvt,u8 id);
BOOL SetTmpAlarmOutTime(ifly_valid_time_t *pvt,u8 id);

inline BOOL GetImageSet(ifly_image_t *pimage,u8 chn);
BOOL SetImageSet(ifly_image_t *pimage,u8 chn);

BOOL SetTmpImageSet(ifly_image_t *pimage,u8 chn); // wrchen 080610
BOOL GetTmpImageSet(ifly_image_t *pimage,u8 chn); // wrchen 080610

BOOL SetTmpEnvelopArea(ifly_area_t *pma,u8 chn);  // wrchen 080610
BOOL GetTmpEnvelopArea(ifly_area_t *pma,u8 chn);  // wrchen 080610

inline BOOL GetMDTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetMDTime(ifly_valid_time_t *pvt,u8 chn);

inline BOOL GetSHTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetSHTime(ifly_valid_time_t * pvt, u8 chn);

inline BOOL GetVLTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetVLTime(ifly_valid_time_t * pvt, u8 chn);

inline BOOL GetEnvelopArea(ifly_area_t *pma,u8 chn);
BOOL SetEnvelopArea(ifly_area_t *pma,u8 chn);

inline BOOL GetMDArea(ifly_md_area_t *pma,u8 chn);
BOOL SetMDArea(ifly_md_area_t *pma,u8 chn);

BOOL GetTmpSHTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetTmpSHTime(ifly_valid_time_t *pvt,u8 chn);

BOOL GetTmpVLTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetTmpVLTime(ifly_valid_time_t *pvt,u8 chn);

BOOL GetTmpMDTime(ifly_valid_time_t *pvt,u8 chn);
BOOL SetTmpMDTime(ifly_valid_time_t *pvt,u8 chn);


BOOL GetVideoLoseSet(ifly_dispose_t*pvt,u8 chn);
BOOL SetVideoLoseSet(ifly_dispose_t*pvt,u8 chn);// changed by wrchen 20080428
BOOL GetTmpVideoLoseSet(ifly_dispose_t *pvt,u8 chn);
BOOL SetTmpVideoLoseSet(ifly_dispose_t *pvt,u8 chn);
BOOL GetVideoCoverSet(ifly_dispose_t*pvt,u8 chn);
BOOL SetVideoCoverSet(ifly_dispose_t *pvt,u8 chn);// changed by wrchen 20080428
BOOL GetTmpVideoCoverSet(ifly_dispose_t *pvt,u8 chn);
BOOL SetTmpVideoCoverSet(ifly_dispose_t *pvt,u8 chn);

//zlb20090915 begin
inline BOOL GetReservedParam(ifly_reserved_t *p_reserved_param);
BOOL SetReservedParam(ifly_reserved_t *p_reserved_param);

inline BOOL GetEmailAlarm(char *p_alarm_email);
BOOL SetEmailAlarm(char *p_alarm_email);

BOOL GetTmpMDEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetTmpMDEmailAlarm(u8 flag, u8 chn);
inline BOOL GetMDEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetMDEmailAlarm(u8 flag, u8 chn);

BOOL GetTmpVLEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetTmpVLEmailAlarm(u8 flag, u8 chn);
inline BOOL GetVLEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetVLEmailAlarm(u8 flag, u8 chn);

BOOL GetTmpVBEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetTmpVBEmailAlarm(u8 flag, u8 chn);
inline BOOL GetVBEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetVBEmailAlarm(u8 flag, u8 chn);

BOOL GetTmpAlarmInEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetTmpAlarmInEmailAlarm(u8 flag, u8 chn);
inline BOOL GetAlarmInEmailAlarm(u8 *p_flag, u8 chn);
BOOL SetAlarmInEmailAlarm(u8 flag, u8 chn);
//zlb20090915 end

BOOL GetTmpMDArea(ifly_md_area_t *pma,u8 chn);
BOOL SetTmpMDArea(ifly_md_area_t *pma,u8 chn);

BOOL GetTmpMDDispose(ifly_image_t *pimg,u8 chn);
BOOL SetTmpMDDispose(ifly_image_t *pimg,u8 chn);

inline BOOL GetExceptionParam(ifly_exception_t *pet,u8 id);
BOOL SetExceptionParam(ifly_exception_t *pet,u8 id);

inline BOOL GetSysParam(ifly_sysparam_t *psp);
BOOL SetSysParam(ifly_sysparam_t *psp);

BOOL SetPPPOEUser(char *username,char *passwd);

inline int GetBitRateValue(int nSel);
inline int GetFrameRateValue(int nSel);
inline int GetFrameRateValue_D1(int nSel);
inline int GetFrameRateValue_HALF_D1(int nSel);
inline int GetPreRecordTimeValue(int nSel);
inline int GetPostRecordTimeValue(int nSel);

inline int GetBitRateSel(int value);
inline int GetFrameRateSel(int value);
inline int GetFrameRateSel_D1(int value);
inline int GetPreRecordTimeSel(int value);
inline int GetPostRecordTimeSel(int value);

inline int GetLogTextInfo(int mast,int sec,char *buf1,char *buf2);
inline int GetLogTextRemote(int mast,int sec,char *buf1,char *buf2,u8 langid);

void control_light(u16 light,u8 status);

BOOL VerifyKernel(char *filename);
BOOL VerifyPanel(char *filename);

#define UPDATE_SUCCESS				0
#define UPDATE_FAILED_FILEERROR		-1
#define UPDATE_FAILED_OUTOFMEMORY	-2
#define UPDATE_FAILED_FLASHDEVICE	-3
#define UPDATE_FAILED_FLASHSTATE	-4
#define UPDATE_FAILED_FLASHERASE	-5
#define UPDATE_FAILED_FLASHWRITE	-6
#define UPDATE_FAILED_FLASHCMP		-7

int update_panel(char *filename);
s32 GetUpdateValue(ifly_update_progress_t *pUpdate);
int flashcopy(char *filename,char *devicename);

int force_sps_pps_frames(void);
u8 GetSuccFlag(void);
void SetSuccFlag(u8);

//find1
//int flash_updata(char *target, char *data_file);
int flash_updata(char *target, FILE *fp_in, u32 offset, u32 size);

/******************************/
inline BOOL GetUserInfo(ifly_usermanage_t *pUser, int index);
BOOL SetUserInfo(ifly_usermanage_t *pUser, int index);
/******************************/

int GetUserNum();
BOOL SetUserNum(int i);

BOOL ReadConfig(ifly_config_manager_t *pManager);
BOOL SetZipConf(u32 offset,ifly_config_manager_t *pManager);
#endif
