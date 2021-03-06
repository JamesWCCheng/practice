#include "KPEFile.h"
#include "common.h"
#include <iostream>
using namespace std;

KPEFile::KPEFile()
	:KPEFile(GetModuleHandle(NULL)) //C++ 11 Delegating Constructor
{

}
// 取得 DOS Header 與 NTHeader
KPEFile::KPEFile(HMODULE hModule) {
	pModule = (const char *)hModule;

	if (IsBadReadPtr(pModule, sizeof(IMAGE_DOS_HEADER))) {
		pDOSHeader = NULL;
		pNTHeader = NULL;
	}
	else {
		pDOSHeader = (PIMAGE_DOS_HEADER)pModule;
		if (IsBadReadPtr(RVA2Ptr(pDOSHeader->e_lfanew), sizeof(IMAGE_NT_HEADERS)))
			pNTHeader = NULL;
		else
			pNTHeader = (PIMAGE_NT_HEADERS)RVA2Ptr(pDOSHeader->e_lfanew);
	}
}

// 傳回目前執行檔中 指定 Directory Table 的 位址
const void * KPEFile::GetDirectory(int id)
{
	return RVA2Ptr(pNTHeader->OptionalHeader.DataDirectory[id].VirtualAddress);
}


// 傳回指定 import DLL module 的 結構資訊描述子
// input: DLL name
// output: 資訊描述子
PIMAGE_IMPORT_DESCRIPTOR KPEFile::GetImportDescriptor(LPCSTR pDllName){

	// Step 1: 取出目前執行檔中 IMAGE_DIRECTORY_ENTRY_IMPORT directory 的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)
		GetDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);

	if (pImport == NULL)
		return NULL;

	// Step 2: 取出每個 Import table 中的 Name 一個一個比較
	while (pImport->FirstThunk){                              // 若有 map 則 FirstThunk 會有 module 位址
		if (_stricmp(pDllName, RVA2Ptr(pImport->Name)) == 0)       // 因為 import 中的 Name 用 RVA 存放,所以要先轉換才能比較
			return pImport;
		pImport++;                                               // 不對,移到下一個比較
	}
	return NULL;
}


// 傳回指定 imported function 的位址 ( __imp__xxx variable )
// input: (1) 輸入你的 function 所在的 DLL module 描述子,  (2) function Name
// output: function 的位址
const unsigned * KPEFile::GetFunctionPtr(PIMAGE_IMPORT_DESCRIPTOR pImport, LPCSTR pProcName)
{
	// Step 1: 由 Import DLL module 描述子中取得 PIMAGE_THUNK_DATA 結構
	//         這個結構存放了 function 的位址
	PIMAGE_THUNK_DATA pThunk;
	pThunk = (PIMAGE_THUNK_DATA)RVA2Ptr(pImport->OriginalFirstThunk);

	// Step 2: 一個一個比較每個 Thunk Data 中存在的 function name,
	for (int i = 0; pThunk->u1.Function; i++) {
		bool match;

		if (pThunk->u1.Ordinal & 0x80000000)    // 如果你用序號(ordinal)表示你的 function name
		{
			match = (pThunk->u1.Ordinal & 0xFFFF) == ((DWORD)pProcName);
		}
		else
		{
			match = _stricmp(pProcName, RVA2Ptr((unsigned)pThunk->u1.AddressOfData) + 2) == 0;
			auto address = RVA2Ptr((unsigned)pThunk->u1.AddressOfData) + 2;
			cout << address << endl;
		}

		if (match) // 傳回相對於 FirstThunk + i 的位址
			return (unsigned *)RVA2Ptr(pImport->FirstThunk) + i;
		pThunk++;
	}

	return NULL;
}

// 重新指定一個 function 的位址
// input: (1) DLL module, (2) function 名稱, (3) 新的 function 位址
// output: 傳回舊的 function 位址
FARPROC KPEFile::SetImportAddress(LPCSTR pDllName, LPCSTR pProcName, FARPROC pNewProc){

	// Step 1: 取出目前這個 DLL module 的描述子
	PIMAGE_IMPORT_DESCRIPTOR pImport = GetImportDescriptor(pDllName);

	if (pImport) {
		// Step 2: 取出 指定 function 的位址
		const unsigned * pfn = GetFunctionPtr(pImport, pProcName);
		if (IsBadReadPtr(pfn, sizeof(DWORD)))   // 檢查該位址是否有問題
			return NULL;

		// Step 3: 儲存舊的 function address
		FARPROC oldproc = (FARPROC)* pfn;

		// Step 4: 把新的位址覆寫過去
		DWORD dwWritten;  // 記錄被寫了幾個 bytes

		DWORD dOldProtect = 0;
		//auto vbb = VirtualProtectEx(GetCurrentProcess(), (void *)pfn, 4, PAGE_READWRITE, &OLD);
		//改變屬性 不然win8這塊似乎是唯獨
		//auto vbb = VirtualProtectEx(GetCurrentProcess(), (void *)pfn, 4, PAGE_READWRITE, &OLD);
		auto success = VirtualProtect((void *)pfn, sizeof(pfn), PAGE_READWRITE, &dOldProtect);
		//memcpy((void *)pfn, &pNewProc, 4);//這邊類似於  *pfn = 一個function address
		success = WriteProcessMemory(GetCurrentProcess(), (void *)pfn, &pNewProc, sizeof(DWORD), &dwWritten);

		return oldproc;
	}
	else
		return NULL;
}

//設定後 會讓之後利用GetProcessAddress的API都回傳新的function entry address
FARPROC KPEFile::SetExportAddress(LPCSTR _pProcName, FARPROC _pNewProc)
{
	PIMAGE_EXPORT_DIRECTORY pExport
		= (PIMAGE_EXPORT_DIRECTORY)GetDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (pExport == 0)
	{
		return  0;
	}

	unsigned ord = 0;
	if ((unsigned)_pProcName < 0xFFFF)
	{
		ord	= (unsigned)_pProcName;
	}
	else
	{
		const  DWORD *  pNames = (const  DWORD *)(pModule + pExport->AddressOfNames);
		const  WORD *  pOrds = (const  WORD *)(pModule + pExport->AddressOfNameOrdinals);
		for (unsigned i = 0; i < pExport->AddressOfNames; ++i)
		{
			if (_stricmp(_pProcName, pModule + pNames[i]) == 0)
			{
				ord	= pExport->Base + pOrds[i];
				break;
			}

		}
	}
	if ((ord < pExport->Base) || (ord > pExport->NumberOfFunctions))
	{
		return  0;
	}

	DWORD  * pRVA = ((DWORD *)(pModule + pExport->AddressOfFunctions)) + ord - pExport->Base;
	DWORD rslt = *pRVA;
	DWORD dwWritten	= 0; 
	DWORD newRVA = (DWORD)_pNewProc - (DWORD)pModule;  
	DWORD dOldProtect = 0;
	auto success = VirtualProtect((void *)pRVA, sizeof(pRVA), PAGE_READWRITE, &dOldProtect);
	success = WriteProcessMemory(::GetCurrentProcess(), pRVA,	&newRVA, sizeof(DWORD), &dwWritten);
	return  (FARPROC)(pModule + rslt);
}
