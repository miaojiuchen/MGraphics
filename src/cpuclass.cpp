////////////////////////////////////
//Filename:cpuclass.cpp
////////////////////////////////////

#include "cpuclass.h"

CpuClass::CpuClass()
{
}

CpuClass::CpuClass(const CpuClass &)
{
}

CpuClass::~CpuClass()
{
}

void CpuClass::Initialize()
{
	PDH_STATUS status;

	//initialize the flag indicating whether this object can read the system cpu usage or not
	m_canReadCpu = true;

	//create a query object to poll cpu usage
	status = PdhOpenQuery(0, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	//set query object to poll all cpus in the system
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}

void CpuClass::Shutdown()
{
	if (m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}
}

void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if (m_lastSampleTime + 1000 < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();
			PdhCollectQueryData(m_queryHandle);
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, 0, &value);

			m_cpuUsage = value.longValue;
		}
	}
}

int CpuClass::GetCpuPercentage()
{
	if (m_canReadCpu)
	{
		return (int)m_cpuUsage;
	}
	else
	{
		return 0;
	}
}