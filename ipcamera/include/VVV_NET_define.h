#ifndef __VVV_NET_DEFINE_H__
#define __VVV_NET_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif 

//����ṹ��ȫ��1�ֽڶ���
#pragma pack (4)


#ifdef WIN32 //windows ����, �ڹ�����ָ���˱���ѡ��
#define VVV_OS_TYPE VVV_OS_WIN32
typedef unsigned __int64  VVV_U64;
#else
#define VVV_OS_LINUX 1
#define VVV_OS_WIN32 2
#define VVV_OS_TYPE VVV_OS_LINUX
typedef unsigned long long  VVV_U64;
#define SOCKET       int
#define SOCKET_ERROR (-1)
#endif

/************************************************************************/
/*   ͨ�ú궨��                                                         */
/************************************************************************/

#define VVV_MAX_CHN_NUM        16
#define VVV_MAX_DISK_NUM       8
#define VVV_NAME_LEN           32
#define VVV_EMAIL_NUM          3
#define VVV_IP_LEN             36
#define VVV_PATHNAME_LEN       128
#define VVV_MAX_DOMAIN_NAME    64
#define VVV_MAX_SENSOR_IN      16
#define VVV_SENSOR_OUT         16
#define ASSIGN_FOUR_BYTES(number) (((number) + 3) / 4 * 4)
#define LIVE_CLIENT_INVALID_THD_ID    (0xFFFFFFFF)

/************************************************************************/
/*    ������                                                            */
/************************************************************************/
#define VVV_SUCCESS                           0   //�ɹ�
#define VVV_ERR_FAILURE                      -1   //�ڲ�����
#define VVV_ERR_PARA                         -2   //��������
#define VVV_ERR_POINT_NULL                   -3   //ָ��Ϊ��
#define VVV_ERR_NOT_SUPPORT_CMD              -4   //��֧�ֵ�����
#define VVV_ERR_NET_ERRO                     -5   //�������
#define VVV_ERR_NEED_FREE                    -6   //��Ҫ�ͷ��ڴ�
#define VVV_ERR_SERVER_USER_OPRATE           -7   //�������û����ڲ���
#define VVV_ERR_BADURL                       -11
#define VVV_ERR_CONN_FAILED                  -12
#define VVV_ERR_SOCKREAD_FAILED              -13
#define VVV_ERR_METHOD_ERR                   -14
#define VVV_ERR_PARSE_ERROR                  -15
#define VVV_ERR_SEQ                          -16
#define VVV_ERR_ALREADY_RUNNING              -17
#define VVV_ERR_START_FAILED                 -18
#define VVV_ERR_INLGAL_PARAM                 -19
#define VVV_ERR_ALREAD_START                 -20
#define VVV_ERR_MALLOC_FAILED                -21
#define VVV_ERR_ERROR_PROTOCAL               -22
#define VVV_ERR_RESP_UNSTANDARD              -23
#define VVV_ERR_RESP_NO_CSEQ                 -24
#define VVV_ERR_RESP_CSEQ_ERR                -25
#define VVV_ERR_RESP_NO_SESSION              -26
#define VVV_ERR_RESP_SESSION_ERR             -27
#define VVV_ERR_RESP_NO_TRANSPORT            -28
#define VVV_ERR_RESP_TRANSPORT_ERR           -29
#define VVV_ERR_RESP_NO_VIDEO_ATTR           -30
#define VVV_ERR_RESP_VIDEO_ATTR_ERR          -31
#define VVV_ERR_RESP_UNRECOGNIZE_VIDEO_ATTR  -32
#define VVV_ERR_RESP_NO_AUDIO_ATTR           -33
#define VVV_ERR_RESP_AUDIO_ATTR_ERR          -34
#define VVV_ERR_START_RECV_THREAD_FAILED     -35
#define VVV_ERR_RESP_UNRECOGNIZE_AUDIO_ATTR  -36
#define VVV_ERR_VIDEO_ALREADY_PAUSED         -37
#define VVV_ERR_AUDIO_ALREADY_PAUSED         -38
#define VVV_ERR_DATA_ALREADY_PAUSED          -39
#define VVV_ERR_VIDEO_ALREADY_PLAYED         -40
#define VVV_ERR_AUDIO_ALREADY_PLAYED         -41
#define VVV_ERR_DATA_ALREADY_PLAYED          -42
#define VVV_ERR_ERROR_TRANSTYPE              -43
#define VVV_ERR_INVALID_USER                 -44
#define VVV_ERR_OVER_SERVER_CONNECTION_NUM   -45
#define VVV_ERR_PAUSE_FAILED                 -46
#define VVV_ERR_REPLAY_FAILED                -47
#define VVV_ERR_SOCKSEND_FAILED              -48
#define VVV_ERR_USER_OR_PASSWORD             -49
#define VVV_ERR_ALMSEN_WAIT_TIMEOUT          -50  //�ȴ������յ���ʱ
#define VVV_ERR_ALMSEN_ERROR_COMMAND         -51  //��������
#define VVV_ERR_ALMSEN_UNKNOW_COMMAND        -52  //δ֪����
#define VVV_ERR_ALMSEN_FULL_DETECTOR         -53  //���̽���������ﵽ����
#define VVV_ERR_ALMSEN_NULL_DETECTOR         -54  //̽��������Ϊ0
#define VVV_ERR_ALMSEN_FULL_TELECTRL         -55  //���ң���������ﵽ����
#define VVV_ERR_ALMSEN_NULL_TELECTRL         -56  //ң��������Ϊ0
#define VVV_ERR_ALMSEN_NOT_RECV_SIGNAL       -57  //ѧϰ״̬��û�н��յ������豸���ź�
#define VVV_ERR_ALMSEN_NOT_STUDY_DEV         -58  //�澯����δ����ѧϰģʽ

/************************************************************************/
/* ý�����ָ��                                                         */
/************************************************************************/
#define    VVV_CMD_SNAP                0x00000001 //ץ��,fun:VVV_NET_Snap,Param:VVV_SNAP_REQ_S
#define    VVV_CMD_SNAP_CHN			   0x0000000F //ץ��,fun:VVV_NET_Snap,Param:VVV_SNAP_CHN_REQ_S
#define    VVV_CMD_REC_GETCALENDER     0x00000002 //��ȡ¼���ļ�����,fun:VVV_NET_GetVideoFileCalendar,Param:VVV_TIME_S
#define    VVV_CMD_REC_SEARCH_START    0x00000003 //¼���ļ�����,fun:VVV_NET_SearchAVFileList,Param:VVV_FILE_LIST_VMS_S
#define    VVV_CMD_REC_SEARCH_FREE     0x00000004 //�ͷ��ļ�����,fun:VVV_NET_FreeAVFileList,Param:VVV_FILE_LIST_S
#define    VVV_CMD_REC_SETFILE_CB      0x00000005 //�����ļ����ػص�,fun:VVV_NET_SetFileDataCallBackVMS,Param:VVV_ON_FILE_CALLBACK
#define    VVV_CMD_REC_DOWNLOAD_START  0x00000006 //����¼���ļ�,fun:VVV_NET_VodDownloadClipVMS,Param:VVV_DownloadFileReq_S
#define    VVV_CMD_REC_DOWNLOAD_STOP   0x00000007 //ֹͣ�����ļ�,fun:VVV_NET_StopDownLoadVideoFile,Param:char* pszSouFileName
#define    VVV_CMD_REC_STARTPB_BYTIME  0x00000008 //������ʱ�������ļ�,fun:VVV_NET_PlayBackByTime
#define    VVV_CMD_REC_STOPPB_BYTIME   0x00000009 //ֹͣ��ʱ������,fun:VVV_NET_StopPlayBack
#define    VVV_CMD_REC_SET_PB_CB       0x0000000A //����ʱ�����ػص�,fun:VVV_NET_SetPlayBackDataCallBack
#define    VVV_CMD_LOG_GETCALENDAR     0x0000000B //������־����,fun:VVV_NET_GetLogCalendar,Param:VVV_TIME_S
#define    VVV_CMD_LOG_DOWNLOAD        0x0000000C //������־,fun:VVV_NET_DownLoadLog,Param:VVV_SEARCH_LOG_CONDITION_S
#define    VVV_CMD_LOG_CLEAR           0x0000000D //�����־,fun:VVV_NET_ClearLog,Param:VVV_LOG_CLEAR_S
#define    VVV_CMD_UPGRADE             0x0000000E //����,fun:VVV_NET_Upgrade,Param:VVV_UPGRADE_ATTR_S
#define    VVV_CMD_SCAN_WIFI           0x00000020 //ɨ��wifi,fun:VVV_NET_ScanWifi,Param:VVV_WIFI_INFO_S

/************************************************************************/
/* ��ָ̨��                                                             */
/************************************************************************/
#define    VVV_CMD_PTZ_UP              0x00001001 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_DOWN            0x00001002 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_LEFT            0x00001003 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_RIGHT           0x00001004 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_FOCUS_SUB       0x00001005 //Focus Far
#define    VVV_CMD_PTZ_FOCUS_ADD       0x00001006 //Focus Near
#define    VVV_CMD_PTZ_ZOOM_SUB        0x00001007 //Zoom Wide
#define    VVV_CMD_PTZ_ZOOM_ADD        0x00001008 //Zoom Tele
#define    VVV_CMD_PTZ_IRIS_SUB        0x00001009 //Iris Close
#define    VVV_CMD_PTZ_IRIS_ADD        0x0000100A //Iris Open
#define    VVV_CMD_PTZ_STOP            0x0000100B
#define    VVV_CMD_PTZ_PRESET          0x0000100C //Ԥ��
#define    VVV_CMD_PTZ_CALL            0x0000100D //����
#define    VVV_CMD_PTZ_AUTO_STRAT      0x0000100E //�Զ�
#define    VVV_CMD_PTZ_AUTO_STOP       0x0000100F
#define    VVV_CMD_PTZ_LIGHT_OPEN      0x00001010 //�ƹ�
#define    VVV_CMD_PTZ_LIGHT_CLOSE     0x00001011
#define    VVV_CMD_PTZ_BRUSH_START     0x00001012 //��ˢ
#define    VVV_CMD_PTZ_BRUSH_STOP      0x00001013
#define    VVV_CMD_PTZ_TRACK_START     0x00001014 //�켣
#define    VVV_CMD_PTZ_TRACK_STOP      0x00001015
#define    VVV_CMD_PTZ_TRACK_RUN       0x00001016
#define    VVV_CMD_PTZ_HOME            0x00001017 //�ص�ԭ��
#define    VVV_CMD_PTZ_CRUISE_V        0x00001018 //����Ѳ��
#define    VVV_CMD_PTZ_CRUISE_H        0x00001019 //����Ѳ��
#define    VVV_CMD_PTZ_CRUISE_HV       0x00001020 //����,����Ѳ��
#define    VVV_CMD_PTZ_UP_STEP         0x00001021 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_DOWN_STEP       0x00001022 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_LEFT_STEP       0x00001023 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S
#define    VVV_CMD_PTZ_RIGHT_STEP      0x00001024 //��̨����,fun:VVV_NET_PTZControl,Param:VVV_PTZ_CMD_S


/************************************************************************/
/* ���÷�������������                                                   */
/************************************************************************/
#define    VVV_CMD_REBOOT              0x00000101  //����������,Param:
#define    VVV_CMD_POWER_DOWN          0x00000102  //�ػ�,Param:
#define    VVV_CMD_RESET               0x00000103  //��λ,�������ָ�Ĭ�ϲ���,Param:
#define    VVV_CMD_RESTORE_SETTING     0x00000105  //�ϴ�����,Param:VVV_SETTING_S
#define    VVV_CMD_UPDATEFLASH         0x00000107  //���浽Flash,Param:
#define    VVV_CMD_SET_TIME            0x00000108  //������ʱ��,Param:VVV_Server_Time_S
#define    VVV_CMD_SET_LANGUAGE        0x00000109  //����������,Param:VVV_LANGUAGE_S
#define    VVV_CMD_SET_MAINTAIN        0x0000010A  //�豸ά��,Param:VVV_DEVICE_MAINTAIN_S
#define    VVV_CMD_SET_CHN_COLOR       0x0000010B  //ͨ��ɫ��,Param:VVV_CHN_COLOR_S
#define    VVV_CMD_SET_CHN_IMAGE       0x0000010C  //ͨ��ͼ������,Param:VVV_CHN_IMAGE_S
#define    VVV_CMD_SET_CHN_OSDINFO     0x0000010D  //ͨ��OSD,Param:VVV_CHN_OSDINFO_S
#define    VVV_CMD_SET_CHN_LOGO        0x0000010E  //ͨ��Logo,Param:VVV_CHN_LOGO_S
#define    VVV_CMD_SET_CHN_OVERLAY     0x0000010F  //ͨ���ڸ�,Param:VVV_CHN_OVERLAY_S

#define    VVV_CMD_SET_VO              0x00000110  //������ͨ�������ʾ,Param:VVV_VO_SETTING_S
#define    VVV_CMD_SET_CHN_VOOSD       0x00000111  //������ͨ�����OSD��ʾ,Param:VVV_CHN_VOOSD_S

#define    VVV_CMD_SET_VENC_CONFIG     0x00000112  //ͨ����Ƶ������Ϣ,Param:VVV_VENC_CONFIG_S
#define    VVV_CMD_SET_AENC_CONFIG     0x00000113  //ͨ����Ƶ������Ϣ,Param:VVV_AENC_CONFIG_S
#define    VVV_CMD_REQUEST_KEYFRAME    0x00000114  //����ؼ�֡,Param:VVV_CHN_FRAMEREQ_S

#define    VVV_CMD_SET_SNAPSHOT        0x00000115  //������ץͼ����,Param:VVV_SNAP_S
#define    VVV_CMD_SET_REC_CONFIG      0x00000116  //¼������,Param:VVV_RECORD_CONFIG_S
#define    VVV_CMD_RECORD_BEGIN        0x00000117  //�ֶ���ʼ������¼��,Param:VVV_RECORD_MANUEL_S
#define    VVV_CMD_RECORD_STOP         0x00000118  //�ֶ�ֹͣ������¼��,Param:VVV_RECORD_MANUEL_S

#define    VVV_CMD_SET_DEC             0x00000119  //����������ط�,Param:VVV_DEC_S

#define    VVV_CMD_FORMAT_DISK         0x0000011A  //��ʽ������,Param:VVV_FORMAT_DISK_S
#define    VVV_CMD_SET_DISK_MNG        0x0000011B  //����¼������,Param:VVV_DISK_MNG_S

#define    VVV_CMD_SET_ALARM           0x0000011C  //�澯�������,Param:VVV_ALARM_SET_S
#define    VVV_CMD_SET_MD_ALARM        0x0000011D  //ͨ���ƶ����澯,Param:VVV_CHN_MD_ALARM_S
#define    VVV_CMD_SET_SENSOR_IN_ALARM 0x0000011E  //̽ͷ�澯,Param:VVV_SENSOR_IN_ALARM_S
#define    VVV_CMD_SET_VIDEO_SHELTER   0x0000011F  //��Ƶ�ڵ��澯,Param:VVV_VIDEO_SHELTER_S
#define    VVV_CMD_SET_EXCEPTION_ALARM 0x00000120  //�쳣�澯,Param:VVV_EXCEPTION_ALARM_S
#define    VVV_CMD_CLEAR_ALARM         0x00000121  //����澯,Param:VVV_CLEAR_ALARM_S

