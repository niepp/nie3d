#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

#if PLATFORM == PLATFORM_WIN32
#include <windows.h>
#elif PLATFORM == PLATFORM_ANDROID
#include <sys/time.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_IOS
#include <mach/mach_time.h>
#endif

class CORE_API Timer
{
public:
	Timer();
	void Reset();
	Float GetElapseMilliseconds() const;
	Float GetElapseSeconds() const;
private:
	Int64 GetTick() const;
private:
	Int64 m_start_count;
	static Double m_reci_freq;
};

#endif // __TIMER_H__
