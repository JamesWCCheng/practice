#pragma once
#include <windows.h>
#include <crtdbg.h>
#include <iostream>

#if defined(_DEBUG)
#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)
#else

#endif

class SystemUtility
{
public:
	class AutoHeapChecker	
	{
	public:
		static bool IsHeapCorruptionOccurr()
		{
			/*_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);*/
			return _CrtCheckMemory() ? false : true;
		}
#ifdef _DEBUG
		AutoHeapChecker(TCHAR * filePath)
		{
			m_LeakFileHandle = CreateFile(filePath, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
			_CrtSetReportFile(_CRT_WARN, m_LeakFileHandle);
			_CrtMemCheckpoint(&m_Begin);
			int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
			//tmpDbgFlag = (tmpDbgFlag & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
			_CrtSetDbgFlag(tmpDbgFlag);
		}
		AutoHeapChecker() : m_LeakFileHandle(nullptr)
		{
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
			_CrtMemCheckpoint(&m_Begin);
			int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
			//tmpDbgFlag = (tmpDbgFlag & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
			_CrtSetDbgFlag(tmpDbgFlag);

		}
		~AutoHeapChecker()
		{
			_CrtMemCheckpoint(&m_End);
			_CrtMemState diff;
			if (_CrtMemDifference(&diff, &m_Begin, &m_End))
			{
				if (_CrtDumpMemoryLeaks())
					std::cout << "Memory Leak Occurred" << std::endl;
				else
					std::cout << "No Memory Leak" << std::endl;
			}

			if (IsHeapCorruptionOccurr())
			{
				std::cout << "Heap Corruption Occurred" << std::endl;
			}
			if (m_LeakFileHandle != nullptr)
			{
				CloseHandle(m_LeakFileHandle);
			}
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
		}
	private:
		HANDLE m_LeakFileHandle;
		_CrtMemState m_Begin;
		_CrtMemState m_End;

#endif
	};
};