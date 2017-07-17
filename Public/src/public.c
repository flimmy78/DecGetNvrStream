#include "public.h"
#include "diskmanage.h"

#include <sys/time.h>

typedef struct
{
    disk_manager hddmanager;
} SPublicManager;

static SPublicManager g_sPublicManager;

s32 PublicInit(void)
{
	static u8 nInitFlag = 0;
	
	if (nInitFlag)
	{
		return -1;
	}
	
	nInitFlag = 1; //�ó�ʼ��״̬Ϊ1
	
	init_disk_manager(&g_sPublicManager.hddmanager);
	
	/*
	int i = 0;
	for(;i<MAX_HDD_NUM; i++)
	{
		if(g_sPublicManager.hddmanager.hinfo[i].is_disk_exist)
		{
			printf("init_disk_manager err %d space %ld %ld\n\n", 
					rtn, 
					g_sPublicManager.hddmanager.hinfo[i].total,
					g_sPublicManager.hddmanager.hinfo[i].free);
		}
	}
	*/
	
	return 0;
}

HDDHDR PublicGetHddManage(void)
{
	return (HDDHDR)&g_sPublicManager.hddmanager;
}

u32 PublicGetTimeStamp(void)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	
	return tv.tv_sec*1000 + (tv.tv_usec)/1000;
}

/***********************************************************/
/*                              �߳���ؽӿ�                                 */

#ifndef WIN32
PUBLICTHREADHANDLE PublicCreateThread(PUBLICLINUXFUNC pvTaskEntry, 
									char* szName, 
									u8 byPriority, 
									u32 dwStacksize, 
									u32 dwParam, 
									u16 wFlag, 
									u32* pdwTaskID)
#else
PUBLICTHREADHANDLE PublicCreateThread(void* pvTaskEntry, 
									char* szName, 
									u8 byPriority, 
									u32 dwStacksize, 
									u32 dwParam, 
									u16 wFlag, 
									u32* pdwTaskID)
#endif
{
	PUBLICTHREADHANDLE  hTask;
	
#ifdef WIN32
	u32 dwTaskID;
	int Priority;

	if(szName == NULL)
	{
		//nothing to do
		//���ڱ��⾯����Ϣ
	} 
	
	wFlag = 0;

	if(byPriority < 50)
	{
		Priority = THREAD_PRIORITY_TIME_CRITICAL;
	}
	else if(byPriority < 100)
	{
		Priority = THREAD_PRIORITY_HIGHEST;
	}
	else if(byPriority < 120)
	{
		Priority = THREAD_PRIORITY_ABOVE_NORMAL;
	}
	else if(byPriority < 150)
	{
		Priority = THREAD_PRIORITY_NORMAL;
	}
	else if(byPriority < 200)
	{
		Priority = THREAD_PRIORITY_BELOW_NORMAL;		
	}	
	else
	{
		Priority = THREAD_PRIORITY_LOWEST;
	}
	
    hTask = CreateThread(NULL, 
							dwStacksize, 
							(LPTHREAD_START_ROUTINE)pvTaskEntry, 
							(char*)dwParam, 
							0, 
							&dwTaskID);
	
	if(hTask != NULL)
    {
		SetThreadAffinityMask(hTask, THREADAFFMASK); // �����̵߳Ĵ�������������

		if(SetThreadPriority(hTask, Priority) != 0)  
		{
			if(pdwTaskID != NULL) 
			{
				*pdwTaskID = dwTaskID;
			}
			return hTask;
		}
    }
#else  //LINUX
	
	int nRet = 0;
	struct sched_param tSchParam;	
	pthread_attr_t tThreadAttr;
	int nSchPolicy;

	pthread_attr_init(&tThreadAttr);

	// ���õ��Ȳ���
	pthread_attr_getschedpolicy(&tThreadAttr, &nSchPolicy);
	nSchPolicy = SCHED_FIFO;
	pthread_attr_setschedpolicy(&tThreadAttr, nSchPolicy);

	// �������ȼ�
	pthread_attr_getschedparam(&tThreadAttr, &tSchParam);
	byPriority = 255-byPriority;
	if(byPriority < 60)
	{
		byPriority = 60;
	}
	tSchParam.sched_priority = byPriority;
	
	pthread_attr_setschedparam(&tThreadAttr, &tSchParam);
	pthread_attr_setstacksize(&tThreadAttr, (size_t)dwStacksize);

	nRet = pthread_create(&hTask, &tThreadAttr, pvTaskEntry, (void*)dwParam);
	if(nRet == 0)
	{
		if(pdwTaskID != NULL)
		{
			*pdwTaskID = (u32)hTask;
		}
		return hTask;
	}
#endif

    return 0;
}

s32 PublicExitThread()
{
#ifdef WIN32
	ExitThread(0);
	return TRUE;
#else
	static int nRetCode = 0;
	pthread_exit(&nRetCode);
	return TRUE;
#endif
}

s32 PublicTerminateThread(PUBLICTHREADHANDLE hTask)
{
#ifdef WIN32
	return TerminateThread(hTask, 0);
#else
	void* temp;
	pthread_cancel(hTask);
	return ( 0 == pthread_join(hTask, &temp) );
#endif
}

