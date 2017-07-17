#ifndef _MUTEX_API_H_
#define _MUTEX_API_H_

#include "common_basetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup MutexAPI API Mutex
/// ������API����װ�˲�ͬ����ϵͳ�Ļ���������
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           DvrMutexCreate
///                |------------+
///      DvrMutexLock,DvrMutexUnlock|
///                |------------+
///           DvrMutexDestory
///                |
///    ===========================
/// \endcode
/// @{

/*/// ����������
enum mutex_type
{
	/// ���ٻ�����������ٽ����Ѿ���һ���̻߳�ȡ�������̻߳��߸��߳��ٴλ�ȡʱ��
	/// �����߳�ֱ�ӱ�����
	MUTEX_FAST = 0,

	/// �ݹ黥�������Ѿ���ȡ�ٽ������߳̿��Զ�λ�ȡ�������������ڲ�Ӧ�л�ȡ����
	/// ���������Ҫ��ȫ�ͷ��ٽ�����һ��Ҫ��֤��ȡ�������ͷŴ���һ�¡�
	MUTEX_RECURSIVE,
};*/


/// ������������
/// 
/// \param [out] phMutex �����������ɹ�ʱ�������Ļ����������
/// \param [in] nType �����������ͣ�ȡmutex_typeö������ֵ
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrMutexCreate(HANDLE* phMutex);


/// ���ٻ�������
/// 
/// \param [in] hMutex �����������
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
s32 DvrMutexDestory(HANDLE* hMutex);


/// �����ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrMutexLock(HANDLE* hMutex);


/// �뿪�ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
s32 DvrMutexUnlock(HANDLE* hMutex);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__MUTEX_API_H__

