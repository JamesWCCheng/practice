#include "TestFactory.h"
#include "resource.h"
#include <Windows.h>
#include "Util\ThreadpoolTimer.h"
#include "Util\DateTime.h"
#include "Util\UnicodeBlock.h"
#include "Util\ITask.h"
#include "Util\ThreadPool.h"
#include "Util\ThreadTask.h"
#include "Util\KPEFile.h"
#include "Util\HandleListener.h"
#include "Util\RegKeyListenerBase.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <memory>
using namespace std;


class TestBase : public ITest
{
public:
	TestBase(int id)
		: m_TestTitle(nullptr)
	{
		GetTestTitle(id);
		
	}
	virtual ~TestBase()
	{
		delete [] m_TestTitle;
	}
protected:
	void GetTestTitle(int id)
	{
		m_TestTitle = new TCHAR[100];
		LoadString(NULL, id, m_TestTitle, 100);
		MessageBox(NULL, m_TestTitle, m_TestTitle, MB_OK);
	}
	TCHAR *m_TestTitle;
};
class TestTimer : public TestBase
{
	class MyTimer : ThreadpoolTimer
	{
	public:
		void SetTimer(PFILETIME pfDueTime, DWORD dwPeriod)
		{
			ThreadpoolTimer::SetTimer(pfDueTime, dwPeriod);
		}
		void StopTimer()
		{
			SetTimer(NULL, 0);
		}
	private:
		virtual void OnTimerTick() {
			cout << "Get Tick" << endl;
		}
	};

	void RunTestTimer()
	{
		//Test Timer
		MyTimer timer;
		DateTime dtTarget = DateTime::Now();
		dtTarget.AddSecond(5);
		ULARGE_INTEGER ulIntInterval = DateTime::Interval(DateTime::Now(), dtTarget);
		ulIntInterval.QuadPart = (ULONGLONG)(-(LONGLONG)ulIntInterval.QuadPart); // relative time

		FILETIME fileTime;
		fileTime.dwHighDateTime = ulIntInterval.HighPart;
		fileTime.dwLowDateTime = ulIntInterval.LowPart;
		timer.SetTimer(&fileTime, 1000); //設定為 五秒來第一發,之後一秒一發
		getchar();
		timer.StopTimer();
	}
public:
	TestTimer(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestTimer();
	}
};

class TestThreadPool : public TestBase
{
private:
	class ThreadPoolTask : public ITask
	{
	public:
		ThreadPoolTask(DWORD delay, DWORD id) : ITask(delay, id)
		{
		}
		~ThreadPoolTask()
		{
			cout << "GG " << m_TaskID << endl;
		}
		virtual void Run()
		{
			cout << "Thread Pool Task Run, ID = " << GetID() << ", System ID = " << GetCurrentThreadId() << endl;
		}

	};

