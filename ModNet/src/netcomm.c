// file description
#include <pthread.h>
#include <time.h>
#include "netcomm.h"
#include "netcommon.h"
#include "loopbuf.h"
#include "CmdProc.h"
#include "remotePreview.h"
#include "remotePlay.h"
#include "remoteDown.h"
#include "remoteUpdate.h"
#include "mydaemon.h"
#include "mail.h"
#include <sys/syscall.h>
#include <unistd.h>

#include <signal.h>

#include "ddns.h"


//** macro

#define ETH_NAME "eth0"

//** typedef 
typedef void (*PTTHREAD_ROUTINE)(void* para);

typedef enum
{
	FRAME_TYPE_P = 1, /*PSLICE types*/
	FRAME_TYPE_I = 5  /*ISLICE types*/
}venc_frame_type_e;

/*
 TCP stream request struct
*/
typedef struct _sNetCommTcpStreamReq
{
	u8 	chn;									//command(0-1)��Ч
	u8 	command;								//0 Ԥ����Ƶ; 1 Ԥ����Ƶ;2 �ļ��ط�;3 ʱ��ط�;4 �ļ�����;5 ��������;6 ������� 
	u32 id;										//��Ӧcommand: cmd(0-1), id=0; cmd(2-4) id = ��������ص�id; cmd(5-6), id = �����ļ���С  
} SNCTSReq, *PSNCTSReq;

// cmd data struct
typedef struct _sNCCmdData
{
	SOCKHANDLE	sock;	// new sock for new connect req 
	ifly_TCP_Stream_Req 	req;	// req struct from ctrlprotocol
	void* pContext;		// context for current command from ctrlprotocol
} SNCCmdData, *PSNCCmdData;

//** pppoe
#define PPPOE_ETH_NAME	"ppp0"
typedef enum
{
	EM_PPPOE_Down,
	EM_PPPOE_PreUp,
	EM_PPPOE_Up,
	EM_PPPOE_MAX
} EM_PPPOE_STATUS;

typedef struct _sPppoeCfg
{
	char			szEthName[10];
	u8				nFlag;			// enable
	EM_PPPOE_STATUS eStatus;
	u8				nDisconnNum;
	u32				nLastDiscTime;
	char			szUser[64];
	char			szPasswd[64];
	char			szIP[24];
	char			szSubMask[24];
	char			szGateway[24];
	char			szDNS[24];
} SPppoeCfg;


// ddns ctrl
typedef struct _sDdnsCfg
{
	EM_NET_DDNS	 eProt;
	u8			 nDdnsFlag; // 0 down 1 up
	u8			 bStart; // 0 no, 1 yes
	u8			 bInit; // 0 no, 1 yes
	SDDNSBaseInfo	sInfo;
} SDdnsCfg;

//** local var

static SPppoeCfg 		sPPPoECfg;
static SDdnsCfg 		sDdnsCfg;
static SNetConnStatus 	sNetConnStatus;

//** global var

SNetCommCtrl sNetCommCtrl;

#if 1 //luo
#define AUTYPENUM	 13
//#define MAX_ALARM_UPLOAD_NUM 5

static int upload_alarm_fd[2] = {-1, -1};
static pthread_mutex_t upload_alarm_mutex = PTHREAD_MUTEX_INITIALIZER;

//u32 alarmupload[AUTYPENUM];
/*

	0       �ź�������
	1       Ӳ��������
	2       ��Ƶ��ʧ����
	3       �ƶ���ⱨ��
	4       Ӳ��δ��ʽ������
	5       ��дӲ�̳�����
	6       IPC�ڸǱ���
	7       ��ʽ��ƥ�䱨��
	8       �Ƿ����ʱ���
	9	IPC�ⲿ����
	10	485��չ����
	
*/
//yaogang modify 20141118
/*
0		//Ӳ��δ��ʽ������
1		//Ӳ�̶�ʧ����
2		//Ӳ�̶�д������
3		//Ӳ��������
4		//���������Ӳ��
5		//�ź�������
6		//��Ƶ��ʧ����
7		//�ƶ���ⱨ��
8		//IPC�ڸǱ���
9		//��ʽ��ƥ�䱨��
10		//�Ƿ����ʱ���
11		//IPC�ⲿ����
12		//485��չ����

*/


typedef struct 
{
 u8 type;      //����
 			//9-IPCEXT
 u8 state;       //  1���� 2�ָ�
 u8 id;        //ͨ��,Ӳ�̺�,���������,ȡ����type 
 u16 reserved1;      //Ԥ��
 u32  reserved2;      //Ԥ��
}ifly_alarmstate_t;

ifly_alarmstate_t m_alarmstate;
ifly_AlarmUploadCenter_t g_AlarmUploadCenter[MAX_ALARM_UPLOAD_NUM];
	
#endif

//** local functions
static void netcomm_ConfigNetwork(SNetPara* pConfig);
static void netComm_DealStreamCmd(SOCKHANDLE sock,  ifly_TCP_Stream_Req* pReq, u32 ip);
static s32 netComm_PPPOE_Start( u8 flag, char *pszUser, char* pszPasswd );
static s32 netComm_NotifyPppoeState( u8 bState );
static BOOL netComm_SetPPPOEUser(char *username,char *passwd);
static s32 netComm_DHCP_Stop();
static s32 netComm_DHCP_Start();
//static BOOL netComm_ReadNetWorkParam(SNetCommCfg* psNCCfg);
static int AddStreamTCPLink(SOCKHANDLE hSock, ifly_TCP_Stream_Req req, void* pContext);
//static void* netCommCmd_ThreadFunc(void* param);
static s32 netComm_Webs_Start( u16 nPort );
//static s32 netComm_Webs_Restart( u16 nPort );
static int	netComm_ReplaceWebRow(const char *fname, const char *src, char *dest);
static void netComm_SaveServerPort(u16 server_port);
//static BOOL netComm_CheckNetLink();
//static void *netComm_CheckState(void *param);
static void* netComm_DDNS_Init(void* para);

static void* netComm_AlarmUpload_init(void* para);//luo

static s32 netComm_Webs_ChangeWebJs( u16 nPort );
static int SendMsgToDaemon(my_daemon_param *pDaemon);

s32 NetComm_ReqConnStatus(SNetConnStatus* psNCS);

//** global functions

BOOL CheckNetLink2(char* pIFName);
BOOL CheckNetLink(void);
u32 GetLocalIp();
u32 GetLocalIp2(char * name);
int SetLocalIp(u32 dwIp);
int GetHWAddr(char *pBuf);
int SetHWAddr(char *pBuf);
u32 GetRemoteIP();
u32 GetNetMask();
u32 GetNetMask2(char * name);
int SetNetMask(u32 dwIp);
u32 GetBroadcast();
int SetBroadcast(u32 dwIp);
u32 GetDefaultGateway();
int SetDefaultGateway(u32 dwIp);
u32 GetDNSServer();
int GetDNSServer2(u32 *pDns, u32 *pDns1);
int	SetDNSServer(u32 dwIp);
int AddDNSServer(u32 dwIp);
u16 		GetNewIdNum();
extern void netcomm_assert( char * file_name, u32 line_no );
extern void cmdPro_Init( PNetCommCommandDeal2 p, u8 nChMax );

void* rtsp_init(void* arg);	// rtsp support

/*
void term_exit(int signo)
{
	time_t cur;
	cur = time(NULL);
	//printf("term_exit:system time:%s\n",  ctime(&cur));	
	printf("!!!!!!recv signal(%d),SIGBUS=%d,SIGPIPE=%d,,%s\n",signo,SIGBUS,SIGPIPE,ctime(&cur));
	if(signo != 17)//�ӽ��̽���
	{
		//sleep(10);
		printf("process quit!!!\n");
		exit(-1);
	}
}
*/

// init ddns para
typedef struct _DDNSInitPara {
	u8 	flag;
	u16 interval;		
	SNetDdnsPara sStart;
} DDNSInitPara;
static 	DDNSInitPara initPara;

/*
 ��ʼ��netcomm module
*/

disk_manager* pHddMgr = NULL;

