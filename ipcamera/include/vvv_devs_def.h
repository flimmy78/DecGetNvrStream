#ifndef __VVV_DEVS_DEF_H__
#define __VVV_DEVS_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


enum devs_protocol_type_e
{
    devs_proto_mcast_v10 = 0,
    devs_proto_bcast_v20 = 1,
    devs_proto_max 
}  ;      
    
enum devs_nodetype
{
    devs_node_device  = 0 , //�豸, ���Ա�������, �����õ�
    devs_node_control = 1 , //���Ƶ�, ���Է������������
    devs_node_mixed   = 2 , //TODO ���Է���, Ҳ��������, 
};

enum devs_sendmode
{
    devs_sendmode_defaultnic = 0xFE,//-- ͨ��Ĭ�����ط���
    devs_sendmode_allnic = 0xFF,  // -- �����������, ��ʹ���������
};

#define MAX_NIC_NUM 8
#define DEVSEARCH_DEVIDSIZE (32)
#define VVV_SEARCH_STRLEN (20)
#define VVV_SEARCH_STRNO_LEN       (20)
#define VVV_SEARCH_CHNNO_LEN       (20)
#define VVV_SEARCH_IP_STRSIZE      (15)
#define VVV_SEARCH_MAC_STRSIZE     (17)
#define VVV_SEARCH_DEVID_STRSIZE   (32)
#define VVV_SEARCH_DEVNAME_STRSIZE (40)
#define VVV_SEARCH_SWVER_STRSIZE   (32)


#define VVV_DEVS_MAX_RESPCODE_SIZE 8
#define VVV_DEVS_MAX_RESPMSG_SIZE  128


//============ V2 
struct devs_msg_head
{
    char client_id[DEVSEARCH_DEVIDSIZE + 1];
    char device_id[DEVSEARCH_DEVIDSIZE + 1 ];
    int  cseq;
    char cmdtype[16];
    char user[32];
    char pwd[32];
};


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

/*�������Ϣ*/
typedef struct tagVVV_SEARCH_CMD_RES
{
    char* pszCmd;        /*����*/
    char* pszResultInfo; /*�������*/
} VVV_SEARCH_CMD_RES_S;

/*���������ӿ�, use by node device*/
typedef int (*pfn_devs_search_req)(char* respcode, VVV_SEARCH_DEVINFO_S* pdevinfo,void* puser);
typedef int (*pfn_devs_cmd_req)(struct devs_msg_head  *msghead, char* cmd,  char* respmsg, char* respcode, void* puser);
 

/*����������Ӧ�ӿ�*/
/*���ּ�����, �ӿڰ���ԭ��������*/
typedef int (*pfn_devs_search_resp)(
                       const void* pvHandle,        /*���*/
                       char* pszRNPCode,            /*����*/
                       VVV_SEARCH_DEVINFO_S* pstruDevInfo,/*���������Ϣ*/
                       void* pvUserData);           /*�û�����*/


/*����������Ӧ�ӿ�*/
typedef int (*pfn_devs_cmd_resp)(
               const void* pvHandle,/*���*/
               char* pszRNPCode,    /*����*/
               VVV_SEARCH_CMD_RES_S* pstruResponseInfo,/*������Ϣ*/
               void* pvUserData);  /*�û�����*/





/*���յ����������Ĵ���ӿ�
msg ���յ�������
resp�ǻظ�*/
typedef int (*pfn_devs_search_req_v2)(struct devs_msg_head * msghead, char* msg,  char* resp ,char* respcode, void* puser);   

/*���յ������Ĵ���ӿ�
msg ���յ�������
resp�ǻظ�, Ҫ���͵�control�˵�*/
typedef int (*pfn_devs_cmd_req_v2)(struct devs_msg_head * msghead, char* msg, char*resp ,char* respcode, void* puser);           


/*���յ���������ظ���Ĵ���ӿ�
����������������
respmsg-- �����ظ��ַ���*/
typedef int (*pfn_devs_search_resp_v2)(struct devs_msg_head * msghead, char* respmsg, void* puser);   

/*���յ���������ظ���Ĵ���ӿ�
����������������
respmsg  ����ظ��ַ���*/
typedef int (*pfn_devs_cmd_resp_v2)(struct devs_msg_head * msghead, char* respmsg, void* puser);   

//֪ͨ��Ϣ
typedef int (*pfn_devs_cmd_notify_v2)(struct devs_msg_head * msghead, char* msg, void* puser);   

struct devs_callback
{
    void* puser;                  /*�豸��Ϣ����ӿ�ʹ�õ��û�����*/


    
    pfn_devs_search_resp pfn_search_resp;   /*�û�ע���豸��Ϣ����ӿ�*/
    pfn_devs_cmd_resp pfn_cmd_resp; /*�û�ע���豸��Ϣ����ӿ�*/

    //node_device
    pfn_devs_search_req      pfn_search_req;
    pfn_devs_cmd_req         pfn_cmd_req;
        
    //node_control, �����node_device , �������ص���Ϊ��
    pfn_devs_search_resp_v2 pfn_search_resp_v2;   /*�û�ע���豸��Ϣ����ӿ�*/
    pfn_devs_cmd_resp_v2 pfn_cmd_resp_v2; /*�û�ע���豸��Ϣ����ӿ�*/
    pfn_devs_cmd_notify_v2 pfn_cmd_notify_v2; /*�û�ע��ص�, ��ipc��֪ͨʱ, ��������ص�*/
    
    //node device
    pfn_devs_search_req_v2 pfn_search_req_v2;   /*�û�ע���豸��Ϣ����ӿ�*/
    pfn_devs_cmd_req_v2 pfn_cmd_req_v2; /*�û�ע���豸��Ϣ����ӿ�*/

};





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
