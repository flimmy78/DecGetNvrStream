
#ifndef __VVV_SEARCH_H__
#define __VVV_SEARCH_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define VVV_SEARCH_STRNO_LEN       (20)
#define VVV_SEARCH_CHNNO_LEN       (20)
#define VVV_SEARCH_IP_STRSIZE      (15)
#define VVV_SEARCH_MAC_STRSIZE     (17)
#define VVV_SEARCH_DEVID_STRSIZE   (32)
#define VVV_SEARCH_DEVNAME_STRSIZE (40)
#define VVV_SEARCH_SWVER_STRSIZE   (32)


/*�豸��ʶ��Ϣ�������޸��豸��Ϣ��*/
typedef struct tagVVV_SEARCH_DEVID
{
    char* pszDevID;      /*�豸��ʶ���豸Ψһ��ʶ*/
    char* pszUserName;   /*�û���*/
    char* pszPasswd;     /*����*/
} VVV_SEARCH_DEVID_S;

/*������Ϣ*/
typedef struct tagVVV_SEARCH_STREAMINFO
{
    char szStream[VVV_SEARCH_STRNO_LEN + 1];/*������("chnno"+1��"chnno"+2)*/
    unsigned int bIsSub;                   /*���������������Ǵ�������0����������,1���������*/
}VVV_SEARCH_STREAMINFO_S;

/*ͨ����Ϣ*/
typedef struct tagVVV_SEARCH_CHNINFO
{
    char szChn[VVV_SEARCH_CHNNO_LEN + 1]; /*ͨ����(1��16)*/
    unsigned int u32StreamCnt;          /*ĳ��ͨ������u32StreamCnt������*/
    VVV_SEARCH_STREAMINFO_S* pstruStreamInfo;/*��ͨ����Ӧ��������Ϣ*/ 
}VVV_SEARCH_CHNINFO_S;

/*�豸��Ϣ*/
typedef struct tagVVV_SEARCH_DEVINFO
{
    char aszIP[VVV_SEARCH_IP_STRSIZE + 1];          /*IP��ַ*/
    char aszMASK[VVV_SEARCH_IP_STRSIZE + 1];        /*��������*/
    char aszMAC[VVV_SEARCH_MAC_STRSIZE + 1];        /*MAC��ַ*/
    char aszGTW[VVV_SEARCH_IP_STRSIZE + 1];         /*���ص�ַ*/
    char aszDevID[VVV_SEARCH_DEVID_STRSIZE + 1];    /*�豸ID��������ɵ�32���ַ�*/
    char aszDevMDL[VVV_SEARCH_DEVNAME_STRSIZE + 1]; /*�豸�ͺ�*/
    char aszSwVersion[VVV_SEARCH_SWVER_STRSIZE + 1];/*����汾*/
    char aszDevName[VVV_SEARCH_DEVNAME_STRSIZE + 1];/*�豸��*/
    char aszHttpPort[VVV_SEARCH_IP_STRSIZE + 1];    /*HTTP �����˿�*/
    char aszMediaPort[VVV_SEARCH_IP_STRSIZE + 1];   /*ý������˿�*/
    unsigned int  u32ChnCnt;                       /*ĳ�豸����u32ChnCnt��ͨ��*/
    VVV_SEARCH_CHNINFO_S* pstruChnInfo;             /*���豸�е�ͨ����Ϣ*/
} VVV_SEARCH_DEVINFO_S;

/*����������Ӧ�ӿ�*/
typedef int (*PTR_VSCP_DEVS_Search_RPNProcFN)(
                       const void* pvHandle,        /*���*/
                       char* pszRNPCode,            /*����*/
                       VVV_SEARCH_DEVINFO_S* pstruDevInfo,/*���������Ϣ*/
                       void* pvUserData);           /*�û�����*/

/*�������Ϣ*/
typedef struct tagVVV_SEARCH_CMD_RES
{
    char* pszCmd;        /*����*/
    char* pszResultInfo; /*�������*/
} VVV_SEARCH_CMD_RES_S;

/*����������Ӧ�ӿ�*/
typedef int (*PTR_VSCP_DEVS_Cmd_RPNProcFN)(
               const void* pvHandle,/*���*/
               char* pszRNPCode,    /*����*/
               VVV_SEARCH_CMD_RES_S* pstruResponseInfo,/*������Ϣ*/
               void* pvUserData);  /*�û�����*/

/*��ʼ���豸����*/
int VVV_SEARCH_Init();

/*ȥ��ʼ���豸����*/
int VVV_SEARCH_DeInit();

/*�����豸����*/
int VVV_SEARCH_Start();

/*�����豸��Ϣ�޸�����*/
int VVV_SEARCH_Modify(const VVV_SEARCH_DEVID_S* pstruDEV,
                     const char* pszCmd);/*�豸�޸�����,����"netconf set -devname eth0 -ipaddr 192.168.0.134 -netmask 255.255.255.0 -gateway 192.168.0.1 -hwaddr 00:00:35:12:01:22"*/

/*ע���豸��������ص�*/
int VVV_SEARCH_RegistSearchResultProc(PTR_VSCP_DEVS_Search_RPNProcFN pfunSearchRProc,void* pvUserData);

/*ע���޸��豸��Ϣ����Ļص�*/
int VVV_SEARCH_RegistModifyResultProc(PTR_VSCP_DEVS_Cmd_RPNProcFN pfunCmdRProc,void* pvUserData);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __VVV_SEARCH_H__ */
    
    

