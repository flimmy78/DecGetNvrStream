#ifndef _PARTITION_INDEX_H_
#define _PARTITION_INDEX_H_

#include <stdio.h>
#include "iflytype.h"

#ifndef WIN32
#include <pthread.h>
#include <sys/time.h>

#endif

//#define _IFLY_DEBUG_DISK_PRT_
//#define _IFLY_DEBUG_DISK_PRT2_

#ifdef _IFLY_DEBUG_DISK_PRT_
	#define dbgprint(format,args...) fprintf(stderr, format, ##args)
#else
	#define dbgprint(format,args...)
#endif

#ifdef _IFLY_DEBUG_DISK_PRT2_
	#define dbgprint2(format,args...) fprintf(stderr, format, ##args)
#else
	#define dbgprint2(format,args...)
#endif

//#define SIZE_OF_FILE_VECTOR	((s64)134217728)//yaogang hdd
//#define SIZE_OF_RESERVED		((s32)393216)
//#define SIZE_OF_RESERVED		((s32)(512*1024))
#define SIZE_OF_RESERVED		((s32)(800*1024))

#define RECTYPE_MASK_TIMER	0x1
#define RECTYPE_MASK_MD		0x2
#define RECTYPE_MASK_ALARM	0x4
#define RECTYPE_MASK_HAND	0x8
#define RECTYPE_MASK_ALL	0x10

#pragma pack( push, 1 )

typedef struct
{
	u16 file_no;
	u16 sect_nums;
	u32 start_time;
	u32 end_time;
}PACK_NO_PADDING  chn_last_use_file;

typedef struct
{
	u32 op_nums;					//
	u32 reserved1;					//
	u32 version;					//
	u32 reserved2;					//
	u32 total_file_nums;			//
	u32 recorded_file_nums;			//
	u32 full_file_nums;				//
	u32 full_file_offset;			//
	chn_last_use_file chn_info[33];	//
	u8  reserved3[80];				//
	u32 verify;						//
}PACK_NO_PADDING  partition_index_header;

typedef struct
{
	u32 file_no;
	u8  chn_no;
	u8  busy;
	u16 sect_nums;
	u32 start_time;
	u32 end_time;
}PACK_NO_PADDING file_use_info;

typedef struct
{
	u8  type;
	u8  image_format;
	u8  stream_flag;
	u8  video_compressor;// 1:xvid; 2:jpeg; 3:h.264
	u8  audio_compressor;// 1:pcmu; 2:pcma; 3:adpcm
	u8  reserved1[11];
	u32 start_time;
	u32 end_time;
	u32 start_position;
	u32 end_position;
}PACK_NO_PADDING segment_use_info;

typedef struct
{
//#ifdef _NVR_
	u32  channel_no;//u32  ChaMask | 1<<chn
//#else
//	u8   channel_no;
//#endif
	u8   play_no;
	u8   type;
	u32  start_time;
	u32  end_time;
	u32  card_no;
	u8   mask;
}PACK_NO_PADDING search_param_t;

typedef struct
{
	u8   channel_no;//u32  channel_no;//csp modify
	u8   type;
	u32  start_time;
	u32  end_time;
	u8   image_format;//3:cif;4:4cif
	u8   stream_flag;//0:��Ƶ��;1:��Ƶ��
	u32	 size;
	u32  offset;
	u8   disk_no;
	u8   ptn_no;
	u16  file_no;
}PACK_NO_PADDING recfileinfo_t;

//yaogang modify 20141225
typedef struct
{
	u8	channel_no;
	u8	type;
	u8	snap_format;  //��ʽ,0-jpeg,1-bmp,Ĭ��jpeg 
	u8	reserver;
	u16 	width;
	u16	height;
	u32	size;
	u32	offset;
	u8	disk_no;
	u8	ptn_no;
	u16	file_no;
	u32	start_time;
	u32	start_timeus;
} PACK_NO_PADDING recsnapinfo_t;

