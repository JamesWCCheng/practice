#include "FileMgr.h"
#include "common.h"
#include <iostream>
using namespace std;
BOOL GetFileSize(LPCTSTR pwszPath, int &nLen)
{
	BOOL bRet = FALSE;
	DWORD dwFileSize = 0;
	DWORD dwFileAttribute = GetFileAttributes(pwszPath);

	HANDLE hFile = CreateFile(pwszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, dwFileAttribute, NULL);
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		dwFileSize = GetFileSize(hFile, NULL);
		if (INVALID_FILE_SIZE == dwFileSize)
		{
			DWORD dwError = GetLastError();
			wcout << (L"[GetFileSize] Get %s Size Error, error code = %d\r\n", pwszPath, dwError);
		}
		else
		{
			nLen = dwFileSize;
			wcout << (L"[GetFileSize] %s Size is: %d\r\n", pwszPath, dwFileSize);
			bRet = TRUE;
		}
	}
	else
	{
		DWORD dwError = GetLastError();
		wcout << (L"[GetFileSize] File %s Not Exist, error code = %d\r\n", pwszPath, dwError);
	}
	CloseHandle(hFile);
	return bRet;
}

BOOL GetFileContent(LPCTSTR pwszPath, BYTE* pBuf, int nLen)
{
	BOOL bRet = FALSE;
	DWORD dwFileAttribute = GetFileAttributes(pwszPath);
	HANDLE hFile = CreateFile(pwszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, dwFileAttribute, NULL);
		
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);

		DWORD dwNumberOfBytesRead = 0; //How many Byte read actually.
		if (INVALID_FILE_SIZE == dwFileSize)
		{
		   DWORD dwError = GetLastError();
		   wcout << (L"[GetFileContent] Get File Size Error, error code = %d\r\n", dwError);
		} 
		else
		{
			if (pBuf)
			{
				//Check buffer size >= dwFileSize
				if (dwFileSize <= (DWORD)nLen)
				{
					BOOL isReadFileSuccess = ReadFile(hFile, pBuf, dwFileSize, &dwNumberOfBytesRead, NULL);
					if (isReadFileSuccess)
					{
						bRet = TRUE;
					}
					else
					{
						DWORD dwError = GetLastError();
						wcout << L"[GetFileContent] Read %s Fail, error code = %d\r\n", pwszPath, dwError;
					}				
				}			
				else
				{
					wcout << (L"[GetFileContent] Buffer is smaller than the file size");
				}
			}
			else
			{
				wcout << (L"[GetFileContent] Buffer is null\r\n");
			}
		}
	}	
	else
	{
		DWORD dwError = GetLastError();
		wcout << (L"[GetFileContent] Create File Fail error code = %d\r\n", dwError);
	}
	CloseHandle(hFile);
	return bRet;
}

BOOL SetFileContent(LPCTSTR pwszPath, BYTE* pBuf, int nLen)
{
	BOOL bRet = FALSE;
	DWORD dwError = 0;
	DWORD dwNumberOfBytesWrite = 0; //How many Byte Write actually.

	// Write
	HANDLE hFile = 0;
	hFile = CreateFile(pwszPath, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
	if (hFile != INVALID_HANDLE_VALUE)
	{			
		//Check the buf if it is not null
		if (pBuf)
		{
			BOOL isWriteFileSuccess = 
				WriteFile(hFile, pBuf, nLen, &dwNumberOfBytesWrite, NULL);
			if (isWriteFileSuccess)
			{
				wcout << (L"[SetFileContent] Write File Success, Written Byte = %d\r\n", dwNumberOfBytesWrite);
			}
			else
			{
				dwError = GetLastError();
				wcout << (L"[SetFileContent] Write File Fail, error code = %d\r\n", dwError);
			}
		}
		else
		{
			wcout << (L"[SetFileContent] Buffer is null\r\n");
		}
		
	}
	else
	{
		DWORD dwError = GetLastError();
		wcout << (L"[SetFileContent] Create File Fail error code = %d\r\n", dwError);
	}
	CloseHandle(hFile);
	return bRet;
}

__int64 GetFolderTotalSize(LPCTSTR pwszPath, int* lFileCount)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LARGE_INTEGER filesize;
	int _lFileCount = 0;
	__int64 _lFileSize = 0;
	wchar_t _FolderContentFormat[] = L"%s%s%s";
	wchar_t FolderContent[1024] = {0};
	swprintf_s(FolderContent,_FolderContentFormat, pwszPath, L"\\", L"*");
	wcout << (L"[GetFolderTotalSize] Folder : %s\r\n", FolderContent);
	hFind = FindFirstFile(FolderContent, &ffd);	
	if (INVALID_HANDLE_VALUE != hFind) 
	{
		do
		{
			  if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			  {
				  //Ignore filename like "." or ".."				  
				  if(L'.' == ffd.cFileName[0])
					  continue;				  
				  int _SubfolderFileCount = 0;
				  __int64 _SubfolderFileSize = 0;
				  //Get subfolder path
				  wchar_t SubFolderContent[1024] = {0};
				  swprintf_s(SubFolderContent,_FolderContentFormat, pwszPath, L"\\", ffd.cFileName);				  
				  //Get subfolder size and count
				  _SubfolderFileSize = GetFolderTotalSize(SubFolderContent, &_SubfolderFileCount);
				  _lFileSize += _SubfolderFileSize;
				  _lFileCount += _SubfolderFileCount;
			  }
			  else
			  {
				 filesize.LowPart = ffd.nFileSizeLow;
				 filesize.HighPart = ffd.nFileSizeHigh;

				 _lFileSize += filesize.QuadPart;
				 _lFileCount ++;
			  }
		}
		while (FindNextFile(hFind, &ffd) != 0);
		if(hFind!= INVALID_HANDLE_VALUE)
			FindClose(hFind);
		*lFileCount = _lFileCount;
		wcout << (L"[GetFolderTotalSize]  Total file size: %ld, File count : %ld\n", _lFileSize, _lFileCount);
		return _lFileSize;
	}
	return 0;
}

