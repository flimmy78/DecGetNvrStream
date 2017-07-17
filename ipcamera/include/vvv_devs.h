
#ifndef __VVV_DEVS_H__
#define __VVV_DEVS_H__

#include "vvv_devs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*��ʼ��devs��*/
//nodetype devs_nodetype
//nodeid, �������NULL. ϵͳ�Զ�����, ��nodetype Ϊ deviceʱ, ����ⲿ����
int devs_init(int nodetype, char* nodeid);

//��ȡ�汾����Ϣver, ��С16Byte(��\0), 
//����Ϊ�汾�ŵ����ֱ�ʶ
//ver���ַ�����ʶ
int devs_getver(char* ver);

//��ʼ���������, protocol_type �� enum devs_protocol_type_e ����, ����������Ĭ�ϵ�0(devs_proto_mcast_v10)
//RET:-1, init error, RET >=0 Handle
//sendmode ��Ӧ�Ľṹ����
//enum devs_sendmode
//{
//    devs_sendmode_defaultnic = 0xFE,-- ͨ��Ĭ�����ط���
//    devs_sendmode_allnic = 0xFF, -- �����������, ��ʹ���������
//};
int devs_init_handle(int protocol_type, int *pHandle, int sendmode);


int devs_deinit();
    
int devs_deinit_handle(int handle);

//�����豸��������, 
int devs_start (int handle);

//�����豸������, ���Ҫ�ֹ�ȥ����
int devs_search(int handle);

//����ע��֪ͨ��Ϣ, only V2
int devs_notify_register(int handle);

//�����豸��������
int devs_sendcmd(int handle, VVV_SEARCH_DEVID_S *pdevinfo, char* cmd); 

int devs_regcb(int handle, struct devs_callback *pcbfuns);




#if defined(VVV_MALLOC)
    #undef VVV_MALLOC
#endif

#if defined(VVV_FREE)
    #undef VVV_FREE
#endif 

#ifdef _WIN32
    #define VVV_MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
    #define VVV_FREE(x) HeapFree(GetProcessHeap(), 0, (x))    
#else
    #define VVV_MALLOC(x) malloc(x)
    #define VVV_FREE(x) free(x)
#endif


#define devs_devinfo_new_p(pdev) \
{\
    pdev = VVV_MALLOC(sizeof(VVV_SEARCH_DEVID_S));\
    pdev->pszDevID = VVV_MALLOC(64);\
    pdev->pszUserName = VVV_MALLOC(32);\
    pdev->pszPasswd = VVV_MALLOC(32);\
}
    
#define devs_devinfo_free_p(pdev) \
{\
    if (pdev) \
    {\
        if (pdev->pszDevID)  VVV_FREE(pdev->pszDevID);\
        if (pdev->pszUserName) VVV_FREE(pdev->pszUserName);\
        if (pdev->pszPasswd) VVV_FREE(pdev->pszPasswd);\
        VVV_FREE(pdev);\
    }\
}

#define devs_devinfo_setvalue(pdev, id, user, pwd) \
{\
            strcpy( pdev->pszDevID, id);\
            strcpy(pdev->pszUserName, user);\
            strcpy(pdev->pszPasswd, pwd);\
}
    
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