#define    VVV_CMD_SET_PTZ_CRUISE      0x00000122  //��̨Ѳ������,Param:VVV_PTZ_CRUISE_S
#define    VVV_CMD_SET_COMINFO         0x00000123  //COM����̨,Param:VVV_SERVER_COMINFO_S
#define    VVV_CMD_SEND_COMDATA        0x00000124  //����COM��Ϣ��������,Param:VVV_COM_DATA_S
#define    VVV_CMD_SET_USERINFO        0x00000125  //�û������Ȩ��,Param:VVV_SERVER_USER_S
#define    VVV_CMD_SET_IECLIENT_USER   0x00000126  //IE�Ϳͻ����û�,Param:VVV_IE_CLIENT_USER_S
#define    VVV_CMD_SET_IP_DEVLIST      0x00000127  //NVR������IP�豸�����б�,Param:VVV_IPDEV_LIST_S

#define    VVV_CMD_SET_NETWORK         0x00000128   //��������������,Param:VVV_SERVER_NETWORK_S
#define    VVV_CMD_SET_DDNSCFG         0x00000129   //����DDNS����,Param:VVV_DDNSCFG_S
#define    VVV_CMD_SET_UPNP            0x0000012A   //UPNP����,Param:VVV_UPNP_CONFIG_S
#define    VVV_CMD_SET_FTP             0x0000012B   //���÷�����FTP�ϴ�����ץ��ͼƬ,Param:VVV_FTP_PARAM_S
#define    VVV_CMD_SET_EMAIL           0x0000012C   //Email����,Param:VVV_EMAIL_PARAM_S
#define    VVV_CMD_SET_NTP             0x0000012D   //NTP����,Param:VVV_NTP_PARAM_S
#define    VVV_CMD_SET_CENTER_INFO     0x0000012E   //�ϱ����ķ�����,Param:VVV_CENTER_INFO_S
#define    VVV_CMD_SET_WIFI            0x0000012F   //WIFI����,Param:VVV_WIFI_CONFIG_S
#define    VVV_CMD_SET_3G              0x00000130   //3G����,Param:VVV_3G_CONFIG
#define    VVV_CMD_SET_MOBILE          0x00000131   //�ֻ��㲥����,Param:VVV_MOBILE_CONFIG_S
#define    VVV_CMD_SET_P2P             0x00000132   //P2P����,Param VVV_P2P_CONFIG_S 
#define    VVV_CMD_SET_MICROWAVE_ALARM 0x00000133   //΢���澯,Param:VVV_MICROWAVE_ALARM_S
#define    VVV_CMD_SET_BUZZER          0x00000135  // ��/�ط�����,Param:VVV_BUZZER_CONFIG
#define    VVV_CMD_SET_RFID_INFO       0x00000136  //ע�������RFID,Param:VVV_SERVER_RFID_S
#define    VVV_CMD_SET_AUDIOIN_GAIN    0x00000137   //����IPC����������С(0~100)0:����,100:���
#define    VVV_CMD_SET_AUDIOOUT_VOL    0x00000138   //����IPC���������С(0~100)0:����,100:���
#define    VVV_CMD_ENABLE_AEC          0x00000139   //���û�����������
#define    VVV_CMD_DISABLE_AEC         0x0000013a   //���û�����������
#define    VVV_CMD_SET_PIR_ALARM       0x0000013b   //PIR�澯,Param:VVV_PIR_ALARM_S
#define    VVV_CMD_SET_HUMI_ALARM      0x0000013c   //HUMI�澯,Param:QV_HUMI_ALARM_S
#define    VVV_CMD_SET_MD_ALARM_EX     0x0000013d  //ͨ���ƶ����澯,Param:VVV_CHN_MD_ALARM_EX_S
#define    VVV_CMD_SET_NETWORK_PORT    0x0000013e   //����������˿�����,Param:VVV_SERVER_NETWORK_PORT_S
#define    VVV_CMD_SET_PTZ_ATTR        0x0000013F   //PTZ����,Param:VVV_PTZ_ATTR_S, *PVVV_VVV_PTZ_ATTR_S;
#define    VVV_CMD_SET_SENSOR_IN_ALARM_EX 0x00000140  //̽ͷ�澯,Param:VVV_SENSOR_IN_ALARM_EX_S
#define    VVV_CMD_SET_REC_CONFIG_EX   0x00000141  //¼������,Param:VVV_RECORD_CONFIG_EX_S
#define    VVV_CMD_SET_ALARM_SNAPSHOT  0x00000142  //������ץͼ����,Param:VVV_ALARM_SNAP_S

/************************************************************************/
/* ��ȡ��������������                                                   */
/************************************************************************/
#define    VVV_CMD_GET_SERVER_INFO     0x10000001  //��������Ϣ,Param:VVV_SERVER_INFO_S
#define    VVV_CMD_GET_SERVER_CAPS     0x10000002  //������������,Param:VVV_SERVER_CAP_S
#define    VVV_CMD_BACKUP_SETTING      0x10000003  //���ݲ���,Param:VVV_SETTING_S
#define    VVV_CMD_GET_TIME            0x10000004  //������ʱ��,Param:VVV_Server_Time_S
#define    VVV_CMD_GET_LANGUAGE        0x10000005  //����������,Param:VVV_LANGUAGE_S
#define    VVV_CMD_GET_MAINTAIN        0x10000006  //�豸ά��,Param:VVV_DEVICE_MAINTAIN_S

#define    VVV_CMD_GET_CHN_COLOR       0x10000007  //ͨ��ɫ��,Param:VVV_CHN_COLOR_S
#define    VVV_CMD_GET_CHN_IMAGE       0x10000008  //ͨ��ͼ������,Param:VVV_CHN_IMAGE_S
#define    VVV_CMD_GET_CHN_OSDINFO     0x10000009  //ͨ��OSD,Param:VVV_CHN_OSDINFO_S
#define    VVV_CMD_GET_CHN_LOGO        0x1000000A  //ͨ��Logo,Param:VVV_CHN_LOGO_S
#define    VVV_CMD_GET_CHN_OVERLAY     0x1000000B  //ͨ���ڸ�,Param:VVV_CHN_OVERLAY_S

#define    VVV_CMD_GET_VO              0x1000000C  //������ͨ�������ʾ,Param:VVV_VO_SETTING_S
#define    VVV_CMD_GET_CHN_VOOSD       0x1000000D  //������ͨ�����OSD��ʾ,Param:VVV_CHN_VOOSD_S

#define    VVV_CMD_GET_VENC_CONFIG     0x1000000E  //ͨ����Ƶ������Ϣ,Param:VVV_VENC_CONFIG_S
#define    VVV_CMD_GET_AENC_CONFIG     0x1000000F  //ͨ����Ƶ������Ϣ,Param:VVV_AENC_CONFIG_S

#define    VVV_CMD_GET_SNAPSHOT        0x10000010  //������ץͼ����,Param:VVV_SNAP_S
#define    VVV_CMD_GET_REC_CONFIG      0x10000011  //¼������,Param:VVV_RECORD_CONFIG_S
#define    VVV_CMD_GET_REC_STATE       0x10000012  //¼��״̬,Param:VVV_RECORD_STATE_S

#define    VVV_CMD_GET_DEC             0x10000013  //����������ط�,Param:VVV_DEC_S

#define    VVV_CMD_GET_DISK_INFO       0x10000014  //������Ϣ,Param:VVV_DISK_INFO_S
#define    VVV_CMD_GET_DISK_MNG        0x10000015  //����¼������,Param:VVV_DISK_MNG_S
#define    VVV_CMD_GET_DISK_STATE      0x10000016  //����״̬,Param:VVV_DISK_STATE_S

#define    VVV_CMD_GET_ALARM           0x10000017  //�澯���,Param:VVV_ALARM_SET_S
#define    VVV_CMD_GET_MD_ALARM        0x10000018  //ͨ���ƶ����澯,Param:VVV_CHN_MD_ALARM_S
#define    VVV_CMD_GET_SENSOR_IN_ALARM 0x10000019  //̽ͷ�澯,Param:VVV_SENSOR_IN_ALARM_S
#define    VVV_CMD_GET_VIDEO_SHELTER   0x1000001A  //��Ƶ�ڵ��澯,Param:VVV_VIDEO_SHELTER_S
#define    VVV_CMD_GET_EXCEPTION_ALARM 0x1000001B  //�쳣�澯,Param:VVV_EXCEPTION_ALARM_S

#define    VVV_CMD_GET_PTZ_CRUISE      0x1000001C  //��̨Ѳ������,Param:VVV_PTZ_CRUISE_S
#define    VVV_CMD_GET_COMINFO         0x1000001D  //COM����̨,Param:VVV_SERVER_COMINFO_S
#define    VVV_CMD_GET_USERINFO        0x1000001E  //�û������Ȩ��,Param:VVV_SERVER_USER_S
#define    VVV_CMD_GET_IECLIENT_USER   0x1000001F  //IE�Ϳͻ����û�,Param:VVV_IE_CLIENT_USER_S
#define    VVV_CMD_GET_IP_DEVLIST      0x10000020  //NVR������IP�豸�����б�,Param:VVV_IPDEV_LIST_S

#define    VVV_CMD_GET_NETWORK         0x10000021   //��������������,Param:VVV_SERVER_NETWORK_S
#define    VVV_CMD_GET_DDNSCFG         0x10000022   //����DDNS����,Param:VVV_DDNSCFG_S
#define    VVV_CMD_GET_UPNP            0x10000023   //UPNP����,Param:VVV_UPNP_CONFIG_S
#define    VVV_CMD_GET_FTP             0x10000024   //���÷�����FTP�ϴ�����ץ��ͼƬ,Param:VVV_FTP_PARAM_S
#define    VVV_CMD_GET_EMAIL           0x10000025   //Email����,Param:VVV_EMAIL_PARAM_S
#define    VVV_CMD_GET_NTP             0x10000026   //NTP����,Param:VVV_NTP_PARAM_S
#define    VVV_CMD_GET_CENTER_INFO     0x10000027   //�ϱ����ķ�����,Param:VVV_CENTER_INFO_S
#define    VVV_CMD_GET_WIFI            0x10000028   //WIFI����,Param:VVV_WIFI_CONFIG_S
#define    VVV_CMD_GET_3G              0x10000029   //3G����,Param:VVV_3G_CONFIG
#define    VVV_CMD_GET_MOBILE          0x1000002A   //�ֻ��㲥����,Param:VVV_MOBILE_CONFIG_S
#define	   VVV_CMD_GET_P2P             0x10000132   //P2P���ԣ�Param:VVV_P2P_CONFIG_S
#define    VVV_CMD_GET_MICROWAVE_ALARM 0x10000133   //΢���澯,Param:VVV_MICROWAVE_ALARM_S
#define    VVV_CMD_GET_BUZZER          0x10000135   // ��/�ط�����,Param:VVV_BUZZER_CONFIG
#define    VVV_CMD_GET_RFID_INFO       0x10000136   //ע�������RFID,Param:VVV_SERVER_RFID_S
#define    VVV_CMD_GET_HUMITURE        0x10000137   //��ʪ��,Param:VVV_SERVER_HUMITURE_S
#define    VVV_CMD_GET_AUDIOIN_GAIN    0x10000138   //��ȡIPC����������С(0~100)0:����,100:���
#define    VVV_CMD_GET_AUDIOOUT_VOL    0x10000139   //��ȡIPC���������С(0~100)0:����,100:���

#define    VVV_CMD_GET_VIDEO_MP4INFO   0x1000013A   //��ȡ ĳ·��Ƶ�� sps/pps ��sdp��Ϣ
#define    VVV_CMD_GET_PIR_ALARM       0x1000013B   //PIR�澯,Param:VVV_PIR_ALARM_S
#define    VVV_CMD_GET_HUMI_ALARM      0x1000013c   //HUMI�澯,Param:QV_HUMI_ALARM_S
#define    VVV_CMD_GET_MD_ALARM_EX     0x1000013D  //ͨ���ƶ����澯,Param:VVV_CHN_MD_ALARM_EX_S
#define    VVV_CMD_GET_NETWORK_PORT    0x1000013E   //����������˿�����,Param:VVV_SERVER_NETWORK_PORT_S
#define    VVV_CMD_GET_PTZ_ATTR        0x1000013F   //PTZ����,Param:VVV_PTZ_ATTR_S, *PVVV_VVV_PTZ_ATTR_S;
#define    VVV_CMD_GET_SENSOR_IN_ALARM_EX 0x10000140  //̽ͷ�澯,Param:VVV_SENSOR_IN_ALARM_EX_S
#define    VVV_CMD_GET_REC_CONFIG_EX   0x10000141  //¼������,Param:VVV_RECORD_CONFIG_EX_S
#define    VVV_CMD_GET_ALARM_SNAPSHOT  0x10000142  //������ץͼ����,Param:VVV_ALARM_SNAP_S


/************************************************************************/
/*   �����㲥���ݽṹ                                                   */
/************************************************************************/

/*����Э��*/
typedef enum tagNET_PROTOCOL /*����Э������*/
{
    NET_PROTOCOL_QV   = 0,/*QV����Э��:ý�崫��֧��RTSPoverHTTP/RTSP/HTTP,����Э��֧��˽��Э���Onvif*/
    NET_PROTOCOL_ONVIF= 1,/*ONVIFЭ��:ý�崫��֧��RTSPtoHTTP/RTSP/HTTP,����Э��֧��CTL_PROTOCOL_TYPE_ONVIF*/
	NET_PROTOCOL_TUTK = 2,	/*	TUTK P2P Э��*/
    NET_PROTOCOL_BUTT
}NET_PROTOCOL_E;
typedef enum tagSTREAM_TRANS_PROTOCOL /*ý�崫��Э������*/
{
    TRANS_PROTOCOL_RTSP_OVER_HTTP= 0, /*RTSP Over Http����Э��*/
    TRANS_PROTOCOL_RTSP          = 1, /*RTSPЭ��*/
    TRANS_PROTOCOL_HTTP          = 2, /*HTTPЭ��*/
    TRANS_PROTOCOL_PRIVATE       = 3, /*˽��Э��*/
    TRANS_PROTOCOL_BUTT
}STREAM_TRANS_PROTOCOL_E;
typedef enum tagNET_SOCKET_TYPE/*�������ĵײ㴫������*/
{
    SOCKET_TYPE_TCP = 0, /*��tcp�ϴ���*/
    SOCKET_TYPE_UDP = 1, /*��udp�ϴ���*/
    SOCKET_TYPE_BUTT
}NET_SOCKET_TYPE_E;
typedef enum tagCONTROL_PROTOCOL_TYPE/*����Э������*/
{
    CTL_PROTOCOL_TYPE_PRIVATE = 0, /*˽�п���Э��,��ʱֻ֧��˽��Э��*/
    CTL_PROTOCOL_TYPE_ONVIF   = 2, /*ONVIF����Э��*/
    CTL_PROTOCOL_TYPE_BUTT
}CONTROL_PROTOCOL_TYPE_E;
typedef struct tagVVV_NET_PROTOCOL/*����Э��*/
{
    NET_PROTOCOL_E          eNetProtocol; /*����Э������*/
    STREAM_TRANS_PROTOCOL_E eStreamTransProtocol;/*��ý�崫��Э��*/
    NET_SOCKET_TYPE_E       eSocketType;  /*�������ĵײ㴫������*/
    CONTROL_PROTOCOL_TYPE_E eControlProtocol;/*����Э������*/
}VVV_NET_PROTOCOL_S;

