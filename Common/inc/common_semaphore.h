#ifndef _SEMAPHORE_API_H_
#define _SEMAPHORE_API_H_

#include "common_basetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup SemaphoreAPI API Semaphore
/// �ź���API����װ�˲�ͬ����ϵͳ���ź�������
///	\n ��������ͼ:
/// \code
///    ================================
///                |
///         DvrSemCreate
///                |------------------+
///    	DvrSemTake,DvrSemGive   		    |
///                |------------------+
///        DvrSemDestory
///                |
///    ================================
/// \endcode
/// @{


/// �����ź�����
/// 
/// \param [out] phSemaphore �ź��������ɹ�ʱ���������ź��������
/// \param [in] dwInitialCount �ź��������ĳ�ʼֵ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrSemCreate(HANDLE *phSem, u32 dwInitialCount);


/// �����ź�����
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
s32 DvrSemDestory(HANDLE hSemaphore);


/// �����ź���������
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrSemTake(HANDLE hSemaphore);


/// �����ź���������
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrSemGive(HANDLE hSemaphore);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__SEMAPHORE_API_H__

