#ifndef DDNS_H_
#define DDNS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_OK (0)
#define ERR_SOCKET (-1)
#define ERR_SOCKET_TIMEOUT (-2)

int  SHX_GetLastError(void); //�ڲ�������������ȫ�棬��Ҫ�����������
int  SHX_Init(void); // 0����ʧ��, 1����ɹ�
int  SHX_ReqDomain(const char *userid, int userlen,const char *password ,int passlen,char * serialnumber,int seriallen,char * key,int keylen); // 0����ʧ��, 1����ɹ�
void SHX_ReqStartClient(const char *userid, int userlen,const char *password ,int passlen);
void SHX_ReqStopClient(const char *userid, int userlen,const char *password ,int passlen);
void SHX_UnInit(void);

/*
ֻ���������ĺ�������˳��Ϊ 1)SHX_Init(��ʼ��) 2)SHX_ReqDomain(����) 3)SHX_UnInit(����ʼ��)
�����Ѿ����룬ֻ��Ҫ�㱨ip�ĺ�������˳��Ϊ 1)SHX_Init(��ʼ��) 2)SHX_ReqStartClient(��ʼ����) 3)SHX_ReqStopClient(ֹͣ����) 4)SHX_UnInit(����ʼ��)
������������������ 1)SHX_Init 2)SHX_ReqDomain 3)SHX_ReqStartClient 4)SHX_ReqStopClient 5)SHX_UnInit
*/
#ifdef __cplusplus
}
#endif

#endif /*DDNS_H_*/
