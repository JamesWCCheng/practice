#include "TestFactory.h"
#include "ITest.h"
#include "stable_partition.h"
#include <memory>

TestFactory::TestFactory()
{

}
TestFactory::~TestFactory()
{

}

std::shared_ptr<ITest> TestFactory::GetTester(TEST_CASE testID)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	ITest * tester = nullptr;
	switch (testID)
	{
	case TEST_CASE::STABLE_PARTITION:
		tester = new TestStablePartition();
		break;
	}
	

	return std::shared_ptr<ITest>(tester);
}
