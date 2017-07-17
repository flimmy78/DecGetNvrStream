#ifndef _MEDIA_STREAM_H_
#define _MEDIA_STREAM_H_

//#include "iflytype.h"
#include "common_basetypes.h"

#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define SOCKHANDLE SOCKET
#define SEMHANDLE  HANDLE
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
typedef struct sockaddr_in SOCKADDR_IN;
#define ADDR_ANY   0
#define SEMHANDLE  sem_t *
#define SOCKHANDLE int
#define INVALID_SOCKET		(-1)
#define SOCKET_ERROR		(-1)
#define HIWORD(l)           ((u16)(((u32)(l) >> 16) & 0xFFFF))  //08-03-08 chenjie
#define LOWORD(l)           ((u16)(l))							//08-03-08 chenjie
#define HIBYTE(w)           ((u8)(((u16)(w) >> 8) & 0xFF))		//08-03-08 chenjie
#define LOBYTE(w)           ((u8)(w))							//08-03-08 chenjie
#endif

#ifndef MEDIA_TYPE
#define MEDIA_TYPE
//ͼ���������
#define  MEDIA_TYPE_H264	 (u8)98//H.264//������109?
#define  MEDIA_TYPE_MP4	     (u8)97//MPEG-4
#define  MEDIA_TYPE_H261	 (u8)31//H.261
#define  MEDIA_TYPE_H263	 (u8)34//H.263
#define  MEDIA_TYPE_MJPEG	 (u8)26//Motion JPEG
#define  MEDIA_TYPE_MP2		 (u8)33//MPEG2 video

//������������
#define	 MEDIA_TYPE_MP3	     (u8)96//mp3
#define  MEDIA_TYPE_PCMU	 (u8)0//G.711 ulaw
#define  MEDIA_TYPE_PCMA	 (u8)8//G.711 Alaw
#define	 MEDIA_TYPE_G7231	 (u8)4//G.7231
#define	 MEDIA_TYPE_G722	 (u8)9//G.722
#define	 MEDIA_TYPE_G728	 (u8)15//G.728
#define	 MEDIA_TYPE_G729	 (u8)18//G.729
#define	 MEDIA_TYPE_RAWAUDIO (u8)19//raw audio
//#define  MEDIA_TYPE_ADPCM	 (u8)20//adpcm
#define  MEDIA_TYPE_ADPCM	 (u8)21//adpcm//wrchen 091117

#endif//MEDIA_TYPE

#define  MEDIASTREAM_NO_ERROR               (u16)0//�ɹ�����ֵ  
#define  ERROR_MEDIA_STREAM_BASE            (u16)16000
#define  ERROR_SND_PARAM					(ERROR_MEDIA_STREAM_BASE+1)//���÷���ģ��������� 
#define  ERROR_SND_NOCREATE					(ERROR_MEDIA_STREAM_BASE+2)//����ģ��û�д���
#define  ERROR_SND_MEMORY					(ERROR_MEDIA_STREAM_BASE+3)//����ģ���ڴ��������
#define  ERROR_SND_CREATE_SOCK				(ERROR_MEDIA_STREAM_BASE+4)//����ģ�鴴��socket
#define  ERROR_RTP_SSRC                     (ERROR_MEDIA_STREAM_BASE+5)//RTPͬ��Դ����.
#define  ERROR_LOOPBUF_CREATE               (ERROR_MEDIA_STREAM_BASE+6)//��״���崴������  
#define  ERROR_RTP_NO_INIT                  (ERROR_MEDIA_STREAM_BASE+7)//RTP����Щ����û����
#define  ERROR_RTCP_NO_INIT                 (ERROR_MEDIA_STREAM_BASE+8)//RTP����Щ����û����
#define  ERROR_RTCP_SET_TIMER               (ERROR_MEDIA_STREAM_BASE+9)//RTCP���ö�ʱ������
#define  ERROR_RTP_SSRC_COLLISION           (ERROR_MEDIA_STREAM_BASE+10)//RTP ͬ��Դ����
#define  ERROR_SOCK_INIT                    (ERROR_MEDIA_STREAM_BASE+11)//socket û�г�ʼ��
#define  ERROR_H261_PACK_NUM                (ERROR_MEDIA_STREAM_BASE+12)//H261�İ���������
#define  ERROR_PACK_TOO_LEN                 (ERROR_MEDIA_STREAM_BASE+13)//G.711�����ٰ�̫��
#define  ERROR_H263_PACK_NUM                (ERROR_MEDIA_STREAM_BASE+14)//H263�İ���������
#define  ERROR_H263_PACK_TOOMUCH            (ERROR_MEDIA_STREAM_BASE+15)//H263�����ݰ�̫��

