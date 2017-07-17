#ifndef _TW2835_H_
#define _TW2835_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sched.h>
#include <pthread.h>

#include <asm/types.h>

#include <linux/videodev2.h>
#include <linux/fb.h>

//#include <linux/media/dev2834.h>

#include "iflytype.h"
#include "common.h"
#include "config.h"
//#include <linux/TL-fpga.h>
//#include <media/dev2834.h>
//#include <video/davincifb.h>
#include "mediacommon.h"
#include <play.h>

#define DBG_MEM_RW_SIZE 5

//typedef unsigned char		u8;
//typedef unsigned short	u16;
//typedef unsigned int		u32;

/****
#define tw2835_MAXLINE		12

typedef struct
{
	u8 channel;
	__u32 type;
	__s32 value;
} tw2835_color;

typedef struct
{
      int chip_number;
	__u8 channel;
	int flag;	//���ñ�־
} tw2835_status;

typedef struct
{
	u8 channel;
	int mode;
} tw2835_mode;

typedef struct
{
    int chip_number;
	__u8 channel;
	int flag;	//���ñ�־
	__u16 sense;
	__u16 area[tw2835_MAXLINE];
} tw2835_motion;

typedef struct
{
	u8 channel;
	__u32 std;
} tw2835_video_std;

typedef struct {
	__u32 video_std[4];
	int brightness[4];
	int contrast[4];
	int saturation[4];
	int hue[4];
} tw2835_config;

typedef struct
{
	u32 loss;		//������Ƶ��ʧ��ͨ��������������Ϊ0��1��2��3
	u32 alive;		//������Ƶ�ָ���ͨ��������������Ϊ0��1��2��3
	u32 blind;		//�����ڵ�������ͨ��������������Ϊ0��1��2��3
	u32 motion;		//�����ƶ�����ͨ��������������Ϊ0��1��2��3
} tw2835_irq_status;
***/

#define INFO_LAYER_OFFSET 0x600000
#define FONT_LOAD_OFFSET (2 << 10)
#define PIC_LOAD_OFFSET INFO_LAYER_OFFSET + FONT_LOAD_OFFSET + 0x10000
#define AUDIO_LOAD_OFFSET PIC_LOAD_OFFSET + 0x20000

#define STATUS_PIC_NUM 7
#define PIC_BUF_SIZE 2048

#define REC_TYPE_HAND 1
#define REC_TYPE_TIMER 2
#define REC_TYPE_ALARM 3
#define REC_TYPE_MD 4

#define FLAG_INDEX_REC 0
#define FLAG_INDEX_MD 5
#define FLAG_INDEX_ALARM 3

#define STATUS_POS_X 100
#define STATUS_POS_Y (SCREEN_HEIGHT - 150)

//ͨ�����ơ��ڸ���Ӽ�ͨ��״̬�Ľṹ��
struct _chn_info_layer {
	u8 chnname_flag;										//�Ƿ���ʾͨ����
	u8 envelop_flag;										//�Ƿ���ʾ�ڸ�
	u8 chnname[32];											//ͨ������������
	WndPos chnname_pos;										//ͨ������ʾλ��
	
	u8 envelop_num;											//ʵ���ڸ���				0-MAX_RGN_NUM(��ͨ������ڸ���)
	WndRect envelop_rect[MAX_RGN_NUM];						//�ڸǾ��ζ�λ���� 
#if 1
    //pw 2010/6/12
    //u8 flag_vloss; //��Ƶ��ʧ
    u8 flag_rec;//�Ƿ���ʾ¼���־
    u8 rec_type_id;//¼������id
    u8 flag_md;//�Ƿ���ʾ�ƶ�����־
    u8 flag_alarm;//�Ƿ���ʾ������־
    WndPos status_pos;//״̬��ʾ����
#endif
};

