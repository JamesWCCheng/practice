#include "KPEFile.h"
#include "common.h"
#include <iostream>
using namespace std;

KPEFile::KPEFile()
	:KPEFile(GetModuleHandle(NULL)) //C++ 11 Delegating Constructor
{

}
// ���o DOS Header �P NTHeader
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

// �Ǧ^�ثe�����ɤ� ���w Directory Table �� ��}
const void * KPEFile::GetDirectory(int id)
{
	return RVA2Ptr(pNTHeader->OptionalHeader.DataDirectory[id].VirtualAddress);
}


// �Ǧ^���w import DLL module �� ���c��T�y�z�l
// input: DLL name
// output: ��T�y�z�l
PIMAGE_IMPORT_DESCRIPTOR KPEFile::GetImportDescriptor(LPCSTR pDllName){

	// Step 1: ���X�ثe�����ɤ� IMAGE_DIRECTORY_ENTRY_IMPORT directory ����m
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)
		GetDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);

	if (pImport == NULL)
		return NULL;

	// Step 2: ���X�C�� Import table ���� Name �@�Ӥ@�Ӥ��
	while (pImport->FirstThunk){                              // �Y�� map �h FirstThunk �|�� module ��}
		if (_stricmp(pDllName, RVA2Ptr(pImport->Name)) == 0)       // �]�� import ���� Name �� RVA �s��,�ҥH�n���ഫ�~����
			return pImport;
		pImport++;                                               // ����,����U�@�Ӥ��
	}
	return NULL;
}


// �Ǧ^���w imported function ����} ( __imp__xxx variable )
// input: (1) ��J�A�� function �Ҧb�� DLL module �y�z�l,  (2) function Name
// output: function ����}
const unsigned * KPEFile::GetFunctionPtr(PIMAGE_IMPORT_DESCRIPTOR pImport, LPCSTR pProcName)
{
	// Step 1: �� Import DLL module �y�z�l�����o PIMAGE_THUNK_DATA ���c
	//         �o�ӵ��c�s��F function ����}
	PIMAGE_THUNK_DATA pThunk;
	pThunk = (PIMAGE_THUNK_DATA)RVA2Ptr(pImport->OriginalFirstThunk);

	// Step 2: �@�Ӥ@�Ӥ���C�� Thunk Data ���s�b�� function name,
	for (int i = 0; pThunk->u1.Function; i++) {
		bool match;

		if (pThunk->u1.Ordinal & 0x80000000)    // �p�G�A�ΧǸ�(ordinal)��ܧA�� function name
		{
			match = (pThunk->u1.Ordinal & 0xFFFF) == ((DWORD)pProcName);
		}
		else
		{
			match = _stricmp(pProcName, RVA2Ptr((unsigned)pThunk->u1.AddressOfData) + 2) == 0;
			auto address = RVA2Ptr((unsigned)pThunk->u1.AddressOfData) + 2;
			cout << address << endl;
		}

		if (match) // �Ǧ^�۹�� FirstThunk + i ����}
			return (unsigned *)RVA2Ptr(pImport->FirstThunk) + i;
		pThunk++;
	}

	return NULL;
}

// ���s���w�@�� function ����}
// input: (1) DLL module, (2) function �W��, (3) �s�� function ��}
// output: �Ǧ^�ª� function ��}
FARPROC KPEFile::SetImportAddress(LPCSTR pDllName, LPCSTR pProcName, FARPROC pNewProc){

	// Step 1: ���X�ثe�o�� DLL module ���y�z�l
	PIMAGE_IMPORT_DESCRIPTOR pImport = GetImportDescriptor(pDllName);

	if (pImport) {
		// Step 2: ���X ���w function ����}
		const unsigned * pfn = GetFunctionPtr(pImport, pProcName);
		if (IsBadReadPtr(pfn, sizeof(DWORD)))   // �ˬd�Ӧ�}�O�_�����D
			return NULL;

		// Step 3: �x�s�ª� function address
		FARPROC oldproc = (FARPROC)* pfn;

		// Step 4: ��s����}�мg�L�h
		DWORD dwWritten;  // �O���Q�g�F�X�� bytes

		DWORD dOldProtect = 0;
		//auto vbb = VirtualProtectEx(GetCurrentProcess(), (void *)pfn, 4, PAGE_READWRITE, &OLD);
		//�����ݩ� ���Mwin8�o�����G�O�߿W
		//auto vbb = VirtualProtectEx(GetCurrentProcess(), (void *)pfn, 4, PAGE_READWRITE, &OLD);
		auto success = VirtualProtect((void *)pfn, sizeof(pfn), PAGE_READWRITE, &dOldProtect);
		//memcpy((void *)pfn, &pNewProc, 4);//�o��������  *pfn = �@��function address
		success = WriteProcessMemory(GetCurrentProcess(), (void *)pfn, &pNewProc, sizeof(DWORD), &dwWritten);

		return oldproc;
	}
	else
		return NULL;
}

//�]�w�� �|������Q��GetProcessAddress��API���^�Ƿs��function entry address
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