#define  ERROR_NET_RCV_PARAM                (ERROR_MEDIA_STREAM_BASE+100)//���ý���ģ���������
#define  ERROR_NET_RCV_NOCREATE             (ERROR_MEDIA_STREAM_BASE+101)//����ģ��û�д���
#define  ERROR_NET_RCV_MEMORY               (ERROR_MEDIA_STREAM_BASE+102)//����ģ���ڴ��������
#define  ERROR_RCV_RTP_CREATE               (ERROR_MEDIA_STREAM_BASE+103)//����ģ��RTP����ʧ��
#define  ERROR_RCV_RTP_CALLBACK             (ERROR_MEDIA_STREAM_BASE+104)//����ģ������RTP�ص�����ʧ��
#define  ERROR_RCV_RTP_SETREMOTEADDR        (ERROR_MEDIA_STREAM_BASE+105)//����ģ������RTPԶ�˵�ַʧ��
#define  ERROR_CREATE_LOOP_BUF              (ERROR_MEDIA_STREAM_BASE+106)//������״����ʧ��
#define  ERROR_RCV_NO_CREATE                (ERROR_MEDIA_STREAM_BASE+107)//����ģ����ն���û�д���

#define  ERROR_WSA_STARTUP                  (ERROR_MEDIA_STREAM_BASE+200)//wsastartup error
#define  ERROR_CREATE_SEMAPORE              (ERROR_MEDIA_STREAM_BASE+201)// create semapore error
#define  ERROR_SOCKET_CALL                  (ERROR_MEDIA_STREAM_BASE+202)//����socket() ��������
#define  ERROR_BIND_SOCKET                  (ERROR_MEDIA_STREAM_BASE+203)//socket �󶨳���
#define  ERROR_CREATE_THREAD                (ERROR_MEDIA_STREAM_BASE+204)//�����̳߳��� 

#define  ERROR_LOOPBUF_FULL                 (ERROR_MEDIA_STREAM_BASE+205)//ѭ��������

#define	 ERROR_SND_FRAME                    (INT32)-1;

#define  MIN_PRE_BUF_SIZE                   (INT32)28// G.711���ټ�һ�ֽ�

#define  MAX_H261_HEADER_LEN				(INT32)292//
#define  MAX_H263_HEADER_LEN				(INT32)3076//

#ifndef  MAX_NETSND_DEST_NUM
#define  MAX_NETSND_DEST_NUM				5
#endif

#define SAFE_DELETE(p) {if(p){free(p);p=NULL;}}
#define IFLYFAILED(p) ((p)!=MEDIASTREAM_NO_ERROR)

//#ifndef MAX_FRAME_SIZE
#define MAX_FRAME_SIZE				(INT32)256*1024//zlb20100802
//#define MAX_FRAME_SIZE				(INT32)80*1024
//#endif

#define MAX_PACK_NUM				0x60// �������128kbyte������֡���ԣ�С���� <  96
#define MAX_PACK_SIZE				(INT32)1450//
#define MAX_RCV_PACK_SIZE			(INT32)8192// ����ʱ���İ���

#define LOOP_BUF_UINT_NUM			(INT32)200//400
#define REPEAT_LOOP_BUF_UINT_NUM	(INT32)600//
#define MAX_SESSION_NUM				(INT32)128//

#define MAX_EXTEND_PACK_SIZE		(INT32)(MAX_PACK_SIZE - MAX_PACK_EX_LEN - EX_HEADER_SIZE)
#define MAX_EXTEND_PACK_NUM			(INT32)((MAX_FRAME_SIZE+MAX_EXTEND_PACK_SIZE-1)/MAX_EXTEND_PACK_SIZE)// 

#define VIDEO_TIME_SPAN				(INT32)40

