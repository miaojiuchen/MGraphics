////////////////////////////////////
// Filename:timerclass.h
// This is a high precision timer, we can use it for timing events 
// and ensuring our application and its various components all synchronize 
// to a common time frame
////////////////////////////////////

#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

//includes
#include<Windows.h>

class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass &);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;

};

#endif

