#pragma once
#include <mutex>
class ITest;
namespace std{
	template<class T>
	class shared_ptr;

}
enum class TEST_CASE;
class TestFactory
{
public:
	TestFactory();
	~TestFactory();
	std::shared_ptr<ITest> GetTester(TEST_CASE testID);
private:
	std::mutex m_Mutex;
};