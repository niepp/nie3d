#include "platform.h"

#if PLATFORM == PLATFORM_ANDROID
#include <android/log.h>
#endif

#include "core.h"
#include "log.h"

LogFile::LogFile(const char* strFileName)
{
	m_fp = ::fopen(strFileName, "w");
}

LogFile::~LogFile()
{
	if (m_fp) {
		::fclose(m_fp);
	}
}

void LogFile::Write(const char *str)
{
	Uint32 len = strlen(str);
	fwrite(str, 1, len, m_fp);
	::fflush(m_fp);
}

LogFile g_log_file("log.txt");

LogUtils* LogUtils::Instance()
{
	static LogUtils sLogUtils;
	return &sLogUtils;
}

void LogUtils::LogInfo(const char *fmt, ...)
{
	char s[1024] = "";
	va_list argList;
	va_start(argList, fmt);
	vsnprintf(s, 1024, fmt, argList);
	va_end(argList);

	Uint32 len = strlen(s);
//	assert(len < 1022);
	s[len++] = '\n';
	s[len] = '\0';

#if PLATFORM == PLATFORM_WIN32
	g_log_file.Write(s);
	printf(s);
#elif PLATFORM == PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "Info", s);
#endif
}
