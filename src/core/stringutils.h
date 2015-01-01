#pragma once
#include <string>

#pragma warning(disable : 4996)

inline String ToString(const Int32& inVal)
{
	String str;
	str.FormatInput("%d", inVal);
	return str;
}

inline String ToString(const Uint32& inVal)
{
	String str;
	str.FormatInput("%d", inVal);
	return str;
}

inline String ToString(const Float& inVal)
{
	String str;
	str.FormatInput("%.3f", inVal);
	return str;
}

inline String ToString(const Double& inVal)
{
	String str;
	str.FormatInput("%.6lf", inVal);
	return str;
}

inline String ToString(const Vector2& inVal)
{
	String str;
	str.FormatInput("%.3f,%.3f", inVal.x, inVal.y);
	return str;
}

inline String ToString(const Vector3& inVal)
{
	String str;
	str.FormatInput("%.3f,%.3f,%.3f", inVal.x, inVal.y, inVal.z);
	return str;
}

inline String ToString(const Vector4& inVal)
{
	String str;
	str.FormatInput("%.3f,%.3f,%.3f,%.3f", inVal.x, inVal.y, inVal.z, inVal.w);
	return str;
}

inline String ToString(const Quaternion& inVal)
{
	String str;
	str.FormatInput("%.3f,%.3f,%.3f,%.3f", inVal.x, inVal.y, inVal.z, inVal.w);
	return str;
}

inline String ToString(const Bool& inVal)
{
	static const String strBool[] = {"false", "true"};
	return strBool[inVal];
}

inline String ToString(const String& inVal)
{
	return inVal;
}

inline String ToString(const std::string& inVal)
{
	return inVal.c_str();
}

inline String ToString(const char *inVal)
{
	return inVal;
}

inline String ToString(const Color& inVal)
{
	String ret;
	ret.FormatInput("%d", inVal.c);
	return ret;
}

inline Bool StringTo(const String& inVal, Bool& outVal)
{
	if (inVal == "true")
	{
		outVal = true;
		return true;
	}
	else if (inVal == "false")
	{
		outVal = false;
		return true;
	}
	return false;
}

inline Bool StringTo(const String& inVal, Int32& outVal)
{
	outVal = (Int32)atoi(inVal.AsCharPtr());
	return true;
}

inline Bool StringTo(const String& inVal, Uint32& outVal)
{
	outVal = (Uint32)atoi(inVal.AsCharPtr());
	return true;
}

inline Bool StringTo(const String& inVal, Float& outVal)
{
	outVal = (Float)(atof(inVal.AsCharPtr()));
	return true;
}

inline Bool StringTo(const String& inVal, Double& outVal)
{
	outVal = (Double)(atof(inVal.AsCharPtr()));
	return true;
}

inline Bool StringTo(const String& inVal, Vector2& outVal)
{
	const char *buf = inVal.AsCharPtr();
	const char *p = strchr(buf, ',');
	if (p == 0) {
		return false;
	}

	char tmp[64];
	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.x);
	StringTo(p + 1, outVal.y);

	return true;
}

inline Bool StringTo(const String& inVal, Vector3& outVal)
{
	const char *buf = inVal.AsCharPtr();
	const char *p = strchr(buf, ',');
	if (p == 0) {
		return false;
	}

	char tmp[64];
	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.x);

	buf = p + 1;
	p = strchr(buf, ',');
	if (p == 0)	{
		return false;
	}

	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.y);

	StringTo(p + 1, outVal.z);
	return true;
}

inline Bool StringTo(const String& inVal, Vector4& outVal)
{
	const char *buf = inVal.AsCharPtr();
	const char *p = strchr(buf, ',');
	if (p == 0) {
		return false;
	}

	char tmp[64];
	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.x);

	// y
	buf = p + 1;
	p = strchr(buf, ',');
	if (p == 0)	{
		return false;
	}

	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.y);

	// z
	buf = p + 1;
	p = strchr(buf, ',');
	if (p == 0)	{
		return false;
	}

	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.y);

	// w
	StringTo(p + 1, outVal.w);
	return true;
}

inline Bool StringTo(const String& inVal, Quaternion& outVal)
{
	const char *buf = inVal.AsCharPtr();
	const char *p = strchr(buf, ',');
	if (p == 0) {
		return false;
	}

	char tmp[64];
	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.x);

	// y
	buf = p + 1;
	p = strchr(buf, ',');
	if (p == 0)	{
		return false;
	}

	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.y);

	// z
	buf = p + 1;
	p = strchr(buf, ',');
	if (p == 0)	{
		return false;
	}

	strncpy(tmp, buf, p - buf);
	tmp[p - buf] = 0;
	StringTo(tmp, outVal.y);

	// w
	StringTo(p + 1, outVal.w);
	return true;
}

inline Bool StringTo(const String& inVal, String& outVal)
{
	outVal = inVal;
	return true;
}

inline Bool StringTo(const String& inVal, std::string& outVal)
{
	outVal = inVal.AsCharPtr();
	return true;
}

inline Bool StringTo(const String& inVal, const char* outVal)
{
	outVal = inVal.AsCharPtr();
	return true;
}

inline Bool StringTo(const String& inVal, Color& outVal)
{
	outVal.c = atoi(inVal.AsCharPtr());
	return true;
}