	class Thread : public ThreadTask
	{
	public:
		Thread(DWORD delay) : ThreadTask(delay)
		{
		}
		virtual void Run()
		{
			cout << "Thread Run, ID = " << GetID() << ", System ID = " << GetCurrentThreadId() << endl;
		}

	};
	void RunTestThreadPool()
	{
		/*cout << "Add Success = " << pool.AddTask(&t) << endl;
		cout << "Add Success = " << pool.AddTask(&t2) << endl;
		cout << "Add Success = " << pool.AddTask(&t3) << endl;
		cout << "Add Success = " << pool.AddTask(&t4) << endl;*/
		ThreadPool pool(1, 2);
		ThreadPoolTask *t = new ThreadPoolTask(5000, 1);
		ThreadPoolTask *t2 = new ThreadPoolTask(6000, 2);
		ThreadPoolTask *t3 = new ThreadPoolTask(1000, 3);
		ThreadPoolTask *t4 = new ThreadPoolTask(0, 4);
		pool.AddTask(shared_ptr<ThreadPoolTask>(t));
		pool.AddTask(shared_ptr<ThreadPoolTask>(t2));
		t4->Cancel();
		pool.AddTask(shared_ptr<ThreadPoolTask>(t3));
		pool.AddTask(shared_ptr<ThreadPoolTask>(t4));

		system("PAUSE");
		cout << "剩下" << pool.GetAvailableThreadCount() << "條thread 可用" << endl;
		pool.AdjustCapacity(1, 1);
		Sleep(100);
		cout << "剩下" << pool.GetAvailableThreadCount() << "條thread 可用" << endl;
		ThreadPoolTask *t5 = new ThreadPoolTask(5000, 5);
		pool.AddTask(shared_ptr<ThreadPoolTask>(t5));
		//Thread t6(1000);
		//t6.Start();
		pool.ReleasePool();
		shared_ptr<Thread> sp(new Thread(1000));
		sp->Start();
		sp->Terminate();
		system("PAUSE");
	}
public:
	TestThreadPool(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestThreadPool();
	}
};
class TestUnicodeBlock : public TestBase
{
private:
	void RunTestUnicodeBlock()
	{
		_setmode(_fileno(stdout), _O_U16TEXT);

		TCHAR *unicodeChineseData = TEXT("中");
		TCHAR *unicodeChineseBopomoData = TEXT("ㄅ");
		TCHAR *unicodeJapaneseData = TEXT("あ");
		wcout << unicodeChineseBopomoData << ", " << UnicodeBlock::BOPOMOFO.Contain(unicodeChineseBopomoData[0]) << endl;
		wcout << unicodeJapaneseData << ", " << UnicodeBlock::HIRAGANA.Contain(unicodeJapaneseData[0]) << endl;
	}

public:
	TestUnicodeBlock(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestUnicodeBlock();
	}
};
class TestPEResolver : public TestBase
{
private:
	static FARPROC OriginalCreateThread;

	static _Ret_maybenull_
		HANDLE
		WINAPI
		MyCreateThread(
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ SIZE_T dwStackSize,
		_In_ LPTHREAD_START_ROUTINE lpStartAddress,
		_In_opt_ __drv_aliasesMem LPVOID lpParameter,
		_In_ DWORD dwCreationFlags,
		_Out_opt_ LPDWORD lpThreadId
		)
	{
		cout << "MyCreateThread" << endl;
		/*auto handle = ((_Ret_maybenull_ HANDLE(WINAPI *)(LPSECURITY_ATTRIBUTES lpThreadAttributes,
		SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter, DWORD dwCreationFlags,
		LPDWORD lpThreadId))OriginalCreateThread)(lpThreadAttributes,
		dwStackSize, lpStartAddress, lpParameter,
		dwCreationFlags,
		lpThreadId);*/
		auto handle = ((decltype(&CreateThread))OriginalCreateThread)(lpThreadAttributes,
			dwStackSize, lpStartAddress, lpParameter,
			dwCreationFlags,
			lpThreadId);
		return handle;

	}
	static DWORD WINAPI ThreadProc(LPVOID parameter) {

		return ERROR_SUCCESS;
	}
	void RunTestResolvePE()
	{
		auto handle = GetModuleHandle(L"Kernel32.dll");
		KPEFile pe;
		OriginalCreateThread = pe.SetImportAddress("Kernel32.dll", "CreateThread", (FARPROC)MyCreateThread);

		auto functionEntry = GetProcAddress(handle, "CreateThread");

		if (OriginalCreateThread == functionEntry)
		{
			cout << "*pfn == GetProcAddress" << endl;
		}

		CreateThread(nullptr, 0, ThreadProc, nullptr, 0, nullptr);

		KPEFile pe2(handle);
		//Hook GetProcAddress return address to ours.
		auto address = pe2.SetExportAddress("CreateThread", (FARPROC)MyCreateThread);
		auto functionEntry2 = GetProcAddress(handle, "CreateThread");

		if ((FARPROC)MyCreateThread == functionEntry2)
		{
			cout << "MyCreateThread == GetProcAddress" << endl;
		}

		system("PAUSE");
	}
public:
	TestPEResolver(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestResolvePE();
	}
};
FARPROC TestPEResolver::OriginalCreateThread = nullptr;
class TestHandleListener : public TestBase
{
private:
	class Thread : public ThreadTask
	{
	public:
		Thread(DWORD delay = 0) : ThreadTask(delay)
		{
		}
		virtual void Run()
		{
			Sleep(5000);
			cout << "Thread Run, ID = " << GetID() << ", System ID = " << GetCurrentThreadId() << endl;
		}

