

#ifndef __MUTEX_API_H__
#define __MUTEX_API_H__

#include "DVRDEF.H"

#ifdef WIN32

#include <stddef.h>
/*----------------------------------------------------------------------*\
* Exported data structures are completely opaque.
* To determine the size the structures, use the given sizeof functions.
\*----------------------------------------------------------------------*/
typedef void pthread_mutex_t;
typedef void pthread_mutexattr_t;
size_t pthread_mutex_sizeof(void);

typedef void pthread_cond_t;
typedef void pthread_condattr_t;
size_t pthread_cond_sizeof(void);

typedef unsigned long pthread_t;
typedef void pthread_attr_t;

typedef void *(pthread_start_t)(void *arg);

/*----------------------------------------------------------------------*\
* Implemented API functions.
\*----------------------------------------------------------------------*/
int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_create(pthread_t *id, pthread_attr_t *attr, 
				   pthread_start_t *start, void *arg);
pthread_t pthread_self(void);
int pthread_equal(pthread_t t1, pthread_t t2);

/*----------------------------------------------------------------------*\
* They are just present to avoid compile errors and memory leaks
* in luathreads under Win32.
\*----------------------------------------------------------------------*/
int pthread_detach(pthread_t th);
int pthread_cleanup(pthread_t th);

/*======================================================================*\
* Pthreads stuff.
\*======================================================================*/
#else 

struct timespec;

#include <pthread.h>

#define pthread_mutex_sizeof() (sizeof(pthread_mutex_t))
#define pthread_cond_sizeof() (sizeof(pthread_cond_t))
int pthread_cleanup(pthread_t th);

#endif // #ifndef LINUX


#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup MutexAPI API Mutex
/// ������API����װ�˲�ͬ����ϵͳ�Ļ���������
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           MutexCreate
///                |------------+
///      MutexEnter,MutexLeave  |
///                |------------+
///           MutexDestory
///                |
///    ===========================
/// \endcode
/// @{

/// ����������
enum mutex_type
{
	/// ���ٻ�����������ٽ����Ѿ���һ���̻߳�ȡ�������̻߳��߸��߳��ٴλ�ȡʱ��
	/// �����߳�ֱ�ӱ�����
	MUTEX_FAST = 0,

	/// �ݹ黥�������Ѿ���ȡ�ٽ������߳̿��Զ�λ�ȡ�������������ڲ�Ӧ�л�ȡ����
	/// ���������Ҫ��ȫ�ͷ��ٽ�����һ��Ҫ��֤��ȡ�������ͷŴ���һ�¡�
	MUTEX_RECURSIVE,
};


/// ������������
/// 
/// \param [out] phMutex �����������ɹ�ʱ�������Ļ����������
/// \param [in] nType �����������ͣ�ȡmutex_typeö������ֵ
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexCreate(VD_HANDLE *phMutex, int nType);


/// ���ٻ�������
/// 
/// \param [in] hMutex �����������
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
int MutexDestory(VD_HANDLE hMutex);


/// �����ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexEnter(VD_HANDLE hMutex);


/// �뿪�ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexLeave(VD_HANDLE hMutex);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__MUTEX_API_H__

