/*
 * @file File: FileMgr.h
 * @brief Registry manipulation.
 */
#ifndef __FILEMGR__
#define __FILEMGR__
#include <windows.h>
/**
* @brief Retrive size of specific file
*
* @param pwszPath [in] : A pointer to a buffer that specific the target file.
* @param nLen [out] : A pointer to the output buffer that is to receive the data returned by the operation.
* @return TRUE : success;  FALSE for failure.
*/
BOOL GetFileSize(LPCTSTR pwszPath, int &nLen);

/**
* @brief Read data from specific file
*
* @param pwszPath [in] : A pointer to a buffer that specific the target file.
* @param pBuf [out] : A pointer to the output buffer that is to receive the data returned by the operation.
* @param nLen [in] : Specifies the size of the buffer pointed to by the pBuf parameter, in bytes.
* @return TRUE : success;  FALSE for failure.
*/
BOOL GetFileContent(LPCTSTR pwszPath, BYTE* pBuf, int nLen);

/**
* @brief Write date to specific file
*
* @param pwszPath [in] : A pointer to a buffer that specific the target file.
* @param pBuf [out] : A pointer to the output buffer that is to receive the data returned by the operation.
* @param nLen [in] : Specifies the size of the buffer pointed to by the pBuf parameter, in bytes.
* @return TRUE : success;  FALSE for failure.
*/
BOOL SetFileContent(LPCTSTR pwszPath, BYTE* pBuf, int nLen);

/**
* @brief Get folder size
*
* @param pwszPath [in] : A pointer to a buffer that specific the target folder.
* @param lFileCount [out] : file count in folder.
* @return __int64 : folder size
*/
__int64 GetFolderTotalSize(LPCTSTR pwszPath, int* lFileCount);

/**
* @brief Check file exist
*
* @param pwszPath [in] : A pointer to a buffer that specific the target file.
* @return TRUE : file exist
*/
bool IsFileExist(LPCTSTR pwszFilePath);

wchar_t* GetFileExtension(LPCTSTR pwszFileName);

__int64 GetTypeFileSize(int Type, LPCTSTR pwszPath, int* lFileCount);

#endif