#define RTP_FIXEDHEADER_SIZE		(INT32)12//
#define EX_HEADER_SIZE				(INT32)4//
#define MAX_PACK_EX_LEN				(INT32)12//1 TOTALNUM 1Index,1mode,1rate,4FrameId,2width,2Height;
#define EX_TOTALNUM_POS				(INT32)0//С�����ڵ�֡�������ܰ���
#define EX_INDEX_POS				(INT32)1//С��������֡�еİ����
#define EX_FRAMEMODE_POS			(INT32)2//��֡Ϊ��Ƶ֡ʱ����Ƶģʽ
#define EX_FRAMERATE_POS			(INT32)3//֡��
#define EX_FRAMEID_POS				(INT32)4//֡ID
#define EX_WIDTH_POS				(INT32)8 //��֡Ϊ��Ƶ֡ʱ�Ŀ�
#define EX_HEIGHT_POS				(INT32)10//��֡Ϊ��Ƶ֡ʱ�ĸ�

#define MAX_LOCAL_IP_NUM			16
#define MAX_SND_NUM					1024

#define MAX_AUDIO_PACK_NUM			0x08//��Ƶ���򻺳���
#define MAX_PACK_QUENE_NUM			0x32//
#define DEFAULT_PACK_QUENE_NUM		0x05//

#define VER_MEDIASTREAM				(const char *)"mediastream 30.01.01.04.040708"


typedef int BOOL32;
typedef	int	INT32;

//Frame Header Structure
typedef struct
{
    u8     m_byMediaType; //ý������
    u8    *m_pData;       //���ݻ���
	u32    m_dwPreBufSize;//m_pData����ǰԤ���˶��ٿռ䣬���ڼ�
	// RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12
	// (FIXED HEADER + Extence option + Extence bit)
    u32    m_dwDataSize;  //m_pDataָ���ʵ�ʻ����С
    u8     m_byFrameRate; //����֡��,���ڽ��ն�
	u32    m_dwFrameID;   //֡��ʶ�����ڽ��ն�
	u32    m_dwTimeStamp; //ʱ���, ���ڽ��ն�
    union
    {
        struct{
			BOOL32    m_bKeyFrame;    //Ƶ֡����(I or P)
			u16       m_wVideoWidth;  //��Ƶ֡��
			u16       m_wVideoHeight; //��Ƶ֡��
		}m_tVideoParam;
        u8    m_byAudioMode;//��Ƶģʽ
    };
}FRAMEHDR,*PFRAMEHDR;

/*�������*/
typedef struct
{
	u32		m_dwRTPAddr;/*RTP��ַ(������)*/
	u16		m_wRTPPort;/*RTP�˿�(������)*/
	u32		m_dwRTCPAddr;/*RTCP��ַ(������)*/
	u16		m_wRTCPPort;/*RTCP�˿�(������)*/

	u32		m_dwContext1;
	
}TNetSession;

/*�����������*/
typedef struct
{
	TNetSession  m_tLocalNet;
	u32			 m_dwRtcpBackAddr;/*RTCP�ط���ַ(������)*/
	u16          m_wRtcpBackPort;/*RTCP�ط��˿�(������)*/        
}TLocalNetParam;

/*���緢�Ͳ���*/
typedef struct
{
	u8	m_byNum;		   /*ʵ�ʵ�ַ����*/
	TNetSession m_tLocalNet;/*���ص�ַ��*/
	TNetSession m_tRemoteNet[MAX_NETSND_DEST_NUM];/*Զ�˵�ַ��*/
}TNetSndParam;

/*����ģ��״̬��Ϣ*/
typedef struct
{
	u8				m_byMediaType;/*ý������*/
	u32				m_dwMaxFrameSize;/*����֡��С*/
	u32  			m_dwNetBand;/*���ʹ���*/
    u32				m_dwFrameID; /*����֡��ʶ*/
	u8				m_byFrameRate;/*����Ƶ��*/
    TNetSndParam    m_tSendAddr;/*���͵�ַ*/
}TSndStatus;

/*����ģ��ͳ����Ϣ*/
typedef struct
{
    u32		  m_dwPackSendNum;		/*�ѷ��͵İ���*/
	u32		  m_dwFrameNum;			/*�ѷ��͵�֡��*/
	u32	      m_dwFrameLoseNum;	    /*���ڻ�������ԭ����ɵķ��͵Ķ�֡��*/
}TSndStatistics;

