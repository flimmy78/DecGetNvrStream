#ifndef _FHDEV_DISCOVER_DEFINE_H_
#define _FHDEV_DISCOVER_DEFINE_H_

#ifdef WIN32    // win32

#else           // linux

#ifndef BOOL
#   define BOOL         int
#endif

#ifndef DWORD
#   define DWORD        unsigned int
#endif

#ifndef LPDWORD
#   define LPDWORD      unsigned int*
#endif

#ifndef WORD
#   define WORD         unsigned short
#endif

#ifndef BYTE
#   define BYTE         unsigned char
#endif

#ifndef LPVOID
#   define LPVOID       void*
#endif

#ifndef DWORD64
#   define DWORD64      unsigned long long
#endif

#ifndef TRUE
#   define TRUE    1
#endif
#ifndef FALSE
#   define FALSE   0
#endif
#ifndef NULL
#   define NULL    0
#endif

#endif



typedef enum
{
    FHNPEN_DType_Client   = 0x00,     // �ͻ���
    FHNPEN_DType_61       = 0x01,     // 61
    FHNPEN_DType_Decoder  = 0x02,     // ������
    FHNPEN_DType_DVR      = 0x03,     // DVR
    FHNPEN_DType_NVR      = 0x04,     // NVR
    FHNPEN_DType_NVS      = 0x05,     // NVS
    FHNPEN_DType_DVS      = 0x06,     // DVS

    FHNPEN_DType_AllDev   = 0xff,     // �����豸
}FHNPEN_DevType_e;


/* IP��ַ�ṹ�� */
typedef struct
{
    char sIPV4[16];
    BYTE sIPV6[128];
} FHDS_IPAddr_t, *LPFHDS_IPAddr_t;


/* ����������Ϣ */
typedef struct
{
    char            sMAC[18];       // MAC
    FHDS_IPAddr_t   stIP;           // IP
    FHDS_IPAddr_t   stMaskIP;       // ��������
    FHDS_IPAddr_t   stGateway;      // ����    
    WORD            wPort;          // PORT
    BYTE            btRes[2];
} FHDS_PubNetAddr_t, *LPFHDS_PubNetAddr_t;


/* ������˽������ */
typedef struct 
{
    BYTE btRes[4];
} FHDS_DECPriInfo_t, *LPFHDS_DECPriInfo_t;


/* DVR˽������ */
typedef struct
{
    BYTE btRes[4];
} FHDS_DVRPriInfo_t, *LPFHDS_DVRPriInfo_t;


/* NVR˽������ */
typedef struct
{
    BYTE btRes[4];
} FHDS_NVRPriInfo_t, *LPFHDS_NVRPriInfo_t;


/* NVS˽������ */
typedef struct
{
    BYTE btRes[4];
} FHDS_NVSPriInfo_t, *LPFHDS_NVSPriInfo_t;


/* DVS˽������ */
typedef struct
{
    BYTE btRes[4];
} FHDS_DVSPriInfo_t, *LPFHDS_DVSPriInfo_t;


/* 61˽������ */
typedef struct
{
    BYTE btRes[4];
} FHDS_61PriInfo_t, *LPFHDS_61PriInfo_t;


/* �豸˽������������ */
typedef union
{
    FHDS_DECPriInfo_t       stDECPriInfo;     // DEC˽������
    FHDS_DVRPriInfo_t       stDVRPriInfo;     // DVR˽������
    FHDS_NVRPriInfo_t       stNVRPriInfo;     // NVR˽������
    FHDS_NVSPriInfo_t       stNVSPriInfo;     // NVS˽������
    FHDS_DVSPriInfo_t       stDVSPriInfo;     // DVS˽������
    FHDS_61PriInfo_t        st61PriInfo;      // 61˽������
} FHDS_DevPrivateInfo_t, *LPFHDS_DevPrivateInfo_t;


/* �豸״̬��Ϣ */
typedef struct 
{
    DWORD                   dwDevType;        // �豸����(FHNPEN_DevType_e)
    FHDS_PubNetAddr_t       stDevNetAddr;     // �����ַ
    char                    sDevVersion[48];  // �豸�汾
    char                    sDeviceName[32];  // �豸����
    char                    sSerialNumber[48];// ���к�
    DWORD                   dwTimeOut;        // δ�������
    BOOL                    bIsAlive;         // ����/����
    FHDS_DevPrivateInfo_t   stPriData;        // �豸˽����Ϣ������
} FHDS_DevState_t, *LPFHDS_DevState_t;


/* �豸�����ַ��Ϣ */
typedef struct 
{
    DWORD                   dwDevType;        // �豸����(FHNPEN_DevType_e)
    FHDS_PubNetAddr_t       stDevNetAddr;     // �����ַ
} FHDS_ModifyNetAddr_t, *LPFHDS_ModifyNetAddr_t;


#endif
