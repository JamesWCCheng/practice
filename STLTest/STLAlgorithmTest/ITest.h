#pragma once
#include <Windows.h>
class ITest
{
public:
	virtual ~ITest(){}
	virtual HRESULT RunTest() = 0;
};

enum class TEST_CASE : int
{
	STABLE_PARTITION = 1,
	ROTATE,

};