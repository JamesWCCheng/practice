#pragma once
#include <windows.h>
#include "AutoResource.h"

class RegKeyListenerBase
{
public:
	RegKeyListenerBase(HKEY hMainKey, LPCWSTR strTargetKey, BOOL bWatchSubtree, DWORD dwNotifyFilter);
	~RegKeyListenerBase();
private:
	RegKeyListenerBase(const RegKeyListenerBase&);
	RegKeyListenerBase& operator=(const RegKeyListenerBase&);

public:
	bool Start();
	void Stop();

private:
	// override to handle key changed
	virtual void OnKeyChanged() = 0;

private:
	bool Init();
	bool RegisterNotify();

	static DWORD WINAPI WaitForNofityThreadProc(LPVOID lpThreadParam);
	DWORD WaitForNofity();

private:
	const HKEY m_hMainKey;
	const LPCWSTR m_strTargetKey;
	const BOOL m_bWatchSubtree;
	const DWORD m_dwNotifyFilter;

	bool m_bInited;
	bool m_bStarted;
	volatile bool m_bContinue;
	Auto_CriticalSection m_acs;
	Auto_HKey m_ahTargetKey;
	Auto_Handle m_ahRegNotifyEvent;
	Auto_Handle m_ahWaitThreadCompleteEvent;
};