#include "stable_partition.h"
#include "stlcommonheader.h"
using namespace std;
//http://www.cplusplus.com/reference/algorithm/stable_partition/
//這高級API 理解....First(包含) 到  Last(的前一個)    滿足第三個 條件的東西 做一個區隔 回傳    不滿足條件的第一個稱作boundary

template<class T>
class Bar
{
public:

	Bar(T i)
	{
		data = i;
	}
	operator bool()
	{
		return static_cast<int>(data) % 2 == 1;
	}

	operator int()
	{
		return static_cast<int>(data) % 2 == 1;
	}
	T data;
};
template<class T>
class UnaryPredicate
{
public:
	Bar<T> operator()(T data)
	{
		return Bar<T>(data);
	}

	
	static UnaryPredicate pred;

	//static UnaryPredicate pred2;


};
template<class T>
UnaryPredicate<T> UnaryPredicate<T>::pred;



template <typename I, typename S>
auto gather(I first, I middle, I last, S s)->std::pair<I, I>
{
	return
	{
		stable_partition(first, middle, std::not1(s)),
		stable_partition(middle, last, s)
	};
	
}


template<class T>
class IsInRange {
public:
	bool operator() (const T& x) const
	{
		if (x >= 10 && x <= 20)
			return true;
		return false;
	}

	typedef T argument_type;
	typedef int result_type;
};


HRESULT TestStablePartition::RunTest()
{
	std::vector<int> myvector;
	// set some values:
	myvector.push_back(2);
	myvector.push_back(20); //1
	myvector.push_back(5);  //2
	myvector.push_back(30); //3
	myvector.push_back(15); //4
	myvector.push_back(7);
	myvector.push_back(16);

	std::rotate(begin(myvector)+1, begin(myvector) + 4, begin(myvector)+6);


	auto pair = gather(begin(myvector), myvector.begin() + 4, myvector.end(), IsInRange<int>());

	int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
	std::vector<int> v(myints, myints + 8);           // 10 20 30 30 20 10 10 20

	std::sort(v.begin(), v.end());                // 10 10 10 20 20 20 30 30

	std::vector<int>::iterator low, up;
	low = std::lower_bound(v.begin(), v.end(), 20); //          ^
	up = std::upper_bound(v.begin(), v.end(), 20); //                   ^

	
	return exception_safe_wrapper(
	[]
	{
		std::vector<int> myvector;

		// set some values:
		for (int i = 1; i < 10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

		std::vector<int>::iterator bound;
		bound = std::stable_partition(myvector.begin(), myvector.end(), UnaryPredicate<int>::pred);

		// print out content:
		std::cout << "odd elements:";
		for (std::vector<int>::iterator it = myvector.begin(); it != bound; ++it)
			std::cout << ' ' << *it;
		std::cout << '\n';

		std::cout << "even elements:";
		for (std::vector<int>::iterator it = bound; it != myvector.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << '\n';
	});
}