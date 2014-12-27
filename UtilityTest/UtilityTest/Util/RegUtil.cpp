#include "common.h"
#include <windows.h>
#include <winreg.h>
#include "RegUtil.h"

#include <iostream>
using namespace std;
bool RegUtil::OpenKey(HKEY hMainKey, LPCWSTR subKey, PHKEY pKey)
{
	HKEY hKey = NULL;
	LSTATUS lError = ::RegOpenKeyExW(hMainKey, subKey, 0, KEY_ALL_ACCESS, &hKey);
	if (ERROR_SUCCESS != lError)
	{
		cout<<(("RegUtil::OpenKey(0x%08x, %s) fail: %d", hMainKey, subKey, lError));
		return false;
	}

	*pKey = hKey;
	return true;
}

bool RegUtil::CloseKey(HKEY hKey)
{
	return ERROR_SUCCESS == ::RegCloseKey(hKey);
}

bool RegUtil::GetValue(HKEY hMainKey, LPCWSTR subKey, LPCWSTR valueName, DWORD valueType, DWORD& dataToOutput)
{
	DWORD data = 0;
	DWORD dataSize = sizeof(data);
	LSTATUS lError = ::RegGetValueW(hMainKey, subKey, valueName, valueType, NULL, &data, &dataSize);
	if (ERROR_SUCCESS != lError)
	{
		cout << (("RegUtil::GetValue(%s) fail: %d", valueName, lError));
		dataToOutput = 0;
		return false;
	}
	dataToOutput = data;
	return true;
}

bool RegUtil::GetValue(HKEY hMainKey, LPCWSTR subKey, LPCWSTR valueName, DWORD valueType, std::wstring& dataToOutput)
{
	WCHAR data[MAX_PATH] = {0};
	DWORD dataSize = sizeof(data);
	LSTATUS lError = ::RegGetValueW(hMainKey, subKey, valueName, valueType, NULL, &data, &dataSize);
	if (ERROR_SUCCESS != lError)
	{
		cout << (("RegUtil::GetValue(%s) fail: %d", valueName, lError));
		dataToOutput = L"";
		return false;
	}
	dataToOutput = data;
	return true;
}

bool RegUtil::SetValue(HKEY hKey, LPCWSTR valueName, DWORD valueType, DWORD data)
{
	LSTATUS lError = ::RegSetValueExW(hKey, valueName, 0, valueType, (const BYTE*)(&data), sizeof(data));
	if (ERROR_SUCCESS != lError)
	{
		cout << (("RegUtil::SetValue(%s, %d) fail: %d", valueName, data, lError));
		return false;
	}
	return true;
}

bool RegUtil::SetValue(HKEY hKey, LPCWSTR valueName, DWORD valueType, LPCWSTR data)
{
	DWORD dwDataSize = sizeof(WCHAR) * (wcslen(data) + 1);
	LSTATUS lError = ::RegSetValueExW(hKey, valueName, 0, valueType, (const BYTE*)(data), dwDataSize);
	if (ERROR_SUCCESS != lError)
	{
		cout<<(("RegUtil::SetValue(%s, %s) fail: %d", valueName, data, lError));
		return false;
	}
	return true;
}