#pragma once
#include <windows.h>
class ThreadpoolTimer
{
public:
	ThreadpoolTimer();
	~ThreadpoolTimer();
private:
	ThreadpoolTimer(const ThreadpoolTimer&);
	ThreadpoolTimer& operator=(const ThreadpoolTimer&);

protected:
	void SetTimer(PFILETIME pfDueTime, DWORD dwPeriod);

private:
	void Init();
	void DeInit();

private:
	static void NTAPI TimerCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER timer);
	virtual void OnTimerTick() {}

private:
	bool m_bInited;

	PTP_POOL m_pThreadPool;
	PTP_TIMER m_pTimer;
};