/*���������� : ��Ƶ��Ƶ�������澯����*/
typedef enum tagSTREAM_TYPE_E
{
    VVV_STREAM_TYPE_VIDEO     = 0x01,/*��Ƶ*/
    VVV_STREAM_TYPE_AUDIO     = 0x02,/*��Ƶ*/
    VVV_STREAM_VIDEO_AUDIO    = 0x03,/*����Ƶ*/
    VVV_STREAM_MD_ONLY        = 0x04,/*�ƶ����澯����*/
    VVV_STREAM_SENSORIN_ONLY  = 0x08,/*̽ͷ����澯����*/
    VVV_STREAM_SHELTER_ONLY   = 0x10,/*��Ƶ�ڵ��澯����*/
    VVV_STREAM_VIDEO_LOSS_ONLY= 0x20,/*��Ƶ��ʧ�澯����*/
    VVV_STREAM_DISK_ERRO_ONLY = 0x40,/*���̴���澯����*/
    VVV_STREAM_SYS_ERRO_ONLY  = 0x80,/*ϵͳ����澯����*/
   	VVV_STREAM_SNAP_PATH_ONLY = 0x100,/*ץ��·������*/
   	VVV_STREAM_MW_ONLY        = 0x200,/*΢���澯����*/
   	VVV_STREAM_PIR_ONLY       = 0x400,/*PIR �澯����*/   	
    VVV_STREAM_ALL            = 0xFFF,/*���������Լ��澯����*/
    VVV_STREAM_TYPE_BUTT
}VVV_STREAM_TYPE_E;

typedef enum tagOUTSIDE_ALARM_TYPE_E
{
    OUTSIDE_ALARM_TYPE_HUMI   = 0x01,   /*��ʪ�ȣ�PM2.5*/
    OUTSIDE_ALARM_TYPE_2,               /*δ����*/
    OUTSIDE_ALARM_TYPE_3,          
    OUTSIDE_ALARM_TYPE_4,
    OUTSIDE_ALARM_TYPE_BUTT
}OUTSIDE_ALARM_TYPE_E;

typedef struct tagVVV_OUTSIDE_ALARM/*�ⲿ�澯��Ϣ*/
{
    unsigned int            dwsize;
    OUTSIDE_ALARM_TYPE_E    eOutsideAlarm; /*�ⲿ�澯����*/
    unsigned int            AlarmInfoLen;    
	char                    aszAlarmInfo[256];    
	unsigned char           reserve[12];
}VVV_OUTSIDE_ALARM_S;


/* ��������״̬*/
typedef enum tagLIVE_NET_STAT
{
    NETSTAT_TYPE_CONNING = 0,    /*��������*/
    NETSTAT_TYPE_CONNING_FAILED, /*���Ӳ��ϻ��û��������*/
    NETSTAT_TYPE_CONNED,         /*������*/
    NETSTAT_TYPE_ABORTIBE_DISCONNED, /*�������ϵ������쳣�Ͽ�*/ 
    NETSTAT_TYPE_NORMAL_DISCONNED,   /*�������û����������Ͽ�*/
    NETSTAT_TYPE_BUTT
}LIVE_NET_STAT_E;


/*�������Զ���*/
typedef enum tagVVV_VIDEO_FORMAT_E/*��Ƶ�����ʽ*/
{
    VVV_VIDEO_FORMAT_H261  = 0,  /*H261  */
    VVV_VIDEO_FORMAT_H263  = 1,  /*H263  */
    VVV_VIDEO_FORMAT_MPEG2 = 2,  /*MPEG2 */
    VVV_VIDEO_FORMAT_MPEG4 = 3,  /*MPEG4 */
    VVV_VIDEO_FORMAT_H264  = 4,  /*H264  */
    VVV_VIDEO_FORMAT_MJPEG = 5,  /*MOTION_JPEG*/
    VVV_VIDEO_FORMAT_BUTT
}VVV_VIDEO_FORMAT_E;
typedef enum tagVVV_AUDIO_FORMAT_E/*��Ƶ�����ʽ*/
{
    VVV_AUDIO_FORMAT_G711A   = 1,   /* G.711 A            */
    VVV_AUDIO_FORMAT_G711Mu  = 2,   /* G.711 Mu           */
    VVV_AUDIO_FORMAT_ADPCM   = 3,   /* ADPCM              */
    VVV_AUDIO_FORMAT_G726    = 4,   /* G.726              */
    VVV_AUDIO_FORMAT_AMR     = 5,   /* AMR encoder format */
    VVV_AUDIO_FORMAT_AMRDTX  = 6,   /* AMR encoder formant and VAD1 enable */
    VVV_AUDIO_FORMAT_AAC     = 7,   /* AAC encoder        */
    VVV_AUDIO_FORMAT_BUTT
}VVV_AUDIO_FORMAT_E;
typedef enum tagVVV_AUDIO_BITWIDTH_E/*��Ƶ�����ʽ*/
{
    VVV_AUDIO_BITWIDTH_8   = 0,   /* Bit width is 8 bits   */
    VVV_AUDIO_BITWIDTH_16  = 1,   /* Bit width is 16 bits  */
    VVV_AUDIO_BITWIDTH_32  = 2,   /* Bit width is 32 bits */ 
    VVV_AUDIO_BITWIDTH_BUTT
} VVV_AUDIO_BITWIDTH_E;
typedef enum tagVVV_AUDIO_SAMPLE_RATE_E/*��Ƶ������*/
{
    VVV_AUDIO_SAMPLE_RATE_8     = 0,   /* 8K Sample rate     */
    VVV_AUDIO_SAMPLE_RATE_11025 = 1,   /* 11.025K Sample rate*/
    VVV_AUDIO_SAMPLE_RATE_16    = 2,   /* 16K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_22050 = 3,   /* 22.050K Sample rate*/
    VVV_AUDIO_SAMPLE_RATE_24    = 4,   /* 24K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_32    = 5,   /* 32K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_441   = 6,   /* 44.1K Sample rate  */
    VVV_AUDIO_SAMPLE_RATE_48    = 7,   /* 48K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_64    = 8,   /* 64K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_882   = 9,   /* 88.2K Sample rate  */
    VVV_AUDIO_SAMPLE_RATE_96    = 10,  /* 96K Sample rate    */
    VVV_AUDIO_SAMPLE_RATE_1764  = 11,  /* 176.4K Sample rate */
    VVV_AUDIO_SAMPLE_RATE_192   = 12,  /* 192K Sample rate   */
    VVV_AUDIO_SAMPLE_RATE_BUTT
} VVV_AUDIO_SAMPLE_RATE_E;
typedef enum tagVVV_SOUND_MODE_E/*������˫����*/
{
    VVV_SOUND_MODE_MOMO =0,          /*������*/
    VVV_SOUND_MODE_STEREO =1,        /*˫����*/
    VVV_SOUND_MODE_BUTT    
}VVV_SOUND_MODE_E;
typedef struct tagSTREAM_AUIDO_ATTR_S
{
    VVV_AUDIO_FORMAT_E    enAudioFormat;  /*��Ƶ��ʽ*/         
    VVV_AUDIO_SAMPLE_RATE_E  enSampleRate;/*������ */        
    VVV_AUDIO_BITWIDTH_E enBitWidth;      /*����λ�� */        
    VVV_SOUND_MODE_E     enChannMode;     /*����ģʽ����������˫����*/
}STREAM_AUIDO_ATTR_S;
typedef struct tagSTREAM_VIDEO_ATTR_S
{
    VVV_VIDEO_FORMAT_E enVedioFormat;     /*��Ƶ�����ʽ*/
    unsigned int     u32PicWidth;        /*��Ƶͼ��Ŀ�*/
    unsigned int     u32PicHeight;       /*��Ƶͼ��ĸ�*/
} STREAM_VIDEO_ATTR_S;
typedef struct tagVVV_STREAM_INFO /*��������*/
{
    STREAM_VIDEO_ATTR_S struVencChAttr;  /* ��Ƶ�������� */
    STREAM_AUIDO_ATTR_S struAencChAttr;  /* ��Ƶ�������� */
}VVV_STREAM_INFO_S;


/* �¼��ص�*/
typedef int (*VVV_ON_EVENT_CALLBACK)(unsigned int u32ChnHandle, /* ͨ����� */
                                    unsigned int u32Event,     /* �¼������LIVE_NET_STAT_E*/
                                    void* pUserData,           /* �û�����*/
                                    VVV_STREAM_INFO_S* pStreamInfo);/*��������*/

/* �����ص�*/
typedef int (*VVV_ON_STREAM_CALLBACK)(unsigned int u32ChnHandle,/* ͨ����� */
                                     unsigned int u32DataType,/* �������ͣ����VVV_STREAM_TYPE_E
                                                                 0x01-��Ƶ��0x02-��Ƶ*/
                                     unsigned char* pu8Buffer,/* ���ݰ���֡ͷ */
                                     unsigned int u32Length,  /* ���ݳ��� */
                                     VVV_U64  u64TimeStamp,    /* ʱ���*/
                                     VVV_STREAM_INFO_S *pStreamInfo,/*��������*/
                                     void* pUserData);        /* �û�����*/

/* �澯�ص�*/
typedef int (*VVV_ON_DATA_CALLBACK)(unsigned int u32ChnHandle,/* ͨ����� */
                                   unsigned int u32DataType, /* ��������,���VVV_STREAM_TYPE_E
                                                                0x04-MD�澯����,
                                                                0x08-̽ͷ����澯����,
                                                                0x10-��Ƶ�ڵ��澯����,
                                                                0x20-��Ƶ��ʧ�澯����,
                                                                0x40-���̴���澯,
                                                                0x80-ϵͳ����澯*/
                                   unsigned char*  pu8Buffer,/* ����,MD�澯����Ϊ
                                   "Alarm:�������������-���Ͻ�X����-���Ͻ�Y����-��-��|�������������-���Ͻ�X����-���Ͻ�Y����-��-��|",
                                   ����"Alarm:1-558-461-120-120|2-520-0-120-120|3-0-360-120-120|4-520-360-120-120|"*/
                                   unsigned int u32Length,          /* ���ݳ��� */
                                   VVV_U64       u64TimeStamp,       /* ʱ���*/
                                   void* pUserData);                /* �û�����*/

/* �Խ���Ƶ�ص�*/
typedef int (*VVV_ON_TALK_CALLBACK)(unsigned int u32DevHandle,/* �豸��� */
                                   unsigned int u32DataType,/* �������ͣ����STREAM_TYPE_E
                                                               0x02-��Ƶ*/
                                   unsigned char* pu8Buffer,/* ���ݰ���֡ͷ */
                                   unsigned int u32Length,  /* ���ݳ��� */
                                   VVV_U64  u64TimeStamp,    /* ʱ���*/
                                   VVV_STREAM_INFO_S *pStreamInfo,/*��������*/
                                   void* pUserData);        /* �û�����*/

/************************************************************************/
/*   ץ��¼��ط����ݽṹ                                               */
/************************************************************************/
/*ץ����������VVV_CMD_SNAP*/
typedef struct tagVVV_SNAP_REQ
{
    unsigned int snapformat;     /*ץ�ĸ�ʽ,0-jpeg,1-bmp,Ĭ��jpeg*/
    unsigned int count;          /*����ץ������,Ĭ��1*/
    unsigned int timeinterval;   /*ץ��ʱ����,Ĭ��500���룬��λ����*/
}VVV_SNAP_REQ_S;

/*ץ����������VVV_CMD_SNAP_CHN*/
typedef struct tagVVV_SNAP_CHN_REQ
{
	unsigned int chn;
    unsigned int snapformat;     /*ץ�ĸ�ʽ,0-jpeg,1-bmp,Ĭ��jpeg*/
    unsigned int count;          /*����ץ������,Ĭ��1*/
    unsigned int timeinterval;   /*ץ��ʱ����,Ĭ��500���룬��λ����*/
	unsigned char reserve[32];
}VVV_SNAP_CHN_REQ_S;

typedef struct tagVVV_TIME
{
	unsigned char year;			
	unsigned char month;		
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}VVV_TIME_S;

/*¼���ļ���ѯ����VVV_CMD_REC_SEARCH_START*/
typedef struct tagVVV_FindFileReq/*¼���ļ���ѯ����VVV_CMD_REC_SEARCH_START*/
{
    int chn;               /*ͨ����0~VVV_MAX_CHN_NUM*/
    VVV_TIME_S BeginTime;   /*BeginTime EndTimeֵ��Ϊ0000-00-00 00:00:00*/
    VVV_TIME_S EndTime;     /*��ʾ��������ʱ��*/
    unsigned int nFileType;/*�ļ�����:1-����¼��,2-����¼��,3-����¼��*/
}VVV_FIND_FILE_REQ_S;

/*��������¼���ļ�����VVV_CMD_REC_SEARCH_START*/
typedef struct
{
    VVV_U64 file_size;       
    int rec_type;        //¼�����ͣ� 1������¼�� 2������¼��3 ������¼�� 
    unsigned int chn;    /*ͨ����, 
                           4��ASCII�룬��ʾ2��16������ʽ���ֽڡ�
                           λֵΪ1��ʾ������Ӧ��ͨ���š�
                           bit0~bit15�ֱ����1~16ͨ��*/
    char file[VVV_PATHNAME_LEN];//¼���ļ���,Ҫ����豸�˵ľ���·��
    unsigned int   start_hour;
    unsigned int   start_min;
    unsigned int   start_sec;/*¼���ļ���ʼʱ��*/
    unsigned int   end_hour;
    unsigned int   end_min;
    unsigned int   end_sec;  /*¼���ļ�����ʱ��*/
	unsigned char reserve[2];
}search_file_setting_s;
typedef struct tag_file_list_s
{
	int num;				// �ļ�����
	search_file_setting_s	*pFileInfo;
}VVV_FILE_LIST_VMS_S;

/*�ͷ�¼���ļ�����VVV_CMD_REC_SEARCH_FREE*/
typedef struct tagfile_info_s
{
	char	*pszFileName;
	int		iLenTime;			// Time length, by minute, use on MDVR only 
} file_info_s;


typedef struct snap_file_list_s
{
	int num;	//�ļ�����
	char   aszFilesName[6][VVV_PATHNAME_LEN]; 
}VVV_SNAP_FILE_LIST_S;

