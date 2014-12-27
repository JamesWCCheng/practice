#pragma once

#include <string>

class RegUtil
{
public:
	static bool OpenKey(HKEY hMainKey, LPCWSTR subKey, PHKEY pKey);
	static bool CloseKey(HKEY hKey);

	static bool GetValue(HKEY hMainKey, LPCWSTR subKey, LPCWSTR valueName, DWORD valueType, DWORD& dataToOutput);
	static bool GetValue(HKEY hMainKey, LPCWSTR subKey, LPCWSTR valueName, DWORD valueType, std::wstring& dataToOutput);

	static bool SetValue(HKEY hKey, LPCWSTR valueName, DWORD valueType, DWORD data);
	static bool SetValue(HKEY hKey, LPCWSTR valueName, DWORD valueType, LPCWSTR data);
};