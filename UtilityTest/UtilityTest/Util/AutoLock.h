#pragma once
#include <Windows.h>
struct AutoLocker_CS
{
	AutoLocker_CS(CRITICAL_SECTION& cs)
		: pCS(&cs)
	{
		::EnterCriticalSection(pCS);
	}

	~AutoLocker_CS()
	{
		::LeaveCriticalSection(pCS);
	}

	CRITICAL_SECTION* pCS;
};

struct AutoLocker_Event
{
	AutoLocker_Event(HANDLE hEvent, DWORD timeOut = INFINITE)
		: m_hEvent(hEvent)
	{
		::WaitForSingleObject(m_hEvent, timeOut);
	}

	~AutoLocker_Event()
	{
		::SetEvent(m_hEvent);
	}

	HANDLE m_hEvent;
	DWORD m_dwTimeOut;
};