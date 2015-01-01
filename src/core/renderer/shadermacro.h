#pragma once

struct CORE_API ShaderMacro
{
	Uint64 m_macro_value;

	static const Uint32 macro_count = 11;
	static const char* macro_str[];

	ShaderMacro() { m_macro_value = 0; }

	inline bool operator<(const ShaderMacro &other) const 
	{
		return m_macro_value < other.m_macro_value;
	}

	inline bool operator!=(const ShaderMacro &other) const
	{
		return m_macro_value != other.m_macro_value;
	}

	inline bool operator==(const ShaderMacro &other) const
	{
		return m_macro_value == other.m_macro_value;
	}

	inline void Reset()
	{
		m_macro_value = 0;
	}

	inline void GetMacro(const String &name, Uint32 &macrovalue) const
	{
		for (Uint32 i = 0; i < macro_count; ++i)
		{
			if(macro_str[i] && name == macro_str[i])
			{
				macrovalue = (m_macro_value >> i) & 0x1;
			}
		}
	}

	inline void SetMacro(const String &name, Uint32 macrovalue)
	{
		for (Uint32 i = 0; i < macro_count; ++i)
		{
			if(macro_str[i] && name == macro_str[i])
			{
				m_macro_value |= (0x1 & macrovalue) << i;
			}
		}	
	}

	void GetDefineStr(char *buf[], Uint32 &define_count) const
	{
		Uint32 index = 0;
		for (Uint32 i = 0; i < macro_count; ++i)
		{
			Uint32 m = (m_macro_value >> i) & 0x1;
			if(m > 0)
			{
				sprintf(buf[index], "#define %s %d \n", macro_str[i], m);
				++index;
			}
		}
		define_count = index;
	}

};
