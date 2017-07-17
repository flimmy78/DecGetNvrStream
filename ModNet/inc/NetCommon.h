#ifndef __NETCOMMON_H_
#define __NETCOMMON_H_

#include <pthread.h>
#include "loopbuf.h"
#include "common_msgcmd.h"
#include "Netcomm.h"
#include "CtrlProtocol.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EACH_STREAM_TCP_LINKS 		5

//#define RECV_CMD_ERR_DELAY_US	(10*1000)//csp modify

//#define SEND_DELAY_DBG			1
//#define _NETCOMM_DEBUG
//#define _MODNETCOMM_ASSERT_TEST_

//#define PRINT_SND_TIME

#define dummy() do{}while(0)

#ifndef WIN32
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

void netcomm_assert( char * file_name, u32 line_no );

#ifdef _MODNETCOMM_ASSERT_TEST_  // ��ʹ�ö��Բ���
#define  NETCOMM_ASSERT( condition )\
do{if (condition) dummy();else netcomm_assert( __FILE__, __LINE__ );}while(0)
#else
#define NETCOMM_ASSERT(condition)
#endif  /* end of ASSERT */

#ifdef _NETCOMM_DEBUG
#define NETCOMM_DEBUG(x)\
	do { printf("\n[NETCOMM] %s %d %s %d\n", __FILE__, __LINE__, __FUNCTION__, (int)x);} while(0)

#define NETCOMM_DEBUG_STR(s,x)\
	do { printf("\n[NETCOMM] %s %d %s %s %d\n", __FILE__, __LINE__, __FUNCTION__, s, (int)x);} while(0)
#else
#define NETCOMM_DEBUG(x)

#define NETCOMM_DEBUG_STR(s,x)
#endif

// typedef 

/*
 Control block for sending stream 
*/
typedef struct _sNetCommSendStreamCtrl
{
	int		sockfd;//[EACH_STREAM_TCP_LINKS];
	u32 	m_dwFrameId;//[EACH_STREAM_TCP_LINKS];
	u32 	m_dwsubFrameId;//[EACH_STREAM_TCP_LINKS];
	u8 		bSub;//[EACH_STREAM_TCP_LINKS];
	#ifdef NETSND_OPT
	BOOL 	bKeyFrame;//[EACH_STREAM_TCP_LINKS];
	u32 	nBps;//[EACH_STREAM_TCP_LINKS];
	#endif
	u32     nLinkId; // id used when link create or link destroy
	u32		nLostFrame;
	pthread_mutex_t sockLock;
	//csp modify 20130423
	u32		VMonitorInfo;
	u32 	m_dwThirdFrameId;//[EACH_STREAM_TCP_LINKS];
	//csp modify 20140318
	u8		streamtype;
} SNCSSCtrl1Link, *PSNCSSCtrl1Link;

typedef struct _sNetCommSendStreamAllCtrl
{
	u8 bLost;
	SNCSSCtrl1Link sLinkCtrl[EACH_STREAM_TCP_LINKS];
	
	//csp modify 20130423
	pthread_mutex_t LinkCtrlLock;
}  SNCSSCtrl, *PSNCSSCtrl;

/*
 Exchange code style 
*/
typedef ifly_stearmsnd_t SCPSSCtrl, *PSCPSSCtrl;	// ctrlprotocol stream send control

typedef struct _sNetCommCtrl
{
	u8				bInit;
	EM_NET_CONN_STATE	eState;
	u32 			yFactor;// = PAL;
	PStreamRequestCB	pCB;
	PRequestKeyFrameCB pReqKeyCB;
	
//	PSNCSSCtrl		psVidTcpCtrl;	//[STREAM_TCP_CHANNELS];
//	PSNCSSCtrl		psAudTcpCtrl;	//[CHN_NUM_MAIN];
	
	//csp modify 20130423
	u32             *pnLostFrame[3]; // 0 main stream, 1 sub stream, 2 third stream
	
	PSCPSSCtrl		psSendCtrl;		//[STREAM_LINK_MAXNUM];
	
	pthread_t 		previewThxd;	// preview thread for a/v stream
	pthread_t 		cmdThxd;		// thread to process cmd from ctrlprotocol
	
	SMsgCmdHdr		pMsgCmd;		// cmd struct for cmd from ctrlprotocol
	
	u16 			nAudStreamNum;// = pCfg->nSubStreamMax;
	u16 			nSubStreamNum;// = pCfg->nSubStreamMax;
	u16 			nChnTcpStreamNum;// = pCfg->nSubStreamMax;
	u16 			nVidStreamNum;// = pCfg->TCPMaxConn;
	u16 			nTotalLinkNum;// = pCfg->nAllLinkMax;
	
	ifly_msgQ_t 	netsndMsgQ;		// stream buff queue
	ifly_msgQ_t 	netsndMbMsgQ;	// stream buff queue
	ifly_msgQ_t 	netsndAudioMsgQ;// stream buff queue	//csp modify
	
	PNetLogWriteCB	pWriteLogCB;
	struct sockaddr_in	linkIP;
	
	SNetCommCfg  	sCommonCfg;
	
	SNetMultiCastInfo	sMultiCastInfo;
	
	PFRemoteOp		pRFormat, pRUpdate;
} SNetCommCtrl, *PSNetCommCtrl;

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
			int    m_bKeyFrame;    //Ƶ֡����(I or P)
			u16       m_wVideoWidth;  //��Ƶ֡��
			u16       m_wVideoHeight; //��Ƶ֡��
		}m_tVideoParam;
        u8    m_byAudioMode;//��Ƶģʽ
    };
}FRAMEHDR,*PFRAMEHDR;

// extern

s32 netComm_Ack( u32 err, int sock, u32 nAckId );
BOOL netComm_CheckConnLost();

#ifdef __cplusplus
}
#endif

#endif // __NETCOMMON_H_

