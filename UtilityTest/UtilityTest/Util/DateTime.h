#pragma once
// based on UTC
class DateTime
{
public:
	~DateTime();
private:
	DateTime(SYSTEMTIME systemTime);

public:
	WORD GetYear() const { return m_systemTime.wYear; }
	WORD GetMonth() const { return m_systemTime.wMonth; }
	WORD GetDayOfWeek() const { return m_systemTime.wDayOfWeek; }
	WORD GetDay() const { return m_systemTime.wDay; }
	WORD GetHour() const { return m_systemTime.wHour; }
	WORD GetMinute() const { return m_systemTime.wMinute; }
	WORD GetSecond() const { return m_systemTime.wSecond; }
	WORD GetMilliseconds() const { return m_systemTime.wMilliseconds; }

	void SetYear(WORD wYear);
	void SetMonth(WORD wMonth);
	void SetDay(WORD wDay);
	void SetHour(WORD wHour);
	void SetMinute(WORD wMinute);
	void SetSecond(WORD wSecond);
	void SetMilliseconds(WORD wMilliseconds);

	void AddMinute(WORD wMinute);
	void AddSecond(WORD wSecond);
	void AddMilliseconds(WORD wMilliseconds);

	SYSTEMTIME ToSystemTime() const { return m_systemTime; }
	FILETIME ToFileTime() const;
	ULARGE_INTEGER ToLargeInteger() const;
	DateTime ToLocalDateTime() const;

public:
	static DateTime Now();
	static ULARGE_INTEGER Interval(const DateTime& dtBegin, const DateTime& dtEnd);

private:
	bool IsValidYear(WORD wYear);
	bool IsValidMonth(WORD wMonth);
	bool IsValidDayOfWeek(WORD wDayOfWeek);
	bool IsValidDay(WORD wDay);
	bool IsValidHour(WORD wHour);
	bool IsValidMinute(WORD wMinute);
	bool IsValidSecond(WORD wSecond);
	bool IsValidMilliseconds(WORD wMilliseconds);

private:
	SYSTEMTIME m_systemTime;
};