/*********************************************************
ͼƬ���淽��
�ļ���������256M��ͼƬ�ͱ������ļ�������
���е�ͼƬ��������Ϣ����¼�������ļ���
�����ļ��ṹ:
index_file_header
file_use_info * 128 		//����n ���ļ�������ÿ�����ݶ�128 ���ļ�����
pic_use_info * 32768	//ͼƬ��Ϣ��ÿ�ļ��������ͼƬ������32768*8k(�ֱ���352*288)
*********************************************************/
//48
typedef struct
{
	u16	file_nums;			//�ļ���������PIC_FILE_NUMS
	u16	file_cur_no;			//��ǰ����ʹ�õ��ļ�����������ؾ�0xffff
	u32	partition_pic_nums;	//���������ͼƬ����
	time_t		start_sec;	//����ʹ�ÿ�ʼʱ��
	suseconds_t	start_usec;
	time_t		end_sec;		//����ʹ�ý���ʱ��
	suseconds_t	end_usec;
	char reserved[24];
	u32 verify; 				//
}PACK_NO_PADDING  partition_pic_index_header;
//32
typedef struct
{
	u16	file_no;
	u8	status;				//ʹ�����: δʹ�á�δд������д��
	u8	pic_type_mask;		//ץͼ�������룬��ʶ��ǰ�ļ��������Ƿ񱣴����Ӧ���͵�ͼƬ0
	u32	chn_mask;			//ͨ�����룬��ʶ��ǰ�ļ��������Ƿ񱣴����Ӧͨ����ͼƬ0
	u32	next_pic_pos;			//д��һ��ͼƬʱ����ʼλ��0xffffffff
	u32	file_pic_nums;		//�ļ��б����ͼƬ����
	time_t		start_sec;	//�ļ������ڵ�һ��ͼƬʱ��
	suseconds_t	start_usec;
	time_t		end_sec;		//�ļ����������һ��ͼƬʱ��
	suseconds_t	end_usec;
}PACK_NO_PADDING file_pic_use_info;

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
ȫ������			0xff
*/
//24
typedef struct
{
	u8	chn;		//ͨ��	
	u8	pic_type;	//ץͼ����
	u16	file_no;	//�����ļ�����
	u16 	width;
	u16	height;
	u32	file_offset;	//ͼƬ�洢λ�����ļ������ڲ�ƫ��
	u32	pic_no;	//ͼƬ���
	u32	pic_size;		//ͼƬ��С
	time_t		tv_sec;	//ͼƬʱ��
	suseconds_t	tv_usec;
}PACK_NO_PADDING pic_use_info;





//���ڱ���ͼƬ�ļ���������
#define PIC_FILE_NUMS		128
#define PRE_RECSNAP_FILES	2

//ͼƬ��С��8K�ƣ��ļ�����������д���ͼƬ����
#define PIC_NUMS_PER_FILE	32768


#pragma pack( pop )

typedef struct
{
	partition_index_header header;	//��������ͷ
	partition_pic_index_header pic_header;// ����ͼƬ����ͷ
	FILE *index1;					//�����ļ�
	FILE *index2;					//���������ļ�
	FILE *pic_idx;					//ͼƬ�����ļ�
	FILE *pic_idx_bk;				//ͼƬ���������ļ�
//	u8   *mapdst1;					//�����ļ��ڴ�ӳ��ָ��
//	u8   *mapdst2;					//���������ļ��ڴ�ӳ��ָ��
//	u32  length1;						//�����ļ�����
//	u32  length2;						//���������ļ�����
	u8  *cachedst;
	u32 cachelen;
	u8  valid;						//�����Ƿ���Ч
#ifndef WIN32
	pthread_mutex_t lock;			//����ʹ����
#endif
	char mountpath[64];				//��������·��
} partition_index;



int fileflush(FILE *fp);
int filecp(char *src,char *dst);
long seek_to_segment(partition_index *index,partition_index_header *pHeader,int file_no,int sect_no);
int init_partition_index(partition_index *index,char *path);
int destroy_partition_index(partition_index *index);
s64 get_partition_total_space(partition_index *index);
s64 get_partition_free_space(partition_index *index);
int get_chn_next_segment(partition_index *index,int chn,int *file_no,int *sect_offset);
int get_first_full_file_end_time(partition_index *index,u32 *end_time);
int get_chn_next_segment_force(partition_index *index,int chn,int *file_no,int *sect_offset);
int update_chn_cur_segment(partition_index *index,int chn,segment_use_info *p_s_u_info,u8 finished);
int search_rec_file(partition_index *index,search_param_t *search,recfileinfo_t *fileinfo_buf,int max_nums,u8 disk_no,u8 ptn_no);
int lock_partition_index(partition_index *index);
int unlock_partition_index(partition_index *index);

//yaogang modify 20141225
int get_index_fileinfo(partition_index* index, u16 file_no, file_pic_use_info *finfo, u8 need_lock);
int refresh_index_fileinfo(partition_index* index, file_pic_use_info *finfo, u8 need_lock);
int refresh_index_header(partition_index* index, partition_pic_index_header *pheader, u8 need_lock);
int refresh_index_picinfo(partition_index* index, pic_use_info* pinfo, u8 need_lock);

int init_partition_pic_index(partition_index* index);
int write_snapshot_to_partition(partition_index* index, u8 chn, u8 pic_type, time_t tv_sec, suseconds_t tv_usec, void *pdata, u32 data_size, u32 width, u32 height);
int search_rec_snap(partition_index *index,search_param_t *search,recsnapinfo_t *snapinfo_buf,int max_nums,u8 disk_no,u8 ptn_no);
int read_snap_file(partition_index *index, u8 *pbuf, u16 nFileNo, u32 nOffset, u32 nSize);


#endif

