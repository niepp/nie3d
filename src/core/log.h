#pragma once
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

class CORE_API LogFile
{
public:
	LogFile(const char* strFileName);
	~LogFile();

	void Write(const char *str);

private:
	FILE *m_fp;
};

class CORE_API LogUtils {
public:
	static LogUtils* Instance();
	void LogInfo(const char *fmt, ...);

};

