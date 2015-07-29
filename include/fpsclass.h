////////////////////////////////////
// Filename:fpsclass.h
// The FpsClass will handle recording the frames 
// per second that the application is running at
////////////////////////////////////

#ifndef _FPSCLASS_H_
#define _FPSCLASS_H_

//linking instructions
#pragma comment(lib,"winmm.lib")

//includes
#include<Windows.h>
#include<mmsystem.h>

class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass &);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;

};

#endif

