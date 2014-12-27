#include <windows.h>
#include "RegKeyListenerBase.h"
#include "AutoLock.h"
#include "common.h"
RegKeyListenerBase::RegKeyListenerBase(HKEY hMainKey, LPCWSTR strTargetKey, BOOL bWatchSubtree, DWORD dwNotifyFilter)
	: m_hMainKey(hMainKey)
	, m_strTargetKey(strTargetKey)
	, m_bWatchSubtree(bWatchSubtree)
	, m_dwNotifyFilter(dwNotifyFilter)
	
	, m_bInited(false)
	, m_bStarted(false)
	, m_bContinue(true)
{
	m_bInited = Init();
}

RegKeyListenerBase::~RegKeyListenerBase()
{
	Stop();
}

bool RegKeyListenerBase::Start()
{
	AutoLocker_CS locker(m_acs);

	if (!m_bInited)
		return false;

	if (m_bStarted)
		return false;

	if (!RegisterNotify())
		return false;

	Auto_Handle ahThread = ::CreateThread(NULL, 0, &RegKeyListenerBase::WaitForNofityThreadProc, this, 0, NULL);
	if (NULL == ahThread)
	{
		//LogError((L"RegKeyListenerBase::Start() create thread fail: %d", ::GetLastError()));
		return false;
	}

	m_bStarted = true;
	return true;
}

void RegKeyListenerBase::Stop()
{
	AutoLocker_CS locker(m_acs);

	if (!m_bStarted)
		return;

	m_bContinue = false;
	::SetEvent(m_ahRegNotifyEvent); // trigger event manually to stop wait thread
	DWORD dwWait = ::WaitForSingleObject(m_ahWaitThreadCompleteEvent, INFINITE); // wait for complete of wait thread
	if (WAIT_OBJECT_0 != dwWait)
	{
		//LogError((L"RegKeyListenerBase::Stop() wait for complete event fail: %d", dwWait));
	}
}

bool RegKeyListenerBase::Init()
{
	HKEY hTargetKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyExW(m_hMainKey, m_strTargetKey, 0, KEY_READ, &hTargetKey))
	{
		//LogError((L"RegKeyListenerBase::Init() open reg key fail: %d", ::GetLastError()));
		return false;
	}
	m_ahTargetKey.Hold(hTargetKey);

	HANDLE hNotify = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	if (NULL == hNotify)
	{
		//LogError((L"RegKeyListenerBase::Init() create event for notify fail: %d", ::GetLastError()));
		return false;
	}
	m_ahRegNotifyEvent.Hold(hNotify);
	
	hNotify = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	if (NULL == hNotify)
	{
		//LogError((L"RegKeyListenerBase::Init() create event for wait complete: %d", ::GetLastError()));
		return false;
	}
	m_ahWaitThreadCompleteEvent.Hold(hNotify);

	return true;
}

bool RegKeyListenerBase::RegisterNotify()
{
	LONG lError = ::RegNotifyChangeKeyValue(m_ahTargetKey, m_bWatchSubtree, m_dwNotifyFilter, m_ahRegNotifyEvent, TRUE);
	if (ERROR_SUCCESS != lError)
	{
		//LogError((L"RegKeyListenerBase::RegisterNotify() register fail: %d", lError));
		return false;
	}
	return true;
}

DWORD WINAPI RegKeyListenerBase::WaitForNofityThreadProc(LPVOID lpThreadParam)
{
	RegKeyListenerBase* pThis = (RegKeyListenerBase*)lpThreadParam;
	if (NULL == pThis)
		return 1;
	
	return pThis->WaitForNofity();
}

DWORD RegKeyListenerBase::WaitForNofity()
{
	DWORD dwRet = 0;
	while (m_bContinue)
	{
		DWORD dwWait = ::WaitForSingleObject(m_ahRegNotifyEvent, INFINITE);
		if (WAIT_OBJECT_0 != dwWait)
		{
			//LogError((L"RegKeyListenerBase::WaitForNofity() wait for notify event fail: %d", dwWait));
		}

		if (m_bContinue)
		{
			// notify key changed
			{
				AutoLocker_CS locker(m_acs);
				OnKeyChanged();
			}

			// re-register
			if (!RegisterNotify())
			{
				dwRet = 2;
				break;
			}
		}
		else
			break;
	}
	::SetEvent(m_ahWaitThreadCompleteEvent);

	return dwRet;
}