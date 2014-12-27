#include "PerformanceMeasure.h"
#include "common.h"

/*!
 * \brief
 * The constructor of Timer.
 * 
 */
PerformanceMeasure::PerformanceMeasure()
 :	m_supportQueryPerformance(false),
	m_frequency(1),
	m_startPerformanceCount(0),
	m_endPerformanceCount(0),
	m_startTick(0),
	m_endTick(0),
	m_ElapsedTime(0.0),
	m_ElapsedTimeMS(0.0)
{
	LARGE_INTEGER li;
	if(QueryPerformanceFrequency(&li))
	{
		m_supportQueryPerformance = true;
		m_frequency = li.QuadPart;
	}
}

/*!
 * \brief
 * The destructor of Timer.
 * 
 */
PerformanceMeasure::~PerformanceMeasure()
{
}

/*!
 * \brief
 * Start the timer.
 * 
 */
void PerformanceMeasure::Start()
{
	m_ElapsedTime = 0.0;
	m_ElapsedTimeMS = 0.0;

	if(m_supportQueryPerformance)
	{
		LARGE_INTEGER li;
		if(QueryPerformanceCounter(&li))
			m_startPerformanceCount = li.QuadPart;
		else
			m_startPerformanceCount = 0;
	}
	else
	{
		m_startTick = GetTickCount();
	}
}

/*!
 * \brief
 * Stop the timer. The elapsed time will be recorded in m_ElapsedTime/m_ElapsedTimeMS.
 * 
 */
void PerformanceMeasure::Stop()
{
	if(m_supportQueryPerformance)
	{
		LARGE_INTEGER li;
		if(QueryPerformanceCounter(&li))		
			m_endPerformanceCount = li.QuadPart;
		else
			m_endPerformanceCount = 0;

		//Utility::DebugLog(L"[D:HTW][Timer] StartPerformaceCount = %lld . \r\n", m_startPerformanceCount);
		//Utility::DebugLog(L"[D:HTW][Timer] EndPerformaceCount = %lld . \r\n", m_endPerformanceCount);
		
		m_ElapsedTime = (m_endPerformanceCount - m_startPerformanceCount)/(double)m_frequency;
		m_ElapsedTimeMS = m_ElapsedTime * 1000.0;
	}
	else
	{
		m_endTick = GetTickCount();

		//Utility::DebugLog(L"[D:HTW][Timer] StartTick = %u (ms). \r\n", m_startTick);
		//Utility::DebugLog(L"[D:HTW][Timer] EndTick = %u (ms). \r\n", m_endTick);
		
		m_ElapsedTimeMS = m_endTick - m_startTick;
		m_ElapsedTime = m_ElapsedTimeMS / 1000.0;
	}
}