#pragma once
#include <Windows.h>
class ITask
{
public:
	ITask(DWORD delayTime = 0, DWORD taskID = 0) :
		m_TaskID(taskID), 
		m_DelayTime(delayTime), 
		m_ThreadCancel(false),
		m_IsRunning(FALSE)
	{
		
	}
	virtual ~ITask()
	{
		
	}
	virtual void Run() { }
	void Cancel() { m_ThreadCancel = true; }
	inline bool IsCancel(){ return m_ThreadCancel; }
	inline int GetID() const { return m_TaskID; }
	inline int GetDelayTime() const { return m_DelayTime; }
	
	volatile BOOL m_IsRunning;
protected:

	bool m_ThreadCancel;
	DWORD m_TaskID;

private:
	DWORD m_DelayTime;


};