void* NetCommInitThxd( void* para )
{
	printf("$$$$$$$$$$$$$$$$$$NetCommInitThxd id:%d\n",getpid());
	
	//printf("NetCommInitThxd-1\n");
	
	sleep(5);//why???
	
	//printf("NetCommInitThxd-2\n");
	
	//u8  			i;
	u32				i;	
	u8 				nSendBufNum, nChnNum;
	u16 			nServerPort 	= 0;
	u32				nFrameSizeMax	= 0;
	s32 			errCode 		= 0;
	PSNetCommCtrl	pNCCIns			= NULL;
	SMsgCmdHdr		pCmd			= NULL;
	pthread_t 		cmdThxd 		= 0;
	PSNetCommCfg 	pCfg 			= (PSNetCommCfg)para;
	
	PStreamRequestCB 	pCB 		= pCfg->pReqStreamCB;
	PRequestKeyFrameCB 	pReqKeyCB  	= pCfg->pReqKeyCB;
	PRemotePlayCB 		pRpCB 		= pCfg->pRpCB;
	PNetCommVOIPOut 	pVOIP 		= pCfg->pVOIPOut;
	PNetCommVOIPIn 		pVOIPIn  	= pCfg->pVOIPIn;
	
	NETCOMM_DEBUG_STR("netinit", 0);
	
	NETCOMM_ASSERT(pCB==NULL || NULL!=pCfg);
	//
	pNCCIns = &sNetCommCtrl;
	
	//printf("NetCommInitThxd - 1, CheckNetLink:%d\n",CheckNetLink());
	
	// save serverport to file
	netComm_SaveServerPort(pCfg->sParaIns.TCPPort);
	
	// write config
	memcpy(&pNCCIns->sCommonCfg, pCfg, sizeof(SNetCommCfg));
	
	//printf("NetCommInitThxd - 1.1, CheckNetLink:%d\n",CheckNetLink());
	
	// set network para at the system beginning..
	netcomm_ConfigNetwork(&pCfg->sParaIns);
	
	//printf("NetCommInitThxd - 1.2, CheckNetLink:%d\n",CheckNetLink());
	
	// create a/v stream tx-ctrl
	u16 nAudStreamNum = pCfg->nSubStreamMax;
	u16 nVidStreamNum = pCfg->sParaIns.TCPMaxConn;
	u16 nTotalLinkNum = pCfg->nAllLinkMax;
	
	printf("NetCommInitThxd:nAudStreamNum=%d,nVidStreamNum=%d,nTotalLinkNum=%d,nChnMax=%d\n",nAudStreamNum,nVidStreamNum,nTotalLinkNum,pCfg->nChnMax);
	
	//sNetCommCtrl.sCommonCfg.pDiskMgr = pCfg->pDiskMgr;
	pHddMgr = pCfg->pDiskMgr;
	
	pNCCIns->nAudStreamNum = nAudStreamNum;
	pNCCIns->nChnTcpStreamNum = pNCCIns->nAudStreamNum;
	pNCCIns->nSubStreamNum = pNCCIns->nAudStreamNum;
	pNCCIns->nVidStreamNum = nVidStreamNum;
	pNCCIns->nTotalLinkNum = nTotalLinkNum;
	
	// main/sub stream lost frame ctrl
	for(i=0; i<3; i++)//csp modify 20130423
	{
		pNCCIns->pnLostFrame[i] = malloc(sizeof(u32)*pCfg->nChnMax);
		if(NULL==pNCCIns->pnLostFrame[i])
		{
			NETCOMM_DEBUG_STR("allocate memory for pnLostFrame failed!!\n", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
		
		//csp modify 20130423
		memset(pNCCIns->pnLostFrame[i],0,sizeof(u32)*pCfg->nChnMax);
	}
	
	pNCCIns->psSendCtrl	  = NULL;
	pNCCIns->psSendCtrl   = malloc(nTotalLinkNum*sizeof(SCPSSCtrl));
	if( NULL == pNCCIns->psSendCtrl )
	{
		NETCOMM_DEBUG_STR("allocate memory for stream block failed!!\n", -1);
		errCode = -1;
		goto NCINIT_ERR;
	}
	
	// create queue for stream get
	nSendBufNum		= pCfg->nSendBufNumPerChn;
	nChnNum			= pCfg->nSubStreamMax;// default chnnum == substreamnum
	nFrameSizeMax	= pCfg->nFrameSizeMax;
	
#if 0//csp modify
	if( !initMsgQ(&pNCCIns->netsndMsgQ, 
			nSendBufNum * nChnNum, 
			nFrameSizeMax * nChnNum,
			64 ) )
	{
		NETCOMM_DEBUG_STR("initMsgQ", -1);
		errCode = -1;
		goto NCINIT_ERR;
	}
	
	// audio buff
	if( !initMsgQ(&pNCCIns->netsndAudioMsgQ, 
			nSendBufNum * nChnNum, 
			nFrameSizeMax * nChnNum,
			64 ) )
	{
		NETCOMM_DEBUG_STR("initMsgQ for audio", -1);
		errCode = -1;
		goto NCINIT_ERR;
	}
	
	printf("#############################initMsgQ bufnum %d chnnum %d framesizemax %d MsgQLen %d\n", nSendBufNum, nChnNum, nFrameSizeMax, nFrameSizeMax * nChnNum);
#else
	if(nChnNum == 8)
	{
		#if defined(_JMV_) || defined(_JUAN_)//R9508S
		//csp modify 20121016//������Ҫ��֤
		if(!initMsgQ(&pNCCIns->netsndMsgQ, nSendBufNum * nChnNum * 2, nFrameSizeMax * nChnNum * 3 / 2, 64))
		{
			NETCOMM_DEBUG_STR("initMsgQ", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
		#else
		//csp modify 20121016//������Ҫ��֤
		int msgnum = nSendBufNum * nChnNum * 2 * 6;//csp modify 20140315
		//if(!initMsgQ(&pNCCIns->netsndMsgQ, nSendBufNum * nChnNum * 2, nFrameSizeMax * nChnNum * 2, 64))
		if(!initMsgQ(&pNCCIns->netsndMsgQ, msgnum, nFrameSizeMax * nChnNum * 2, 64))
		{
			NETCOMM_DEBUG_STR("initMsgQ", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
		#endif
	}
	else
	{
		//csp modify 20121016//������Ҫ��֤
		int msgnum = nSendBufNum * nChnNum * 2 * 6;//csp modify 20140315
		//if(!initMsgQ(&pNCCIns->netsndMsgQ, nSendBufNum * nChnNum * 2, nFrameSizeMax * nChnNum * 2, 64))
		if(!initMsgQ(&pNCCIns->netsndMsgQ, msgnum, nFrameSizeMax * nChnNum * 2, 64))
		{
			NETCOMM_DEBUG_STR("initMsgQ", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
	}
	
	//4SDI:#############################initMsgQ bufnum 6 chnnum 4 framesizemax 1048576 MsgQLen 8388608
	printf("#############################initMsgQ bufnum %d chnnum %d framesizemax %d MsgQLen %d\n", nSendBufNum * 2, nChnNum, nFrameSizeMax, nFrameSizeMax * nChnNum * 2);
#endif
	
	if(nChnNum == 8)
	{
		#if defined(_JMV_) || defined(_JUAN_)//R9508S
		if( !initMsgQ(&pNCCIns->netsndMbMsgQ, 
			nSendBufNum * nChnNum, 
			nFrameSizeMax * nChnNum * 3 / 4,
			64 ) )
		{
			NETCOMM_DEBUG_STR("initMsgQ for mobile", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
		#else
		if( !initMsgQ(&pNCCIns->netsndMbMsgQ, 
			nSendBufNum * nChnNum, 
			nFrameSizeMax * nChnNum,
			64 ) )
		{
			NETCOMM_DEBUG_STR("initMsgQ for mobile", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
		#endif
	}
	else
	{
		if( !initMsgQ(&pNCCIns->netsndMbMsgQ, 
			nSendBufNum * nChnNum, 
			nFrameSizeMax * nChnNum,
			64 ) )
		{
			NETCOMM_DEBUG_STR("initMsgQ for mobile", -1);
			errCode = -1;
			goto NCINIT_ERR;
		}
	}
	
	// init cmdproc
	#ifndef DEBUG_TEMP
	cmdPro_Init(NULL, pCfg->nChnMax);
	#endif
	
	//printf("initMsgQ over\n");
	
	//printf("NetCommInitThxd - 2, CheckNetLink:%d\n",CheckNetLink());
	
	// preview init
	if( remotePreview_Init( pCfg->yFactor,
							pNCCIns->nChnTcpStreamNum, 
							nVidStreamNum, 
							nAudStreamNum,
							pCfg->nVoipMode,
							pCfg->nAudioMediaType,
							pCfg->nVideoMediaType,
							pCB,
							pReqKeyCB,
							pVOIP,
							pVOIPIn )
	)
	{
		errCode = -1;
		goto NCINIT_ERR;
	}
	
	//printf("remotePreview_Init over\n");
	
	// init remote play
	errCode = remotePlay_Init(
		pRpCB,
		pCfg->nVideoMediaType,
		pCfg->nAudioMediaType,
		pCfg->nVideoFrameRate,
		pCfg->nAudioFrameRate,
		pCfg->nAudioMode,
		pCfg->nChnMax
	);
	if(0!=errCode)
	{
		errCode = -1; // remotePlay_Init init failed
		goto NCINIT_ERR;
	}
	//printf("remotePlay_Init over\n");
	
	// down init
	remoteDownload_Init(pRpCB);
	//printf("remoteDownload_Init over\n");
	
	// update init
	remoteUpdate_Init(pCfg->pDiskMgr);
	//printf("remoteUpdate_Init over\n");
	
	// init psSendCtrl
	PSCPSSCtrl pCtrl = pNCCIns->psSendCtrl;
	
	for(i=0; i<nTotalLinkNum; i++)
	{
		pCtrl[i].sockfd = INVALID_SOCKET;
	}
	
	// create netcomm module cmd process thread
	// create cmd struct 
	pCmd = CreateMsgCmd( sizeof(SNCCmdData) );
	if(NULL==pCmd)
	{
		NETCOMM_DEBUG_STR("CreateMsgCmd failed!!!", -1);
		errCode = -1; // create cmmand fail
		goto NCINIT_ERR;
	}
	
	NETCOMM_DEBUG_STR("CreateMsgCmd ", 0);
	
	//printf("CreateMsgCmd over\n");
	
	//set callback
	pNCCIns->pCB = pCB;
	SetAddStreamLinkCB(AddStreamTCPLink, &pNCCIns->linkIP);
	SetMsgCallBack(DealCommand, NULL);
	
	NETCOMM_DEBUG_STR("SetAddStreamLinkCB&SetMsgCallBack ", 0);
	
	//printf("SetAddStreamLinkCB&SetMsgCallBack over\n");
	
	//Alarm upload init
	//luo
	// ���ڼ����̳߳�ʼ��֮ǰ��������ԴΪ��ʼ���ͽ��յ������ϴ���������Ч��Դ���в��� byspliang		
	#if 1
	pthread_t thxalarmupload;
	errCode = pthread_create(&thxalarmupload,
							 NULL,
							 netComm_AlarmUpload_init,
							 (void*)pHddMgr);
	if(0 != errCode)
	{
		//pthread_exit(&thxalarmupload);
		
		printf("alarmUpload_init error!\n");
		
		errCode = -1;// fail to create rtsp thread
		
		goto NCINIT_ERR;
	}
	#endif
	
	//printf("NetCommInitThxd - 3, CheckNetLink:%d\n",CheckNetLink());
	
	// init ctrlprotocol	
	nServerPort = pCfg->sParaIns.TCPPort;
	errCode = CPLibInit(nServerPort);
	if( CTRL_SUCCESS!=errCode )
	{
		errCode = -1; // ctrlprotocol init failed
		goto NCINIT_ERR;
	}
	
	if( pCfg->sParaIns.DhcpFlag )
	{
		netComm_DHCP_Start();
	}
	
	//printf("CPLibInit over\n");
	
	// mobile init
    if( 0 != mobile_init(
    	pCfg->sParaIns.MobilePort, 
    	pCfg->nSubStreamMax, 
    	pCfg->nSubStreamMax, 
    	pCfg->pReqStreamCB,
		pCfg->nVideoMediaType,
		pCfg->yFactor
    ) )
    {
		NETCOMM_DEBUG_STR("mobile_init", -1);
		errCode = -1;
		goto NCINIT_ERR;
    }
	
    #if 0
	// rtsp init
	pthread_t thxRtsp = 0;
	printf("rtsp para nVidStreamNum %d \n", nVidStreamNum);
	errCode = pthread_create(&thxRtsp,
							 NULL,
							 rtsp_init,
							 (void*)(u32)nVidStreamNum);
	if(0 != errCode)
	{
		//pthread_exit(&thxRtsp);
		errCode = -1;//fail to create rtsp thread
		goto NCINIT_ERR;
	}
	#endif
	
	//printf("NetCommInitThxd - 4, CheckNetLink:%d\n",CheckNetLink());
	
	// Server_Tcp_Daemon??
	// nat_fxn ??
	
	NETCOMM_DEBUG_STR("CPLibInit ok, chech the port :  ", 0);
	NETCOMM_DEBUG(nServerPort);
	
	pNCCIns->pMsgCmd = pCmd;
	
	NETCOMM_DEBUG_STR("netCommCmd_ThreadFunc created, ok! ", 0);
	
	pNCCIns->cmdThxd = cmdThxd;
	
	// register writelog callback
	pNCCIns->pWriteLogCB = pCfg->pWriteLogCB;
	if(NULL == pNCCIns->pWriteLogCB)
	{
		NETCOMM_DEBUG_STR("null logwrite callback pointer", -1);
		exit(1);
	}
	// register update & format callback
	pNCCIns->pRFormat = pCfg->pRFomat;
	pNCCIns->pRUpdate = pCfg->pRUpdate;
	if(NULL == pNCCIns->pRFormat || NULL == pNCCIns->pRUpdate)
	{
		NETCOMM_DEBUG_STR("null remote update/format callback pointer", -1);
		exit(1);
	}
	
	//printf("NetCommInitThxd - 5, CheckNetLink:%d\n",CheckNetLink());
	
	// change web js
	netComm_Webs_ChangeWebJs(nServerPort);
	
	// web start
	netComm_Webs_Start(pCfg->sParaIns.HttpPort);
	
	// ddns init
	memset(&initPara, 0, sizeof(initPara));
	initPara.flag = pCfg->sParaIns.DDNSFlag;
	initPara.interval = pCfg->sParaIns.UpdateIntvl;
	strcpy(initPara.sStart.szDomain, (char *)pCfg->sParaIns.DDNSDomain);
	strcpy(initPara.sStart.szUser, (char *)pCfg->sParaIns.DDNSUser);
	strcpy(initPara.sStart.szPasswd, (char *)pCfg->sParaIns.DDNSPasswd);
	
	//printf("NetCommInitThxd - 6, CheckNetLink:%d\n",CheckNetLink());
	
	pthread_t ddnsInitThread;
	pthread_create(&ddnsInitThread, NULL, netComm_DDNS_Init, &initPara);
	
	// pppoe auto dial ...
	if(pCfg->sParaIns.PPPOEFlag)
	{
		netComm_PPPOE_Start(1, 
			(char*)pCfg->sParaIns.PPPOEUser, 
			(char*)pCfg->sParaIns.PPPOEPasswd
		);
	}
	else
	{
		//csp modify 20130321
		if(pCfg->sParaIns.UPNPFlag)
		{
			printf("here,start upnp\n");
			NetComm_RestartUPnP(pCfg->sParaIns.UPNPFlag);
		}
	}

	//yaogang modify 20150324 //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	if (pCfg->nNVROrDecoder == 1)
	{
		//yaogang modify 20141209 ���ƽ̨
		net_sg_init(&pNCCIns->sCommonCfg);
	}
	
NCINIT_ERR:
	if(errCode!=0)
	{
		NETCOMM_DEBUG_STR("NetCommInit failed!!!",errCode);
	}
	else
	{
		pNCCIns->bInit = 1;
		NETCOMM_DEBUG_STR("NetCommInit success!!!",errCode);
	}
	
	return NULL;
}

s32 NetCommInit( PSNetCommCfg pCfg )
{
	printf("NetCommInit......\n");
	pthread_t netThread;
	return pthread_create(&netThread, NULL, NetCommInitThxd, (void*)pCfg);
}

//csp modify 20130423
static SNetThirdStreamProperty g_sNetThirdStreamProperty;
s32 NetComm_SetThirdStreamProperty(SNetThirdStreamProperty *psProperty)
{
	if(psProperty == NULL)
	{
		return -1;
	}
	g_sNetThirdStreamProperty = *psProperty;
	return 0;
}
s32 NetComm_GetThirdStreamProperty(SNetThirdStreamProperty *psProperty)
{
	if(psProperty == NULL)
	{
		return -1;
	}
	*psProperty = g_sNetThirdStreamProperty;
	return 0;
}

#if SEND_DELAY_DBG // def PRINT_SND_TIME
struct timeval start,end;
long span;
#endif

s32 NetCommSendPreviewFrame(PSNetComStmHead pHead, u8* pBuf)
{
	u8*	pQBuf = NULL;
	u32 nDataLen = 0;
	PSNetCommCtrl pNCCIns = NULL;
	
	pNCCIns = &sNetCommCtrl;
	
	if(NULL==pHead || NULL==pBuf)
	{
		NETCOMM_DEBUG_STR("met null pointer", -1);
		//printf("~~~~~~~~~null!\n");
		return -1;
	}
	
	switch(pHead->eFrameType)
	{
		case EM_TALK:
		{
			if(pHead->eFrameType == EM_TALK)
			{
				SVOIPFrame sFIns;
				
				sFIns.nMediaType = pHead->byMediaType;
				sFIns.nAudioMode = pHead->nAudioMode;
				sFIns.nFrameRate = pHead->nFrameRate;
				sFIns.nTStamp    = pHead->timeStamp;
				sFIns.nSize      = pHead->dwlen;
				sFIns.pData      = pBuf;
				
				return RemoteVoipSendData(&sFIns);
			}
		} break;
		case EM_AUDIO:
		{
			//fill msg buf
			nDataLen = pHead->dwlen+sizeof(SNetComStmHead);
			
			//csp modify
			//if(GetMsgQWriteInfo(&pNCCIns->netsndAudioMsgQ, &pQBuf, &nDataLen))
			if(GetMsgQWriteInfo(&pNCCIns->netsndMsgQ, &pQBuf, &nDataLen))
			{
				memcpy(pQBuf, pHead, sizeof(SNetComStmHead));
				memcpy(pQBuf+sizeof(SNetComStmHead), pBuf, nDataLen);
				
				//csp modify
				//post msg queue
				//skipWriteMsgQ(&pNCCIns->netsndAudioMsgQ);
				skipWriteMsgQ(&pNCCIns->netsndMsgQ);
				
				return 0;
			}
			else
			{
				printf("too many audio frames to send ...\n");
				
				//NETCOMM_DEBUG_STR("audio GetMsgQWriteInfo failed!!!", pHead->eFrameType);				
			}
		} break;
		case EM_VIDEO:
		{
			//fill msg buf
			nDataLen = pHead->dwlen+sizeof(SNetComStmHead);
			
			if((pHead->bSub & 0x2) || (pHead->bSub == 0))
			{
				//csp modify 20140318
				if(pHead->bSub)
				{
					if(pNCCIns->pnLostFrame[1][pHead->byChnIndex])
					{
						if(pHead->byFrameType)
						{
							pNCCIns->pnLostFrame[1][pHead->byChnIndex] = 0;
						}
						else
						{
							pNCCIns->pnLostFrame[1][pHead->byChnIndex]++;
							break;
							//return -1;
						}
					}
				}
				else
				{
					if(pNCCIns->pnLostFrame[0][pHead->byChnIndex])
					{
						if(pHead->byFrameType)
						{
							pNCCIns->pnLostFrame[0][pHead->byChnIndex] = 0;
						}
						else
						{
							pNCCIns->pnLostFrame[0][pHead->byChnIndex]++;
							break;
							//return -1;
						}
					}
				}
				
				#if 0
				int try_count = 1;
				if(pHead->byFrameType)
				{
					try_count = 5;
				}
				
				while(1)
				{
					//send to preview
					if(GetMsgQWriteInfo(&pNCCIns->netsndMsgQ, &pQBuf, &nDataLen))
					{
						//printf("chn%d w:%d, h:%d, len:%d\n", pHead->byChnIndex, pHead->nWidth, pHead->nHeight, pHead->dwlen);
						memcpy(pQBuf, pHead, sizeof(SNetComStmHead));
						memcpy(pQBuf+sizeof(SNetComStmHead), pBuf, pHead->dwlen/*nDataLen*/);
						//post msg queue
						skipWriteMsgQ(&pNCCIns->netsndMsgQ);
						//printf("len:%d\n", pHead->dwlen);
						//NETCOMM_DEBUG_STR("GetMsgQWriteInfo success!!!", 0);
						
						break;
					}
					try_count--;
					if(try_count <= 0)
					{
						//if(pHead->byChnIndex == 0) printf("chn%d net video get buf failed, len:%d\n", pHead->byChnIndex, pHead->dwlen);
						//NETCOMM_DEBUG_STR("video GetMsgQWriteInfo failed!!!", pHead->eFrameType);
						
						//calc lost frames
						if(pHead->bSub)
						{
							pNCCIns->pnLostFrame[1][pHead->byChnIndex]++;
						}
						else
						{
							pNCCIns->pnLostFrame[0][pHead->byChnIndex]++;
						}
						
						break;
					}
					usleep(4000);
				}
				#else
				//send to preview
				if(GetMsgQWriteInfo(&pNCCIns->netsndMsgQ, &pQBuf, &nDataLen))
				{
					//printf("chn%d w:%d, h:%d, len:%d\n", pHead->byChnIndex, pHead->nWidth, pHead->nHeight, pHead->dwlen);
					memcpy(pQBuf, pHead, sizeof(SNetComStmHead));
					memcpy(pQBuf+sizeof(SNetComStmHead), pBuf, pHead->dwlen/*nDataLen*/);
					//post msg queue
					skipWriteMsgQ(&pNCCIns->netsndMsgQ);
					//printf("len:%d\n", pHead->dwlen);
					//NETCOMM_DEBUG_STR("GetMsgQWriteInfo success!!!", 0);
				}
				else
				{
					//if(pHead->byChnIndex == 0) printf("chn%d net video get buff failed, len:%d\n", pHead->byChnIndex, pHead->dwlen);
					//NETCOMM_DEBUG_STR("video GetMsgQWriteInfo failed!!!", pHead->eFrameType);
					
					//calc lost frames
					if(pHead->bSub)
					{
						pNCCIns->pnLostFrame[1][pHead->byChnIndex]++;
					}
					else
					{
						pNCCIns->pnLostFrame[0][pHead->byChnIndex]++;
					}
					
					return -1;
				}
				#endif
			}
			else
			{
				//printf("error video frame type!\n");
				//NETCOMM_DEBUG_STR("useless frame type, pHead->bSub %d!!!", pHead->bSub);
			}
		} break;
		case EM_THIRD://csp modify 20130423
		{
			//csp modify 20140525
			if(pHead->bSub & 0x2)
			{
				#if 1//csp modify 20140315
				//printf("EM_THIRD-1,chn%d w=%d h=%d FrameType=%d\n",pHead->byChnIndex,pHead->nWidth,pHead->nHeight,pHead->byFrameType);
				if(pNCCIns->pnLostFrame[2][pHead->byChnIndex])
				{
					if(pHead->byFrameType)
					{
						pNCCIns->pnLostFrame[2][pHead->byChnIndex] = 0;
					}
					else
					{
						pNCCIns->pnLostFrame[2][pHead->byChnIndex]++;
						break;
						//return -1;
					}
				}
				//printf("EM_THIRD-2\n");
				#endif
				
				//fill msg buf
				nDataLen = pHead->dwlen+sizeof(SNetComStmHead);
				
				//send to preview
				if(GetMsgQWriteInfo(&pNCCIns->netsndMsgQ, &pQBuf, &nDataLen))
				{
					memcpy(pQBuf, pHead, sizeof(SNetComStmHead));
					memcpy(pQBuf+sizeof(SNetComStmHead), pBuf, pHead->dwlen/*nDataLen*/);
					
					skipWriteMsgQ(&pNCCIns->netsndMsgQ);
				}
				else
				{
					pNCCIns->pnLostFrame[2][pHead->byChnIndex]++;
					
					return -1;
				}
				//printf("EM_THIRD-3\n");
			}
		}break;
		default:
		{
			//usleep(5000);
			return -1;
		}
	}
	
	if(pHead->bSub & 0x1)
	{
		//printf("chn%d mobile stream eFrameType=%d\n",pHead->byChnIndex,pHead->eFrameType);
		
		//csp modify 20140525
		//fill msg buf
		nDataLen = pHead->dwlen+sizeof(SNetComStmHead);
		
		//send to mobile monitor
		if(GetMsgQWriteInfo(&pNCCIns->netsndMbMsgQ, &pQBuf, &nDataLen))
		{
			memcpy(pQBuf, pHead, sizeof(SNetComStmHead));
			memcpy(pQBuf+sizeof(SNetComStmHead), pBuf, nDataLen);
			
			//post msg queue
			skipWriteMsgQ(&pNCCIns->netsndMbMsgQ);
			//NETCOMM_DEBUG_STR("GetMsgQWriteInfo success!!!", 0);
		}
		else
		{
			//usleep(5000);
			NETCOMM_DEBUG_STR("GetMsgQWriteInfo failed!!!", -1);
			
			return -1;
		}
	}
	
	return 0;
}

int AddStreamTCPLink(SOCKHANDLE hSock, ifly_TCP_Stream_Req req, void* pContext)
{
	s32 errCode = 0;
	
	netComm_DealStreamCmd(hSock, &req, ((struct sockaddr_in*)pContext)->sin_addr.s_addr);
	
	usleep(40*1000);
	
	return errCode;
}

void netcomm_assert( char * file_name, u32 line_no )
{
	printf( "\n[netcomm] Assert failed: %s, line %u\n",
	       file_name, line_no );
	abort();
}

int loopsend_ex(SOCKET s, char * buf, unsigned int sndsize)
{
	int remian = sndsize;
	int sendlen = 0;
	int ret = 0;
	ifly_TCP_Stream_Ack* pAck;
	
	pAck = (ifly_TCP_Stream_Ack*)buf;
	
	while(remian > 0)
	{
		ret=send(s,buf+sendlen,remian,0);
		if(ret <= 0)
		{
			return ret;
		}
		sendlen += ret;
		remian -= ret;
	}
	
	return sndsize;
}

s32 netComm_Ack( u32 err, int sock, u32 nAckId )
{
	ifly_TCP_Stream_Ack ack;
	
	ack.ackid = htonl(nAckId);
	ack.errcode = htonl(err);
	
	loopsend_ex((SOCKHANDLE)sock, (char *)&ack, sizeof(ack));
	
	if(err)
		close(sock);
	
	return 0;
}

#include <linux/tcp.h>

#if 1 // DEAL_CMD_NOTHREAD
static void netComm_DealStreamCmd(SOCKHANDLE sock, ifly_TCP_Stream_Req* pReq, u32 ip)
{
	s32				rtn;
	u32 			nAckId;
	SNetLog			sNLIns;
	PSNetCommCtrl	pNCCIns = &sNetCommCtrl;
	
	nAckId = GetNewIdNum();
	switch(pReq->command)
	{
		case 9: // format
		{
			rtn = remoteFormat_Request(pReq, sock, nAckId);
			if(rtn != 0)
			{
				netComm_Ack(rtn, sock, nAckId);
			}
		}
		break;
		///*
		case 5: // VOIP
		{
			rtn = remoteVoip_Request(pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
		}
		break;
		//*/
		case 0://preview
		{
			#if 0
			if(ip == inet_addr("127.0.0.1"))
			{
				printf("p2p remotePreview......\n");
				
				//����NODELAYѡ��
				#ifdef WIN32
				char optval = 0;
				#else
				int optval = 0;
				#endif
				int ret = setsockopt( sock, IPPROTO_TCP/*SOL_SOCKET*/, TCP_NODELAY, (char *)&optval, sizeof(optval) );
				if( SOCKET_ERROR == ret )
				{
					printf("SetSockLinkOpt:set socket nodelay error!\n");
					//return CTRL_FAILED_UNKNOWN;
				}
			}
			#endif
			
			rtn = remotePreview_Request(pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
		}
		break;
		case 1://Fileplay	
		{
			// �ֽ���ת��
			pReq->FilePlayBack_t.offset = ntohl(pReq->FilePlayBack_t.offset);
			
			rtn = remotePlay_Request(pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
		}
		break;		
		case 4:// update
		{
			//printf("%s update\n", __func__);
			sNLIns.nLogId = EM_NETLOG_UPDATE;
			sNLIns.sockfd = sock;
			sNLIns.ip	  = ip;
			pNCCIns->pWriteLogCB(&sNLIns);
			
			// �ֽ���ת��
			pReq->Update_t.size 		= ntohl(pReq->Update_t.size);
			pReq->Update_t.verify 		= ntohl(pReq->Update_t.verify);
			pReq->Update_t.version 		= ntohs(pReq->Update_t.version);
			pReq->Update_t.updateType 	= ntohs(pReq->Update_t.updateType);
			
			rtn = remoteUpdate_Request(pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
		}
		break;		
		case 3://FileDownload
		{
			sNLIns.nLogId = EM_NETLOG_START_BACKUP;
			sNLIns.sockfd = sock;
			sNLIns.ip	  = ip;
			pNCCIns->pWriteLogCB(&sNLIns);
			
			// �ֽ���ת��
			pReq->FileDownLoad_t.offset 		= ntohl(pReq->FileDownLoad_t.offset);
			pReq->FileDownLoad_t.size			= ntohl(pReq->FileDownLoad_t.size);

			rtn = remoteDownload_Request(pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
		}
		break;
		case 11: // multitimeplay	
		{
			#if 0
			// �ֽ���ת��
			// ��ǰ�ṹ��֧�����16·
			req.MultiTimePlayBack_t.chnMask 		= ntohs(req.MultiTimePlayBack_t.chnMask);
			req.MultiTimePlayBack_t.type			= ntohs(req.MultiTimePlayBack_t.type);
			req.MultiTimePlayBack_t.start_time		= ntohl(req.MultiTimePlayBack_t.start_time);
			req.MultiTimePlayBack_t.end_time			= ntohl(req.MultiTimePlayBack_t.end_time);
			
			NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.offset ", req.MultiTimePlayBack_t.chnMask);
			NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.type ", req.MultiTimePlayBack_t.type);
			NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.start_time ", req.MultiTimePlayBack_t.start_time);
			NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.end_time ", req.MultiTimePlayBack_t.end_time);
			
			rtn = remoteTimePlay_Request(&req, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
			#else
			netComm_Ack(-1, sock, nAckId);
			#endif
		}
		break;
		case 2: //timeplay
		{
			// �ֽ���ת��
			u32 nChnMask 		= (1<<pReq->TimePlayBack_t.channel);
			u16 nType			= ntohs(pReq->TimePlayBack_t.type);
			u32 nStartTime		= ntohl(pReq->TimePlayBack_t.start_time);
			u32 nEndTime		= ntohl(pReq->TimePlayBack_t.end_time);
			
			//req.MultiTimePlayBack_t.chnMask = nChnMask;
			pReq->MultiTimePlayBack_t.type = nType;
			pReq->MultiTimePlayBack_t.start_time = nStartTime;
			pReq->MultiTimePlayBack_t.end_time = nEndTime;
			
			rtn = remoteTimePlay_Request(nChnMask, pReq, sock, nAckId);
			if(rtn!=0)
			{
				netComm_Ack(rtn, sock, nAckId);
				break;
			}
			
			//���ش������id
			//netComm_Ack(0, sock, nAckId);
		}
		break;
		default:
			printf("Error command = %d\n", pReq->command);
			//���ش������id
			netComm_Ack(CTRL_FAILED_RESOURCE, sock, nAckId);
			break;
	}
}
#endif

#if 0
static void* netCommCmd_ThreadFunc(void* param)
{
	SNCCmdData	data;
	s32 		rtn;//, i;
	SMsgCmdHdr 	pCmd;
	PSNetCommCtrl	pNCCIns;
	SOCKHANDLE	sock;	// new sock for new connect req 
	ifly_TCP_Stream_Req 	req;	// req struct from ctrlprotocol

	u16			nTotalLinkNum;
	u16			nChnTcpStreamNum;

	SNetLog			sNLIns;
	
	u32 			nAckId;
	
	pNCCIns = &sNetCommCtrl;

	nTotalLinkNum  = pNCCIns->nTotalLinkNum;
	nChnTcpStreamNum = pNCCIns->nChnTcpStreamNum;
		
	NETCOMM_DEBUG_STR("Enter netcomm cmd thread...",0); // read cmd error	
		
	// check thread id
	//printf("netCommCmd_ThreadFunc id: %d\n", syscall(__NR_gettid));
	
	while(1)
	{
		memset(&data, 0, sizeof(data));
		
		pCmd = pNCCIns->pMsgCmd;			
		
		// read cmd queue
		rtn = ReadMsgCmd(pCmd, &data);
		if(0!=rtn)
		{
			//NETCOMM_DEBUG_STR("read cmd error",-1); // read cmd error
			
			usleep(RECV_CMD_ERR_DELAY_US); // cmd err, sleep 1 seconds to continue
			
			continue;
		}
		else
		{
			sock = data.sock;
			memcpy(&req, &data.req, sizeof(req));
			
			NETCOMM_DEBUG_STR("Recv this cmd", req.command);
			NETCOMM_DEBUG_STR("sock : ", sock);
		}
		
		rtn = AckMsgCmd(pCmd);
		if(rtn)
		{
			NETCOMM_DEBUG_STR("ack fail!\n",0);
			NETCOMM_DEBUG(rtn);
		}

		// parse cmd

		NETCOMM_DEBUG_STR("check nTotalLinkNum",nTotalLinkNum);

		nAckId = GetNewIdNum();
		switch (req.command)
		{
			case 5: // VOIP
			{
				NETCOMM_DEBUG_STR("req voip ... ", 0);
				
				rtn = remoteVoip_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);
					NETCOMM_DEBUG_STR("req voip invalid!", -1);

					break;
				}
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;			
			case 0://preview
			{
				NETCOMM_DEBUG_STR("req.Monitor_t.chn     : ", req.Monitor_t.chn);
				NETCOMM_DEBUG_STR("req.Monitor_t.type : ", req.Monitor_t.type);
				
				rtn = remotePreview_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;
			
			case 1://Fileplay	
			{
				NETCOMM_DEBUG_STR(req.FilePlayBack_t.filename, 0);

				// �ֽ���ת��
				req.FilePlayBack_t.offset = ntohl(req.FilePlayBack_t.offset);
				
				NETCOMM_DEBUG_STR("req.FilePlayBack_t.offset ", req.FilePlayBack_t.offset);

				rtn = remotePlay_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				
				//���ش������id
				//printf("###netComm_Ack \n");
				//netComm_Ack(0, sock, nAckId);
			}
			break;		
			case 4:// update
			{
				sNLIns.nLogId = EM_NETLOG_UPDATE;
				pNCCIns->pWriteLogCB(&sNLIns);
				
				// �ֽ���ת��
				req.Update_t.size 		= ntohl(req.Update_t.size);
				req.Update_t.verify 	= ntohl(req.Update_t.verify);
				req.Update_t.version 	= ntohs(req.Update_t.version);
				req.Update_t.updateType = ntohs(req.Update_t.updateType);
				
				NETCOMM_DEBUG_STR("req.Update_t.size ", req.Update_t.size);
				NETCOMM_DEBUG_STR("req.Update_t.verify ", req.Update_t.verify);
				NETCOMM_DEBUG_STR("req.Update_t.version ", req.Update_t.version);
				NETCOMM_DEBUG_STR("req.Update_t.updateType ", req.Update_t.updateType);

				rtn = remoteUpdate_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;		
			case 3://FileDownload	
			{
				sNLIns.nLogId = EM_NETLOG_START_BACKUP;
				pNCCIns->pWriteLogCB(&sNLIns);
				
				NETCOMM_DEBUG_STR(req.FileDownLoad_t.filename, 0);
				
				// �ֽ���ת��
				req.FileDownLoad_t.offset 		= ntohl(req.FileDownLoad_t.offset);
				req.FileDownLoad_t.size			= ntohl(req.FileDownLoad_t.size);
				
				NETCOMM_DEBUG_STR("req.FileDownLoad_t.offset ", req.FileDownLoad_t.offset);
				NETCOMM_DEBUG_STR("req.FileDownLoad_t.size ", req.FileDownLoad_t.size);

				rtn = remoteDownload_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;
			case 11: // multitimeplay	
			{
				#if 0
				// �ֽ���ת��
				// ��ǰ�ṹ��֧�����16·
				req.MultiTimePlayBack_t.chnMask 		= ntohs(req.MultiTimePlayBack_t.chnMask);
				req.MultiTimePlayBack_t.type			= ntohs(req.MultiTimePlayBack_t.type);
				req.MultiTimePlayBack_t.start_time		= ntohl(req.MultiTimePlayBack_t.start_time);
				req.MultiTimePlayBack_t.end_time			= ntohl(req.MultiTimePlayBack_t.end_time);
			
				NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.offset ", req.MultiTimePlayBack_t.chnMask);
				NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.type ", req.MultiTimePlayBack_t.type);
				NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.start_time ", req.MultiTimePlayBack_t.start_time);
				NETCOMM_DEBUG_STR("req.MultiTimePlayBack_t.end_time ", req.MultiTimePlayBack_t.end_time);

				rtn = remoteTimePlay_Request(&req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				#else
				netComm_Ack(-1, sock, nAckId);
				#endif
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;
			case 2: //timeplay
			{				
				// �ֽ���ת��
				u32 nChnMask 		= (1<<req.TimePlayBack_t.channel);
				u16 nType			= ntohs(req.TimePlayBack_t.type);
				u32 nStartTime		= ntohl(req.TimePlayBack_t.start_time);
				u32 nEndTime		= ntohl(req.TimePlayBack_t.end_time);
			
				NETCOMM_DEBUG_STR("req.TimePlayBack_t.channel ", nChnMask);
				NETCOMM_DEBUG_STR("req.TimePlayBack_t.type ", nType);
				NETCOMM_DEBUG_STR("req.TimePlayBack_t.start_time ", nStartTime);
				NETCOMM_DEBUG_STR("req.TimePlayBack_t.end_time ", nEndTime);

				//req.MultiTimePlayBack_t.chnMask = nChnMask;
				req.MultiTimePlayBack_t.type = nType;
				req.MultiTimePlayBack_t.start_time= nStartTime;
				req.MultiTimePlayBack_t.end_time= nEndTime;
				
				rtn = remoteTimePlay_Request(nChnMask, &req, sock, nAckId);
				if(rtn!=0)
				{
					netComm_Ack(rtn, sock, nAckId);

					break;
				}
				
				//���ش������id
				//netComm_Ack(0, sock, nAckId);
			}
			break;
			
			default:
				printf("Error command = %d\n", req.command);
				//���ش������id
				netComm_Ack(CTRL_FAILED_RESOURCE, sock, nAckId);
				break;
		}
	}

	return NULL;
}
#endif

u16 GetNewIdNum()
{
	static u32 dwIDNum = 0;

	if (++dwIDNum == 0)
	{
		dwIDNum = 1;
	}
	
	return dwIDNum;
}

BOOL netComm_CheckConnLost()
{
	return FALSE;
}

s32 NetComm_ReadParaEx( SNetPara* pConfig )
{
	SNetPara *pNetPara;
	
	pNetPara = &sNetCommCtrl.sCommonCfg.sParaIns;
	
	GetHWAddr((char *)pConfig->MAC);//��ȡ��ǰ�������õ�MAC��ַ
	
	printf("read mac %s\n", pConfig->MAC);
	
	#if 1//csp modify
	if(CheckNetLink2("ppp0"))
	{
		pConfig->HostIP 	=	pNetPara->HostIP 	= GetLocalIp2("ppp0");
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask2("ppp0");
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetRemoteIP();
	}
	else if(CheckNetLink2("ppp1"))
	{
		pConfig->HostIP 	=	pNetPara->HostIP 	= GetLocalIp2("ppp1");
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask2("ppp1");
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetRemoteIP();
	}
	else
	{
		pConfig->HostIP 	= 	pNetPara->HostIP 	= GetLocalIp();
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask();
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetDefaultGateway();
	}
	#else
	if(!CheckNetLink2("ppp0"))
	{
		pConfig->HostIP 	= 	pNetPara->HostIP 	= GetLocalIp();
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask();
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetDefaultGateway();
	}
	else
	{
		pConfig->HostIP 	=	pNetPara->HostIP 	= GetLocalIp2("ppp0");
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask2("ppp0");
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetRemoteIP();
	}
	#endif
	
	GetDNSServer2(&pNetPara->DNSIP, &pNetPara->DNSIPAlt);
	
	pConfig->DNSIP 		=	 pNetPara->DNSIP;
	pConfig->DNSIPAlt 	=	 pNetPara->DNSIPAlt;
	
	return 0;
}

s32 NetComm_ReadPara( SNetPara* pConfig )
{
	struct in_addr sin;
	
	SNetPara *pNetPara = NULL;
	pNetPara = &sNetCommCtrl.sCommonCfg.sParaIns;
	
	GetHWAddr((char *)pConfig->MAC);//��ȡ��ǰ�������õ�MAC��ַ
	
#if 1
	pConfig->HostIP 	=	pNetPara->HostIP 	= GetLocalIp();	
	pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask();	
	pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetDefaultGateway();
#else
	if(!CheckNetLink2("ppp0"))
	{
		pConfig->HostIP 	=	pNetPara->HostIP 	= GetLocalIp();	
		pConfig->Submask 	=	pNetPara->Submask 	= GetNetMask();	
		pConfig->GateWayIP 	=	pNetPara->GateWayIP = GetDefaultGateway();
	}
	else
	{
		pConfig->HostIP 	=	pNetPara->HostIP	= GetLocalIp2("ppp0");	
		pConfig->Submask 	=	pNetPara->Submask	= GetNetMask2("ppp0");	
		pConfig->GateWayIP 	=	pNetPara->GateWayIP	= GetDefaultGateway();		
	}
#endif
	
	memcpy(&sin, &pConfig->Submask, sizeof(sin));
	memcpy(&sin, &pConfig->GateWayIP, sizeof(sin));
	
	GetDNSServer2(&pNetPara->DNSIP, &pNetPara->DNSIPAlt);
	pConfig->DNSIP 		=	 pNetPara->DNSIP;
	pConfig->DNSIPAlt 	=	 pNetPara->DNSIPAlt;
	
	return 0;
}


char *inet_ntoa_user(struct in_addr sin)
{
	static char ipbuf[16];
	u32 dwIp = sin.s_addr;
	sprintf(ipbuf,"%03d.%03d.%03d.%03d",(u8)dwIp,(u8)(dwIp>>8),(u8)(dwIp>>16),(u8)(dwIp>>24));
	
	return ipbuf;
}

s32 netComm_PPPOE_GetCfg(SPppoeCfg* pCfg)
{
	memcpy( pCfg, &sPPPoECfg, sizeof(sPPPoECfg) );
	
	return 0;
}

s32 netComm_PPPOE_SetCfg(SPppoeCfg* pCfg)
{
	memcpy( &sPPPoECfg, pCfg, sizeof(sPPPoECfg) );
	
	return 0;
}

s32 netComm_PPPOE_Init( PSNetCommCfg pNetCfg )
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	struct in_addr 	sin;
	
	pCfg->nFlag = pNetCfg->sParaIns.PPPOEFlag;

	strcpy(pCfg->szUser, (char *)pNetCfg->sParaIns.PPPOEUser);
	strcpy(pCfg->szPasswd, (char *)pNetCfg->sParaIns.PPPOEPasswd);

	switch(pCfg->eStatus)
	{
		case EM_PPPOE_Up:
		{
			sin.s_addr = GetLocalIp2("ppp0");
			printf("pppoe ip %s\n",inet_ntoa_user(sin));
			strcpy(pCfg->szIP, inet_ntoa_user(sin));

			sin.s_addr=GetNetMask2("ppp0");
			strcpy(pCfg->szSubMask, inet_ntoa_user(sin));
			sin.s_addr=GetRemoteIP();
			strcpy(pCfg->szGateway, inet_ntoa_user(sin));
			sin.s_addr=GetDNSServer();
			strcpy(pCfg->szDNS, inet_ntoa_user(sin));			
		} break;
		case EM_PPPOE_Down:
		{
			strcpy(pCfg->szIP, "000.000.000.000");
			strcpy(pCfg->szSubMask, "255.255.255.255");
			strcpy(pCfg->szGateway, "000.000.000.000");
			strcpy(pCfg->szDNS, "000.000.000.000");			
		} break;
		default:
		{
			printf("pppoe status error!\n");
		} break;
	}
	
	return 0;
}

s32 netComm_PPPOE_Start( u8 flag, char *pszUser, char* pszPasswd )
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	my_daemon_param daemon;
	
	sPPPoECfg.nFlag = flag;
	
	if(CheckNetLink2("ppp0"))
	{
		daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
	}
	else
	{
		//csp modify
		//daemon.op_type = MY_DAEMON_OPTYPE_START;
		daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
	}
	
	if( !netComm_SetPPPOEUser(pszUser, pszPasswd) )
	{
		return -1;
	}
	
	//printf("set network param,%d pppd\n", daemon.op_type);
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_PPPD;
	if(pszUser)
		strcpy(pCfg->szUser, pszUser);
	if(pszPasswd)
		strcpy(pCfg->szPasswd, pszPasswd);
	
	sprintf(daemon.arg1,"%s",pCfg->szUser);
	sprintf(daemon.arg2,"%s",pCfg->szPasswd);
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
	{
		return 0;
	}
	else
		return -1;
}

s32 netComm_PPPOE_Stop(u8 flag)
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	my_daemon_param daemon;
	
	sPPPoECfg.nFlag = flag;
	
	if(EM_PPPOE_Down!=pCfg->eStatus)
	{
		printf("set network param,stop pppd\n");
		
		daemon.proc_type = MY_DAEMON_PROCTYPE_PPPD;
		daemon.op_type = MY_DAEMON_OPTYPE_STOP;
		sprintf(daemon.arg1,"%s",pCfg->szUser);
		sprintf(daemon.arg2,"%s",pCfg->szPasswd);
		
		if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
		{
			return 0;
		}
		else
			return -1;
	}
	
	return 0;
}

s32 netComm_PPPOE_Restart( char *pszUser, char* pszPasswd )
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	my_daemon_param daemon;
	
	//if(EM_PPPOE_Up==pCfg->eStatus)
	{
		printf("set network param,restart pppd\n");
		
		daemon.proc_type = MY_DAEMON_PROCTYPE_PPPD;
		daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
		
		if(pszUser)
			strcpy(pCfg->szUser, pszUser);
		if(pszPasswd)
			strcpy(pCfg->szPasswd, pszPasswd);
			
		sprintf(daemon.arg1,"%s",pCfg->szUser);
		sprintf(daemon.arg2,"%s",pCfg->szPasswd);
				
		if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
		{
			return 0;
		}
		else
			return -1;
	}
	
	return 0;
}

s32 netComm_PPPOE_CmdPreup()
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;

	pCfg->eStatus = EM_PPPOE_PreUp;
	
	netComm_NotifyPppoeState(0);
	
	return 0;
}

s32 NetComm_DDNSIpUpdate( void )
{
	SDdnsCfg* 		pDdnsCfg = &sDdnsCfg;
	
	if(pDdnsCfg->nDdnsFlag)
	{
		DDNS_SvcRefresh(pDdnsCfg->eProt, NULL);
	}

	return 0;
}

s32 netComm_PPPOE_CmdUp()
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	SDdnsCfg* 		pDdnsCfg = &sDdnsCfg;
	
	pCfg->eStatus = EM_PPPOE_Up;
	pCfg->nDisconnNum = 0;
	
	if(pDdnsCfg->nDdnsFlag)
	{
		// update ddns ip
		DDNS_SvcRefresh(pDdnsCfg->eProt, NULL);
	}
	
	netComm_NotifyPppoeState(0);
	
	return 0;	
}

// bState - 0 : OK
//        - 1 : ERR
//
s32 netComm_NotifyPppoeState( u8 bState )
{
	SNetStateNotify sNotify;
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	memset(&sNotify, 0, sizeof(sNotify));
	
	sNotify.eAct 		= EM_NET_STATE_PPPOE;	
	sNotify.sRslt.state = bState;
	sNotify.sRslt.nErr 	= 0;
	pNCCIns->sCommonCfg.pNotify((void*)&sNotify);
	
	return 0;
}

s32 netComm_PPPOE_CmdDown()
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	my_daemon_param daemon;
	
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	SetDNSServer(pNCCIns->sCommonCfg.sParaIns.DNSIP);
	if(pNCCIns->sCommonCfg.sParaIns.GateWayIP) 
		SetDefaultGateway(pNCCIns->sCommonCfg.sParaIns.GateWayIP);
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_PPPD;

	if(pCfg->nFlag && pCfg->eStatus!=EM_PPPOE_Down)
	{
		daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
	}
	else
	{
		printf("MY_DAEMON_OPTYPE_STOP ... %d %d \n", pCfg->nFlag, pCfg->eStatus);
		daemon.op_type = MY_DAEMON_OPTYPE_STOP;
	}

	pCfg->eStatus = EM_PPPOE_Down;
	
	sprintf(daemon.arg1,"%s",pCfg->szUser);
	sprintf(daemon.arg2,"%s",pCfg->szPasswd);

	netComm_NotifyPppoeState(1);
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
	{	
		return 0;
	}
	else
		return -1;
}

s32 netComm_PPPOE_CmdDisconnect()
{
	SPppoeCfg* 		pCfg = &sPPPoECfg;
	my_daemon_param daemon;
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;

	SetDNSServer(pNCCIns->sCommonCfg.sParaIns.DNSIP);
	if(pNCCIns->sCommonCfg.sParaIns.GateWayIP) 
		SetDefaultGateway(pNCCIns->sCommonCfg.sParaIns.GateWayIP);
		
	if(pCfg->nFlag && pCfg->eStatus!=EM_PPPOE_Down)
	{
		printf("disconnect %d \n", pCfg->eStatus);
		
		pCfg->eStatus = EM_PPPOE_Down;
	
		daemon.proc_type = MY_DAEMON_PROCTYPE_PPPD;
		daemon.op_type = MY_DAEMON_OPTYPE_START;
		
		sprintf(daemon.arg1,"%s",pCfg->szUser);
		sprintf(daemon.arg2,"%s",pCfg->szPasswd);
		
		if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
			return 0;
		else
			return -1;
	}
	else
	{
		netComm_NotifyPppoeState(1);
	}
	
	return 0;
}

s32 NetComm_PPPoE_Start(char *pszUser, char* pszPasswd)
{
	return netComm_PPPOE_Start(1, pszUser, pszPasswd);
}

s32 NetComm_PPPoE_Stop(u8 flag)
{
	return netComm_PPPOE_Stop(flag);
}

//** dhcp
s32 netComm_DHCP_Start()
{
	my_daemon_param daemon;
	
	daemon.proc_type 	= MY_DAEMON_PROCTYPE_UDHCPC;
	daemon.op_type 		= MY_DAEMON_OPTYPE_RESTART;
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
		return 0;
	else
		return -1;
}

s32 netComm_DHCP_Stop()
{
	my_daemon_param daemon;
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_UDHCPC;
	daemon.op_type = MY_DAEMON_OPTYPE_STOP;
	
	if( SendMsgToDaemon(&daemon)<sizeof(daemon) )
		return -1;
	
	SNetStateNotify sNotify;
	
	memset(&sNotify, 0, sizeof(sNotify));
	
	sNotify.eAct 		= EM_NET_STATE_DHCP;
	sNotify.sRslt.state = 1;
	sNotify.sRslt.nErr 	= 0;
	
	if(pNCCIns->sCommonCfg.pNotify)
		pNCCIns->sCommonCfg.pNotify((void*)&sNotify);
	
	sNetConnStatus.eWay = ECW_STATIC;
	
	return 0;
}

s32 netComm_DHCP_Active( char* pIFName )
{	
	SNetCommCtrl* 	pNCCIns 	= &sNetCommCtrl;
	SDdnsCfg* 		pDdnsCfg 	= &sDdnsCfg;
	
	if(pIFName)
	{
		#if 1
		if(0 == strcmp(pIFName,"eth0"))
		{
			#if 0
			SetLocalIp(pNCCIns->sCommonCfg.sParaIns.HostIP);
			SetDefaultGateway(pNCCIns->sCommonCfg.sParaIns.GateWayIP);
			SetNetMask(pNCCIns->sCommonCfg.sParaIns.Submask);
			SetDNSServer();
			#else
			pNCCIns->sCommonCfg.sParaIns.HostIP 	= GetLocalIp();
			pNCCIns->sCommonCfg.sParaIns.GateWayIP 	= GetDefaultGateway();
			pNCCIns->sCommonCfg.sParaIns.Submask 	= GetNetMask();
			pNCCIns->sCommonCfg.sParaIns.DNSIP 		= GetDNSServer();
			#endif
		}
		#endif
		
		if(pDdnsCfg->bStart)
		{
			DDNS_SvcRefresh(pDdnsCfg->eProt, NULL);
		}

		sNetConnStatus.eWay = ECW_DHCP;
				
		SNetStateNotify sNotify;
		
		memset(&sNotify, 0, sizeof(sNotify));
		
		sNotify.eAct 		= EM_NET_STATE_DHCP;
		sNotify.sRslt.state = 0;
		sNotify.sRslt.nErr 	= 0;
		pNCCIns->sCommonCfg.pNotify((void*)&sNotify);
	}
	
	return 0;
}

s32 NetComm_StartDHCP()
{
	netComm_DHCP_Start();

	return 0;
}

s32 NetComm_StopDHCP()
{	
	netComm_DHCP_Stop();
	
	return 0;
}

//csp modify 20130321
//**  UPNP
s32 NetComm_RestartUPnP(u8 upnp_enable)
{
#ifndef CHIP_HISI351X_2X
	my_daemon_param daemon;
	daemon.proc_type = MY_DAEMON_PROCTYPE_UPNP;
	if(upnp_enable)
	{
		daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
	}
	else
	{
		daemon.op_type = MY_DAEMON_OPTYPE_STOP;
	}
	sprintf(daemon.arg1,"%d",sNetCommCtrl.sCommonCfg.sParaIns.HttpPort);
	sprintf(daemon.arg2,"%d",sNetCommCtrl.sCommonCfg.sParaIns.TCPPort);
	sprintf(daemon.arg3,"%d",sNetCommCtrl.sCommonCfg.sParaIns.MobilePort);
	u32 dwIp = sNetCommCtrl.sCommonCfg.sParaIns.HostIP;
	if(CheckNetLink2("ppp0"))
	{
		dwIp = GetLocalIp2("ppp0");
	}
	else
	{
		dwIp = GetLocalIp();
	}
	memcpy(daemon.arg4,&dwIp,sizeof(dwIp));
	SendMsgToDaemon(&daemon);
#endif
	
	return 0;
}

//**  WEBS
s32 NetComm_WebRestart(u16 nPort)
{
	return netComm_Webs_Start(nPort);
}

s32 netComm_Webs_Start( u16 nPort )
{	
	my_daemon_param daemon;
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_WEBS;
	daemon.op_type = MY_DAEMON_OPTYPE_RESTART;
	
	nPort = nPort?nPort:80;
	
	sprintf(daemon.arg1,"%d", nPort);
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
	{
		printf("restart web services port %d \n", nPort);
		
		return 0;
	}
	else
		return -1;
}

#if 0
s32 netComm_Webs_Restart( u16 nPort )
{	
	my_daemon_param daemon;
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_WEBS;
	daemon.op_type = MY_DAEMON_OPTYPE_START;
	
	sprintf(daemon.arg1,"%d",nPort?nPort:80);
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
		return 0;
	else
		return -1;
}
#endif

s32 netComm_Webs_Stop()
{
	my_daemon_param daemon;
	
	daemon.proc_type = MY_DAEMON_PROCTYPE_WEBS;
	daemon.op_type = MY_DAEMON_OPTYPE_STOP;
	
	if( SendMsgToDaemon(&daemon)>=sizeof(daemon) )
		return 0;
	else
		return -1;
}

// op 0-get, 1-add, 2-del
// 
s32 NetComm_NetUserCtrl( u8 op, SNetUser* pUser)
{
	switch(op)
	{
		case 0:
		{
			return cmdProc_GetLoginUser(pUser);
		} break;
		case 1:
		{
			return cmdProc_AddNetUser(pUser);
		} break;
		case 2:
		{
			return cmdProc_DelNetUser(pUser);
		} break;
	}
	
	printf("op %d ip %d sock %d user %s mac %s\n", op, pUser->ip, pUser->sock, pUser->name,pUser->mac);
	
	return -1;
}

#include <sys/stat.h>

//ʹ��dest�滻ָ���ļ��еİ���src����
int	netComm_ReplaceWebRow(const char *fname, const char *src, char *dest)
{
	FILE *fp, *fp_tmp;
	char line[256];
	char tmp_file[128];
	char *s;
	struct stat orgstat;
	
	memset(tmp_file, 0x0, sizeof(tmp_file));
	sprintf(tmp_file, "%s.tmp", fname);
	
	if(fname == NULL || src == NULL)
	{	
		printf("ERROR: Invalid parameter.\n");
		return 1;
	}
	
	fp_tmp = fopen(tmp_file, "w");
	if(fp_tmp == NULL) 
	{
		printf("-replace_any_row-1-ERROR: Unable to open file: %s.\n", tmp_file);
		return 1;
	}
	
	fp = fopen(fname, "r");
	if(fp == NULL) 
	{
		printf("-replace_any_row-2-ERROR: Unable to open file: %s.\n", fname);
		fclose(fp_tmp);
		return 1;
	}
	
	memset(line, 0x0, sizeof(line));
	while(fgets(line, sizeof(line), fp))
	{
		if(strstr(line, src))
		{
            if(dest)
            {       
                fputs(dest, fp_tmp);
                for(s=dest; *s; s++) ;
                if(*(--s) != '\n')
                    fputs("\n", fp_tmp);
            }
		}
		else
		{
			if(strlen(line) >= 1)
				fputs(line, fp_tmp);
		}
	}
	
	fclose(fp);
	fclose(fp_tmp);
	
	stat(fname, &orgstat);
	
	if(rename(tmp_file, fname) == -1)
	{
		printf("ERROR: Unable to rename file from %s to %s.\n", tmp_file, fname);
		return 1;
	}
	
	//restore origin file property
	chmod(fname, orgstat.st_mode);
	
	fp = fopen(tmp_file, "w");
	fclose(fp);
	
	return 0;
}


void netComm_SaveServerPort(u16 server_port)
{
	char str_port[16] = {0};
	FILE *pf = NULL;
	
	remove("/root/DVR_CTRL_PORT.txt");
	sprintf(str_port, "%u", server_port);
	
	pf = fopen("/root/DVR_CTRL_PORT.txt","w");
	if(pf == NULL)
	{
		printf("error fopen DVR_CTRL_PORT.txt\n");
	}
	else
	{
		fwrite(str_port,sizeof(char),sizeof(str_port),pf);
		fflush(pf);
		fsync(fileno(pf));
		fclose(pf);
	}
}

BOOL netComm_CheckNetLink()
{
    int s;
	
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        perror("netComm_CheckNetLink-socket");
        return FALSE;
    }
	
	struct ifreq ifr;
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ-1] = 0;
	
	BOOL bRet = FALSE;
	
	if(ioctl(s, SIOCGIFFLAGS, &ifr) < 0)
	{
		perror("netComm_CheckNetLink-SIOCGIFFLAGS");
	}
	else
	{
		/*printf("flags=0x%x,IFF_UP=0x%x,IFF_RUNNING=0x%x,flags&IFF_UP=0x%x,flags&IFF_RUNNING=0x%x\n",
			ifr.ifr_flags,IFF_UP,IFF_RUNNING,ifr.ifr_flags&IFF_UP,ifr.ifr_flags&IFF_RUNNING);*/
		bRet = (ifr.ifr_flags&IFF_UP) && (ifr.ifr_flags&IFF_RUNNING);
	}
	
	close(s);
	
    return bRet;
}

int SetLocalIpV2(u32 dwIp)
{
	int	sock;
	struct ifreq ifr;
	
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1)
	{
		perror("socket");
		return -1;
	}
    
	strncpy(ifr.ifr_name,ETH_NAME,IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1] = 0;
    
	if(ioctl(sock,SIOCGIFADDR,&ifr)<0)
	{
		perror("SetLocalIp:ioctl--1");
		printf("SetLocalIp:ioctl:error:(%d,%s)\n",errno,strerror(errno));
		close(sock);
		return 0;
	}
	
	((struct sockaddr_in*)(&ifr.ifr_addr))->sin_addr.s_addr = dwIp;
	if(ioctl(sock,SIOCSIFADDR,&ifr)<0)
	{
		perror("SetLocalIp:ioctl--2");
		printf("SetLocalIp:ioctl:error:(%d,%s)\n",errno,strerror(errno));
		close(sock);
		return -1;
	}
	
	close(sock);
	
	return 0;
}

void netcomm_ConfigNetwork(SNetPara* 	pConfig)
{
	if(!pConfig) return;
	
	//printf("netcomm_ConfigNetwork - 1, CheckNetLink:%d\n",CheckNetLink());
	
	if(0 != SetHWAddr((char *)pConfig->MAC))
	{
		printf("SetHWAddr %s failed! \n", pConfig->MAC);			
	}
	
	//printf("netcomm_ConfigNetwork - 2, CheckNetLink:%d\n",CheckNetLink());
	
	if(0 == pConfig->DhcpFlag)
	{
		struct in_addr addr;
		
		memcpy(&addr, &pConfig->HostIP, 4);
		if( 0 != SetLocalIpV2(pConfig->HostIP) )
		{
			printf("SetLocalIpV2 failed!\n");
		}
		
		memcpy(&addr, &pConfig->Submask, 4);		
		if( 0 != SetNetMask(pConfig->Submask) )
		{
			printf("SetNetMask failed!\n");
		}
		
		if( 0 != SetDNSServer(pConfig->DNSIP) )
		{
			printf("SetDNSServer failed!\n");
		}
		
		memcpy(&addr, &pConfig->GateWayIP, 4);		
		if( 0 != SetDefaultGateway(pConfig->GateWayIP) )
		{
			printf("SetDefaultGateway failed!\n");
		}
		
		if( 0 != AddDNSServer(pConfig->DNSIPAlt) )
		{
			printf("AddDNSServer failed!\n");
		}
	}
}

s32 NetComm_WritePara( SNetPara* pConfig )
{
	SNetPara* pPara = &sNetCommCtrl.sCommonCfg.sParaIns;
	
	if(0 != strcmp((char *)pConfig->MAC, (char *)pPara->MAC))
	{
		if( 0 != SetHWAddr((char *)pConfig->MAC) )
		{
			printf("SetHWAddr %s failed! \n", pConfig->MAC);			
		}
	}
	
	if(0 == pConfig->DhcpFlag)
	{
		//struct in_addr addr;
		if(pConfig->HostIP != pPara->HostIP)
		{
			//memcpy(&addr, &pConfig->HostIP, 4);
			if( 0 != SetLocalIpV2(pConfig->HostIP) )
			{
				printf("SetLocalIpV2 failed!\n");
			}
		}
		
		//csp modify
		//if(pConfig->Submask != pPara->Submask)
		{
			//memcpy(&addr, &pConfig->Submask, 4);
			if( 0 != SetNetMask(pConfig->Submask) )
			{
				printf("SetNetMask failed!\n");
			}
		}
		
		//csp modify
		//if(pConfig->GateWayIP != pPara->GateWayIP)
		{
			//memcpy(&addr, &pConfig->GateWayIP, 4);
			if( 0 != SetDefaultGateway(pConfig->GateWayIP) )
			{
				printf("SetDefaultGateway failed!\n");
			}
		}
		
		//csp modify
		//if(pConfig->DNSIP != pPara->DNSIP)
		{
			if( 0 != SetDNSServer(pConfig->DNSIP) )
			{
				printf("SetDNSServer failed!\n");
			}
		}
		
		//csp modify
		//if(pConfig->DNSIPAlt != pPara->DNSIPAlt)
		if(pConfig->DNSIPAlt != 0)
		{
			if( 0 != AddDNSServer(pConfig->DNSIPAlt) )
			{
				printf("AddDNSServer failed!\n");
			}
		}
	}
	
	memcpy( pPara, pConfig, sizeof(SNetPara) );
	
	return 0;	
}

u8 NetComm_GetState()
{
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	return pNCCIns->eState;
}

s32 NetComm_SendMail(SMailContent* pMail)
{
	s32 nRet;
	
	if( sendmail(
		pMail->pSrcUsr,
		pMail->pSrcBox,
		pMail->pToUsr,
		pMail->pToBox,
		pMail->pUserName,
		pMail->pPasswd,
		pMail->pSmtp,
		pMail->pSubject,
		pMail->pBody,
		NULL,
		"base64"
	) )
	{
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
	
	return nRet;
}

// nFlag 	- prot type
// nUpIntvl - update interval
//
void* netComm_DDNS_Init(void* para)
{
	pthread_detach(pthread_self());//csp modify
	
	//printf("$$$$$$$$$$$$$$$$$$netComm_DDNS_Init id:%d\n",getpid());
	
	s32 ret = 0;
	
	DDNSInitPara* pPara = (DDNSInitPara*)para;
	
	sDdnsCfg.bStart = 0;
	sDdnsCfg.eProt = -1;
	
	sDdnsCfg.bInit = 0;
	
	// ddns init
	ret = DDNS_Init(pPara->flag, pPara->interval);
	if(ret == 0)
	{
		sDdnsCfg.bInit = 1;
		
		if(pPara->flag>=EM_NET_DDNS_TL && pPara->flag<EM_NET_MAX)
		{
			sDdnsCfg.eProt = pPara->flag;
			
			NetComm_DDNS_Start(&pPara->sStart);
		}
	}
	
	//printf("[ddns] func %s line %d err %d\n", __FUNCTION__, __LINE__, ret);
	
	return NULL;
}

EM_DDNS_PROT netComm_GetInsideProt(EM_NET_DDNS eProt)
{
	return eProt;
}

s32 NetComm_DDNS_Install(EM_NET_DDNS eProt)
{
	s32 ret = 0;
	
	if(sDdnsCfg.eProt >= EM_NET_DDNS_TL && sDdnsCfg.eProt < EM_NET_MAX)
	{
		ret = DDNS_UnInstall(netComm_GetInsideProt(sDdnsCfg.eProt));
		if(ret != 0) return -1;
	}
	
	printf("NetComm_DDNS_Install eProt=%d\n",eProt);
	
	ret = DDNS_Install( eProt, NULL );
	
	/*
	switch(eProt)
	{
		case 	EM_NET_DDNS_TL:
		{
			ret = DDNS_Install( EM_DDNS_PROT_TL, NULL );
		} break;
		case 	EM_NET_DDNS_3322:
		{
			ret = DDNS_Install( EM_DDNS_PROT_3322, NULL );
		} break;
		case 	EM_NET_DDNS_DYDNS:
		{
			ret = DDNS_Install( EM_DDNS_PROT_DYNDNS, NULL );
		} break;
		case 	EM_NET_DDNS_DVRNET:
		{
			ret = DDNS_Install( EM_DDNS_PROT_DVRNET, NULL );
		} break;
		case 	EM_NET_DDNS_NOIP:
		{
			ret = DDNS_Install( EM_DDNS_PROT_NOIP, NULL );
		} break;
		default:
		{
			ret = -1;
		} break;
	}
	*/
	if(ret == 0)
	{
		printf("NetComm_DDNS_Install - DDNS_SvcInit - eProt=%d\n",eProt);
		
		sDdnsCfg.eProt = eProt;
		ret = DDNS_SvcInit(eProt, NULL);
	}
	
	//printf("func %s prot %d \n", __FUNCTION__, eProt);
	
	return ret;
}

s32 NetComm_DDNS_Start(SNetDdnsPara* pStart)
{
	s32				ret = 0;
	SNetConnStatus	sStatus;
	
	// ���粻ͨ����ʧ��
	if(0 == NetComm_ReqConnStatus(&sStatus))
	{
		if(sStatus.eState != ECS_ONLINE)
		{
			printf("connection invalid!\n");
			//return -1;//csp modify 20130322
		}
		else
		{
			printf("check net online!\n");
		}
	}
	else
	{
		printf("check net failed!\n");
		return -1;
	}
	
	//printf("check net finish!\n");
	
	if(sDdnsCfg.bInit)
	{
		SDDNSStartPara 	sStart, sStartBack;
		memset(&sStart, 0, sizeof(sStart));
		
		if(pStart)
		{
			strcpy(sStart.sBase.szDomain, pStart->szDomain);
			strcpy(sStart.sBase.szUser, pStart->szUser);
			strcpy(sStart.sBase.szPasswd, pStart->szPasswd);
		}
		else
		{
			strcpy(sStart.sBase.szDomain, sDdnsCfg.sInfo.szDomain);
			strcpy(sStart.sBase.szUser, sDdnsCfg.sInfo.szUser);
			strcpy(sStart.sBase.szPasswd, sDdnsCfg.sInfo.szPasswd);
		}
		
		memcpy(&sStartBack, &sStart, sizeof(sStart));
		
		ret = DDNS_SvcInit(netComm_GetInsideProt(sDdnsCfg.eProt), NULL);	
		if(0 == ret)
		{
			//printf("######\n");
			sStart.bTry = 0;
			ret = DDNS_SvcStart(netComm_GetInsideProt(sDdnsCfg.eProt), &sStart);
			if(ret == 0)
			{
				if(pStart)
				{
					strcpy(sDdnsCfg.sInfo.szDomain, pStart->szDomain);
					strcpy(sDdnsCfg.sInfo.szUser, pStart->szUser);
					strcpy(sDdnsCfg.sInfo.szPasswd, pStart->szPasswd);
				}

				sDdnsCfg.bStart = 1;
			}
		}
		else
		{
			printf("%s svc_init failed!\n", __FUNCTION__);			
		}
	}
	else
	{
		printf("%s init failed!\n", __FUNCTION__);
		ret = -1;
	}
	
	return ret;
}

s32 NetComm_DDNS_Stop( void )
{
	s32 ret = 0;
	
	if(!sDdnsCfg.bInit)
	{
		printf("DDNS not inited 1\n");
		return -1;
	}
	
	if(sDdnsCfg.eProt<EM_NET_DDNS_TL || sDdnsCfg.eProt>=EM_NET_MAX)
	{
		printf("DDNS not supported protocol %d\n",sDdnsCfg.eProt);
		return -1;
	}
	
	SDDNSStopPara sStop;
	memset(&sStop, 0, sizeof(sStop));
	
	strcpy(sStop.sBase.szDomain, sDdnsCfg.sInfo.szDomain);
	strcpy(sStop.sBase.szUser, sDdnsCfg.sInfo.szUser);
	strcpy(sStop.sBase.szPasswd, sDdnsCfg.sInfo.szPasswd);
	
	ret = DDNS_SvcStop(netComm_GetInsideProt(sDdnsCfg.eProt), &sStop);
	if(0 == ret)
	{
		sDdnsCfg.bStart = 0;
	}
	
	printf("[ddns] func %s line %d err %d\n", __FUNCTION__, __LINE__, ret);
	
	return ret;
}

s32 NetComm_DDNS_Register(SNetDdnsPara* pReg)
{
	s32 nRet = 0;

	if(!sDdnsCfg.bInit)
	{
		printf("DDNS not inited 1\n");
		return -1;
	}
	SDDNSRegPara sReg;

	memset(&sReg, 0, sizeof(sReg));

	if(pReg)
	{
		nRet = DDNS_SvcInit(netComm_GetInsideProt(sDdnsCfg.eProt), NULL);
		if(0 == nRet)
		{
			strcpy(sReg.sBase.szDomain, pReg->szDomain);
			strcpy(sReg.sBase.szUser, pReg->szUser);
			strcpy(sReg.sBase.szPasswd, pReg->szPasswd);
			if(0 != DDNS_SvcRegist(netComm_GetInsideProt(sDdnsCfg.eProt), &sReg))
			{
				nRet = -1;
			}
		}
		else
			printf("DDNS_SvcInit failed err %d \n", nRet);
	}
	else
	{
 		nRet = -1;
	}
	
	return nRet;	
}

s32 netComm_Webs_ChangeWebJs( u16 nPort )
{
	char buf[1024] = {0};
	
	#if 1//csp modify
	sprintf(buf, "	MyNetDvrCtrl.ServerPort   = %d;//~ServerPort~", nPort);
	netComm_ReplaceWebRow("/web/pages/js/web_preview.js", "~ServerPort~", buf);
	
	strcpy(buf, "	MyNetDvrCtrl.OEMVersion   = 24;//~OEMVersion~");
	netComm_ReplaceWebRow("/web/pages/js/web_preview.js", "~OEMVersion~", buf);
	#else
	sprintf(buf, "	TLNetDvrCtrl.ServerPort   = %d;//~ServerPort~", nPort);
	netComm_ReplaceWebRow("/web/pages/js/web_preview.js", "~ServerPort~", buf);
	
	strcpy(buf, "	TLNetDvrCtrl.OEMVersion   = 24;//~OEMVersion~");
	netComm_ReplaceWebRow("/web/pages/js/web_preview.js", "~OEMVersion~", buf);
	#endif
	
	return 0;
}

static int SendMsgToDaemon(my_daemon_param *pDaemon)
{
	if(pDaemon == NULL)
	{
		return -1;
	}
	pDaemon->magic_no = MY_DAEMON_MAGIC;
	int s = socket(AF_INET,SOCK_DGRAM,0);
	if(s < 0)
	{
		return -1;
	}
	struct sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr("127.0.0.1");
	to.sin_port = htons(MY_DAEMON_PORT);
	int ret = sendto(s,pDaemon,sizeof(my_daemon_param),0,(struct sockaddr *)&to,sizeof(to));
	//printf("SendMsgToDaemon ret=%d\n",ret);
	close(s);
	
	return ret;
}

void NetComm_InitMultiCastInfo(SNetMultiCastInfo* pMultiCastInfo)
{
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	if(!pMultiCastInfo) return;
	
	// �����豸��Ϣ�Թ��鲥�����豸
	ifly_DeviceInfo_t sDevInfo;
	memset(&sDevInfo, 0, sizeof(sDevInfo));
	
	memcpy(&pNCCIns->sMultiCastInfo, pMultiCastInfo, sizeof(SNetMultiCastInfo));
	memcpy(&sDevInfo, pMultiCastInfo, sizeof(SNetMultiCastInfo));
	
	sDevInfo.reserved[0] 	= 2;
	sDevInfo.devicePort 	= htons(sDevInfo.devicePort);
	
	SetDeviceInfo(sDevInfo);
}

void NetComm_GetMultiCastInfo(SMultiCastInfo* pInfo)
{
	SNetCommCtrl* 	pNCCIns = &sNetCommCtrl;
	
	if(pInfo)
	{
		pInfo->deviceIP		=	pNCCIns->sMultiCastInfo.deviceIP;
		pInfo->devicePort	=	pNCCIns->sMultiCastInfo.devicePort;
		strcpy(pInfo->device_name, pNCCIns->sMultiCastInfo.device_name);
	}
}

void NetComm_SetMultiCastInfo(SMultiCastInfo* pInfo)
{
	SNetCommCtrl* pNCCIns = &sNetCommCtrl;
	
	if(pInfo)
	{
		pNCCIns->sMultiCastInfo.deviceIP 	= pInfo->deviceIP;
		pNCCIns->sMultiCastInfo.devicePort 	= pInfo->devicePort;
		strcpy(pNCCIns->sMultiCastInfo.device_name, pInfo->device_name);
		
		ifly_DeviceInfo_t sDevInfo;
		memcpy(&sDevInfo, &pNCCIns->sMultiCastInfo, sizeof(sDevInfo));
		sDevInfo.reserved[0] 	= 2;
		sDevInfo.devicePort 	= htons(pInfo->devicePort);
		SetDeviceInfo(sDevInfo);
	}
}

s32 NetComm_ReqConnStatus(SNetConnStatus* psNCS)
{	
	if(!psNCS) return -1;
	
	if(CheckNetLink2("ppp0"))
	{
		psNCS->eWay = ECW_PPPOE;
		psNCS->eState = ECS_ONLINE;
	}
	else
	{
		if(CheckNetLink())
		{
			/*
			if(pNCCIns->sCommonCfg.sParaIns.DhcpFlag)
			{
				psNCS->eWay = ECW_DHCP;
			}
			else
				psNCS->eWay = ECW_STATIC;
			*/
			psNCS->eWay = sNetConnStatus.eWay;
			psNCS->eState = ECS_ONLINE;
			//printf("NetComm_ReqConnStatus online\n");
		}
		else
		{
			psNCS->eWay = ECW_STATIC;
			psNCS->eState = ECS_OFFLINE;
			//printf("NetComm_ReqConnStatus offline\n");
		}
	}
	
	//memcpy(psNCS, &sNetConnStatus, sizeof(SNetConnStatus));
	
	return 0;
}

BOOL netComm_SetPPPOEUser(char *username,char *passwd)
{
	//csp modify 20140513
	if(0 == access("/root/3G.conf", F_OK))
	{
		return TRUE;
	}
	
	if(username == NULL || username[0] == '\0')
	{
		return FALSE;
	}
	
	//printf("netComm_SetPPPOEUser enter line %d\n", __LINE__);
	
	char dataline[256];
	if(passwd && passwd[0])
	{
		sprintf(dataline," %s * %s",username,passwd);
	}
	else
	{
		sprintf(dataline," %s *",username);
	}
	
	FILE *fp = NULL;
	
	fp = fopen("/etc/ppp/chap-secrets","w");
	if(fp == NULL)
	{
		//printf("netComm_SetPPPOEUser enter line %d\n", __LINE__);
		return FALSE;
	}
	fputs(" # Secrets for authentication using CHAP\n",fp);
	fputs(" # client    server  secret          IP addresses\n",fp);
	fputs(dataline,fp);
	fputs("\n",fp);
	fclose(fp);
	chmod("/etc/ppp/chap-secrets",S_IRUSR | S_IWUSR);
	
	//printf("netComm_SetPPPOEUser enter line %d\n", __LINE__);
	fp = fopen("/etc/ppp/pap-secrets","w");
	if(fp == NULL)
	{
		//printf("netComm_SetPPPOEUser enter line %d\n", __LINE__);
		return FALSE;
	}
	fputs(" # Secrets for authentication using PAP\n",fp);
	fputs(" # client    server  secret          IP addresses\n",fp);
	fputs(dataline,fp);
	fputs("\n",fp);
	fclose(fp);
	chmod("/etc/ppp/pap-secrets",S_IRUSR | S_IWUSR);
	
	//printf("netComm_SetPPPOEUser ok\n");
	
	return TRUE;
}

u8 NetComm_GetVoipState( void )
{
	return RemoteVoip_GetStatus();
}

void NetComm_UpdateProgNotify(u8 nProgress)
{
	remoteUpdate_ProgNotify(nProgress);
}

//luo 
void net_write_upload_alarm(u32 key)
{
	pthread_mutex_lock(&upload_alarm_mutex);
	//printf("write_pipe:key=%d\n",key);
	write(upload_alarm_fd[1], &key, sizeof(key));
	pthread_mutex_unlock(&upload_alarm_mutex);
}

//void upload_alarm(u8 type, u8 id, u8 state) 
void upload_alarm(u32 key)
{
	//u32 key = (type << 16) | (id << 8) | state;
	net_write_upload_alarm(key);
}

static u8 page_check_statue = 0;
static u8 alarm_check_statue = 0;

void SetRefreshDiskStatue(u8 flag)
{
	page_check_statue = flag;
}

u8 GetRefreshDiskStatue()
{
	return page_check_statue;
}

u8 GetCheckDiskStatue()
{
	return alarm_check_statue;
}

extern int get_disk_info_ex(HddInfo *pHddInfo, int  disk_index, u32* pPrtnNum);
void Alarm_CheckDisk(disk_manager* NetDisk)
{
	//return;
	
	DiskInfo 	dinfo;

	int j = 0;
	int fd = -1;
	int ret = 0;
	u32 nPrtnNum = 0;
	char diskname[64];

	alarm_check_statue = 1;
	
	disk_manager* pDskMgr;
	pDskMgr = (disk_manager*)NetDisk;
	
	for(j=0; j<64; j++)
	{
		sprintf(diskname,"/dev/sd%c",'a'+j);
		
		fd = open(diskname, O_RDONLY);		
		if(fd == -1)
		{
			//printf("update_disk_manager %s is not exist\n",diskname);
			continue;
		}
		else
		{
			close(fd);
				
			ret = ifly_diskinfo(diskname, &dinfo);			
			if(ret != 0)
			{
				//printf("update_disk_manager %s is not exist 3 \n",diskname);
				continue;
			}

			HddInfo* pHddInfo = NULL;

			pHddInfo = &pDskMgr->hinfo[j];

			pHddInfo->is_disk_exist = 1;

			if( 0 == get_disk_info_ex(pHddInfo,0,&nPrtnNum) )
			{		
				printf("%s total = %ld,free = %ld,nPrtnNum = %d\n",__func__, pHddInfo->total,pHddInfo->free,(u32)nPrtnNum);
				if(pHddInfo->total == 0)
				{
					//δ��ʽ��
					u32 key = (4 << 16) | (0 << 8) | 1;
					net_write_upload_alarm(key);
					break;
					//printf("δ��ʽ��!\n");
				}
				else
				{
					//����
				}

				if(0 == pHddInfo->free)
				{
					//Ӳ����
					u32 key = (1 << 16) | (0 << 8) | 1;
					net_write_upload_alarm(key);
					break;
					//printf("Ӳ����!\n");
				}

				//if(nPrtnNum != 4 || pHddInfo->total < pHddInfo->free)
				if(nPrtnNum != MAX_PARTITION_NUM || pHddInfo->total < pHddInfo->free)//3T4T
				{
					u32 key = (5 << 16) | (0 << 8) | 1;
					net_write_upload_alarm(key);
					break;
					//printf("Ӳ�̳���!\n");
				}
			}
			else
			{
				printf("get_disk_info_ex error!\n");
			}
		}
	}

	alarm_check_statue = 0;
}

void* netComm_AlarmUpload_init(void* para)
{
	printf("$$$$$$$$$$$$$$$$$$netComm_AlarmUpload_init id:%d\n",getpid());
	
	//u8 nVideoChn =*(u8*)para;
	u32 key = 0;
	u32 alarmupload[AUTYPENUM];
	
	int j = 0;
	int id = 0;
	int ret = 0;
	int type = 0;
	int state = 0;
	u32 illegal_access = 0;
	u16 MaxChnNum = 32;

	static int check_start_flag = 0;
	static u32 check_start_time = 0;

	disk_manager* NetDisk = (disk_manager*)para;
	
	fd_set action_fds;
	struct timeval tv;

	memset(alarmupload,0,sizeof(alarmupload));
 	memset(&m_alarmstate,0,sizeof(m_alarmstate));
 	memset(&g_AlarmUploadCenter,0,sizeof(g_AlarmUploadCenter));
	
	if (pipe(upload_alarm_fd) < 0)	//init pipe fd for transFd
	{
		printf("upload_alarm_fd pipe:failed,errno=%d,result=%s\n", errno, strerror(errno));
		return NULL;
	}
	
	check_start_time = time(NULL);
	
	while(1) 
	{
		if (1 == alarmupload[8]) 
		{//�Ƿ����ʱ�������
			if (time(NULL) > illegal_access + 5) 
			{
				illegal_access = time(NULL);
				u32 nkey = (8 << 16) | (0 << 8) | 0;
				upload_alarm(nkey);
			}
		}

		if(check_start_flag == 0)//�߳�����30sec����Ӳ��
		{
			if(time(NULL) - check_start_time > 30)
			{
				check_start_flag = 1;
			}
		}
		else
		{
			if((time(NULL) - check_start_time > 60))//1���Ӽ��һ��
			{
				u8 flag = GetRefreshDiskStatue();
				
				if(!flag)
				{
					Alarm_CheckDisk(NetDisk);
					check_start_time = time(NULL);
				}
			}
		}
		
		FD_ZERO(&action_fds);
		FD_SET(upload_alarm_fd[0], &action_fds);
		
		tv.tv_sec  = 60;
		tv.tv_usec = 0;
		
		ret = select(upload_alarm_fd[0] + 1, &action_fds, NULL, NULL, &tv);

		if (ret > 0) 
		{
			if(FD_ISSET(upload_alarm_fd[0], &action_fds)) 
			{
				read(upload_alarm_fd[0], &key, sizeof(key));
			}
		}
		else
		{
			usleep(10 * 1000);
			continue;
		}
		
		if (0xffff0000 == (key & 0xffff0000)) 
		{//��ʼ��ֹͣ�ϴ��������⴦��
			j = key & 0xff;
			if (j >= MAX_ALARM_UPLOAD_NUM)
			{
				continue;
			}

			u8 cmd;
			cmd = (key >> 8) & 0xff;
			
			if (1 == cmd) 
			{// 1 request to start uploading alarm
				g_AlarmUploadCenter[j].g_state = 1;
				//upload all alarm
				if (g_AlarmUploadCenter[j].g_cph && (g_AlarmUploadCenter[j].g_state == 1)) 
				{
					for(type=0;type<AUTYPENUM;type++) 
					{
						m_alarmstate.type = type;
						for(id=0;id<MaxChnNum;id++)
						{
							state = (alarmupload[type] >> id) & 0x1;
							if(1==state) 
							{
								m_alarmstate.state = 2 - state;
								m_alarmstate.id = id;
								//printf("type : %d ------- state : 0x%x---id:%d----- \n",m_alarmstate.type,m_alarmstate.state,m_alarmstate.id);
								ret = CPPost(g_AlarmUploadCenter[j].g_cph,CTRL_NOTIFY_ALARMINFO,&m_alarmstate,sizeof(m_alarmstate));
								if (ret != CTRL_SUCCESS) 
								{ 
									printf("all cppost error ! ret : %d \n",ret);
									g_AlarmUploadCenter[j].g_state = 0;
									g_AlarmUploadCenter[j].g_cph = NULL;
								}
							}
						}
					}
				}	
			} 
			else
			{//0 request to stop uploading alarm
				g_AlarmUploadCenter[j].g_state = 0;
				g_AlarmUploadCenter[j].g_cph = NULL;	
			}
		}
		else 
		{//�ϴ��仯��״̬
			
			type = (key >> 16) & 0xff;
			id = (key >> 8) & 0xff;
			state = key & 0x01;

			printf("yg netComm_AlarmUpload_init : \n");
			printf("\t type: %d\n", type);
			printf("\t id: %d\n", id);
			printf("\t state: %d\n", state);
			
			if (id >= MaxChnNum)
			{
				continue;
			}
			
			if (type >= AUTYPENUM)
			{
				continue;
			}

			if (8 == type && 1 == state) //�Ƿ�����
			{
				illegal_access = time(NULL);
			}
			
			u8 last_state = (alarmupload[type] >> id) & 0x1;
			
			if (last_state == state) 
			{
				continue;
			}
			
			alarmupload[type] &= ~(1 << id);
			alarmupload[type] |= state << id;
			printf("yg netComm_AlarmUpload_init alarmupload[%d]: 0x%x\n", type, alarmupload[type]);
			
			for(j=0;j<MAX_ALARM_UPLOAD_NUM;j++)
			{
				if (g_AlarmUploadCenter[j].g_cph && (g_AlarmUploadCenter[j].g_state == 1))
				{	
					m_alarmstate.type = type;
					m_alarmstate.state = 2 -state;
					m_alarmstate.id=id;
					
					printf("--type : %d ------- id:%d----- state : 0x%x--- \n",m_alarmstate.type, m_alarmstate.id, m_alarmstate.state);
					ret = CPPost(g_AlarmUploadCenter[j].g_cph,CTRL_NOTIFY_ALARMINFO,&m_alarmstate,sizeof(m_alarmstate));
					if (ret != CTRL_SUCCESS)
					{ 
						//printf("cppost error ! ret : %d \n",ret);
						g_AlarmUploadCenter[j].g_state = 0;
						g_AlarmUploadCenter[j].g_cph = NULL;
					}
				}
				else
				{
					g_AlarmUploadCenter[j].g_state = 0;
					g_AlarmUploadCenter[j].g_cph = NULL;
				}
			}
		}
	}

	return NULL;
}

