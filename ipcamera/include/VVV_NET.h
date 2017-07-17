#ifndef __VVV_NET_H__
#define __VVV_NET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif 

#define LINUX

#if defined(WIN32)

#if defined(VVV_NET_DLL_EXPORTS)
#define VVV_NET_API      __declspec( dllexport )
#define VVV_NET_APICALL  __stdcall

#elif	defined(VVV_NET_LIB)
#define VVV_NET_API      
#define VVV_NET_APICALL  

#else 
#define VVV_NET_API      __declspec( dllimport )
#define VVV_NET_APICALL  __stdcall
#error "win32 platform!"
#endif
#elif defined(LINUX)
#define VVV_NET_API
#define VVV_NET_APICALL
#else
#error "Please specify a design-in platform!"
#endif

#include "VVV_NET_define.h"



/************************************************************************/
/* ý�庯����                                                           */
/************************************************************************/

/* ��ʼ����ȥ��ʼ��ֻ�����һ��  */
VVV_NET_API int VVV_NET_APICALL VVV_NET_Init();

VVV_NET_API int VVV_NET_APICALL VVV_NET_DeInit();

/*  �û���¼ */
VVV_NET_API int VVV_NET_APICALL VVV_NET_Login(unsigned int* pu32DevHandle, /*�����豸���,���������豸���������ڸ��豸�������*/
                 const char* psUsername,     /*�����û���*/
                 const char* psPassword,     /*��������*/
                 const char* psHost,         /*������IP*/
                 unsigned short u16Port,     /*�������˿ں�*/
                 VVV_NET_PROTOCOL_S stNetProtocol, /*����Э��*/
                 int s32ConnectTimeout,/*�������ӳ�ʱʱ�䣬Ĭ�ϳ�ʱ��5�룬��λ�Ǻ���*/
                 VVV_ON_EVENT_CALLBACK cbEventCallBack,/*�¼��ص�����ҪΪ��������״̬*/
                 userpassword_s *pUserAuth,  /*���ص�ǰ�û�Ȩ��*/
                 void* pUserData);           /*�û����ݣ�����¼��ص������з��س���*/


/* �û��ǳ�*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_Logout(unsigned int u32DevHandle);


/* �������糬ʱʱ��*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SetTimeout(unsigned int u32DevHandle,
                      int s32ConnectTimeout,/*�������ӳ�ʱʱ�䣬Ĭ�ϳ�ʱ��5�룬��λ�Ǻ���*/
                      int s32TransTimeout); /*���紫�䳬ʱʱ�䣬Ĭ�ϳ�ʱ��10�룬��λ�Ǻ���*/


/* ��ʼ������ */
VVV_NET_API int VVV_NET_APICALL VVV_NET_StartStream(unsigned int *pu32ChnHandle,/*����ͨ���������������ͨ�����������ڸ�ͨ���������*/
					   unsigned int u32DevHandle,/*�����豸���*/
                       unsigned int chn,/*ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��*/
                       unsigned int u32StreamType,/*����������,���VVV_STREAM_TYPE_E
                                                              0x01:VVV_STREAM_TYPE_VIDEO,
                                                              0x02:VVV_STREAM_TYPE_AUDIO,
                                                              0x03:VVV_STREAM_VIDEO_AUDIO,
                                                              0x04:VVV_STREAM_MD_ONLY,
                                                              0x08:VVV_STREAM_SENSORIN_ONLY,
                                                              0x10:VVV_STREAM_SHELTER_ONLY,
                                                              0x20:VVV_STREAM_VIDEO_LOSS_ONLY,
                                                              0x40:VVV_STREAM_DISK_ERRO_ONLY,
                                                              0xFFF:VVV_STREAM_ALL*/
                       unsigned int u32StreamFlag,/*��������,0:����������,1:������,2:��������,3:��������*/
                       VVV_STREAM_INFO_S *pstruStreamInfo,/*����������Ϣ*/
                       VVV_ON_STREAM_CALLBACK cbStreamCallBack,/*�����ص�*/
                       VVV_ON_DATA_CALLBACK cbDataCallBack,/*�澯���ݻص�*/
                       void* pUserData);/*�û�����*/

/* ��ʼ������ */
VVV_NET_API int VVV_NET_APICALL VVV_NET_StartStream_EX(unsigned int *pu32ChnHandle,/*����ͨ���������������ͨ�����������ڸ�ͨ���������*/
					   unsigned int u32DevHandle,/*�����豸���*/
                       unsigned int chn,/*ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��*/
                       unsigned int u32StreamType,/*����������,���VVV_STREAM_TYPE_E
                                                              0x01:VVV_STREAM_VIDEO_ONLY,
                                                              0x02:VVV_STREAM_AUDIO_ONLY,
                                                              0x03:VVV_STREAM_VIDEO_AUDIO,
                                                              0x04:VVV_STREAM_MD_ONLY,
                                                              0x08:VVV_STREAM_SENSORIN_ONLY,
                                                              0x10:VVV_STREAM_SHELTER_ONLY,
                                                              0x20:VVV_STREAM_VIDEO_LOSS_ONLY,
                                                              0x40:VVV_STREAM_DISK_ERRO_ONLY,
                                                              0xFFF:VVV_STREAM_ALL*/
                       unsigned int u32StreamFlag,/*��������,0:����������,1:������,2:��������,3:��������*/
                       VVV_STREAM_INFO_S *pstruStreamInfo,/*����������Ϣ*/
                       VVV_ON_STREAM_CALLBACK cbStreamCallBack,/*�����ص�*/
                       VVV_ON_DATA_CALLBACK cbDataCallBack,/*�澯���ݻص�*/
                       void* pUserData,/*�û�����*/
                       unsigned int u32MaxPacketBuff);

