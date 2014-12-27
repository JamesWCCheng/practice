#pragma once
#include "ITask.h"
#include <Windows.h>
#include <memory>
class ThreadTask : public ITask
{
public:
	virtual ~ThreadTask();
	DWORD Start();
	DWORD Pause();
	void Terminate();
	inline HANDLE GetHandle()const { return m_ThreadHandle; }

protected:
	ThreadTask(DWORD delayTime = 0);
private:
	void Init();
	void DeInit();
	ThreadTask(const ThreadTask &);
	ThreadTask& operator=(const ThreadTask&);
	static DWORD WINAPI ThreadProc(LPVOID parameter);
private:
	HANDLE m_ThreadHandle;
	bool m_bInited;


};