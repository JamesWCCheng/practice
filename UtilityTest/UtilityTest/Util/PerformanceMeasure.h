#ifndef _PERFORMANCEMEASURE_H_
#define _PERFORMANCEMEASURE_H_

#include <windows.h>

/*!
 * \brief
 * An utility to evalutae the performance of the execution time.
 */
class PerformanceMeasure
{
public:
	PerformanceMeasure();
	~PerformanceMeasure();

	void Start();
	void Stop();

	double m_ElapsedTime;	// second.
	double m_ElapsedTimeMS;	// millisecond.

private:

	// For QueryPerformanceFrequency / QueryPerformanceCounter
	bool m_supportQueryPerformance;
	long long m_frequency;
	long long m_startPerformanceCount;
	long long m_endPerformanceCount;

	// For GetTickCount
	DWORD	m_startTick;
	DWORD	m_endTick;
};

#endif