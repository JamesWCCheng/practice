#include <windows.h>
#include "DateTime.h"
#include "common.h"
DateTime::~DateTime()
{}

DateTime::DateTime(SYSTEMTIME systemTime)
	: m_systemTime(systemTime)
{
}

void DateTime::SetYear(WORD wYear)
{
	if (IsValidYear(wYear))
		m_systemTime.wYear = wYear;
}

void DateTime::SetMonth(WORD wMonth)
{
	if (IsValidMonth(wMonth))
		m_systemTime.wMonth = wMonth;
}

void DateTime::SetDay(WORD wDay)
{
	if (IsValidDay(wDay))
		m_systemTime.wDay = wDay;
}

void DateTime::SetHour(WORD wHour)
{
	if (IsValidHour(wHour))
		m_systemTime.wHour = wHour;
}

void DateTime::SetMinute(WORD wMinute)
{
	if (IsValidMinute(wMinute))
		m_systemTime.wMinute = wMinute;
}

void DateTime::SetSecond(WORD wSecond)
{
	if (IsValidSecond(wSecond))
		m_systemTime.wSecond = wSecond;
}

void DateTime::SetMilliseconds(WORD wMilliseconds)
{
	if (IsValidMilliseconds(wMilliseconds))
		m_systemTime.wMilliseconds = wMilliseconds;
}

void DateTime::AddMinute(WORD wMinute)
{
	FILETIME fileTime = ToFileTime();

	ULARGE_INTEGER ulInt;
	ulInt.LowPart = fileTime.dwLowDateTime;
	ulInt.HighPart = fileTime.dwHighDateTime;
	ulInt.QuadPart += (10 * 1000 * 1000 * 60) * wMinute;

	fileTime.dwLowDateTime = ulInt.LowPart;
	fileTime.dwHighDateTime = ulInt.HighPart;
	::FileTimeToSystemTime(&fileTime, &m_systemTime);
}

void DateTime::AddSecond(WORD wSecond)
{
	FILETIME fileTime = ToFileTime();

	ULARGE_INTEGER ulInt;
	ulInt.LowPart = fileTime.dwLowDateTime;
	ulInt.HighPart = fileTime.dwHighDateTime;
	ulInt.QuadPart += (10 * 1000 * 1000) * wSecond;

	fileTime.dwLowDateTime = ulInt.LowPart;
	fileTime.dwHighDateTime = ulInt.HighPart;
	::FileTimeToSystemTime(&fileTime, &m_systemTime);
}

void DateTime::AddMilliseconds(WORD wMilliseconds)
{
	//這樣就不用考慮進位
	FILETIME fileTime = ToFileTime();

	ULARGE_INTEGER ulInt;
	ulInt.LowPart = fileTime.dwLowDateTime;
	ulInt.HighPart = fileTime.dwHighDateTime;
	ulInt.QuadPart += (10 * 1000) * wMilliseconds;

	fileTime.dwLowDateTime = ulInt.LowPart;
	fileTime.dwHighDateTime = ulInt.HighPart;
	::FileTimeToSystemTime(&fileTime, &m_systemTime);
}

FILETIME DateTime::ToFileTime() const
{
	FILETIME fileTime;
	::SystemTimeToFileTime(&m_systemTime, &fileTime);
	return fileTime;
}

ULARGE_INTEGER DateTime::ToLargeInteger() const
{
	FILETIME fileTime = ToFileTime();
	ULARGE_INTEGER ulInt;
	ulInt.LowPart = fileTime.dwLowDateTime;
	ulInt.HighPart = fileTime.dwHighDateTime;
	return ulInt;
}

DateTime DateTime::ToLocalDateTime() const
{
	FILETIME fileTime;
	FILETIME fileTimeLocal;
	SYSTEMTIME systemTimeLocal;
	
	fileTime = ToFileTime();
	::FileTimeToLocalFileTime(&fileTime, &fileTimeLocal);
	::FileTimeToSystemTime(&fileTimeLocal, &systemTimeLocal);
	return DateTime(systemTimeLocal);
}

DateTime DateTime::Now()
{
	SYSTEMTIME systemTime;
	::GetSystemTime(&systemTime);
	return DateTime(systemTime);
}

ULARGE_INTEGER DateTime::Interval(const DateTime& dtBegin, const DateTime& dtEnd)
{
	ULARGE_INTEGER ulIntBegin = dtBegin.ToLargeInteger();
	ULARGE_INTEGER ulIntEnd = dtEnd.ToLargeInteger();
	
	ULARGE_INTEGER ulIntInterval;
	ulIntInterval.QuadPart = max(ulIntBegin.QuadPart, ulIntEnd.QuadPart) - min(ulIntBegin.QuadPart, ulIntEnd.QuadPart);
	return ulIntInterval;
}

bool DateTime::IsValidYear(WORD wYear)
{
	const WORD MIN_YEAR = 1601;
	const WORD MAX_YEAR = 30827;
	return MIN_YEAR <= wYear && wYear <= MAX_YEAR;
}

bool DateTime::IsValidMonth(WORD wMonth)
{
	const WORD MIN_MONTH = 1;
	const WORD MAX_MONTH = 12;
	return MIN_MONTH <= wMonth && wMonth <= MAX_MONTH;
}

bool DateTime::IsValidDayOfWeek(WORD wDayOfWeek)
{
	const WORD MIN_DAY_OF_WEEK = 1;
	const WORD MAX_DAY_OF_WEEK = 7;
	return MIN_DAY_OF_WEEK <= wDayOfWeek && wDayOfWeek <= MAX_DAY_OF_WEEK;
}

bool DateTime::IsValidDay(WORD wDay)
{
	const WORD MIN_DAY = 1;
	const WORD MAX_DAY = 31;
	return MIN_DAY <= wDay && wDay <= MAX_DAY;
}

bool DateTime::IsValidHour(WORD wHour)
{
	const WORD MIN_HOUR = 0;
	const WORD MAX_HOUR = 23;
	return MIN_HOUR <= wHour && wHour <= MAX_HOUR;
}

bool DateTime::IsValidMinute(WORD wMinute)
{
	const WORD MIN_MINUTE = 0;
	const WORD MAX_MINUTE = 59;
	return MIN_MINUTE <= wMinute && wMinute <= MAX_MINUTE;
}

bool DateTime::IsValidSecond(WORD wSecond)
{
	const WORD MIN_SECOND = 0;
	const WORD MAX_SECOND = 59;
	return MIN_SECOND <= wSecond && wSecond <= MAX_SECOND;
}

bool DateTime::IsValidMilliseconds(WORD wMilliseconds)
{
	const WORD MIN_MILLISECONDS = 0;
	const WORD MAX_MILLISECONDS = 999;
	return MIN_MILLISECONDS <= wMilliseconds && wMilliseconds <= MAX_MILLISECONDS;
}