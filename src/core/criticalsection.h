#pragma once

#if PLATFORM != PLATFORM_WIN32
#include <pthread.h>
typedef pthread_mutex_t CRITICAL_SECTION;
typedef CRITICAL_SECTION * LPCRITICAL_SECTION;
#define InitializeCriticalSection(X) pthread_mutex_init(X, NULL)
#define DeleteCriticalSection(X) pthread_mutex_destroy(X)
#define EnterCriticalSection(X) pthread_mutex_lock(X)
#define LeaveCriticalSection(X) pthread_mutex_unlock(X)
#endif

class CORE_API CriticalSection
{
public:
	CriticalSection(void){
		::InitializeCriticalSection(&m_sect);
	}
	~CriticalSection(void){
		::DeleteCriticalSection(&m_sect);
	}
	
	void Enter() const {
		::EnterCriticalSection(&m_sect); 
	}

	void Leave() const {
		::LeaveCriticalSection(&m_sect); 
	}
private:
	mutable CRITICAL_SECTION m_sect;
};
