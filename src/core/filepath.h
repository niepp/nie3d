#pragma once

#include <sys/stat.h>   

class CORE_API Filepath
{
public:
	inline Filepath(const char *str);
	inline Filepath(const String &str);
	inline bool operator<(const Filepath &other) const;
	inline const char* AsCharPtr() const;
	inline const String& AsString() const;
	inline Bool IsExist() const;
	inline char* GetLastSlash() const;
	inline String GetFileExtension() const;
	inline void StripFileExtension();
	inline void ExtractFullDirName();
private:	
	inline void Standardize();

private:
	String m_file_path;
};

Filepath::Filepath(const char *str) : m_file_path(str)
{
#if PLATFORM == PLATFORM_WIN32
	Standardize();
#endif
}

Filepath::Filepath(const String &str) : m_file_path(str)
{
#if PLATFORM == PLATFORM_WIN32
	Standardize();
#endif

}

bool Filepath::operator<(const Filepath &other) const
{
	return m_file_path < other.m_file_path;
}

const char* Filepath::AsCharPtr() const
{
	return m_file_path.AsCharPtr();
}

const String& Filepath::AsString() const
{
	return m_file_path;
}

Bool Filepath::IsExist() const
{
	struct stat buf;
	int ret = stat(m_file_path.AsCharPtr(), &buf);
	return (ret == 0) && (buf.st_mode & S_IFREG);
}

char* Filepath::GetLastSlash() const
{
	const char* s = this->AsCharPtr();
	const char* lastSlash = strrchr(s, '/');
	if (0 == lastSlash) lastSlash = strrchr(s, '\\');
	return const_cast<char*>(lastSlash);
}

String Filepath::GetFileExtension() const
{
	const char* str = m_file_path.AsCharPtr();
	const char* ext = strrchr(str, '.');
	if (ext != NULL)
	{
		return String(ext + 1);
	}
	return "";
}

void Filepath::StripFileExtension()
{
	const char* str = m_file_path.AsCharPtr();
	char* ext = const_cast<char*>(strrchr(str, '.'));
	if (ext != NULL)
	{
		*ext = 0;
		m_file_path = str;
	}
}

void Filepath::ExtractFullDirName()
{
	char* lastSlash = GetLastSlash();
	// special case if path ends with a slash
	if (lastSlash) {
		if (0 == lastSlash[1]) {
			*lastSlash = 0;
			lastSlash = GetLastSlash();
		}

		if (lastSlash) {
			*++lastSlash = 0;
		}
	}
	else {
		m_file_path = "/";
	}

}

void Filepath::Standardize()
{
	m_file_path.ToLower();
	m_file_path.ReplaceChar('\\', '/');
}