bool IsFileExist(LPCTSTR pwszFilePath)
{
	(L"[IsFileExist] ++\r\n");
	WIN32_FIND_DATA fileData = {0};
	bool bResult = false;
	HANDLE handle = FindFirstFile(pwszFilePath,&fileData);
	if(handle == INVALID_HANDLE_VALUE)
	{
		bResult = false;
		wcout << (L"[IsFileExist]  FindFirstFile :%x \n", GetLastError());
	}
	else
	{
		FindClose(handle);
		bResult = true;	
	}
	return bResult;
}

wchar_t* GetFileExtension(LPCTSTR pwszFileName)
{
	wcout << (L"[GetFileExtension] ++\n");
	DWORD dwStrLen = wcsnlen_s(pwszFileName, 512);
	wcout << (L"[GetFileExtension] len: %d\n", dwStrLen);
	wchar_t* pwszFileExt = NULL;
	for(int i=dwStrLen-1;i>=0;i--)
	{		
		if(L'.' == pwszFileName[i])
		{
			wcout << (L"[GetFileExtension] match\n");
			pwszFileExt = new wchar_t[dwStrLen-i+1];
			memset(pwszFileExt, 0, (dwStrLen-i+1)*sizeof(wchar_t));
			memcpy(pwszFileExt, pwszFileName+i+1, dwStrLen-i+1);
			_wcsupr_s(pwszFileExt, dwStrLen - i + 1);
			return pwszFileExt;
		}
	}
	wcout << (L"[GetFileExtension] --\n");
	return pwszFileExt;
}

__int64 GetTypeFileSize(int Type, LPCTSTR pwszPath, int* lFileCount)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LARGE_INTEGER filesize;	
	int _lFileCount = 0;
	__int64 _lFileSize = 0;
	wchar_t _FolderContentFormat[] = L"%s%s%s";		
	wchar_t FolderContent[1024] = {0};
	wcout << (L"[SVC_GetSpecifiedTypeFileSize] Folder : %s\r\n", pwszPath);
	swprintf_s(FolderContent,_FolderContentFormat, pwszPath, L"\\", L"*");	
	hFind = FindFirstFile(FolderContent, &ffd);	
	if (INVALID_HANDLE_VALUE != hFind) 
	{
		do
		{
			  if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			  {
				  //Ignore filename like "." or ".."				  
				  if(L'.' == ffd.cFileName[0])
					  continue;				  
				  int _SubfolderFileCount = 0;
				  __int64 _SubfolderFileSize = 0;
				  //Get subfolder path
				  wchar_t SubFolderContent[1024] = {0};
				  swprintf_s(SubFolderContent,_FolderContentFormat, pwszPath, L"\\", ffd.cFileName);
				  wcout << (L"[SVC_GetSpecifiedTypeFileSize] sub Folder : %s\r\n", SubFolderContent);
				  //Get subfolder size and count
				  _SubfolderFileSize = GetTypeFileSize(Type, SubFolderContent, &_SubfolderFileCount);
				  _lFileSize += _SubfolderFileSize;
				  _lFileCount += _SubfolderFileCount;
			  }
			  else
			  {
				  wchar_t* pwszFileExt = NULL;
				  switch (Type)
				  {
				  case 1:
					  pwszFileExt = GetFileExtension(ffd.cFileName);
					  wcout << (L"[SVC_GetSpecifiedTypeFileSize] photo type, file ext : %s\r\n", pwszFileExt);
					  if(wcscmp(pwszFileExt, L"JPG") == 0 || wcscmp(pwszFileExt, L"PNG")==0)
					  {
						  wcout << (L"[SVC_GetSpecifiedTypeFileSize] Match photo format\r\n");
						  filesize.LowPart = ffd.nFileSizeLow;
						  filesize.HighPart = ffd.nFileSizeHigh;

						  _lFileSize += filesize.QuadPart;
						  _lFileCount ++;
					  }					  
					  break;
				  default:
					  break;
				  }
				  if(NULL!=pwszFileExt)
				  {
					  delete[] pwszFileExt;
					  pwszFileExt = NULL;
				  }				
			  }
		}
		while (FindNextFile(hFind, &ffd) != 0);
		wcout << (L"[SVC_GetSpecifiedTypeFileSize] No file\r\n");
		if(hFind!= INVALID_HANDLE_VALUE)
			FindClose(hFind);
		wcout << (L"[SVC_GetSpecifiedTypeFileSize] Close file\r\n");
		*lFileCount = _lFileCount;		
		return _lFileSize;
	}
	return 0;
}