//������Ϣ�ṹ��(ʱ���/ͨ����Ϣ��)
typedef struct
{
	u8	b_time_disp;										
	u8	display_mode;	
#ifdef PREVIEW_6WIN_
	u8	num6chn;
#endif
	u8	num8chn;
	u8	onechnnum;
	u8	last_display_mode;									//wrchen 090626
	u8	display_chn_id[MAX_CHN_NUM];						//ͨ����ʾ˳��
//	u32 i_timestamp;											//ʱ���
	char s_now[20];											//��ǰʱ���ַ���
	
	WndPos time_info_pos;									//ʱ�����ʾλ��
	
	struct _chn_info_layer chn_info_layer[MAX_CHN_NUM];		//��ͨ��֮���ͨ�����ơ��ڸ���Ϣ�ṹ������
} info_layer;

extern info_layer g_p_info_layer;

extern WndPos time_info_pos;
extern WndPos chnname_info_pos[MAX_CHN_NUM];
extern WndRect envelop_info_rect[MAX_CHN_NUM][MAX_RGN_NUM];
extern u8 envelop_info_num[MAX_CHN_NUM];

extern WndPos tmp_time_info_pos;
extern WndPos tmp_chnname_info_pos[MAX_CHN_NUM];
extern WndRect tmp_envelop_info_rect[MAX_CHN_NUM][MAX_RGN_NUM];
extern u8 tmp_envelop_info_num[MAX_CHN_NUM];

#define MAX_FRAME_LEN_A (AUDIO_SAMPLE_SIZE >> 2)
#define ENC_BUF_NUM_A MAX_CHN_NUM
#define VOIP_BUF_NUM 3

typedef struct									//��Ƶ����֡����ṹ��
{
	u32 length;									//֡����
	u8 data[MAX_FRAME_LEN_A];					//֡����
	ifly_audio_header_t aheader;				//�Զ���adpcmͷ
	volatile u8 rw_status; 						//0 ��ʾ��д 1 ��ʾ�ɶ�
	u8 frame_type;								//֡����
	u8 chn_index;								//��Ӧͨ��index
} ifly_frame_a_t;

typedef struct									//��Ƶ����֡����ṹ��
{
	u32 length;									//֡����
	u8 data[AUDIO_VOIP_SAMPLE_SIZE];			//֡����
	volatile u8 rw_status; 						//0 ��ʾ��д 1 ��ʾ�ɶ�
} ifly_frame_voip_t;

typedef struct									//��Ƶ����֡����ؽṹ��
{
	ifly_frame_a_t frames[ENC_BUF_NUM_A];		//��Ƶ����֡����ṹ������
	u8 b_cap[MAX_CHN_NUM];						//��ͨ���Ƿ���Ҫ��Ƶ�ɼ������־
#if 1
	ifly_frame_voip_t voip_local[VOIP_BUF_NUM];	//�����Խ����زɼ�����
	ifly_frame_voip_t voip_remote[VOIP_BUF_NUM];//�����Խ�Զ�̽��ܻ���
#endif
	u8 b_voip_cap;								//�Ƿ���Ҫ�����Խ��ɼ�����
} ifly_pool_a_t;

extern ifly_pool_a_t g_p_audio_pool;


//ȫ�ֱ���
extern int gfd_tl, gfd_tw2834;

#if 1
void set16ch(const u8 *);
void set12ch(const u8 *);
void set9ch(const u8 *);
void set4ch(const u8 *);
void set1ch(const u8 *);
#endif
void set_mode_conf(u8 mode, const u8 *p_strchn);

void set_top_chnum(unsigned int);
u8 ch_cton(const u8);
u8 ch_ntoc(const u8);
void refresh_pos(const u8, const u8);
int get_display_index(const u8);
void set_video_chn(int mode);
void set_audio_chn();
void set_volume(int flag);//zlb20090907

unsigned char *get_preview_map(void);
void load_dvm(const char *);

int enc_control(int);

void change_vga_resol(int mode,int video_format);
void *test_tw2815_irq(void *);
int get_video_status_change(int, u32, u32);

unsigned int cpld_rd(unsigned int);
void cpld_wr(unsigned int, unsigned int);
void alarm_out(int, int);

void changepinmux0(u32 value);
void changetrans(u8 trans);
void printfvgareg();
void vgapreview(u32 ntsc_pal_mode);

void clear_lights(void);

#endif