/*����¼���ļ�����VVV_CMD_REC_DOWNLOAD_START*/
typedef struct tagVVV_DownloadFileReq
{
    char         pszSouFileName[128];/*Ҫ���ص�¼���ļ���*/
    unsigned int lOffSetFlag; /*ƫ�Ʊ�־ 0-����¼�� 1-��Сƫ�� 2-ʱ��ƫ��*/
    unsigned int lBeginOffSet;
    unsigned int lEndOffSet;
    unsigned int lOffSet;/*ƫ����,���������¼��,������ڿ�ʼ,����Ǽ���,����ڼ����Ŀ�ʼ,
                          ע������Ǽ������豸Ĭ�Ϸ��ļ�ͷ�������Ҫ�ļ�ͷ��loffSet=64*1204,
                          �ü�ʱȷ�� if(lBeginOffSet!=0) lBeginOffSet >= 64*1024*/
}VVV_DownloadFileReq_S;
//�豸�����������ļ���Ӧ�𲹳�ṹ
typedef struct tagfile_return_vms
{
	VVV_U64	filesize;				/*�ļ���ʵ��С*/
	int sess_id;                /*�ỰID*/
}VVV_file_return_vms;


/*ץ�Ļص�VVV_CMD_SNAP*/
typedef int(*VVV_ON_SNAP_CALLBACK)(unsigned int u32ChnHandle,/* ͨ����� */
                                  char *SnapData,         /*ץ������ָ��*/
                                  unsigned int DataSize,  /*ץ�����ݳ���*/
                                  int width,
                                  int height,/*ץ�ķ��ص�ͼ����*/
                                  int count, /*ץ�ĵĵڼ���*/
                                  void* pUserData);

/* Զ��¼�����ػص�*/
typedef struct tagCallBackData_S

{ 
	int lBufferLen;	//lBufferLen=CALLBACK_OTHER or lBufferLen>0
	VVV_U64 lTotalLen;		//if(lBufferLen==CALLBACK_OTHER) {lTotalLen=CALLBACK_LINKBUILD or CALLBACK_LINKBREAK}
	void* pVoid;
}CallBackData_S;
typedef int (*VVV_ON_FILE_CALLBACK)(void* pUserData, const CallBackData_S* pstuDataInfo);

/*��ʱ��طŻص�*/
typedef int (*VVV_ON_pfnPlayBackCallback)(unsigned int u32PlayBackHandle,int dwDataType,
                                           char *pBuffer,unsigned int dwBufSize,void* pUserData);


/*��־��ѯ����VVV_CMD_LOG_DOWNLOAD*/
typedef enum _taglogtype_/*��־����*/
{
    VVV_LT_IO = 0,      /*I/O����,����д��־*/
    VVV_LT_MD = 1,      /*�ƶ����澯,����д��־*/
    VVV_LT_HDD = 2,     /*Ӳ�̴���,����д��־*/
    VVV_LT_VIDEOLOSS = 3,/*��Ƶ��ʧ�澯,����д��־*/
    VVV_LT_OPERATION = 4,/*��ѯ��д�빲��*/
    VVV_LT_ALARM,       /*��ѯʹ�ã�����io��md����Ƶ��ʧ��Ӳ��*/
    VVV_LT_ALL,         /*��ѯʹ�ã���ѯȫ��*/
    VVV_LT_INVALID
}VVV_logtype_e;
typedef enum _taglogoperation_e
{
    VVV_FIRSTPAGE_SEARCH,   /*��һ��������ص���һҳ*/
    VVV_NEXTPAGE_SEARCH,    /*��һҳ*/
    VVV_PREPAGE_SEARCH,     /*��һҳ*/
    VVV_LASTPAGE_SEARCH     /*���һҳ*/
}VVV_logoperation_e;
typedef struct tagVVV_SEARCH_LOG_CONDITION
{
    unsigned int    lpnDataCount; /*��־��ѯ����*/
    VVV_TIME_S       start;
    VVV_TIME_S       end;
    VVV_logtype_e       type;
    VVV_logoperation_e  operation;   /*�û�����:��һ����ʾ;��һҳ;��һҳ;���һҳ*/
    int             searchmax;    /*ϣ�����ص�������*/
    int             ChannelBitMap;
}VVV_SEARCH_LOG_CONDITION_S;

/*��־��ѯ���*/
typedef struct
{
	VVV_TIME_S	  date; 		//ʱ��
	unsigned int  dwMajorType;	//������ 1-����; 2-�쳣; 3-����; 0xff-ȫ��
	unsigned int  dwMinorType;	//������ 0-ȫ��;
	char		  RemoteHostAddr[VVV_NAME_LEN];//Զ��������ַ
	char		  sOpUser[VVV_NAME_LEN];  //�����˵��û���
	unsigned int bChannel;	   //ͨ����
	unsigned int bDiskNumber;  //Ӳ�̺�
	unsigned int bAlarmInPort; //��������˿�
	unsigned int bAlarmOutPort;//��������˿�
	unsigned int bRecorded;    //�Ƿ�����¼�����
	char		 csBuf[VVV_NAME_LEN];//�û�����ʱ��¼���������û�
}log_info_s;
typedef struct tagVVV_LOG_DATA   //size = 44
{
	int num;   //��־�ļ�����
	log_info_s *pLogInfo;
}VVV_LOG_DATA;

/*��־�������VVV_CMD_LOG_CLEAR*/
typedef struct tagVVV_LOG_CLEAR
{
    unsigned int bSysLog;   //ϵͳ��־
    unsigned int bAccessLog;//������־
}VVV_LOG_CLEAR_S;

//�����ṹ��VVV_CMD_UPGRADE
typedef struct tagVVV_UPGRADE_ATTR
{
    unsigned int  dwSize;
    char          file[VVV_PATHNAME_LEN];//�����ļ�·��
    unsigned int  UpgradeAddr; /*����λ�ã�����ڲ����������׵�ַƫ��12���ֽ�*/
}VVV_UPGRADE_ATTR_S;

//ɨ��wifi���VVV_NET_ScanWifi
typedef struct tagwifi_info
{
    char            szEssid[VVV_NAME_LEN];
    unsigned int    nSecurity;   //0: none,
                                 //1:wep
                                 //2:wpa/wpa2
    unsigned int  byMode;  //1. managed 2. ad-hoc
    int           quality; //�ź�����0~100
    char          szWebKey[VVV_NAME_LEN];
}wifi_info;
typedef struct tagVVV_WIFI_INFO_S
{
    unsigned int  num;      //ɨ�����wifi����
	wifi_info    *pWifiInfo;//wifi��Ϣ
}VVV_WIFI_INFO_S;


//��̨���ƽṹ��,���"��ָ̨��"
typedef struct tagVVV_PTZ_CMD
{
    unsigned int  dwSize;
    unsigned int  chn;     /*ͨ����*/
    unsigned int  dwValue; /*��̨����ֵ*/
    int  speed;            /*��̨�ٶ�,-1������һ�ε��ٶ�*/
}VVV_PTZ_CMD_S;

/************************************************************************/
/* ������������Ϣ�ṹ                                                   */
/************************************************************************/
//��������ϢVVV_CMD_GET_SERVER_INFO
typedef struct tagVVV_SERVER_INFO
{
    unsigned int   dwSize;
    char           dwServerFlag[VVV_NAME_LEN];    //����������
    char           dwServerSubFlag[VVV_NAME_LEN]; //������������
    unsigned int   ServerCPUType;   //��ǰCPU����:0X3518,0X3520A,0X3531
    char           serialNum[VVV_NAME_LEN];  //�豸���к�
    char           DevID[VVV_NAME_LEN];      //�豸ID,ÿ̨�豸Ψһ,AutoIDҲʹ�ø�ID
    char           model[VVV_NAME_LEN];      //�ͺ�
    char           name[VVV_NAME_LEN];       //����������
    char           hardVersion[VVV_NAME_LEN];//Ӳ���汾
    char           softVersion[VVV_NAME_LEN];//����汾
    char           startdate[VVV_NAME_LEN];  //ϵͳ��������ʱ��,����"2006.09.06.09.11.30"
    unsigned int   runtimes;                //ϵͳ������ʱ��,��λ:����
    char           szServerIp[VVV_NAME_LEN];//������IP(�ַ�����ʾ��ʽ)
    char           szMACAddr[VVV_NAME_LEN]; //�������������ַ
    unsigned int   ServerPort;      //�������˿�
    unsigned int   ChannelNum;      //ͨ������
}VVV_SERVER_INFO_S,*PVVV_SERVER_INFO_S;

//������������VVV_CMD_GET_SERVER_CAPS
typedef struct tagVVV_SERVER_CAP
{
    unsigned int dwSize;
    unsigned int EncChnMaxNum;   //��������
    unsigned int DecChnMaxNum;   //��������
    unsigned int Stream1Height;  //��Ƶ��(1)
    unsigned int Stream1Width;   //��Ƶ��
    unsigned int Stream1CodecID; //��Ƶ�������ͺţ�H264Ϊ0x01,MJPEGΪ0x02,MPEG4Ϊ0x04��
    unsigned int Stream2Height;  //��Ƶ��(2)
    unsigned int Stream2Width;   //��Ƶ��
    unsigned int Stream2CodecID; //��Ƶ�������ͺţ�H264Ϊ0x01,MJPEGΪ0x02,MPEG4Ϊ0x04��
    unsigned int Stream3Height;  //��Ƶ��(2)
    unsigned int Stream3Width;   //��Ƶ��
    unsigned int Stream3CodecID; //��Ƶ�������ͺţ�H264Ϊ0x01,MJPEGΪ0x02,MPEG4Ϊ0x04��
    unsigned int AudioMaxNum;    //�����Ƶ��
    unsigned int DisplayDev;     //��ʾ���:0x01-CVBS,0x02-VGA,0x04-HDMI
    unsigned int DISKMaxNum;     //���Ӳ����
    unsigned int SDMaxNum;       //���SD����
    unsigned int AlarmInNum;     //�澯����·��
    unsigned int AlarmOutNum;    //�澯���·��
    unsigned int UserRight;      //��ǰ�û�Ȩ��:0X01-������̨,0x02-¼��/ץ��,0x04-�ط�/����,0x08-���ò���,0x10-��/������
    unsigned int NetPriviewRight;//����ۿ�Ȩ��:ͨ������bitλ����
    unsigned int NetProtocol;//֧�ֵ�����Э��(0x01֧��RTSPtoHTTP,0x02֧��RTSP,0x04֧��HTTP,0x08֧��ONVIF)
    unsigned int RemoteUserMaxNum;//���Զ���û���
}VVV_SERVER_CAP_S,*PVVV_SERVER_CAP_S;

//�������ϴ������ṹ��VVV_CMD_RESTORE_SETTING,VVV_CMD_BACKUP_SETTING
typedef struct tagVVV_SETTING
{
    unsigned int dwSize;
    char         file[VVV_PATHNAME_LEN];//�����ļ�·��
}VVV_SETTING_S;

/*ʱ��VVV_CMD_SET_TIME,VVV_CMD_GET_TIME*/
typedef struct VVV_Server_Time
{
    unsigned int  dwSize;
    unsigned int  year;   //ʱ����
    unsigned int  month;  //ʱ����
    unsigned int  day;    //ʱ����
    unsigned int  hour;   //ʱ��ʱ
    unsigned int  minute; //ʱ���
    unsigned int  second; //ʱ����
    unsigned char DateMode;	/*���ڸ�ʽ��0ΪMM/DD/YY��1ΪYY-MM-DD*/
    unsigned char TimeMode;	/*ʱ���ʽ��0Ϊ24Сʱ�ƣ�1Ϊ12Сʱ��*/
    unsigned char bdstmode; //�Ƿ�����ʱ��, 0--�ر�,  1--����
    char          timezone; //ʱ��-12~12,����citytimezone�����ֲ�ͬ��ʱ����﷽ʽ
    char          citytimezone[VVV_NAME_LEN];//����ʱ��
                        /*
                        "Etc/GMT-12",
                        "Pacific/Apia",
                        "Pacific/Honolulu",
                        "America/Anchorage",
                        "America/Los_Angeles",
                        "America/Denver",
                        "America/Tegucigalpa",
                        "America/Phoenix",
                        "America/Winnipeg",
                        "America/Mexico_City",
                        "America/Chicago",
                        "America/Costa_Rica",
                        "America/Indianapolis",
                        "America/New_York",
                        "America/Bogota",
                        "America/Santiago",
                        "America/Caracas",
                        "America/Montreal",
                        "America/St_Johns",
                        "America/Thule",
                        "America/Buenos_Aires",
                        "America/Sao_Paulo",
                        "Atlantic/South_Georgia",
                        "Atlantic/Cape_Verde",
                        "Atlantic/Azores",
                        "Europe/Dublin",
                        "Africa/Casablanca",
                        "Europe/Amsterdam",
                        "Europe/Belgrade",
                        "Europe/Brussels",
                        "Europe/Warsaw",
                        "Africa/Lagos",
                        "Europe/Athens",
                        "Europe/Bucharest",
                        "Africa/Cairo",
                        "Africa/Harare",
                        "Europe/Helsinki",
                        "Asia/Jerusalem",
                        "Asia/Baghdad",
                        "Asia/Kuwait",
                        "Europe/Moscow",
                        "Africa/Nairobi",
                        "Asia/Tehran",
                        "Asia/Dubai",
                        "Asia/Baku",
                        "Asia/Kabul",
                        "Asia/Yekaterinburg",
                        "Asia/Karachi",
                        "Asia/Calcutta",
                        "Asia/Katmandu",
                        "Asia/Almaty",
                        "Asia/Dhaka",
                        "Asia/Colombo",
                        "Asia/Rangoon",
                        "Asia/Bangkok",
                        "Asia/Krasnoyarsk",
                        "Asia/Hong_Kong",
                        "Asia/Irkutsk",
                        "Asia/Kuala_Lumpur",
                        "Australia/Perth",
                        "Asia/Taipei",
                        "Asia/Tokyo",
                        "Asia/Seoul",
                        "Asia/Yakutsk",
                        "Australia/Adelaide",
                        "Australia/Brisbane",
                        "Australia/Sydney",
                        "Pacific/Guam",
                        "Australia/Hobart",
                        "Asia/Vladivostok",
                        "Asia/Magadan",
                        "Pacific/Auckland",
                        "Pacific/Fiji",
                        "Pacific/Tongatapu"*/
    unsigned char Dst;/*����ʱ��0-�رգ�1-��*/
    unsigned char DstMode;/*����ʱģʽ��0-Ĭ�ϣ�1-�Զ���*/
    unsigned char StartWeek;/*��ʼ�ܣ�0-��һ�ܣ�1-�ڶ��ܣ�2-�����ܣ�3-������*/
    unsigned char StartMonth;/*��ʼ�£���12����:0---11*/
    unsigned char EndWeek;/*�����ܣ�0-��һ�ܣ�1-�ڶ��ܣ�2-�����ܣ�3-������*/
    unsigned char EndMonth;/*�����£���12����:0---11*/
    unsigned char StartHour;
    unsigned char EndHour;
    unsigned char Offset;
    unsigned char MenuAutoExitTime; //�˵��Զ��˳�ʱ��,0:�ر� 1:1���� 2:2���� 3:3���� 4:4���� 5:5����
    unsigned char LockGUITime;      //�˵���ʱ���� 0:�ر� 1:1���� 2:2���� 3:3���� 4:4���� 5:5����
    unsigned char bSaveFlash;
}VVV_Server_Time_S;