/*����ģ��״̬��Ϣ*/
typedef struct
{
	BOOL	          m_bRcvStart;/*�Ƿ�ʼ����*/
    u32               m_dwFrameID;/*����֡ID*/
    TLocalNetParam    m_tRcvAddr;/*���յ��ص�ַ*/
}TRcvStatus;

/*������ͳ����Ϣ*/
typedef struct
{
	u32        m_dwPackNum;/*�ѽ��յİ���*/
    u32        m_dwPackLose;/*�G����*/
    u32        m_dwPackIndexError;/*��������*/
	u32        m_dwFrameNum;/*�ѽ��յ�֡��*/
	//u32	   m_dwFrameLoseNum;/*��������Դ�л���ն��е�ԭ����ɵĽ��յĶ�֡��*/
}TRcvStatistics;

typedef struct
{
    u8	     m_byMark;					//�Ƿ�֡�߽�1��ʾ���һ��
    u8       m_byExtence;				//�Ƿ�����չ��Ϣ
    u8       m_byPayload;				//�غ�
    u32		 m_dwSSRC;					//ͬ��Դ
    u16      m_wSequence;				//���к�
    u32      m_dwTimeStamp;				//ʱ���
    u8       *m_pExData;                //��չ����
    INT32    m_nExSize;					//��չ��С:sizeof(u32)�ı�����
    u8		 *m_pRealData;				//ý������ 
    INT32    m_nRealSize;				//���ݴ�С 
    INT32    m_nPreBufSize;				//m_pRealDataǰԤ����Ŀռ�;
}TRtpPack;

typedef struct
{
    u16  m_wFirstTimeSpan;	 //��һ���ش�����
	u16  m_wSecondTimeSpan;  //�ڶ����ش�����
	u16  m_wThirdTimeSpan;   //�������ش�����
	u16  m_wRejectTimeSpan;  //���ڶ�����ʱ����
}TRSParam;

//���Ͷ˸߼����ò���
typedef struct
{
	s32      m_nMaxSendNum; //���ݴ�����������ʹ���;	
	BOOL32	 m_bRepeatSend; //���� (mp4/H.264) �Ƿ��ط�
	u16      m_wBufTimeSpan;
}TAdvancedSndInfo;

//���ն˸߼����ò���
typedef struct
{
	BOOL32	  m_bConfuedAdjust;  //���� (mp3) �Ƿ����������	
	BOOL32	  m_bRepeatSend;     //���� (mp4/H.264) �Ƿ��ط�
	TRSParam  m_tRSParam;
}TAdvancedRcvInfo;

struct ifly_netsnd_t;
struct ifly_netrcv_t;

typedef struct
{
	struct ifly_netsnd_t *m_pcNetSnd;
	SEMHANDLE m_hSndSynSem;  //���ڶ���ĵ��̲߳�����ͬ����
}ifly_mediasnd_t;

typedef struct
{
	struct ifly_netrcv_t *m_pcNetRcv;
	SEMHANDLE m_hRcvSynSem;  //���ڶ���ĵ��̲߳�����ͬ����
}ifly_mediarcv_t;

//ȫ��MediaSnd�б�ṹ
typedef struct
{
	s32 m_nMediaSndCount;               //���Ͷ������� <= MAX_SND_NUM
	ifly_mediasnd_t *m_tMediaSndUnit[MAX_SND_NUM];
}TMediaSndList;

//ȫ��MediaRcv�б�ṹ
typedef struct
{
	s32 m_nMediaRcvCount;               //���ն������� <= FD_SETSIZE
	ifly_mediarcv_t *m_tMediaRcvUnit[FD_SETSIZE];
}TMediaRcvList;

//Frame Rcv CallBack Function
typedef void (*PFRAMEPROC)(PFRAMEHDR pFrmHdr, u32 dwContext);
//RTP PACK Call back
typedef void (*PRTPCALLBACK)(TRtpPack *pRtpPack, u32 dwContext);