		//Functor
		void operator()(bool IsTimeOut)
		{
			cout << "CallBack, IsTimeOut = " << IsTimeOut << endl;
		}
	};

	static void Callback(bool IsTimeOut)
	{
		cout << "Event Callback, IsTimeOut = " << IsTimeOut << endl;
	}
	void RunTestHandleListener()
	{
		Thread t1;

		HandleListener<Thread> handleWaiter(t1.GetHandle(), t1);
		t1.Start();
		handleWaiter.Start();
		HANDLE hEvent = CreateEvent(
			NULL,    // default security attributes
			FALSE,    // manual reset event //AUTORESET
			TRUE,   // not signaled   //TRUE的話 馬上會來第一發
			L"Test");   // name
		HandleListener<decltype(Callback)> handleWaiter2(hEvent, Callback, true, 3000);
		handleWaiter2.Start();
		Sleep(1000);
		SetEvent(hEvent); //1
		Sleep(6000);
		SetEvent(hEvent); //2
		Sleep(1000);
		SetEvent(hEvent); //3
		handleWaiter2.Stop();
		SetEvent(hEvent); //4
		Sleep(1000);
		handleWaiter2.Start();

		//SetEvent(hEvent); //5
		system("PAUSE");
	}
public:
	TestHandleListener(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestHandleListener();
	}
};
class TestRegListener : public TestBase
{
private:
#define TARGET_MAIN_KEY (HKEY_CURRENT_USER)
#define TARGET_KEY (L"Software\\Meitu")
#define TARGET_VALUE_NAME (L"KanKanSetupCount")
#define TARGET_VALUE_TYPE (RRF_RT_DWORD)

	class TestKeyListener : public RegKeyListenerBase
	{
	public:
		TestKeyListener()
			:RegKeyListenerBase(TARGET_MAIN_KEY, TARGET_KEY, FALSE, REG_NOTIFY_CHANGE_LAST_SET)
		{

		}
		virtual void OnKeyChanged()
		{

			DWORD dwData = 0;
			DWORD dwDataSize = sizeof(dwData);
			//LSTATUS lError = RegGetValue(TARGET_MAIN_KEY, TARGET_KEY, TARGET_VALUE_NAME, RRF_RT_DWORD, NULL, &dwData, &dwDataSize);
			LSTATUS lError = RegGetValue(HKEY_CURRENT_USER, L"Software\\Meitu", L"KanKanSetupCount", RRF_RT_DWORD, NULL, &dwData, &dwDataSize);
			cout << "Key Changed to " << dwData << endl;

		}
	};


	void RunTestRegListener()
	{
		TestKeyListener tl;

		cout << tl.Start();
		system("PAUSE");
	}
public:
	TestRegListener(int id)
		:TestBase(id)
	{

	}
	virtual void RunTest()
	{
		RunTestRegListener();
	}
};
TestFactory::TestFactory()
{

}
TestFactory::~TestFactory()
{

}
void TestFactory::ReleaseTester(ITest * tester)
{
	delete tester;
}
ITest *TestFactory::GetTester(int testID)
{
	ITest * tester = nullptr;
	switch (testID)
	{
	case IDS_STRING_TestTimer:
		tester = new TestTimer(testID);
		break;
	case IDS_STRING_TestUnicodeBlock:
		tester = new TestUnicodeBlock(testID);
		break;
	case IDS_STRING_TestThreadPool:
		tester = new TestThreadPool(testID);
		break;
	case IDS_STRING_TestRegListener:
		tester = new TestRegListener(testID);
		break;
	case IDS_STRING_TestResolvePE:
		tester = new TestPEResolver(testID);
		break;
	case IDS_STRING_TestHandleListener:
		tester = new TestHandleListener(testID);
		break;
	default:
		break;
	}
	return tester;
}