//����������VVV_CMD_SET_LANGUAGE,VVV_CMD_GET_LANGUAGE
#define VVV_LUNGUAGE_CHINESE        0
#define VVV_LUNGUAGE_ENGLISH        1
#define VVV_LUNGUAGE_RUSSIAN        2   //���
#define VVV_LUNGUAGE_FRENCH         3   //����
#define VVV_LUNGUAGE_ITALIAN        4   //�����
#define VVV_LUNGUAGE_POLAND         5   //����
#define VVV_LUNGUAGE_PORTUGUESE     6   //������
#define VVV_LUNGUAGE_SPAISH         7   //������
#define VVV_LUNGUAGE_THAI           8   //̩��
#define VVV_LUNGUAGE_TURKISH        9   //������
#define VVV_LUNGUAGE_KOREAN         10  //����
#define VVV_LUNGUAGE_JAPANESE       11  //�ձ�
#define VVV_LUNGUAGE_CHINESEBIG5    12  //����
#define VVV_LUNGUAGE_VIETNAMES      13  //Խ��
typedef struct tagVVV_LANGUAGE
{
    unsigned int dwSize;
    unsigned int language;/*ϵͳ���ԣ�0-���ģ�1-Ӣ��*/
    unsigned int langenc;
    int          bSaveFlash;
}VVV_LANGUAGE_S,*PVVV_LANGUAGE_S;

//�豸ά��VVV_CMD_SET_MAINTAIN��VVV_CMD_GET_MAINTAIN
typedef struct tagVVV_DEVICE_MAINTAIN
{
    unsigned int dwSize;
    unsigned int time[4];/*ά���ľ���ʱ�䣬ʱ��*/
    unsigned int automaintain;/*�Զ�ά�����أ�0-�رգ�1-����*/
    unsigned int maintainperiod1;/*ά������һ��0-ÿ�죬1-ÿ�ܣ�2-ÿ��*/
    unsigned int maintainperiod2;/*ά�����ڶ���0--6ÿ�ܵ��ܼ���7--37ÿ�µļ���*/
    int          bSaveFlash;
}VVV_DEVICE_MAINTAIN_S,*PVVV_DEVICE_MAINTAIN_S;

/*--------------------------ͨ�����ýṹ--------------------------------*/
//ͨ����ɫ����VVV_CMD_SET_CHN_COLOR,VVV_CMD_GET_CHN_COLOR
typedef struct tagVVV_CHN_COLOR
{
    unsigned int dwSize;
    unsigned int chn;        //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int Hue;        //ɫ��0~100
    unsigned int Color;      //ɫ��0~100
    unsigned int Contrast;   //�Աȶ�0~100
    unsigned int Brightness; //����0~100
    unsigned int Definition; //������0~100
    int          bSaveFlash;
}VVV_CHN_COLOR_S,*PVVV_CHN_COLOR_S;

/*ͼ������VVV_CMD_SET_CHN_IMAGE,VVV_CMD_GET_CHN_IMAGE*/
typedef struct tagVVV_CHN_IMAGE
{
    unsigned int dwSize;
    unsigned char chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    char         scene[VVV_NAME_LEN];    //����"outdoor|indoor|auto|manual|lowlight|highlight"
    char         powerfreq[VVV_NAME_LEN];//��ԴƵ��"50", "60"
    char         flip[VVV_NAME_LEN];     //"on", "off"
    char         mirror[VVV_NAME_LEN];   //"on", "off"
    unsigned char ircut;                 //0�ر�,1����
    char         awbswitch[VVV_NAME_LEN];//�Զ���ƽ�⿪��"on","off"
    char         color[VVV_NAME_LEN];    //��ɫ����:��"color", ��"baw"
    unsigned char bSaveFlash;
    unsigned char wdr; //BTaligned,Ϊ�˶�������, 20140116
}VVV_CHN_IMAGE_S,*PVVV_CHN_IMAGE_S;


//ͨ��OSD��ʾ�Լ�ͨ����������VVV_CMD_SET_CHN_OSDINFO,VVV_CMD_GET_CHN_OSDINFO
typedef struct tagVVV_CHN_OSDINFO
{
    unsigned int  dwSize;
    unsigned char chn;        //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char bShowTime;  //�Ƿ���ʾʱ��
    unsigned char TimePosition;/*0-���ϣ�1-���£�2-���ϣ�3-����*/
    unsigned char bShowString; //�Ƿ���ʾ�û������ַ���(ͨ�������ڴ�����)
    unsigned char StringPosition;	  /*0-���ϣ�1-���£�2-���ϣ�3-����*/
    char          csString[48];       //�û������ַ���
    unsigned char bSaveFlash;
    unsigned char rev1; //BTaligned,Ϊ�˶�������, 20140116
    unsigned char rev2; //BTaligned,Ϊ�˶�������, 20140116
}VVV_CHN_OSDINFO_S,*PVVV_CHN_OSDINFO_S;

//LOGO��������VVV_CMD_SET_CHN_LOGO,VVV_CMD_GET_CHN_LOGO
typedef struct tagVVV_CHN_LOGO
{
    unsigned int   dwSize;
    unsigned char  chn;     //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char  bEnable; //�Ƿ�����LOGO��ʾ
    unsigned char  resv1;    //BTaligned,Ϊ�˶�������, 20140116
    unsigned char  resv2;     //BTaligned,Ϊ�˶�������, 20140116
    unsigned int   x;       //LOGO��ʾ��ʼλ��
    unsigned int   y;
    unsigned char  bLogoData[VVV_PATHNAME_LEN];//��LOGO���ݿ��Ե���...��á��������Ϊ40K
    unsigned char  bSaveFlash;
    unsigned char  resv3;     //BTaligned,Ϊ�˶�������, 20140116
    unsigned char  resv4;     //BTaligned,Ϊ�˶�������, 20140116
    unsigned char  resv5;      //BTaligned,Ϊ�˶�������, 20140116   
}VVV_CHN_LOGO_S;

//�ڸ���������VVV_CMD_SET_CHN_OVERLAY,VVV_CMD_GET_CHN_OVERLAY
typedef struct tagrect_t
{
    int left;
    int top;
    int right;
    int bottom;
}rect_t;
typedef struct tagVVV_CHN_OVERLAY
{
    unsigned int  dwSize;
    unsigned char chn;            //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char bEnable[4];     //�Ƿ���������ڵ�
    unsigned char resv1; //BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2; //BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3; //BTaligned,Ϊ�˶�������, 20140116   
    rect_t        rcShelter[4];   //�ڵ��������֧��4��������ڵ���
    unsigned char u32Color[4];    //������ɫ,���ظ�ʽ��RGB8888,Ĭ��Ϊ��ɫ:ֵΪ0 
    unsigned char bSaveFlash;
    unsigned char resv4; //BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv5; //BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv6; //BTaligned,Ϊ�˶�������, 20140116      
}VVV_CHN_OVERLAY_S,*PVVV_CHN_OVERLAY_S;

//�����������ʾVVV_CMD_SET_VO,VVV_CMD_GET_VO
typedef struct _tagvoattr_
{
    struct _tagchnselect_
    {
        unsigned char twentychn[20];	//��ʮ����
        unsigned char sixteenchn[16];	//ʮ������
        unsigned char eightchn[8];		//1+7����
        unsigned char sixchn[6];		//1+5����
        unsigned char fourchn[4];		//�Ļ���
        unsigned char ninechn[9];		//�Ż���
        unsigned char onechn;			//һ����
        unsigned char key;				//��OutputMode���Ӧ�ã�ex: OutputMode:0  key=0 -->ͨ��1ȫ��; OutputMode:1  key=0 -->��һ���ķ���
        unsigned char reserve[63];
    } chnselect_s;/*����Ԥ��ͨ����ֻ��ѡ������һ��Ԥ��ģʽ*/
    unsigned char PollingTime;		/*�����ѯʱ����: 5�롫1��2��5������ѡ����λ��5�룬0��ʾ����ѯ*/
    unsigned char StaticOrDynamic;	/*0-static��1-dynamic(�û��ֶ�����)*/
    unsigned char OutputMode;		/*�����Ԥ��ģʽ:0-һ���棬1-�Ļ��棬2-�Ż��棬3-ʮ������*/
    unsigned char Margin[4];		/*0:��߾�, 1:�ұ߾�, 2:�ϱ߾�, 3:�±߾�, 2����Ϊ��λ*/
    unsigned char reserve1[2];		/*�����ֽ�*/
} voattr_s;
typedef struct tagVVV_VO_SETTING
{
    unsigned int dwSize;
    unsigned char IsVoutOrSpot;/*0-Vout,1-Spot*/
    unsigned char vgaresolution;/*�ֱ���:0-600x480,1-800x600,2-1024x768*/
    unsigned char Bt1120Resolution;/*�ֱ���:0-800x600��1-1024x768, 2-1280x1024, 3-1920x1080*/
    unsigned char cameramode;/*����ͷ��ʽ:0-TS_PAL,1-TS_NTSC*/
    unsigned char vgatype;/*VGA�������:0-VGA, 1-BT1120 */
    voattr_s voattr[3];/*voattr[0]-OD_VOUT, voattr[1]-OD_VSPOT, voattr[2]-OD_VGA*/
    unsigned char bSaveFlash;
}VVV_VO_SETTING_S,*PCMD_SET_VO_S;

//������ͨ�����OSD��ʾVVV_CMD_SET_CHN_VOOSD,VVV_CMD_GET_CHN_VOOSD
typedef struct tagVVV_CHN_VOOSD
{
    unsigned int  dwSize;
    unsigned char Preview[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];/*Ԥ��osd����*/
    unsigned char chnosdPosition[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];/*0-���ϣ�1-���£�2-���ϣ�3-����*/
    char          ChnName[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)][VVV_NAME_LEN];/*ͨ������*/
    unsigned char PreviewTimeSet;/*0-�رգ�1-��*/
    unsigned char audio[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];/*��Ƶ����Ƶͨ��*/
    unsigned char voice;/*0-�رգ�1-��*/
    unsigned char bSaveFlash;
}VVV_CHN_VOOSD_S,*PVVV_CHN_VOOSD_S;

//ͨ����Ƶ��������VVV_CMD_SET_VENC_CONFIG,VVV_CMD_GET_VENC_CONFIG
typedef struct tagVVV_VENC_CONFIG
{
    unsigned int  dwSize;
    unsigned int  chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  Stream;       //0-������,1-������,2-��������
    unsigned int  VideoFormat;  //��Ƶ�����ʽ:3-MPEG4,4-H264,5-MJPEG,���VVV_VIDEO_FORMAT_E
    unsigned int  w;            //��
    unsigned int  h;            //��
    unsigned int  FrameRate;    //֡�� (1-50/60) PALΪ25��NTSCΪ30
    unsigned int  Standard;     //��ʽ (0ΪPAL��,1ΪNTSC��)
    unsigned int  RateType;     //��ģʽ(0Ϊ��������1Ϊ������)
    unsigned int  resolution;   //¼��ϸ��:0-��ϸ(D1)��1-�ḻ(HD1)��2-��ͨ(CIF),3-QVGA,4-VGA,5-720P,6-1080P
    unsigned int  dwBitRate;    //����(8-16384),��λKbps
    unsigned int  ImageQuality; //��������(0-5),0Ϊ���,5Ϊ���
    unsigned int  nMaxKeyInterval;//�ؼ�֡���(1-100)
    int           bSaveFlash;
}VVV_VENC_CONFIG_S,*PVVV_VENC_CONFIG_S;

typedef struct tagVVV_VENC_MP4INFO
{
    unsigned int dwSize;
    unsigned int chn; //0, ipc�̶�Ϊ0
    unsigned int stream; //0 ������, 1, ������, 2, �ֻ�����
    unsigned int infolen;
    unsigned char info[128];
}VVV_VENC_MP4INFO_S;

/*��Ƶ����VVV_CMD_SET_AENC_CONFIG,VVV_CMD_GET_AENC_CONFIG*/
typedef struct tagVVV_AENC_CONFIG
{
    unsigned int  dwSize;
    unsigned int  chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  bEncodeAudio; //�Ƿ������Ƶ
    unsigned int  AudioFormat;  //��Ƶ�����ʽ:1-G711A,2-G711U,3-ADPCM,4-G726,���VVV_AUDIO_FORMAT_E
    unsigned int  samplerate;   //������: 0-8K,2-16K,4-24K,5-32K,6-44.1K,7-48K,���VVV_AUDIO_SAMPLE_RATE_E
    unsigned int  bitwidth;     //λ��: 0-8λ,1-16λ,���VVV_AUDIO_BITWIDTH_E
    unsigned int  soundmode;    //����ģʽ, 0-MOMO,1-STEREO,���VVV_SOUND_MODE_E
    unsigned int  bindvideochn; //��Ƶ�󶨵���Ƶͨ����
    int           bSaveFlash;
}VVV_AENC_CONFIG_S,*PVVV_AENC_CONFIG_S;

//����ؼ�֡VVV_CMD_REQUEST_KEYFRAME
typedef struct tagVVV_CHN_FRAMEREQ
{
    unsigned int  dwSize;
    unsigned int  chn;         //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  u32StreamFlag;//��������,0:������,1:������,2:��������,3:��������
    unsigned int  FrameType;   // 0 - I�����ౣ��
}VVV_CHN_FRAMEREQ_S, *PVVV_CHN_FRAMEREQ_S;

//ץ�Ĵ����˲���VVV_CMD_SET_SNAPSHOT,VVV_CMD_GET_SNAPSHOT
typedef struct tagVVV_SNAP
{
    unsigned int  dwSize;
    unsigned char chn;        //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char group;      //�ڼ���ץ��
    unsigned char bEnable;    //ץ��ʹ��
    unsigned char operation;  //(1:snap,4:snap&mail,0x10:snap&ftp)
    int           interval;   //��ʱץ��ʱ����
    unsigned char hour[7][96];//ץ�ļƻ�
    unsigned char Qval;       //Qֵ0~2
	unsigned char continueNum;   //����ץ������0~5
    unsigned char bSaveFlash;
	unsigned char resv1;
}VVV_SNAP_S, *PVVV_SNAP_S;

//�澯��ץ�Ĵ����˲���VVV_CMD_SET_ALMSNAPSHOT,VVV_CMD_GET_ALMSNAPSHOT
typedef struct tagVVV_ALARM_SNAP
{
    unsigned int  dwSize;
    unsigned char enable;
    unsigned char interval; //�澯ץ�ļ��
    unsigned char time; //�澯ץ��ʱ��
    unsigned char quality; //ץ��ͼƬ������1:��� 2: �� 3:һ�㣬����Ĭ�������
    unsigned char bSaveFlash;
	unsigned char resv[39];
}VVV_ALARM_SNAP_S, *PVVV_ALARM_SNAP_S;


