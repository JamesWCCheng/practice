#include "TestFactory.h"
#include "resource.h"
#include "Util\SystemUtility.h"
#include <memory>

#include <windows.h>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <new>
using namespace std;
class Base
{
public:
	Base()
	{
	}
	Base(const Base & rhs)
	{
		x = rhs.x;
	}
#if 1
	Base(Base &&ref) : x(ref.x) {
	}
#endif
	Base & operator =(const Base& rhs)
	{
#if 0
		if (this != &rhs)
		{
			Base b(rhs);
			swap(b);
		}
		return *this;
#else
		return ((*this) = Base(rhs));
#endif
	}
#if 1
	Base& operator=(Base &&rhs) {
		swap(rhs);
		return (*this);
	}
#endif
	~Base()
	{
	}
protected:
	void swap(Base & rhs)
	{
		std::swap(x, rhs.x);
	}
	int x;
};
class Foo : public Base
{
public:
	int Fooo(){
		return 5566;
	}

	Foo()
	{
		ptr = new int;
	}
	Foo(const Foo& rhs) : Base(rhs)
	{
		this->ptr = new int;
		*this->ptr = *rhs.ptr;
		this->num = rhs.num;
	}
#if 1
	Foo(Foo &&ref) : Base(std::move(ref)), ptr(ref.ptr), num(ref.num) {
		ref.ptr = nullptr;
	}
#endif
	Foo& operator=(const Foo &rhs)
	{
#if 0
		Base::operator=(rhs);
		if (this != &rhs)
		{
			Foo temp(rhs);
			swap(temp);
		}
		return *this;
#else
		return ((*this) = Foo(rhs));
#endif
	}
#if 1
	Foo& operator=(Foo &&rhs) {
		Base::operator=(std::move(rhs));
		swap(rhs);
		return (*this);
	}
#endif
	~Foo()
	{
		delete ptr;
	}
	int *ptr;
private:
	void swap(Foo & rhs)
	{
		std::swap(ptr, rhs.ptr);
		std::swap(num, rhs.num);
	}

	float num;
};
struct myfree
{
	int a;
	void operator()(void* ptr)
	{
		free(ptr);
	}
	void* operator&()
	{
		return &a;
	}
};

void QQ(Foo *& xd)
{

}
DWORD WINAPI ThreadFunction(LPVOID pContext)
{
	int i = 0;
	while (true)
	{
		i++;

	}
	return ERROR_SUCCESS;
}
#include <algorithm>

#include "Util\PerformanceMeasure.h"
int main()
{



	//{
	//	SystemUtility::AutoHeapChecker autoMemChecker;
	//	Foo f;
	//	Foo f2;
	//	*f.ptr = 123;
	//	*f2.ptr = 5566;
	//	f = f2;
	//	std::cout << *f.ptr;
	//}
	//{
	//	
	//	SystemUtility::AutoHeapChecker autoMemChecker;
	//	//std::unique_ptr<Foo[]> ufoo(new (std::nothrow) Foo[2]);
	//	std::unique_ptr<void, myfree> ufoo2(malloc(123));
	//}
	//SystemUtility::AutoHeapChecker autoMemChecker;
	//std::unique_ptr<Foo[]> ufoo2;
	//std::unique_ptr<Foo[]> ufoo(new Foo[2]);

	//ufoo.swap(ufoo2);
	////std::shared_ptr<Foo[]> sfoo = ufoo;
	//auto qq = ufoo.get();
	//SystemUtility::AutoHeapChecker autoMemChecker(TEXT("leak.txt"));
	//int *ptr = new  int;
	/*TestFactory factory;
	auto tester = factory.GetTester(IDS_STRING_TestTimer);
	tester->RunTest();
	factory.ReleaseTester(tester);*/

	PerformanceMeasure pm;
	pm.Start();

	while (true)
	{
		Sleep(1000);
		pm.Stop();
		cout<<"Elpased Time = "<<pm.m_ElapsedTimeMS <<endl;
		pm.Start();
	}


	system("PAUSE");
	return 0;
}