#pragma once
#include "ITask.h"
#include "AutoResource.h"
#include <Windows.h>
#include <list>
#include <memory>
class ThreadPool: public ITask
{
	struct ThreadData
	{
		ThreadPool* pThreadPoolThis;
		std::shared_ptr<ITask> pThreadTask;
	};
public:
	ThreadPool(DWORD min, DWORD max);
	~ThreadPool();

	bool AddTask(std::shared_ptr<ITask> task);
	bool AdjustCapacity(DWORD min, DWORD max);
	void ReleasePool();
	LONG GetAvailableThreadCount();

protected:
	DWORD m_ThreadMaxCount;
	DWORD m_ThreadMinCount;
	virtual void Run() override;

	
private:
	bool AddTask(std::shared_ptr<ITask> task, ThreadData* threadData);
	void Init();
	void DeInit();
	static VOID CALLBACK
		ThreadCallback(
		PTP_CALLBACK_INSTANCE Instance,
		PVOID                 Parameter,
		PTP_WORK              Work);
	ThreadPool(const ThreadPool&);
	ThreadPool& operator=(const ThreadPool&);

private:
	volatile LONG m_RunningThreadCount;
	std::list<ThreadData*> m_PendingTaskList;
	bool m_bInited;
	TP_CALLBACK_ENVIRON m_CallBackEnviron;
	Auto_CriticalSection m_AutoCSLock;
	PTP_POOL m_pThreadPool;
	PTP_CLEANUP_GROUP m_pCleanupgroup;
};