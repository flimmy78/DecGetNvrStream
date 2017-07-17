#ifndef _BIZ_ERRCODE_H_
#define _BIZ_ERRCODE_H_

#define BIZ_SUCCESS 0 //�ɹ�

#define BIZ_ERR_INIT_TOOMUCH -1 //�ظ���ʼ��
#define BIZ_ERR_INIT_LACKOFMEM -2 //��ʼ������ҵ������ṹ���ڴ�ʧ��
#define BIZ_ERR_INIT_DISPATCH_OPEN -3 //����ҵ�������߳�ʧ��
#define BIZ_ERR_EVENT_NULL -4 //�����ص��¼�Ϊ�մ���
#define BIZ_ERR_INNERMSG_FAILEDCMD -6 //�¼�����ʧ��

#define BIZ_ERR_INIT_CONFIG_INIT -50 //����ģ���ʼ��ʧ�� 
#define BIZ_ERR_CONFIG_FAILEDCMD -51 //д����ʧ��

#define BIZ_ERR_INIT_SYSCOMPLEX_INIT -100 //����ģ���ʼ��ʧ��

#define BIZ_ERR_INIT_PREVIEW_INIT -150 //����ģ���ʼ��ʧ��
#define BIZ_ERR_PREVIEW_NULLPARA -151 //����Ԥ�����󣻲�������Ϊ��
#define BIZ_ERR_PREVIEW_FAILEDCMD -152 //����Ԥ�����󣻲�������Ϊ��

#define BIZ_ERR_INIT_ENCODE_INIT -200 //����ģ���ʼ��ʧ��

#define BIZ_ERR_INIT_RECORD_INIT -250 //¼��ģ���ʼ��ʧ��
#define BIZ_ERR_RECORD_FAILEDCMD -251 //¼��д����ʧ��

#define BIZ_ERR_INIT_PLAYBACK_INIT -300 //�ط�ģ���ʼ��ʧ��
#define BIZ_ERR_PLAYBACK_FAILEDCMD -301 //�ط�д����ʧ��

#define BIZ_ERR_INIT_PTZ_INIT -350 //��̨ģ���ʼ��ʧ��
#define BIZ_ERR_PTZ_FAILEDCMD -351 //д����ʧ��

#define BIZ_ERR_INIT_ALARM_INIT -400 //����ģ���ʼ��ʧ��
#define BIZ_ERR_INIT_SNAP_INIT -401//ͼƬ�����߼���ʼ��ʧ�� 

#define BIZ_ERR_INIT_USER_INIT -450 //�û�ģ���ʼ��ʧ��
#define BIZ_ERR_USER_AUTHOR_NO -451 //�û���Ȩ��

#define BIZ_ERR_INIT_LOG_INIT -490 //��־ģ���ʼ��ʧ��


#define BIZ_ERR_PARASET_NULLPARA -500 //�������ô��󣻲�������Ϊ��
#define BIZ_ERR_PARASET_FAILEDCMD -501 //������������ʧ��

#define BIZ_ERR_PARAGET_NULLPARA -600 //������ô��󣻲�������Ϊ��
#define BIZ_ERR_PARAGET_FAILEDCMD -601 //������������ʧ��

#define BIZ_ERR_SYSCOMPLEXDTSET_NULLPARA -700
#define BIZ_ERR_SYSCOMPLEXDTGET_NULLPARA -701
#define BIZ_ERR_SYSCOMPLEXDTSTR_NULLPARA -702

#define BIZ_ERR_DEINIT_UNKNOWN -1000 //����ʼ��ʧ��,��Ϊҵ���δ����ʼ��


#define PRINT_HERE //printf(">>>>>>>>File:%s, Func:%s, Line:%d\n", __FILE__,__FUNCTION__,__LINE__);

#endif