//¼������VVV_CMD_SET_REC_CONFIG,VVV_CMD_GET_REC_CONFIG
typedef struct tagVVV_RECORD_CONFIG
{
    unsigned int  dwSize;
    unsigned char  chn;         //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char  enable;      //¼�񿪹�:0-��¼��,1-¼��
    unsigned char  RecordMode;  //¼��ģʽ:0-��ʱ¼��1-����¼��
    unsigned char hour[7][24]; //¼���ܼƻ�:Ϊ0��ʾ��¼��Ϊ2��ʾ����¼��1-����¼��
    unsigned char reserver[8];
    unsigned char  PackTime;    //¼����ʱ�䣬0-15���ӣ�1-30���ӣ�2-45���ӣ�3-60����
    unsigned char  bRecAudio;   //�Ƿ�¼����
    unsigned char  bSaveFlash;
    unsigned char  bPrerecord;  //�Ƿ���Ԥ¼ 0-close   1-open
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
}VVV_RECORD_CONFIG_S,*PVVV_RECORD_CONFIG;

//¼������VVV_CMD_SET_REC_CONFIG_EX,VVV_CMD_GET_REC_CONFIG_EX
typedef struct tagVVV_RECORD_CONFIG_EX
{
    unsigned int  dwSize;
    unsigned char  chn;         //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char  enable;      //¼�񿪹�:0-��¼��,1-¼��
    unsigned char  RecordMode;  //¼��ģʽ:0-��ʱ¼��1-����¼��
    unsigned char hour[7][96]; //¼���ܼƻ�:Ϊ0��ʾ��¼��Ϊ2��ʾ����¼��1-����¼��
    unsigned char timerrecStatus;
    unsigned char manualrecStatus;    
    unsigned char reserver[6];
    unsigned int  PackTime;    //¼����ʱ�䣬0-15���ӣ�1-30���ӣ�2-45���ӣ�3-60����
    unsigned char  bRecAudio;   //�Ƿ�¼����
    unsigned char  bSaveFlash;
    unsigned char  bPrerecord;  //�Ƿ���Ԥ¼ 0-close   1-open
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
}VVV_RECORD_CONFIG_EX_S,*PVVV_RECORD_CONFIG_EX;


//�����ֶ�¼��VVV_CMD_RECORD_BEGIN,VVV_CMD_RECORD_STOP
typedef struct tagVVV_RECORD_BEGIN
{
    unsigned int  dwSize;
    unsigned int  chn;         //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  Trigertype;  //��������:0x1-��ʱ,0x02-�¼�,0x04-�ֶ�
    unsigned int  bTimeControl;//�Ƿ�ʱ�����
    unsigned int  RecordTime;  //������¼��ʱ��(��Ϊ��λ)��������ʱ����¼���Զ�ֹͣ
    unsigned int  bSizeControl;//�Ƿ��ļ���С����
    unsigned int  RecordSize;  //������¼���С,�����ô�С¼���Զ�ֹͣ
}VVV_RECORD_BEGIN_S;

//¼��״̬VVV_CMD_GET_REC_STATE
typedef struct tagVVV_RECORD_STATE
{
    unsigned int  dwSize;
    unsigned int  chn;                //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  chnRecordState;     //��ǰͨ��¼��״̬
    unsigned int  ServerSpace;        //��MΪ��λ
    unsigned int  SpaceFree;          //���ɿռ�
}VVV_RECORD_STATE_S,*PVVV_RECORD_STATE_S;

//����������ط�VVV_CMD_SET_DEC,VVV_CMD_GET_DEC
typedef struct tagVVV_DEC
{
    unsigned int  dwSize;
    unsigned int  selectchn; //�ط�ͨ��ѡ��ÿһ��ͨ��һ��bitλ���ر�ʱΪ0,����6(������Ϊ110)��ʾ�طŵڶ�����ͨ��
    int           Volume;    //-0-100
    int           Mute;      //-0-�رգ�1-��
    int           bSaveFlash;
} VVV_DEC_S,*PVVV_DEC_S;

/*������ϢVVV_CMD_GET_DISK_INFO*/
typedef struct tagDISK_ITEM
{
    unsigned char dwDiskID;       //��Itemָ���ǵڼ���Ӳ���ϵ�
    unsigned char dwDiskType;     //��������
    unsigned char dwPartitionIndex; 
    unsigned char dwPartitionType;
    unsigned char dwPartitionState;//����״̬,0:��Ӳ��;1:δ��ʽ��;2:����;3:��
    unsigned char resv1;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116
    unsigned int  dwPartitionSize; //��������,��MΪ��λ
    unsigned int  dwPartitionFree; //����ʣ��ռ�
}DISK_ITEM;
typedef struct tagVVV_DISK_INFO
{
    unsigned int dwSize;
    unsigned int dwItemCount;
    DISK_ITEM    DiskItem[VVV_MAX_DISK_NUM];
}VVV_DISK_INFO_S,*PVVV_DISK_INFO_S;

/*���̸�ʽ��VVV_CMD_FORMAT_DISK*/
typedef struct tagVVV_FORMAT_DISK
{
    unsigned int dwSize;
    unsigned int DiskType;         //��������:0-Ӳ��,1-USB��,2-SD��
    unsigned int dwDiskID;          //����ID,ÿһ������һ��bitλ
    unsigned int dwPartitionIndex;  //Ҫ��ʽ���Ĵ��̷���
    unsigned int bFastFormat;       //�Ƿ���п��ٸ�ʽ��
    unsigned int dwClusterSize;     //��ʽ���ش�С
    unsigned int dwPartitionSize;   //��ʽ������С(G)
}VVV_FORMAT_DISK_S;

/*����¼������VVV_CMD_SET_DISK_MNG,VVV_CMD_GET_DISK_MNG*/
typedef struct tagVVV_DISK_MNG
{
    unsigned int  dwSize;
    unsigned int  Redundancy;/*32��bit, ÿ��bit��Ӧһ��Ӳ�����ԣ�=0, ���̣� =1,  ������*/
    unsigned int  OverWrite; /*�Զ����ǹ���¼������ʶ��0-�ر�,1-�Զ�,2-1Сʱ,3-3Сʱ,4-1��,5-7��,6-30��,7-90��*/
    int           bSaveFlash;
}VVV_DISK_MNG_S,*PVVV_DISK_MNG_S;

/*����״̬VVV_CMD_GET_DISK_STATE*/
typedef struct tagVVV_DISK_STATE
{
    unsigned int  dwSize;
    unsigned char DISKState[VVV_MAX_DISK_NUM];//Ӳ��״̬ 0:��Ӳ��;1:δ��ʽ��;2:����;3:��
    VVV_U64 DISKTotalSize[VVV_MAX_DISK_NUM];//��λ:�ֽ�
    VVV_U64 DISKFreeSize[VVV_MAX_DISK_NUM];
    VVV_U64 DISKFreeRecord[VVV_MAX_DISK_NUM];
}VVV_DISK_STATE_S,*PVVV_DISK_STATE_S;

/**********************************************************/
/*     �澯����                                           */
/**********************************************************/
//�澯�������VVV_CMD_SET_ALARM,VVV_CMD_GET_ALARM
typedef struct tagsensorout_alarm_info/*̽ͷ�澯���*/
{
    unsigned char bEnable;     //ʹ��
    unsigned char AlarmOutTime;//̽ͷ�����������ʱ��:0��10��20��40��60s
    unsigned char ActiveMode;  //̽ͷ�澯�������ģʽ:0-Open,1-ground
}sensorout_alarm_info;
typedef struct tagVVV_ALARM_SET
{
    unsigned int dwSize;
    unsigned char BuzzerMooTime;           //���������У�����ʱ��:0��10��20��40��60s
    unsigned char RecordDelayTime;         //¼����ʱʱ��:0��10��20��40��60s
    unsigned char SnapDelayTime;           //ץ����ʱʱ��:0��10��20��40��60s
    unsigned char FullSreenTime;           //ȫ�������ʱ�����ʱ��:0,5,10��30��60��120��300S
    unsigned char PreRecordTime;           //Ԥ¼��Ԥ¼ʱ��:0��10s
    sensorout_alarm_info  stSensorAlarmOut;//̽ͷ�澯�������
    unsigned char bSaveFlash;
    unsigned char resv1;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116
}VVV_ALARM_SET_S,*PVVV_ALARM_SET_S;

//��Ƶ�ƶ��澯VVV_CMD_SET_MD_ALARM,VVV_CMD_GET_MD_ALARM
typedef struct tagVVV_CHN_MD_ALARM
{
    unsigned int dwSize;
    unsigned char chn;                //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char bEnable;            //ʹ��
    unsigned char Sensitive;          //������,4���ȼ�:0-�ߣ�1-�ϸߣ�2-�У�3:��
    unsigned char RegionSetting[32]; //�������ã�180��������32���ֽڱ�ʾ��ÿλ��ʾһ������
    unsigned char hour[7][24];       //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;         //����������ʹ��
    unsigned char bAlarmOut;          //�������
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;         //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;          //�Ƿ��Զ�ץͼ
    unsigned char bPtzMove;           //PTZ����
    unsigned char bSendEmail;         //�Ƿ�Email
    unsigned char bFTP;               //�Ƿ�FTP����
    unsigned char bESMS;              //�Ƿ񷢶���
    unsigned char bFullSreen;         //ͨ��ȫ��
    unsigned char bSendCenter;        //�Ƿ����ķ�����
    unsigned char bSaveFlash;
    unsigned char interval;           //�澯���
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
}VVV_CHN_MD_ALARM_S,*PVVV_CHN_MD_ALARM_S;

typedef struct tagVVV_CHN_MD_ALARM_EX
{
    unsigned int dwSize;
    unsigned char chn;                //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char bEnable;            //ʹ��
    unsigned char Sensitive;          //������,4���ȼ�:0-�ߣ�1-�ϸߣ�2-�У�3:��
    unsigned char RegionSetting[32]; //�������ã�180��������32���ֽڱ�ʾ��ÿλ��ʾһ������
    unsigned char hour[7][96];       //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char bNotifyRec;         //�Ƿ�notifySDK
    unsigned char reserver[7];
    unsigned char bBuzzerMoo;         //����������ʹ��
    unsigned char bAlarmOut;          //�������
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;         //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;          //�Ƿ��Զ�ץͼ
    unsigned char bPtzMove;           //PTZ����
    unsigned char bSendEmail;         //�Ƿ�Email
    unsigned char bFTP;               //�Ƿ�FTP����
    unsigned char bESMS;              //�Ƿ񷢶���
    unsigned char bFullSreen;         //ͨ��ȫ��
    unsigned char bSendCenter;        //�Ƿ����ķ�����
    unsigned char bSaveFlash;
    unsigned char interval;           //�澯���
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
}VVV_CHN_MD_ALARM_EX_S,*PVVV_CHN_MD_ALARM_EX_S;

/*̽ͷ����澯VVV_CMD_SET_SENSOR_IN_ALARM,VVV_CMD_GET_SENSOR_IN_ALARM*/
typedef struct tagVVV_SENSOR_IN_ALARM
{
    unsigned int dwSize;
    unsigned char nSensorID;        //̽ͷ����,0~VVV_MAX_SENSOR_IN,0xFF��������̽ͷ
    unsigned char bEnable;          //I/O״̬����,0-������1-���أ�2-�ر�
    unsigned char hour[7][24];     //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;       //����������ʹ��
    unsigned char bAlarmOut;        //�������(���Դ�������澯���)
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;               //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;//�Ƿ��Զ�ץͼ(���Դ�������ͨ��ץͼ)
    unsigned char bPtzMove; //PTZ����(���Դ������PTZ����)
    unsigned char bSendEmail;               //�Ƿ�Email
    unsigned char bFTP;                     //�Ƿ�FTP����
    unsigned char bESMS;                    //�Ƿ񷢶���
    unsigned char bFullSreen;               //ͨ��ȫ��
    unsigned char bSendCenter;              //�Ƿ����ķ�����
    unsigned char bSaveFlash;
    unsigned char interval;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116
}VVV_SENSOR_IN_ALARM_S,*PVVV_SENSOR_IN_ALARM_S;

typedef struct tagVVV_SENSOR_IN_ALARM_EX
{
    unsigned int dwSize;
    unsigned char nSensorID;        //̽ͷ����,0~VVV_MAX_SENSOR_IN,0xFF��������̽ͷ
    unsigned char bEnable;          //I/O״̬����,0-������1-���أ�2-�ر�
    unsigned char hour[7][96];     //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;       //����������ʹ��
    unsigned char bAlarmOut;        //�������(���Դ�������澯���)
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;               //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;//�Ƿ��Զ�ץͼ(���Դ�������ͨ��ץͼ)
    unsigned char bPtzMove; //PTZ����(���Դ������PTZ����)
    unsigned char bSendEmail;               //�Ƿ�Email
    unsigned char bFTPSNAP;                     //�Ƿ�FTP����SNAP
    unsigned char bFTPREC;                    //�Ƿ�FTP����REC
    unsigned char bFullSreen;               //ͨ��ȫ��
    unsigned char bSendCenter;              //�Ƿ����ķ�����
    unsigned char bSaveFlash;
    unsigned char interval;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116
}VVV_SENSOR_IN_ALARM_EX_S,*PVVV_SENSOR_IN_ALARM_EX_S;

typedef struct tagVVV_MICROWAVE_ALARM
{
    unsigned int dwSize;
    unsigned char bEnable;          //״̬����,0-�ر� 1-��
    unsigned char hour[7][24];     //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;       //����������ʹ��
    unsigned char bAlarmOut;        //�������(���Դ�������澯���)
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;               //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;//�Ƿ��Զ�ץͼ(���Դ�������ͨ��ץͼ)
    unsigned char bPtzMove; //PTZ����(���Դ������PTZ����)
    unsigned char bSendEmail;               //�Ƿ�Email
    unsigned char bFTP;                     //�Ƿ�FTP����
    unsigned char bESMS;                    //�Ƿ񷢶���
    unsigned char bFullSreen;               //ͨ��ȫ��
    unsigned char bSendCenter;              //�Ƿ����ķ�����
    unsigned char bSaveFlash;
}VVV_MICROWAVE_ALARM_S,*PVVV_MICROWAVE_ALARM_S;

typedef struct tagVVV_PIR_ALARM
{
    unsigned int dwSize;
    unsigned char bEnable;          //״̬����,0-������1-���أ�2-�ر�
    unsigned char hour[7][24];     //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;       //����������ʹ��
    unsigned char bAlarmOut;        //�������(���Դ�������澯���)
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;               //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;//�Ƿ��Զ�ץͼ(���Դ�������ͨ��ץͼ)
    unsigned char bPtzMove; //PTZ����(���Դ������PTZ����)
    unsigned char bSendEmail;               //�Ƿ�Email
    unsigned char bFTP;                     //�Ƿ�FTP����
    unsigned char bESMS;                    //�Ƿ񷢶���
    unsigned char bFullSreen;               //ͨ��ȫ��
    unsigned char bSendCenter;              //�Ƿ����ķ�����
    unsigned char bSaveFlash;
}VVV_PIR_ALARM_S,*PVVV_PIR_ALARM_S;


