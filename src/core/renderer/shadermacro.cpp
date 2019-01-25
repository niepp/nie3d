#include "../core.h"
#include "shadermacro.h"

const char* ShaderMacro::macro_str[] = {
		"disfogon",
		"heightfogon",
		"lightmapon",
		"shadowmapon",
		"specon",
		"emissiveon",
		"uvanim",
		"normalon",
		"envspec",
		"enablemask",
};

void ShaderMacro::GetMacro(const String &name, Uint32 &macrovalue) const
{
	for (Uint32 i = 0; i < ArraySize(macro_str); ++i)
	{
		if (macro_str[i] && name == macro_str[i])
		{
			macrovalue = (m_macro_value >> i) & 0x1;
		}
	}
}

void ShaderMacro::SetMacro(const String &name, Uint32 macrovalue)
{
	for (Uint32 i = 0; i < ArraySize(macro_str); ++i)
	{
		if (macro_str[i] && name == macro_str[i])
		{
			m_macro_value |= (0x1 & macrovalue) << i;
		}
	}
}

void ShaderMacro::GetDefineStr(char *buf[], Uint32 &define_count) const
{
	Uint32 index = 0;
	for (Uint32 i = 0; i < ArraySize(macro_str); ++i)
	{
		Uint32 m = (m_macro_value >> i) & 0x1;
		if (m > 0)
		{
			sprintf(buf[index], "#define %s %d \n", macro_str[i], m);
			++index;
		}
	}
	define_count = index;
}

