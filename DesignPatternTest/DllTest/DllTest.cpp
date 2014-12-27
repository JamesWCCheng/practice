// DllTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DllTest.h"

#include <cstdlib>
#include <Windows.h>
#include <crtdbg.h>
#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)
// This is an example of an exported variable
int nDllTest=0;

// This is an example of an exported function.
int fnDllTest(int a)
{

	int *ptr = new int;
	CreateFile(L"", 0,0,NULL,0,0, NULL );
	nDllTest = 7788;
	int b = a;
	return b;
}
DLLTEST_API double __cdecl fnCDELCall(double a)
{
	return 11.0;
}
DLLTEST_API double __stdcall fnSTDCall(double a, int b, float c, char* p)
{
	return a;
}


DLLTEST_API void fnFree(void *ptr)
{
	free(ptr);
}
DLLTEST_API void* fnNew(int size)
{
	return malloc(size);
}
// This is the constructor of a class that has been exported.
// see DllTest.h for the class definition
CDllTest::CDllTest()
{
	return;
}

float CDllTest::ClassFunction()
{
	this->data = 123.546f;
	return this->data;
}
int CDllTest::ClassFunctionWithDEF()
{
	return (int)this->data;
}