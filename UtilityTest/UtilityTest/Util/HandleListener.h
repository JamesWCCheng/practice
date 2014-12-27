#pragma once
#include <Windows.h>
#include "common.h"
template<typename T>
class HandleListener
{
public:
	HandleListener(HANDLE handle, T& callback, bool continueListening = false, DWORD millisecond = 0);
	virtual ~HandleListener();
	void Start();
	void Stop();
protected:
	T& m_CallbackFunction;
	HANDLE m_WaitHandle;
	DWORD m_TimeoutMillisecond;
private:
	void Init();
	void DeInit();
	HandleListener(const HandleListener &);
	HandleListener& operator=(const HandleListener&);

	static VOID CALLBACK WaitOrTimerCallback(
		PTP_CALLBACK_INSTANCE /*Instance*/,
		PVOID lpParameter,
		PTP_WAIT Wait,
		TP_WAIT_RESULT WaitResult
		);

	//若我會variadic template我就可以試試看用這取代上面的
	 template<class ... ARG_>
	 static	VOID CALLBACK WaitOrTimerCallbackTemp(ARG_... args)
	{
		//m_CallbackFunction(args...);

	}
private:
	PTP_WAIT m_WaitObject;
	bool m_bInited;
	bool m_ContinueListening;
	bool m_IsCancel;
};


template<typename T>
VOID CALLBACK HandleListener<T>::WaitOrTimerCallback(
	PTP_CALLBACK_INSTANCE /*Instance*/,
	PVOID lpParameter,
	PTP_WAIT Wait,
	TP_WAIT_RESULT WaitResult
	)
{

	HandleListener *pThis = (HandleListener *)lpParameter;
	if (!pThis->m_IsCancel)
	{
		switch (WaitResult)
		{
		case WAIT_OBJECT_0:
			pThis->m_CallbackFunction(false);
			break;
		case WAIT_TIMEOUT:
			pThis->m_CallbackFunction(true);
			break;
		}

		if (pThis->m_ContinueListening)
		{
			if (pThis->m_TimeoutMillisecond == 0)
			{
				SetThreadpoolWait(pThis->m_WaitObject, pThis->m_WaitHandle, nullptr);
			}
			else
			{
				FILETIME timeout = { 0 };
				ULARGE_INTEGER ulTimeout;
				// Timeout in ms, FILETIME is in 100 ns increments
				ulTimeout.QuadPart = (ULONGLONG)(-((LONGLONG)pThis->m_TimeoutMillisecond * 10000)); // relative time
				timeout.dwHighDateTime = ulTimeout.HighPart;
				timeout.dwLowDateTime = ulTimeout.LowPart;
				SetThreadpoolWait(pThis->m_WaitObject, pThis->m_WaitHandle, &timeout);

			}
		}
	}

}
template<typename T>
HandleListener<T>::HandleListener(HANDLE handle, T &callback, bool continueListening, DWORD millisecond)
	: m_WaitHandle(handle),
	m_CallbackFunction(callback),
	m_WaitObject(nullptr),
	m_bInited(false),
	m_TimeoutMillisecond(millisecond),
	m_ContinueListening(continueListening),
	m_IsCancel(false)
{
	Init();
}
template<typename T>
HandleListener<T>::~HandleListener()
{
	DeInit();
}
template<typename T>
void HandleListener<T>::Start()
{
	m_IsCancel = false;
	if (m_TimeoutMillisecond == 0)
	{
		SetThreadpoolWait(m_WaitObject, m_WaitHandle, nullptr);
	}
	else
	{
		FILETIME timeout = { 0 };
		ULARGE_INTEGER ulTimeout;
		// Timeout in ms, FILETIME is in 100 ns increments
		ulTimeout.QuadPart = (ULONGLONG)(-((LONGLONG)m_TimeoutMillisecond * 10000)); // relative time
		timeout.dwHighDateTime = ulTimeout.HighPart;
		timeout.dwLowDateTime = ulTimeout.LowPart;
		SetThreadpoolWait(m_WaitObject, m_WaitHandle, &timeout);
	}
}
template<typename T>
void HandleListener<T>::Stop()
{
	m_IsCancel = true;
}
template<typename T>
void HandleListener<T>::Init()
{
	PTP_WAIT WaitObj = CreateThreadpoolWait(WaitOrTimerCallback, this, nullptr);
	if (WaitObj == nullptr)
	{
		return;
	}
	m_WaitObject = WaitObj;

	m_bInited = true;
}
template<typename T>
void HandleListener<T>::DeInit()
{
	if (m_WaitObject != nullptr)
	{
		CloseThreadpoolWait(m_WaitObject);
		m_WaitObject = nullptr;
	}
	m_ContinueListening = false;
	m_IsCancel = true;
	m_bInited = false;
}