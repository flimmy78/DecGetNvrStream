#ifndef _THREAD_H_
#define _THREAD_H_

#include "common_basetypes.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup Thread API 
/// �߳�API����װ�˲�ͬ����ϵͳ���̶߳���
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           DvrThreadCreate
///                |------------+
///        DvrThreadShareSocket    |
///           DvrThreadGetID       |
///                |------------+
///           DvrThreadExit
///          DvrThreadDestory
///                |
///    ===========================
/// \endcode
/// @{


/// �����̡߳�
/// 
/// \param [out] phThread �̴߳����ɹ�ʱ���������߳̾����
/// \param [in] pStartAddress �̺߳����ĵ�ַ��
/// \param [in] pParameter �̺߳����Ĳ�����
/// \param [in] nPriority �߳����ȼ���ȡֵ0-127��ֵԽС��ʾ���ȼ�Խ�ߣ��ᱻת��
///        �ɶ�Ӧ����ϵͳ�����ȼ���
/// \param [in] dwStatckSize Ϊ�߳�ָ���Ķ�ջ��С���������0����С�ڱ����ֵ��
///        ��ʹ��ȱʡֵ��
/// \param [in] pName �߳������ַ�����
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrThreadCreate(HANDLE* phThread,
				 void(* pStartAddress)(void* p),
				 void* pParameter,
				 s32 nPriority,
				 u32 dwStatckSize,
				 const char* pName);


/// �����̡߳���ָ�����߳��˳�ǰ�����������̡߳�
/// 
/// \param [in] hThread ָ���������̵߳ľ����
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
s32 DvrThreadDestory(HANDLE* hThread);


/*/// ����̵߳Ĺ����׽��֡�ĳЩ����ϵͳ��ͬ���̲߳���ʹ��ͬһ���׽��֣���Ҫ����ת����
/// �������ʹ��ͬһ���׽��֣���ֱ�ӷ��ش������������
/// 
/// \param [in] hThread ָ��Ҫʹ���׽��ֵ��߳̾����
/// \param [in] nSocket �������߳��д������׽��ֵ���������
/// \return �������׽���������
S32 DvrThreadShareSocket(HANDLE hThread, S32 nSocket);*/


/// �˳������̡߳�
void DvrThreadExit(void);


/*/// ���ص����̵߳�ID��
/// 
/// \return �߳�ID
S32 DvrThreadGetID(void);*/

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //_THREAD_H_