typedef struct tagVVV_HUMI_ALARM
{
    unsigned int dwSize;
	int tempLow;                    // -10~50
	int tempHigh;                   // -10~50
	int RHLow;                      // 0~100
	int RHHigh;	                    // 0~100
	int PM25;                       // high-1;normail-2;low-3	    
    unsigned char bEnable;          //״̬����,0-�رգ�0x1-�¶ȱ�����0x2-ʪ�ȱ�����0x4-PM2.5����,���0x7��all
    unsigned char hour[7][24];     //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;       //����������ʹ��
    unsigned char bAlarmOut;        //�������(���Դ�������澯���)
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;               //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;//�Ƿ��Զ�ץͼ(���Դ�������ͨ��ץͼ)
    unsigned char bPtzMove; //PTZ����(���Դ������PTZ����)
    unsigned char bSendEmail;               //�Ƿ�Email
    unsigned char bFTP;                     //�Ƿ�FTP����
    unsigned char bESMS;                    //�Ƿ񷢶���
    unsigned char bFullSreen;               //ͨ��ȫ��
    unsigned char bSendCenter;              //�Ƿ����ķ�����
    unsigned char bSaveFlash;
}VVV_HUMI_ALARM_S,*PVVV_HUMI_ALARM_S;

typedef struct tagSHELTER_S
{
	unsigned char bEnable[4];   /* �Ƿ����� */
    unsigned char Sensitive[4]; //������,4���ȼ�:0-�ߣ�1-�ϸߣ�2-�У�3:��
    int           s32X[4];
    int           s32Y[4];
    unsigned int  u32Width[4];
    unsigned int  u32Height[4];
}SHELTER_S;
//��Ƶ�ڵ��澯VVV_CMD_SET_VIDEO_SHELTER,VVV_CMD_GET_VIDEO_SHELTER
typedef struct tagVVV_VIDEO_SHELTER
{
    unsigned int dwSize;
    unsigned char chn;                //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned char resv1;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116    
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116    
	SHELTER_S     chnvc;              //�ڸ�����
    unsigned char hour[7][24];        //�����ܼƻ�:Ϊ0��ʾ��ʹ�ܣ�1-ʹ��
    unsigned char reserver[8];
    unsigned char bBuzzerMoo;         //����������ʹ��
    unsigned char bAlarmOut;          //�������
    unsigned char bRecord[VVV_MAX_CHN_NUM];  //����¼��(���Դ�������ͨ��¼��)
    unsigned char bPreRecord;         //�Ƿ�Ԥ¼��
    unsigned char bShotSnap;          //�Ƿ��Զ�ץͼ
    unsigned char bPtzMove;           //PTZ����
    unsigned char bSendEmail;         //�Ƿ�Email
    unsigned char bFTP;               //�Ƿ�FTP����
    unsigned char bESMS;              //�Ƿ񷢶���
    unsigned char bFullSreen;         //ͨ��ȫ��
    unsigned char bSendCenter;        //�Ƿ����ķ�����
    unsigned char bSaveFlash;
    unsigned char resv4; //BTaligned,Ϊ�˶�������, 20140116   
}VVV_VIDEO_SHELTER_S,*PVVV_VIDEO_SHELTER_S;

/*�쳣��������VVV_CMD_SET_EXCEPTION_ALARM,VVV_CMD_GET_EXCEPTION_ALARM*/
typedef struct tagVVV_EXCEPTION_ALARM
{
    unsigned int dwSize;
    unsigned char InvalidAlarmBuzzer; /*����ʧЧ�������������أ�0-�رգ�1-����*/
    unsigned char InvalidAlarmOutput; /*����ʧЧ����������أ�0-�رգ�1-����*/
    unsigned char NoSpaceAlarmBuzzer;/*���̿ռ䲻�����������,����ʣ��ռ�:�رգ�1G,   5G,  10G,  1Сʱ,  5Сʱ,  10Сʱ, 20Сʱ*/
    unsigned char NoSpaceAlarmOutput;/*���̿ռ䲻��澯���,����ʣ��ռ�:�رգ�1G,   5G,  10G,  1Сʱ,  5Сʱ,  10Сʱ, 20Сʱ*/
    unsigned char VideoLossAlarmBuzzer;/*��Ƶ��ʧ�������������أ�0-�رգ�1-����*/
    unsigned char VideoLossAlarmOutput;/*��Ƶ��ʧ����������أ�0-�رգ�1-����*/
    unsigned char VideoCoverAlarmBuzzer;/*��Ƶ�ڵ��������������أ�0-�رգ�1-����*/
    unsigned char VideoCoverAlarmOutput;/*��Ƶ�ڵ�����������أ�0-�رգ�1-����*/	
    unsigned char SDLossAlarmBuzzer;/*SD��ʧЧ�������������أ�0-�رգ�1-����*/	
    unsigned char SDLossAlarmOutput;/*SD��ʧЧ����������أ�0-�رգ�1-����*/
    unsigned char sd_interval;      /*SD�������澯Ƶ��,�����ʱ����ڷ����ĸ澯, �������´���*/
    unsigned char bSaveFlash;
}VVV_EXCEPTION_ALARM_S,*PVVV_EXCEPTION_ALARM_S;

/*����澯VVV_CMD_CLEAR_ALARM*/
typedef struct tagVVV_CLEAR_ALARM
{
    unsigned int dwSize;
    unsigned int bBuzzerMoo;  //������
    unsigned int bFullSreen;  //ȫ��״̬
    unsigned int bAlarmOut;   //�������
}VVV_CLEAR_ALARM_S,*PVVV_CLEAR_ALARM_S;


//��̨Ѳ������VVV_CMD_SET_PTZ_CRUISE,VVV_CMD_GET_PTZ_CRUISE
typedef struct tagVVV_PTZ_CRUISE
{
    unsigned int   dwSize;
    unsigned int   chn;            //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int   nTotalPts;
    unsigned char  Operate[256];   //0-����Ԥ�õ�,1-���Ԥ�õ�,2-gotoԤ�õ�
    unsigned char nPTZPoint[256]; //Ԥ�õ�(���֧��256��Ԥ�õ�)
    unsigned char nPauseSec[256]; //�ڴ˵�ͣ��ʱ��(��)
}VVV_PTZ_CRUISE_S,*PVVV_PTZ_CRUISE_S;

//������485�ߵ�COM����VVV_CMD_SET_COMINFO,VVV_CMD_GET_COMINFO
typedef struct tagVVV_SERVER_COMINFO
{
    unsigned int  dwSize;
    unsigned int  chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  Protocol;     //Э�����ͣ�0-Pelco-D��1-Pelco-P
    unsigned int  BaudRate;     //������:1200,2400,4800,9600,19200,38400,43000,56000,57600,115200
    unsigned int  nDataBits;    //����λ:8,7,6,5
    unsigned int  nStopBits;    //ֹͣλ:1,2
    unsigned int  nParity;      //У��λ:0-None��1-Odd��2-Even��3-Mark��4-Space
    unsigned int  Number;       //��ţ���ΧΪ1--63
    int   nStreamControl;//������
    int   bTransferState;//͸������
    int   nPrePos;       //��̨Ԥ��λ
    int   nCruise;       //��̨Ѳ��
    int   nTrack;        //��̨�켣
    int   nPTZSpeed;     //��̨�ٶ�
    int   nAddress;      //��̨��ַ
    char  szPTZName[VVV_NAME_LEN];//��̨Э������
    char  PtzContent[512];    //Э������
    int          bSaveFlash;
}VVV_SERVER_COMINFO_S,*PVVV_SERVER_COMINFO_S;

typedef enum tagVVV_PTZ_SPEED_E
{
	VVV_PTZ_SPEED_HIGH,
	VVV_PTZ_SPEED_MID,
	VVV_PTZ_SPEED_LOW,
	VVV_PTZ_SPEED_BUFF
}VVV_PTZ_SPEED_E;

typedef enum tagVVV_PTZ_STEP_E
{
	VVV_PTZ_STEP_SMALL = 50,
	VVV_PTZ_STEP_MID = 100,
	VVV_PTZ_STEP_BIG = 200,
	VVV_PTZ_STEP_BUFF
}VVV_PTZ_STEP_E;

typedef struct tagVVV_PTZ_MOTOR_ATTR
{
    char bMirror;  /*ˮƽת�������Ƿ�ת*/
	char bFlip;    /*��ֱת�������Ƿ�ת*/	
	char bMirror_ko;  /*ˮƽת�������Ƿ�ת*/
	char bFlip_ko;    /*��ֱת�������Ƿ�ת*/    
	VVV_PTZ_SPEED_E enPanSpd;  /*ˮƽת���ٶ�*/
	VVV_PTZ_SPEED_E enTiltSpd; /*��ֱת���ٶ�*/
	int enPanStep; /*ˮƽ�˶�����*/
	int enTiltStep;/*��ֱ�˶�����*/
	int s32MaxPanStep; /*ˮƽ�˶���󲽳�*/
	int s32MaxTiltStep;/*��ֱ�˶���󲽳�*/
    int s32StayTime;//second
    int s32TourCnt;//Ѳ������
	int s32GuardPosEnable;//�Ƿ�ʹ�ܿ���λ
	int s32GuardPosInterval;//����λ���
	int s32GuardPos;//����λ��
}VVV_PTZ_MOTOR_ATTR_S;

typedef struct tagVVV_PTZ_RS485_ATTR
{
    char  szPTZName[VVV_NAME_LEN];//��̨Э������
    unsigned char	m_bTrack;
	unsigned char	m_bCruise;
	unsigned char	m_wPrePos;
	unsigned char	m_wPTZSpeed;
	unsigned int    baud; //������:1200,2400,4800,9600,19200,38400,43000,56000,57600,115200
	unsigned int 	databits; //����λ:8,7,6,5
	unsigned int 	parity;    //У��λ:0-None��1-Odd��2-Even��3-Mark��4-Space
	unsigned int 	stopbits; //ֹͣλ:1,2   
    int Address;
}VVV_PTZ_RS485_ATTR_S;

typedef struct tagVVV_PTZ_ATTR
{
	int s32Type;/*��̨���� 0:rs485 1:���ٵ��*/
    VVV_PTZ_MOTOR_ATTR_S stMotorInfo;
    VVV_PTZ_RS485_ATTR_S stRS485Info;
    int s32track[4][4];
}VVV_PTZ_ATTR_S, *PVVV_VVV_PTZ_ATTR_S;

//ֱ��ͨ��COM���շ�����VVV_CMD_SEND_COMDATA
typedef struct tagVVV_COM_DATA
{
    unsigned int  dwSize;
    unsigned int  chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    unsigned int  bIs485;       //485 or 232
    char          DataBuf[128]; //���128
}VVV_COM_DATA_S;

/*�û�����VVV_CMD_SET_USERINFO,VVV_CMD_GET_USERINFO*/
typedef enum
{
	VVV_LOCAL_MANUALRECORD_RIGHT = 0,	
	VVV_LOCAL_PTZ_RIGHT, 			
	VVV_LOCAL_PLAYBACK_RIGHT,		
	VVV_LOCAL_SETPARAM_RIGHT,		
	VVV_LOCAL_AUTOROLL_RIGHT,		
	VVV_LOCAL_ZOOM_RIGHT,			
	VVV_LOCAL_SWITCHDEV_RIGHT,		
	VVV_LOCAL_PIP_RIGHT, 			
	VVV_LOCAL_SYS_RIGHT,		
}VVV_LOCAL_PRI_INDEX_E;
typedef enum
{
	VVV_REMOTE_PTZ_RIGHT = 0,			
	VVV_REMOTE_MANUALRECORD_RIGHT,	
	VVV_REMOTE_PLAYBACK_RIGHT,		
	VVV_REMOTE_SETPARAM_RIGHT,		
	VVV_REMOTE_SWITCHDEV_RIGHT, 	
}VVV_REMOTE_PRI_INDEX_E;
typedef struct _userpri_s
{
	unsigned char byLocalRight[10]; /* Ȩ�� */
                        	/*����1: ¼��*/
                        	/*����2: PTZ����*/
                        	/*����3: ¼��ط�/����*/
                        	/*����4: ���ò���*/
                        	/*����5: ����/����/ά��/��ʽ��/�ָ�����*/
                        	/*����6: ����/����*/
                        	/*����7:PIP����*/
                        	/*����8:�Զ���ѭ*/
                        	/*����9:���ӷŴ�*/
	unsigned char byRemoteRight[10]; /* Ȩ�� */
                        	/*����1: ��̨����*/
                        	/*����2: ¼��/ץ��*/
                        	/*����3: �ط�/����*/
                        	/*����4: ���ò���*/
                        	/*����5: ��/����*/
	unsigned char byLocalPreviewRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)]; //���ؿ���Ԥ����ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byRemotePreviewRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//Զ�̿���Ԥ����ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
#if 0
	unsigned char byLocalPlaybackRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//���ؿ��Իطŵ�ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byRemotePlaybackRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//Զ�̿��Իطŵ�ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byLocalRecordRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)]; //���ؿ���¼���ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byRemoteRecordRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//Զ�̿���¼���ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byLocalPTZRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//���ؿ���PTZ��ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byRemotePTZRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//Զ�̿���PTZ��ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
	unsigned char byLocalBackupRight[ASSIGN_FOUR_BYTES(VVV_MAX_CHN_NUM)];//���ر���Ȩ��ͨ�� 0-��Ȩ�ޣ�1-��Ȩ��
#endif
}userpri_s;

typedef struct _userpassword_s
{
	char 		  UserName[VVV_NAME_LEN];/*�û���[���ֳ���]*/
	char 		  UserPsw[VVV_NAME_LEN];/*�û�����*/	
	userpri_s 	  UserPri;
	char 		  DeviceID1[VVV_NAME_LEN];	/*�������*/
    unsigned char DeviceID;		/*�������*/
    unsigned char  bHavePwd;		/*�Ƿ�ʹ�����룬0��ʾ��ʹ�ã�1��ʾʹ��*/
	unsigned char  bUserPrew;	/*�Ƿ�����Ԥ��Ȩ��*/
}userpassword_s;
typedef struct tagVVV_SERVER_USER
{
    unsigned int   dwSize;
	int 		   UserId;/*0, ��һ��Ĭ������ΪAdmin*/
    userpassword_s stUser;
    unsigned char  resv; //BTaligned,Ϊ�˶�������, 20140116
    int            bSaveFlash;
}VVV_SERVER_USER_S,*PVVV_SERVER_USER_S;

/*IE�ͻ����û�����VVV_CMD_SET_IECLIENT_USER,VVV_CMD_GET_IECLIENT_USER*/
typedef struct tagVVV_IE_CLIENT_USER
{
    unsigned int  dwSize;
    unsigned int  BitRate;     /*���ʣ���λΪK*/
    char          UserName[VVV_NAME_LEN];//��ͨ�û���
    char          UserPwd[VVV_NAME_LEN]; //��ͨ�û�����
    char          AdminName[VVV_NAME_LEN];/*����Ա����*/
    char          AdminPwd[VVV_NAME_LEN];/*����Ա����*/
    unsigned int  HavePwd;     /*�Ƿ�ʹ�����룬0��ʾ��ʹ�ã�1��ʾʹ��*/
    unsigned int  QualityOrFrame;/*0Ϊ��������(CIF 6FPS)��1Ϊ֡������(QCIF 25FPS)*/
    int          bSaveFlash;
}VVV_IE_CLIENT_USER_S,*PVVV_IE_CLIENT_USER_S;

