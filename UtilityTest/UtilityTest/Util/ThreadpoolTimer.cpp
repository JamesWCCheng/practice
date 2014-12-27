#include "common.h"

#include "ThreadpoolTimer.h"
#include <iostream>
using namespace std;
//Win32 Timer 
ThreadpoolTimer::ThreadpoolTimer()
	: m_bInited(false)
	, m_pThreadPool(NULL)
	, m_pTimer(NULL)
{
	Init();
}

ThreadpoolTimer::~ThreadpoolTimer()
{
	DeInit();
}


void ThreadpoolTimer::SetTimer(PFILETIME pfDueTime, DWORD dwPeriod)
{
	if (!m_bInited)
		return;
	//If this parameter is zero, the timer is signaled once.If this parameter is greater than zero, the timer is periodic.
	::SetThreadpoolTimer(m_pTimer, pfDueTime, dwPeriod, 0);
}

void ThreadpoolTimer::Init()
{
	TP_CALLBACK_ENVIRON callbackEnviron;
	InitializeThreadpoolEnvironment(&callbackEnviron);

	PTP_POOL pThreadPool = ::CreateThreadpool(NULL);
	if (NULL == pThreadPool)
	{
		cout << (("ThreadpoolTimer::Init() create thread pool fail: %d", ::GetLastError()));
		return;
	}
	m_pThreadPool = pThreadPool;

	::SetThreadpoolThreadMaximum(m_pThreadPool, 1);
	if (!::SetThreadpoolThreadMinimum(m_pThreadPool, 1))
	{
		cout << (("ThreadpoolTimer::Init() set thread minimum fail: %d", ::GetLastError()));
		return;
	}

	::SetThreadpoolCallbackPool(&callbackEnviron, m_pThreadPool);

	PTP_TIMER pTimer = ::CreateThreadpoolTimer(&ThreadpoolTimer::TimerCallback, this, &callbackEnviron);
	if (NULL == pTimer)
	{
		cout << (("ThreadpoolTimer::Init() create thread pool timer fail: %d", ::GetLastError()));
		return;
	}
	m_pTimer = pTimer;

	m_bInited = true;
}

void ThreadpoolTimer::DeInit()
{
	if (NULL != m_pTimer)
	{
		::CloseThreadpoolTimer(m_pTimer);
		m_pTimer = NULL;
	}

	if (NULL != m_pThreadPool)
	{
		::CloseThreadpool(m_pThreadPool);
		m_pThreadPool = NULL;
	}
	m_bInited = false;
}

void NTAPI ThreadpoolTimer::TimerCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER timer)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(timer);

	ThreadpoolTimer* pThis = (ThreadpoolTimer*)context;
	if (NULL != pThis)
		pThis->OnTimerTick();
}