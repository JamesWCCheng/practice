#pragma once

class ITest
{
public:
	virtual ~ITest(){}
	virtual void RunTest() = 0;

};
class TestFactory
{
public:
	TestFactory();
	~TestFactory();
	ITest *GetTester(int testID);
	void ReleaseTester(ITest * tester);

};