/* ֹͣ������  */
VVV_NET_API int VVV_NET_APICALL VVV_NET_StopStream(unsigned int u32ChnHandle); /*����ͨ�����*/


/* �����Խ�*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_StartTalk(unsigned int u32DevHandle,
                     VVV_ON_TALK_CALLBACK cbTalkCallBack,/*���öԽ���Ƶ���ݻص�:�����->�ͻ���*/
                     void* pUserData);

VVV_NET_API int VVV_NET_APICALL VVV_NET_StopTalk(unsigned int u32DevHandle);


/* ������������:�ͻ���->�����*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SendTalkData(unsigned int u32DevHandle, unsigned char* psBuf, unsigned int u32BufLen, VVV_U64 u64Pts);

/*�ֶ�ץ��VVV_CMD_SNAP*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_Snap(unsigned int u32ChnHandle,         /*����ͨ�����*/
                VVV_SNAP_REQ_S stSnapAttr,          /*ץ������*/
                VVV_ON_SNAP_CALLBACK cbSnapCallBack,/*ץ�����ݻص�*/
                void* pUserData);

/*�ֶ�ץ��VVV_CMD_SNAP_CHN*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_Snap_Chn(unsigned int u32ChnHandle,         /*����ͨ�����*/
                VVV_SNAP_CHN_REQ_S stSnapAttr,          /*ץ������*/
                VVV_ON_SNAP_CALLBACK cbSnapCallBack,/*ץ�����ݻص�*/
                void* pUserData);

/*��ȡ¼���ļ�����VVV_CMD_REC_GETCALENDER*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_GetVideoFileCalendar(unsigned int u32DevHandle, VVV_TIME_S date, int* lCalendarMap);


/*¼���ļ�����VVV_CMD_REC_SEARCH_START*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SearchAVFileList(unsigned int u32DevHandle,
                            VVV_FIND_FILE_REQ_S SearchReq,/*��ѯ����*/
                            VVV_FILE_LIST_VMS_S* FileList); /*��ѯ�����ļ��б���*/

/*�ͷ�¼���ļ�����VVV_CMD_REC_SEARCH_FREE*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_FreeAVFileList(unsigned int u32DevHandle,
                          VVV_FILE_LIST_VMS_S* FileList);

/*����¼���ļ����ػص�VVV_CMD_REC_SETFILE_CB*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SetFileDataCallBackVMS(unsigned int u32DevHandle,void* pUserData,VVV_ON_FILE_CALLBACK cbAVFileCallBack);


/*Զ��¼���ļ�������(��������)VVV_CMD_REC_DOWNLOAD_START*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_VodDownloadClipVMS(unsigned int u32DevHandle, 
                              VVV_DownloadFileReq_S DownloadFileReq);/*��������*/


/*ֹͣ¼���ļ�����VVV_CMD_REC_DOWNLOAD_STOP*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_StopDownLoadVideoFile(unsigned int u32DevHandle);


// ��ʱ��ط�	
/*VVV_CMD_REC_STARTPB_BYTIME*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_PlayBackByTime(unsigned int *pPlayBackHandle, unsigned int u32DevHandle,
                          VVV_FIND_FILE_REQ_S DownloadFileReq);/*��ʱ��ط�����*/

/*VVV_CMD_REC_STOPPB_BYTIME*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_StopPlayBack(unsigned int u32PlayBackHandle);



/*VVV_CMD_REC_SET_PB_CB*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SetPlayBackDataCallBack(unsigned int u32PlayBackHandle,void* pUserData,VVV_ON_pfnPlayBackCallback fnPlayBackDataCallback);

/*��ȡ��־�ļ�����VVV_CMD_LOG_GETCALENDAR*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_GetLogCalendar(unsigned int u32DevHandle,VVV_TIME_S date, int* lCalendarMap);

/*������־VVV_CMD_LOG_DOWNLOAD*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_DownLoadLog(unsigned int u32DevHandle,
                       VVV_SEARCH_LOG_CONDITION_S lpLogQueryInfo,/*��־��ѯ����*/
                       VVV_LOG_DATA* lpLogData );    /*��־��ѯ���*/

/*�����־VVV_CMD_LOG_CLEAR*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_ClearLog(unsigned int u32DevHandle, VVV_LOG_CLEAR_S stLogAttr);

/*��̨����,dwPTZCmdȡֵ���"��ָ̨��"*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_PTZControl(unsigned int u32DevHandle, unsigned int chnNo, unsigned int dwPTZCmd,unsigned int dwValue);

/*����,VVV_CMD_UPGRADE����*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_Upgrade(unsigned int u32DevHandle, VVV_UPGRADE_ATTR_S stUpgradeAttr);

/*ɨ��wifi,VVV_CMD_SCAN_WIFI����*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_ScanWifi(unsigned int u32DevHandle, VVV_WIFI_INFO_S *pWifiInfo);

/*���÷���������,dwCmdȡֵ���"���÷�������������"*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_SetServerConfig(unsigned int u32DevHandle, unsigned int dwCmd, void* lpData, unsigned int Size);

/*��ȡ����������,dwCmdȡֵ���"��ȡ��������������"*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_GetServerConfig(unsigned int u32DevHandle, unsigned int dwCmd, void* lpData, unsigned int *pSize);

/*��ȡ�����汾��*/
VVV_NET_API int VVV_NET_APICALL VVV_NET_GetVersion(char *pVersion);


#if defined(WIN32)
#define HI_LOG_DEBUG
#define HI_LOG_SYS
#else
#define HI_LOG_DEBUG(pszMode,u32LogLevel, args ...) printf(args)
#define HI_LOG_SYS(pszMode,u32LogLevel, args ...) printf(args)


#endif

 
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  //__VVV_NET_H__


