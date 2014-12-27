#include <memory>
#include <thread>
#include <vector>
#include <list>
#include "TestFactory.h"
#include "ITest.h"
#include "SystemUtility.h"
#include "LinkedList.h"
#include "Stack_LinkedList.h"
#include "Stack_Array.h"
#include <stack>
using namespace std;

int* FooPtr(char* ptr)
{
	return 0;
}
int* (*FPTR[])(char* ptr) = {FooPtr, FooPtr};
class Base
{
public:
	virtual void Foo1()
	{
		Foo2();
	}
	
	void Foo2()
	{
		Foo3();
	}
	virtual void Foo3()
	{
	
	}
};
class Derived : public Base
{
public:
	virtual void Foo1()
	{ 
		Foo2(); 
	}
	void Foo2() 
	{ 
		Foo3(); 
	}
	virtual void Foo3()
	{
	
	}
};

template<typename T>
void Swap(T &l, T& r)
{
	T temp = l;
	l = r;
	r = temp;
}

int strlength(const char* str)
{
	const char* current = str;
	while (*current != '\0')
	{
		current++;
	}
	return current - str;
}

void Reverse(char* str)
{
	int len = strlength(str);

	int head = 0;
	int tail = len - 1;
	while (head < tail)
	{
		Swap(str[head], str[tail]);
		head++;
		tail--;
	}
}

template<typename Type, size_t N>
Type MaxSubsequence(const Type (&arr)[N])
{
	Type current_max = 0;
	Type sum = 0;
	for (size_t i = 0; i < N; i++)
	{
		
		sum += arr[i];
		if (sum < 0)
		{
			sum = 0;
		}
		if (current_max < sum)
		{
			current_max = sum;
		}
	}
	return current_max;
}

template<size_t N1, size_t N2>
void CheckSubString(const char(&str1)[N1], const char(&str2)[N2])
{
	int str1_len = strlength(str1);
	int str2_len = strlength(str2);

	for (size_t start = 0; start < str1_len; start++)
	{
		for (size_t check = 0; check < str2_len + 1; check++)
		{
			if (str2[check] == '\0')
			{
				//find out since to the end.
				cout << "index = " << start << endl;
			}
			if (str1[start + check] != str2[check]) // Not the same
			{
				break;
			}
		}
	}
}
template<size_t N>
void Indexing(vector<int>(&vec_arr)[N], const char * str)
{
	auto current = str;
	int index = 0;
	while (*current != '\0')
	{
		vec_arr[static_cast<int>(*current)].push_back(index);
		current++;
		index++;
	}
}


template<class Iterator, class Condition>
Iterator partition(Iterator first, Iterator last, Condition condition) {
	Iterator itr = first;
	while (first != last) {
		if (condition(*first)) {
			std::swap(*first, *itr);
			++itr;
		}
		++first;
	}
	return itr;
}

class ORZ
{
public:
	ORZ(int a){
	
	}
	ORZ(const ORZ & rhs){
		cout << "ORZC";
	}
	ORZ(ORZ && rhs){
		cout << "ORZM";
	}

};

void GG(ORZ rhs)
{

}
ORZ RETURN(){ return ORZ(1); }
int main()
{
	ORZ zz(1);
	GG(std::move(zz));
	vector<int> indexList[256];
	Indexing(indexList, "abcdacbbcbabba");
	auto current = "ab";
	while (*current != '\0')
	{
		int index = static_cast<int>(*current);
		const auto &indexes = indexList[index];
		cout << *current << "occurred in" << endl;
		for (const auto index : indexes)
		{
			cout << index << "\t";
		}
		cout << endl;
		current++;
	}
	/*std::vector<int> v;
	for (int i = 0; i < 10; ++i) {
		v.push_back(i);
	}
	partition(v.begin(), v.end(), [](int n) -> bool 
	{
		return ((n & 1) != 0);
	}	);*/
	CheckSubString("abcdacbbcbabba", "ab");

	int arr[] = { 1, 2, -1, 100, 200, -400, 500 };
	cout << MaxSubsequence(arr) << endl;

	//char str[] = "abc";
	char str[] = "abcd";
	Reverse(str);


	


	SystemUtility::AutoHeapChecker check;
	{
		
		auto specifiedNode = new Node<int>(2);
		LinkedList<int> linkedList;
		auto tail2 = linkedList.RemoveTail();
		linkedList.Insert(new Node<int>(1));
		linkedList.Insert(specifiedNode);
		linkedList.Insert(new Node<int>(3));
		linkedList.Insert(new Node<int>(4));

		while (auto item = linkedList.RemoveTail())
		{
			cout << item->m_data;
		}
		//linkedList.PrintList();

		//auto tail = linkedList.RemoveTail();
		//delete tail;
		//linkedList.PrintList();

		////linkedList.RemoveSpecifyNode(specifiedNode);

		//linkedList.Reverse();

		//linkedList.PrintList();

		//tail = linkedList.RemoveTail();

		//linkedList.PrintList();

		//linkedList.RemoveSpecifyNode(specifiedNode);

		//linkedList.PrintList();

		//linkedList.DeleteAll();

		//delete tail;

		/*Stack_LinkedList<int> stack;
		stack.Push(1);
		stack.Push(2);
		stack.Push(3);
		std::cout << stack.GetSize() <<endl;
		while (auto item = stack.Pop())
		{
			cout << item << endl;
		}*/

		/*Stack_Array<int, 10> stack;
		cout << stack.GetMax() << endl;
		stack.Push(1);
		stack.Push(100);
		stack.Push(2);
		stack.Push(3);
		stack.Push(200);
		cout << stack.GetMax() << endl;
		std::cout << stack.GetSize() << endl;
		try
		{
			while (auto item = stack.Pop())
			{

				cout << item <<", CurrentMax = "<< stack.GetMax()<< endl;
			}
		}
		catch (...)
		{
		}*/
	}
	//
	/*
	TestFactory factory;
	
	auto sp = factory.GetTester(TEST_CASE::STABLE_PARTITION);
	
	auto error = sp->RunTest();*/

	system("PAUSE");
	return 0;
}