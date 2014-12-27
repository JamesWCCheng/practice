// Copyright 2014. by James Cheng, QQMAX Inc.
// All rights reserved.
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <complex>
#include "./PatternTesterHeader.h"
#include "./DllTest.h"
#include "./SystemUtility.h"
#include <memory>

typedef void(*PatternTestFunc)();
PatternTestFunc testCaseArray[] = { DecoratorTest, FactoryTest,
StrategyTest, ProxyTest, BridgeTest, CompositeTest, CommandTest };
PatternTestFunc TestPattern = testCaseArray[6];

int main()
{
	SystemUtility::AutoHeapChecker autochecker;
	TestPattern();

	system("PAUSE");
	return 0;
}