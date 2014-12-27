#include <windows.h>
class KPEFile {
  const char        * pModule;
  PIMAGE_DOS_HEADER pDOSHeader;
  PIMAGE_NT_HEADERS pNTHeader;

public:
  // �N RVA ��}�ন virtual address
  const char * RVA2Ptr(unsigned rva) {
    if ( (pModule!=NULL) && rva )
      return pModule + rva;
    else
      return NULL;
  }

  // �غc�l: ���o DOS Header �P NTHeader
  KPEFile(HMODULE hModule);
  //�ϥΦۤv��process hModule
  KPEFile();
  // �Ǧ^�ثe�����ɤ� ���w�� Directory ��}
  const void * GetDirectory(int id);

  // �Ǧ^���w import DLL module �� ���c��T�y�z�l
  PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor(LPCSTR pDllName);

  // �Ǧ^���w imported function ����} ( __imp__xxx variable )
  const unsigned * GetFunctionPtr(PIMAGE_IMPORT_DESCRIPTOR
    pImport, LPCSTR pProcName);

  FARPROC SetImportAddress(LPCSTR pDllName, LPCSTR pProcName,
    FARPROC pNewProc);

  FARPROC SetExportAddress(LPCSTR pProcName, FARPROC pNewProc);
};
