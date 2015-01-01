#include "core.h"
#include "timer.h"

Double Timer::m_reci_freq = 0;

Timer::Timer()
{
	if (!m_reci_freq) {
#if PLATFORM == PLATFORM_WIN32
		LARGE_INTEGER temp;
		::QueryPerformanceFrequency(&temp);
		m_reci_freq = (Double)(1000.0 / temp.QuadPart);
#elif PLATFORM == PLATFORM_ANDROID
		m_reci_freq = 1.0 / 1000.0;
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_IOS
		mach_timebase_info_data_t sTimebaseInfo;
		mach_timebase_info(&sTimebaseInfo);
		m_reci_freq = Double(sTimebaseInfo.numer) / (sTimebaseInfo.denom * 1000000.0);
#endif
	}
	Reset();
}

Int64 Timer::GetTick() const
{
	Int64 tick = 0;
#if PLATFORM == PLATFORM_WIN32
	::QueryPerformanceCounter((LARGE_INTEGER*)&tick);
#elif PLATFORM == PLATFORM_ANDROID
	struct timeval t_current;
	gettimeofday(&t_current, NULL);
	tick = ((long)t_current.tv_sec) * 1000000 + (long)t_current.tv_usec;
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_IOS
	tick = mach_absolute_time();
#endif
	return tick;
}

void Timer::Reset()
{
	m_start_count = GetTick();
}

Float Timer::GetElapseMilliseconds() const
{
	Int64 tick = GetTick();
	return (Float)((tick - m_start_count) * m_reci_freq);
}

Float Timer::GetElapseSeconds() const
{
	Int64 tick = GetTick();
	return (Float)((tick - m_start_count) * m_reci_freq / 1000.0f);
}
