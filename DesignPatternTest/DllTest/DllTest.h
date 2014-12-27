// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLLTEST_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLLTEST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLTEST_EXPORTS
#define DLLTEST_API __declspec(dllexport)
#else
#define DLLTEST_API __declspec(dllimport)
#endif

// This class is exported from the DllTest.dll
class DLLTEST_API CDllTest {
public:
	CDllTest(void);
	float ClassFunction();
	int ClassFunctionWithDEF();
	// TODO: add your methods here.

private:
	float data;
};

extern int nDllTest;



DLLTEST_API double __cdecl fnCDELCall(double a);

DLLTEST_API double __stdcall fnSTDCall(double a, int b, float c, char* p);
DLLTEST_API void fnFree(void *ptr);

extern "C"
{
int fnDllTest(int a);
DLLTEST_API void* fnNew(int size);

}