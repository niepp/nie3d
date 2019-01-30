#pragma once
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>

#pragma warning(disable : 4996)

class CORE_API String
{
public:
	String();
	~String();
	String(const char *pstr);
	String(const char *pstr, Uint32 len);
	String(const String &rhs);
	String& operator =(const char *pstr);
	String& operator =(const String &rhs);
	String& operator +=(const char *pstr);
	String& operator +=(const String &rhs);
	String& operator +=(char c);
	char operator[](Uint32 index) const;
	char& operator[](Uint32 index);
	bool operator==(const String &rhs) const;
	bool operator==(const char* pstr)const;
	bool operator!=(const String &rhs) const;
	bool operator<(const String &rhs) const;
	bool operator>(const String &rhs) const;
	bool Equals(const String& rhs, bool isCaseSensitive = true) const;
	Uint32 GetLength() const;
	bool IsEmpty() const;
	const char* AsCharPtr() const;
	String SubString(Uint32 begin = 0, Uint32 end = cInvalidIndex) const; // string index is [begin, end)
	void SubStringToSelf(Uint32 begin = 0, Uint32 end = cInvalidIndex);
	Uint32 FindChar(char c) const;
	Uint32 RFindChar(char c) const;
	void ReplaceChar(char from, char to);
	Uint32 FindStr(const char *str) const;
	void FormatInput(const char *pfmtstr, ...);
	void ToLower();
	void ToUpper();

private:
	void Resize(Uint32 nchar, bool keepdata = true);
	void InitBufferWhenConstruct();

private:
	static const Uint32 LocalBufferSize = 32;
	char *m_buffer;
	char m_local_buffer[LocalBufferSize];
	Uint32 m_char_count;
};

inline String operator +(const String &str1, const String &str2)
{
	String result(str1);
	result += str2;
	return result;
}

inline String operator +(const String &str, const char *pstr)
{
	String result(str);
	result += pstr;
	return result;
}

inline String operator +(const char *pstr, const String &str)
{
	String result(pstr);
	result += str;
	return result;
}

inline String::String()
{
	InitBufferWhenConstruct();
}

inline String::String(const char *pstr)
{
	InitBufferWhenConstruct();
	*this = pstr;
}

inline String::String(const char *pstr, Uint32 len)
{
	InitBufferWhenConstruct();
	if(pstr == NULL)
	{
		Resize(0, false);
	}
	else
	{
		Resize(len, false);
		strncpy(m_buffer, pstr, len);
	}
}

inline String::String(const String &rhs)
{
	InitBufferWhenConstruct();
	*this = rhs;
}

inline String::~String()
{
	if (m_buffer != m_local_buffer)
	{
		delete(m_buffer);
	}
	m_buffer = m_local_buffer;
	m_char_count = 0;
}

inline String& String::operator =(const char *pstr)
{
	if(pstr == NULL)
	{
		Resize(0, false);
	}
	else
	{
		Uint32 len = strlen(pstr);
		Resize(len, false);
		strncpy(m_buffer, pstr, len);
	}
	return *this;
}

inline String& String::operator =(const String &rhs)
{
	Resize(rhs.m_char_count, false);
	strncpy(m_buffer, rhs.m_buffer, m_char_count);
	return *this;
}

inline String& String::operator +=(const char *pstr)
{
	String tmp(pstr);
	*this += tmp;
	return *this;
}

inline String& String::operator +=(const String &rhs)
{
	Uint32 pos = m_char_count;
	Resize(m_char_count + rhs.m_char_count);
	strncpy(m_buffer + pos, rhs.m_buffer, rhs.m_char_count);
	m_buffer[m_char_count] = 0;
	return *this;
}

inline String& String::operator +=(char c)
{
	Resize(m_char_count + 1);
	m_buffer[m_char_count - 1] = c;
	m_buffer[m_char_count] = 0;
	return *this;
}

inline char String::operator[](Uint32 index) const
{
	assert(index < m_char_count);
	return m_buffer[index];
}

inline char& String::operator[](Uint32 index)
{
	assert(index < m_char_count);
	return m_buffer[index];
}

inline Bool String::operator==(const String &rhs) const
{
	return (strcmp(m_buffer, rhs.m_buffer) == 0);
}

inline Bool String::operator==(const char* pstr) const
{
	return (strcmp(m_buffer, pstr) == 0); 
}

inline Bool String::operator!=(const String &rhs) const
{
	return (strcmp(m_buffer, rhs.m_buffer) != 0);
}

