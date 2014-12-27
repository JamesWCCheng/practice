#include "common.h"

#include "ThreadPool.h"
#include "ITask.h"
#include "AutoLock.h"
#include <iostream>
#include <memory>
using namespace std;


VOID
CALLBACK
ThreadPool::ThreadCallback(
PTP_CALLBACK_INSTANCE Instance,
PVOID                 Context,
PTP_WORK              Work
)
{
	ThreadData* pData = (ThreadData*)Context;
	shared_ptr<ITask> pTask = pData->pThreadTask;
	ThreadPool *pThreadPool = pData->pThreadPoolThis;
	
	if (NULL != pTask)
	{
		{
			AutoLocker_CS lock(pThreadPool->m_AutoCSLock);
			pThreadPool->m_PendingTaskList.remove(pData);
			InterlockedIncrement(&pThreadPool->m_RunningThreadCount);
		}
		if (!pTask->IsCancel())
		{
			Sleep(pTask->GetDelayTime());

			if (!pTask->IsCancel())
			{
				pTask->Run();
			}
			pTask->m_IsRunning = FALSE;

		}
		delete pData;
		InterlockedDecrement(&pThreadPool->m_RunningThreadCount);
	}
	return;
}
ThreadPool::ThreadPool(DWORD min, DWORD max)
	: m_ThreadMaxCount(max), 
	m_ThreadMinCount(min), 
	m_bInited(false), 
	m_pCleanupgroup(NULL), 
	m_pThreadPool(NULL), 
	m_RunningThreadCount(0)
{
	Init();
}
ThreadPool::~ThreadPool()
{
	DeInit();
}
bool ThreadPool::AddTask(shared_ptr<ITask> task)
{
	return AddTask(task, nullptr);
}
bool ThreadPool::AddTask(shared_ptr<ITask> task, ThreadData *threadData)
{
	ThreadData *_threadData = threadData;
	AutoLocker_CS lock(m_AutoCSLock);
	if (_threadData == nullptr)
	{
		_threadData = new ThreadData{ this, task };
	}
	PTP_WORK work = CreateThreadpoolWork(ThreadPool::ThreadCallback,
		_threadData,
		&m_CallBackEnviron);

	//push into list, pop when executing.
	m_PendingTaskList.push_front(_threadData);

	if (work != NULL)
	{
		task->m_IsRunning = TRUE;
		
		SubmitThreadpoolWork(work);
		return true;
	}
	return false;
}

bool ThreadPool::AdjustCapacity(DWORD min, DWORD max)
{
	AutoLocker_CS lock(m_AutoCSLock);
	//DeInit Current
	DeInit();
	//Set Max and Min Count
	m_ThreadMaxCount = max;
		
	m_ThreadMinCount = min;

	//Init Again.
	Init();

	//Add the previous pending task to new pool.
	for (auto itr = m_PendingTaskList.cbegin(); itr != m_PendingTaskList.cend(); ++itr)
	{ 
		AddTask((*itr)->pThreadTask, *itr);
	}

	return m_bInited;

}

LONG ThreadPool::GetAvailableThreadCount()
{
	AutoLocker_CS lock(m_AutoCSLock);
	auto avalaible = (LONG)m_ThreadMaxCount - m_RunningThreadCount;
	if (avalaible < 0)
	{	
		return 0;
	}
	return avalaible;
}
void ThreadPool::Run()
{
	return;
}
void ThreadPool::Init()
{
	InitializeThreadpoolEnvironment(&m_CallBackEnviron);

	PTP_POOL pThreadPool = CreateThreadpool(NULL);
	if (NULL == pThreadPool)
	{
		cout << (("ThreadpoolTimer::Init() create thread pool fail: %d", ::GetLastError()));
		return;
	}
	SetThreadpoolThreadMaximum(pThreadPool, m_ThreadMaxCount);

	if (!SetThreadpoolThreadMinimum(pThreadPool, m_ThreadMinCount))
	{
		return;
	}
	PTP_CLEANUP_GROUP cleanupgroup = NULL;

	cleanupgroup = CreateThreadpoolCleanupGroup();

	m_pThreadPool = pThreadPool;
	m_pCleanupgroup = cleanupgroup;
	SetThreadpoolCallbackPool(&m_CallBackEnviron, pThreadPool);

	SetThreadpoolCallbackCleanupGroup(&m_CallBackEnviron,
		cleanupgroup,
		NULL);
	
	m_bInited = true;

}
void ThreadPool::ReleasePool()
{
	//Spin Lock for Finish All Task.
	while (true)
	{
		//if (m_RunningThreadCount == 0) {auto temp = m_RunningThreadCount; m_RunningThreadCount = 0; return temp}¤§Ãþªº
		if (InterlockedCompareExchange(&m_RunningThreadCount, 0, 0) == 0)
		{
			// lock acquired 
			break;
		}
	}
}
void ThreadPool::DeInit()
{
	if (NULL != m_pCleanupgroup)
	{
		CloseThreadpoolCleanupGroupMembers(m_pCleanupgroup,
			TRUE,
			NULL);

		CloseThreadpoolCleanupGroup(m_pCleanupgroup);
	}
	if (NULL != m_pThreadPool)
	{
		::CloseThreadpool(m_pThreadPool);
		m_pThreadPool = NULL;
	}

	m_ThreadCancel = true;
	m_bInited = false;
}