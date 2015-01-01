#pragma once

#ifndef __REF_OBJECT_H__
#define __REF_OBJECT_H__

#if PLATFORM == PLATFORM_WIN32
	#include <windows.h>
#elif PLATFORM == PLATFORM_ANDROID
	#include <sys/atomics.h>
#elif PLATFORM == PLATFORM_IOS
	#include <libkern/OSAtomic.h>
#endif

class CORE_API RefObject
{
public:
	RefObject():
		m_ref_count(0)
	{}
	
	virtual ~RefObject()
	{
	}

	void IncRefCount() const
	{
#if PLATFORM == PLATFORM_WIN32
		InterlockedIncrement((LONG*)&m_ref_count);
#elif PLATFORM == PLATFORM_ANDROID
		__atomic_inc(static_cast<int32_t*>(&m_ref_count));
#elif PLATFORM == PLATFORM_IOS
		OSAtomicIncrement32(static_cast<Int32*>(&m_ref_count));
#endif
	}

	void DecRefCount() const
	{
#if PLATFORM == PLATFORM_WIN32
		InterlockedDecrement((LONG*)&m_ref_count);
#elif PLATFORM == PLATFORM_ANDROID
		__atomic_dec(static_cast<int32_t*>(&m_ref_count));
#elif PLATFORM == PLATFORM_IOS
		OSAtomicDecrement32(static_cast<Int32*>(&m_ref_count));
#endif
	}

	Int32 GetRefCount() const
	{
		return m_ref_count;
	}

protected:
#if PLATFORM == PLATFORM_WIN32
	volatile mutable Int32 m_ref_count;
#elif PLATFORM == PLATFORM_ANDROID || PLATFORM == PLATFORM_IOS
	mutable Int32 m_ref_count;
#endif
};

#endif // __REF_OBJECT_H__
