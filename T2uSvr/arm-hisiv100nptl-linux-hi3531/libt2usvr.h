/***************************************************************************
 *   Copyright (C) 2013 by Yexr   *
 *   yexr@vveye.com   *
 ***************************************************************************/
#ifndef _LIB_T2U_SVR_H_
#define _LIB_T2U_SVR_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
��ʼ��������P2P����
svraddr		��������ַ������
svrport		�������˿�
svrkey		��������Կ
uuid			����UUID
threadnum	�߳���

����ֵ:
0			�ɹ�
-1			ʧ��
*/
int	t2u_svr_init(const char* svraddr,unsigned short svrport,const char* svrkey,const char* uuid,int threadnum);

/*
�����Զ��帽�Ӳ���

param		�Զ������
len			�������ȣ���󲻳���1024�ֽ�

����ֵ:
0:			�ɹ�
-1:			ʧ��

*/
int	t2u_svr_set_param(const char* param,int len);

/*
�ر�P2P����
*/
void	t2u_svr_exit();

/*
��ѯ״̬

����ֵ:
1:		��ע�ᵽ��������״̬����
0:		��δע�ᵽ������
-1:		SDKδ��ʼ��
-2:		��������Կ��Ч
-3:		��UUID������ע��
*/
int	t2u_svr_status();

/*
��ѯP2P����

outbuff			���ս���Ļ����ַ
				(�������Ϊ�ַ�����ÿ��һ�����ӣ���ʽΪ: ip:port|������|�������д���)
buffsize			���ս���Ļ����С


����ֵ:

int	��ǰP2P������

*/
int	t2u_svr_conn_stats(char* outbuff,int buffsize);

#ifdef __cplusplus
}
#endif

#endif