/*NVR������IP�豸�����б�VVV_CMD_SET_IP_DEVLIST,VVV_CMD_GET_IP_DEVLIST*/
typedef struct tagdev_stream_info
{
    unsigned char s32Index;
    unsigned char bSrteamValid;
    char szStream[VVV_NAME_LEN];
}dev_stream_info_t;
typedef struct tagdev_chn_info
{
    unsigned char bChnValid;//ʹ��״̬
    char szChn[VVV_NAME_LEN];
    dev_stream_info_t strStreamInfo[2];
}dev_chn_info_t;
typedef struct tagipdev_t
{
    char szDevName[VVV_NAME_LEN];  /*device name*/
    char szDevDescribe[VVV_NAME_LEN];/*device description*/
    char aszIP[VVV_NAME_LEN];      /*IP��ַ*/
    char szMacaddr[VVV_NAME_LEN]; /*device mac address*/
    char aszHttpPort[VVV_NAME_LEN];/*HTTP �����˿�*/
	char aszMediaPort[VVV_NAME_LEN];   /*ý��˿�*/
    char aszUserName[VVV_NAME_LEN];
    char aszPwd[VVV_NAME_LEN];
    dev_chn_info_t strChnInfo[1];
    unsigned char s32Wnd;           /*�豸�󶨵Ĵ���ͨ��,С��0��ʾδ�󶨴���*/
    unsigned char Nettype; /*��������0������1������*/
    unsigned char Netstate;/*����״̬,0�Ͽ�;1������;2������*/
    unsigned char bSaveFlash;
}ipdev_t;
typedef struct tagVVV_IPDEV_LIST
{
    unsigned int  dwSize;
    ipdev_t       IpDevInfo[64];
    int           bSaveFlash;
}VVV_IPDEV_LIST_S,*PVVV_IPDEV_LIST_S;

/*�����ַVVV_CMD_SET_NETWORK,VVV_CMD_GET_NETWORK*/
typedef struct tagVVV_SERVER_NETWORK
{
    unsigned int  dwSize;
    unsigned int  NetworkMode; /*������ʽ��0-DHCP��1-PPPOE��2-�ֶ�����*/
    unsigned int  IPAddr[4];  //IP��ַ
    unsigned int  NetMask[4]; //����
    unsigned int  GateWay[4]; //����
    char          csPPPoEUserName[VVV_NAME_LEN]; //PPPoE�û���
    char          csPPPoEPassword[VVV_NAME_LEN]; //PPPoE����
    unsigned int  DnsFirst[4]; //��һDNS
    unsigned int  DnsSecond[4];//�ڶ�DNS
    unsigned int  HttpPort;    //Http�˿� 
    unsigned int  DataPort;    //���ݶ˿�
    unsigned int  szMacAddr[6];//����MAC��ַ
    int           bSaveFlash;
}VVV_SERVER_NETWORK_S,*PVVV_SERVER_NETWORK_S;

/*�����ַVVV_CMD_SET_NETWORK_PORT,VVV_CMD_GET_NETWORK_PORT*/
typedef struct tagVVV_SERVER_NETWORK_PORT
{
    unsigned int  dwSize;
    unsigned int  HttpPort;    //Http�˿� 
    unsigned int  RtspPort;    //Rtsp�˿�
    unsigned int  OnvifPort;   //Onvif�˿�
    unsigned int  DnsDyn;   
    unsigned int  Port[16];
    int           bSaveFlash;
}VVV_SERVER_NETWORK_PORT_S,*PVVV_SERVER_NETWORK_PORT_S;


//DDNS��������VVV_CMD_SET_DDNSCFG,VVV_CMD_GET_DDNSCFG
typedef struct tagVVV_DDNSCFG
{
    unsigned int  dwSize;
    unsigned int  bEnableDDNS;        //�Ƿ�����DDNS
    struct
    {
        unsigned int nDDNSType;/*DDNS����������, �����������ͣ�
                                0:3322.org
                                1:dyndns
                                2:perfecteyes
                                3:nvrddns,�п�DDNS,������
                                4:nightowldvr
                                5:no-ip
                                6:greatek
                                10:��ͨ�ġ�
                                */
        char csDDNSUsername[VVV_NAME_LEN];
        char csDDNSPassword[VVV_NAME_LEN];
        char csDNSAddress[VVV_IP_LEN];//DNS��������ַ��������IP��ַ������
        unsigned int dwDNSPort;                //DNS�������˿ڣ�Ĭ��Ϊ6500
        unsigned int updateTime;               //ˢ��������ʱ��
    }stDDNS;
    int           bSaveFlash;
}VVV_DDNSCFG_S, *PVVV_DDNSCFG_S;

//UPNP����VVV_CMD_SET_UPNP,VVV_CMD_GET_UPNP
typedef struct tagVVVUPnPConfig
{
    unsigned int dwSize;
    unsigned int bEnable;        /*�Ƿ�����upnp*/
    unsigned int dwMode;         /*upnp������ʽ.0Ϊ�Զ��˿�ӳ�䣬1Ϊָ���˿�ӳ��*/
    unsigned int dwLineMode;     /*upnp����������ʽ.0Ϊ��������,1Ϊ��������*/
    char         csServerIp[VVV_NAME_LEN]; /*upnpӳ������.������·����IP*/
    unsigned int dwDataPort;     /*upnpӳ�����ݶ˿�*/
    unsigned int dwWebPort;      /*upnpӳ������˿�*/
    unsigned int dwMobilePort;   /*upnpӳ���ֻ��˿�*/  
    unsigned int dwDataPort1;    /*upnp��ӳ��ɹ������ݶ˿�*/
    unsigned int dwWebPort1;     /*upnp��ӳ��ɹ�������˿�*/
    unsigned int dwMobilePort1;  /*upnpӳ��ɹ����ֻ��˿�*/
	unsigned int wDataPortOK;
	unsigned int wWebPortOK;
    unsigned int wMobilePortOK;
    int          bSaveFlash;
}VVV_UPNP_CONFIG_S;

/*ftp�ϴ�����VVV_CMD_SET_FTP,VVV_CMD_GET_FTP*/
typedef struct tagVVV_FTP_PARAM
{
    unsigned int   dwSize;
    unsigned int   dwEnableFTP;        //�Ƿ�����ftp�ϴ�����
    char           csFTPIpAddress[VVV_NAME_LEN];//ftp ������
    unsigned int   dwFTPPort;          //ftp�˿�
    char           sUserName[VVV_NAME_LEN];//�û���
    char           sPassword[VVV_NAME_LEN];//����
    unsigned int   wTopDirMode;        //0x0 = ʹ���豸ip��ַ,0x1 = ʹ���豸��,0x2 = OFF
    unsigned int   wSubDirMode;        //0x0 = ʹ��ͨ���� ,0x1 = ʹ��ͨ����,0x2 = OFF
    unsigned int   bAutoUpData;        //�Ƿ������Զ��ϴ�ͼƬ����
    unsigned int   dwAutoTime;         //�Զ���ʱ�ϴ�ʱ��
    unsigned int   chn;          //ͨ����0~VVV_MAX_CHN_NUM,���Ϊ0xFF��Ϊ����ͨ��
    int            bSaveFlash;
}VVV_FTP_PARAM_S, *LVVV_FTP_PARAM_S;

/*Email����VVV_CMD_SET_EMAIL,VVV_CMD_GET_EMAIL*/
typedef struct tagVVV_EMAIL_PARAM
{
    unsigned int  dwSize;
    char          csEmailServer[VVV_NAME_LEN];//��������ַ
    char          csEMailUser[VVV_NAME_LEN];  //�û���
    char          csEmailPass[VVV_NAME_LEN];  //����
    char          csEmailFrom[VVV_NAME_LEN];
    char          csEmailTo[VVV_EMAIL_NUM][VVV_NAME_LEN];
    char          subject[VVV_NAME_LEN];      //����
    char          text[128];       //����
    char          attachment[VVV_NAME_LEN];  //������ַ
    unsigned char bEnableEmail;
    unsigned char resv1;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116    
    unsigned int  port;             //�������˿�
    unsigned char ssl;              //�Ƿ�SSl����, 0--��, 1--�� 
    unsigned char logintype;        //��½����,0-CramLogin 1-AuthLogin 2-AuthLoginPlain 3-no_login
    unsigned char intervaltime;     /*�ʼ����ͼ��0-180,1-300,2-600s*/
    unsigned char bSaveFlash;
}VVV_EMAIL_PARAM_S, *PVVV_EMAIL_PARAM_S;

/*NTP����VVV_CMD_SET_NTP,VVV_CMD_GET_NTP*/
typedef struct tagVVV_NTP_PARAM
{
    unsigned int   dwSize;
    unsigned int   bEnableNTP;      //NTPʹ��
    char           csNtpServer[128];//��������ַ
    unsigned int   port;            //�������˿�
    unsigned int   Interval;        //ntpͬ����� 0:ÿСʱ��1:ÿ�� 2:ÿ�� 3:ÿ��
    int           bSaveFlash;
}VVV_NTP_PARAM_S, *PVVV_NTP_PARAM_S;

//���ķ�����VVV_CMD_SET_CENTER_INFO,VVV_CMD_GET_CENTER_INFO
typedef struct tagVVV_CENTER_INFO
{
    unsigned int  dwSize;
    unsigned int  bEnable;
    char          CenterIp[VVV_NAME_LEN];    //����IP
    unsigned int  dwCenterPort;    //���Ķ˿�
    char          csServerNo[VVV_NAME_LEN];  //���������к�
    unsigned int  Status;          //����������״̬ 0Ϊδ���� 1Ϊ������ 2���ӳɹ�
    char          csUsername[VVV_NAME_LEN];
    char          csUserpass[VVV_NAME_LEN];
    int           bSaveFlash;
}VVV_CENTER_INFO_S;

//Wifi����VVV_CMD_SET_WIFI,VVV_CMD_GET_WIFI
#define VVV_WIRELESS_WIFI           0x01/*��������*/
#define VVV_WIRELESS_TDSCDMA_ZX     0x02
#define VVV_WIRELESS_EVDO_HUAWEI    0x03
#define VVV_WIRELESS_WCDMA          0x04
typedef struct tagVVV_WIFI_CONFIG
{
    unsigned int    dwSize;
    unsigned int    bWifiEnable; // 0: disable, 1:static ip address, 2:dhcp
    char            NetIpAddr[VVV_NAME_LEN];
    char            NetMask[VVV_NAME_LEN];
    char            Gateway[VVV_NAME_LEN];
    char            MAC[VVV_NAME_LEN];
    char            szEssid[VVV_NAME_LEN];
    unsigned int    nSecurity;   //0: none,
                                 //1:wep
                                 //wpa/wpa2
    unsigned int  byMode;  //1. managed 2. ad-hoc
    unsigned int  byStatus;//0.�ɹ�,����ֵ�Ǵ�����
    unsigned int  byRes;
    char          szWebKey[VVV_NAME_LEN];
    int           bSaveFlash;
}VVV_WIFI_CONFIG_S,*PVVV_WIFI_CONFIG_S;

//3G��������VVV_CMD_SET_3G,VVV_CMD_GET_3G
typedef struct tagVVV_3G_CONFIG
{
    unsigned int    dwSize;
    unsigned int    bCdmaEnable;
    unsigned int    byDevType;
    unsigned int    byStatus;
    char            NetIpAddr[VVV_NAME_LEN]; //IP  (wifi enable)
    int           bSaveFlash;
}VVV_3G_CONFIG_S,*PVVV_3G_CONFIG_S;

/*�ֻ��㲥����VVV_CMD_SET_MOBILE,VVV_CMD_GET_MOBILE*/
typedef struct tagVVV_MOBILE_CONFIG
{
    unsigned int    dwSize;
    unsigned int    chn;
    char name[16];     //�ֻ��û���
    char pwd[16];      //����
    int  port;         //�˿ں�
    int  quality;      //����0���ţ�1������2�ã�3һ��
    int           bSaveFlash;
}VVV_MOBILE_CONFIG_S,*PVVV_MOBILE_CONFIG_S;

typedef struct tagVVV_P2P_CONFIG	
{
	unsigned int dwSize;
	char	param[128];
    int           bSaveFlash;
}VVV_P2P_CONFIG_S,*PVVV_P2P_CONFIG_S;

/*��ʪ��VVV_CMD_SET_HUMITURE,VVV_CMD_GET_HUMITURE*/
typedef struct tagVVV_HUMITURE_S
{
    unsigned int  dwSize;
	int humidity;
	int temperature;
    int pm25;
    int resv[6];
    int           bSaveFlash;
}VVV_HUMITURE_S,*PVVV_HUMITURE_S;

//RFID ����VVV_CMD_SET_RFID_INFO,VVV_CMD_GET_RFID_INFO
typedef struct tagVVVRfidConfig
{
    unsigned int dwSize;
    char		 action;         /*0-ע��; 1-��ʼע��;-1-ֹͣע��;2-�뿪;3-����;4-��ע��,5-ע��*/
	char         power;
    char 		 resv1;
    char 		 resv2;
    char         csRfid[VVV_NAME_LEN]; /*Rfid����id ��*/
    int          bSaveFlash;
}VVV_Rfid_CONFIG_S, *PVVV_Rfid_CONFIG_S;
//����VVV_CMD_SET_BUZZER,VVV_CMD_GET_BUZZER
typedef struct tagVVVBuzzerConfig
{
    unsigned int dwSize;
    char		 action;        /*0-�ر�; 1-��*/
    unsigned char resv1;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv2;//BTaligned,Ϊ�˶�������, 20140116
    unsigned char resv3;//BTaligned,Ϊ�˶�������, 20140116
    
    int          bSaveFlash;
}VVV_BUZZER_CONFIG, *PVVV_BUZZER_CONFIG;

//����ACODEC��ƵоƬ 
//VVV_CMD_SET_AUDIOOUT_VOL VVV_CMD_GET_AUDIOOUT_VOL
//VVV_CMD_SET_AUDIOIN_GAIN VVV_CMD_GET_AUDIOIN_GAIN
typedef struct tagVVVACODECCFG
{
	int s32AcodecDev;
	int s32Value;
}VVV_ACODEC_CONFIG, *PVVV_ACODEC_CONFIG;

//����ACODEC��ƵоƬ 
//VVV_CMD_ENABLE_AEC VVV_CMD_DISABLE_AEC
typedef struct tagVVVACODECAEC
{
	int s32AIDevID;
	int s32AIChn;
	int s32AODevID;
	int s32AOChn;
}VVV_ACODEC_AEC_CONFIG, *PVVV_ACODEC_AEC_CONFIG;


/* ��������ص�*/
typedef int (*VVV_ON_CMD_CALLBACK)(unsigned int dwCmd,       /* ����*/
                                  int          socket,      /*����Socket*/
                                  void         *lpInData,   /* ���� */
                                  unsigned int Size,        /* �������ݴ�С*/
                                  void         *lpOutData,  /* �������� */
                                  unsigned int *pSize,      /* �������ݴ�С*/
                                  char         Username[VVV_NAME_LEN],/* �û���*/
                                  char         Password[VVV_NAME_LEN],/* ����*/
                                  void         *pUserdata); /* �û�����*/

//�ֽڶ���
#pragma pack ()


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  //__VVV_NET_DEFINE_H__