inline Bool String::operator<( const String &rhs ) const
{
	return (strcmp(m_buffer, rhs.m_buffer) < 0);
}

inline Bool String::operator>( const String &rhs ) const
{
	return (strcmp(m_buffer, rhs.m_buffer) > 0);
}

inline Bool String::Equals(const String& rhs, bool isCaseSensitive /*= true*/) const
{
	if (isCaseSensitive)
	{
		return *this == rhs;
	}
	else
	{
		Uint32 len = GetLength();
		if (len != rhs.GetLength()) {
			return false;
		}
		else {
			for (Uint32 i = 0; i < len; ++i)
			{
				char a = (char)tolower(m_buffer[i]);
				char b = (char)tolower(rhs[i]);
				if (a != b) {
					return false;
				}
			}
			return true;
		}
	}
}

inline Uint32 String::GetLength() const
{
	return m_char_count;
}

inline Bool String::IsEmpty() const
{
	return m_char_count == 0;
}

inline const char* String::AsCharPtr() const
{
	return m_buffer;
}

inline String String::SubString(Uint32 begin, Uint32 end) const
{
	if(end > m_char_count) {
		end = m_char_count;
	}

	String result;
	if(begin < end)
	{
		Uint32 nchar = end - begin;
		result.Resize(nchar, false);
		memcpy(result.m_buffer, m_buffer + begin, nchar * sizeof(char));
	}
	return result;
}

inline void String::SubStringToSelf(Uint32 begin, Uint32 end)
{
	if(end > m_char_count) {
		end = m_char_count;
	}

	if(begin <= end)
	{
		Uint32 nchar = end - begin;
		if(nchar){
			memmove(m_buffer, m_buffer + begin, nchar * sizeof(char));
		}
		m_buffer[nchar] = 0;
		m_char_count = nchar;
	}
}

inline Uint32 String::FindChar(char c) const
{
	if (m_char_count == 0)
	{
		return cInvalidIndex;
	}

	Uint32 index = cInvalidIndex;
	for(Uint32 i = 0; i < m_char_count; ++i)
	{
		if(m_buffer[i] == c ) {
			index = i;
			break;
		}
	}
	return index;
}

inline Uint32 String::RFindChar(char c) const
{
	if (m_char_count == 0)
	{
		return cInvalidIndex;
	}

	Uint32 index = cInvalidIndex;
	for(Int32 i = m_char_count - 1; i >= 0; --i)
	{
		if(m_buffer[i] == c ) {
			index = i;
			break;
		}
	}
	return index;
}

inline void String::ReplaceChar(char from, char to)
{
	for (Uint32 i = 0; i <= m_char_count; ++i)
	{
		if (m_buffer[i] == from)
		{
			m_buffer[i] = to;
		}
	}
}

inline Uint32 String::FindStr(const char *str) const
{
	assert(str != NULL);
	if(m_char_count == 0)
	{
		return cInvalidIndex;
	}
	const char* p = strstr(m_buffer, str);
	return (p == NULL) ? cInvalidIndex : (p - m_buffer);
}

inline void String::FormatInput(const char *pfmtstr, ...)
{
	va_list	argptr;
	va_start(argptr, pfmtstr);
	char buf[1024] = {0};
	vsnprintf(buf, sizeof(buf), pfmtstr, argptr);
	va_end(argptr);
	*this = buf;
}

inline void String::ToLower()
{
	for (Uint32 i = 0; i < m_char_count; ++i)
	{
		m_buffer[i] = (char)tolower(m_buffer[i]);
	}
}

inline void String::ToUpper()
{
	for (Uint32 i = 0; i < m_char_count; ++i)
	{
		m_buffer[i] = (char)toupper(m_buffer[i]);
	}
}

inline void String::Resize(Uint32 nchar, bool keepdata)
{
	if(nchar + 1 > LocalBufferSize)
	{
		char *pbuf = (char *)new char[(nchar + 1) * sizeof(char)];
		if(keepdata && m_char_count > 0)
		{
			strncpy(pbuf, m_buffer, m_char_count);
		}
		if (m_buffer != m_local_buffer)
		{
			delete(m_buffer);
		}
		m_buffer = pbuf;
	}

	m_char_count = nchar;
	m_buffer[m_char_count] = 0;
}

inline void String::InitBufferWhenConstruct()
{
	m_buffer = m_local_buffer;
	m_buffer[0] = 0;
	m_char_count = 0;
}