#ifdef __cplusplus
extern "C" {
#endif

//ģ��汾��Ϣ�ͱ���ʱ�� �� �����Ŀ�İ汾�ͱ���ʱ��
void mediastreamver();

//ģ�������Ϣ �� �����Ŀ�İ�����Ϣ
void mediastreamhelp();

u16  mediastreamStartup();

BOOL mediastreamCleanup();

//��������ģ��
ifly_mediasnd_t* CreateMediaSnd(u32 dwMaxFrameSize, u32 dwNetBand, u8 byFrameRate, u8 byMediaType, u32 dwSSRC);

//ɾ������ģ��
u16 DestroyMediaSnd(ifly_mediasnd_t* pMediaSnd);

//�������緢�Ͳ���(���еײ��׽��ֵĴ������󶨶˿�,�Լ�����Ŀ���ַ���趨�ȶ���)
u16 SetMediaSndNetParam(ifly_mediasnd_t* pMediaSnd,TNetSndParam tNetSndParam);

//�Ƴ����緢�ͱ��ص�ַ����(���еײ��׽��ֵ�ɾ�����ͷŶ˿ڵȶ���)
u16 RemoveMediaSndLocalNetParam(ifly_mediasnd_t* pMediaSnd);

//����֡ID
u16 ResetMediaSndFrameId(ifly_mediasnd_t* pMediaSnd);
//����ͬ��ԴSSRC
u16 ResetMediaSndSSRC(ifly_mediasnd_t* pMediaSnd,u32 dwSSRC);

//���÷��Ͷ˶���mpeg4����H.264���õ��ش�����Ŀ���,�رպ󣬽������Ѿ����͵����ݰ����л���
u16 ResetMediaSndRSFlag(ifly_mediasnd_t* pMediaSnd, BOOL32 bRepeatSnd);

//���÷���ѡ��
u16 SetMediaSndInfo(ifly_mediasnd_t* pMediaSnd,u32 dwNetBand, u8 byFrameRate);
//�������ݰ�
u16 SendMediaFrame(ifly_mediasnd_t* pMediaSnd,PFRAMEHDR pFrmHdr);

//�õ�״̬
u16 GetMediaSndStatus(ifly_mediasnd_t* pMediaSnd,TSndStatus *pSndStatus);
//�õ�ͳ��
u16 GetMediaSndStatistics(ifly_mediasnd_t* pMediaSnd,TSndStatistics *pSndStatistics);
//�õ����Ͷ˸߼����ò���(�ش���)
u16 GetMediaSndAdvancedInfo(ifly_mediasnd_t* pMediaSnd,TAdvancedSndInfo *pAdvancedSndInfo);


//��������ģ��
ifly_mediarcv_t* CreateMediaRcv(u32 dwMaxFrameSize, PFRAMEPROC pFrameCallBackProc, u32 dwContext, u32 dwSSRC);
ifly_mediarcv_t* CreateMediaRcvRtp(u32 dwMaxFrameSize, PRTPCALLBACK PRtpCallBackProc, u32 dwContext, u32 dwSSRC);

//���ý��յ�ַ����(���еײ��׽��ӵĴ������󶨶˿ڵȶ���)
u16 SetMediaRcvLocalParam(ifly_mediarcv_t* pMediaRcv,TLocalNetParam tLocalNetParam);

//�Ƴ����յ�ַ����(���еײ��׽��ӵ�ɾ�����ͷŶ˿ڵȶ���)
u16 RemoveMediaRcvLocalParam(ifly_mediarcv_t* pMediaRcv);

//���ý��ն˶���mpeg4����H.264���õ��ش�����Ŀ���,�رպ󣬽��������ش�����
u16 ResetMediaRcvRSFlag(ifly_mediarcv_t* pMediaRcv,TRSParam tRSParam, BOOL bRepeatSnd);

//��ʼ����
u16 StartMediaRcv(ifly_mediarcv_t* pMediaRcv);
//ֹͣ����
u16 StopMediaRcv(ifly_mediarcv_t* pMediaRcv);
//�õ�״̬
u16 GetMediaRcvStatus(ifly_mediarcv_t* pMediaRcv,TRcvStatus *pRcvStatus);
//�õ�ͳ��
u16 GetMediaRcvStatistics(ifly_mediarcv_t* pMediaRcv,TRcvStatistics *pRcvStatistics);

//ɾ������ģ��
u16 DestroyMediaRcv(ifly_mediarcv_t* pMediaRcv);

#ifdef __cplusplus
}
#endif

#endif
