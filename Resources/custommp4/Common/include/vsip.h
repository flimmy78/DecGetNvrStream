#ifndef _VSIP_H_
#define _VSIP_H_

#include "ctrlprotocol.h"

#ifdef CSVISION

#include "controlglobal.h"
#include "MsgDefines.h"
#include "VSWCDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

//�豸������
typedef struct VsipCapability
{
	u8		byVideoEncNum;			//��Ƶ����ͨ������
	u8		byVideoDecNum;			//��Ƶ����ͨ������
	u8		byAudioEncNum;			//��Ƶ����ͨ������
	u8		byAudioDecNum;			//��Ƶ����ͨ������
	u8		bySerialPortNum;		//��������
	u8		byInputPinNum;			//������������
	u8		byOutputPinNum;			//�����������
	u8		byVideoInputPortNum;	//��Ƶ����˿�����
}VSIP_CAPABILITY, *PVSIP_CAPABILITY;

extern SOCKHANDLE vsip_udp_socket;
extern struct VSWC_VCODEC_ENC_CONTEXT	m_VCodecEncContext[4];

void n2hVSIPHeader(VSWC_CONTROL_HEADER *pHeader);
void h2nVSIPHeader(VSWC_CONTROL_HEADER *pHeader);
void n2hGUID(GUID *pID);
void h2nGUID(GUID *pID);

u16 VSIPInit();

u16 SetVsipServAddr(u32 dwIp,u16 wPort);

u16 GetVsipGuid(GUID *pGuid);
u16 SetVsipGuid(char *pMac);

u16 SetVsipCapability(VSIP_CAPABILITY *pCapability);

unsigned short CountVsipCapability();

u16 GetVsipPingRespMsg(VSWC_PING_RESPOND_MSG *pMsg);
u16 SetVsipPingRespMsg(VSWC_PING_RESPOND_MSG *pMsg);

u16 GetVsipSysPubParam(VSWC_D_GET_CONFIG_RESP *pMsg);
u16 SetVsipSysPubParam(VSWC_D_GET_CONFIG_RESP *pMsg);

u16 SendVsipDataToServ(char *buf,int len);

u16 SendPingRespMsgToServ();

u16 RecvVsipMsg();

#ifdef __cplusplus
}
#endif

#endif

#endif
