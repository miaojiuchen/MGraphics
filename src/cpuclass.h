////////////////////////////////////
// Filename:cpuclass.h
// The CpuClass will handle recording the cpu usage
// and display the current percentage of cpu use oo the screen
////////////////////////////////////

#ifndef _CPUCLASS_H_
#define _CPUCLASS_H_

#pragma comment(lib,"pdh.lib")

//includes
#include<Pdh.h>

class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass &);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;

};

#endif

