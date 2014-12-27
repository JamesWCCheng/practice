#include "common.h"
#include "ThreadTask.h"
#include <iostream>
using namespace std;
DWORD WINAPI ThreadTask::ThreadProc(LPVOID parameter) {

	ThreadTask *pTask = (ThreadTask *)parameter;

	if (NULL != pTask)
	{
		if (!pTask->IsCancel())
		{
			Sleep(pTask->GetDelayTime());
			pTask->Run();
			pTask->m_IsRunning = FALSE;
		}
	}
	return ERROR_SUCCESS;
}
ThreadTask::ThreadTask(DWORD delayTime)
	:ITask(delayTime), m_bInited(false), m_ThreadHandle(nullptr)
{
	Init();
}
ThreadTask::~ThreadTask()
{
	DeInit();
}
DWORD ThreadTask::Start()
{
	if (m_bInited)
	{
		m_IsRunning = TRUE;
		return ResumeThread(m_ThreadHandle);
	}
	return (DWORD)-1;
}
DWORD ThreadTask::Pause()
{
	if (m_bInited)
	{
		m_IsRunning = FALSE;
		return SuspendThread(m_ThreadHandle);
	}
	return (DWORD)-1;
}

void ThreadTask::Terminate()
{
	//Spin Lock for Finish All Task.
	while (true)
	{
		//if (m_IsRunning == FALSE) {auto temp = m_IsRunning; m_IsRunning = 100; return temp}¤§Ãþªº
		if (InterlockedCompareExchange((LONG*)&m_IsRunning, 100, FALSE) == 0)
		{
			// lock acquired 
			break;
		}
	}
}
void ThreadTask::Init()
{
	m_ThreadHandle = CreateThread(nullptr, 0, ThreadTask::ThreadProc, this, CREATE_SUSPENDED, &m_TaskID);
	m_bInited = true;
}

void ThreadTask::DeInit()
{
	if (m_ThreadHandle != nullptr)
	{
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle = nullptr;
	}
}