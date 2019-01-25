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

	inline void GetMacro(const String &name, Uint32 &macrovalue) const;

	inline void SetMacro(const String &name, Uint32 macrovalue);

	void GetDefineStr(char *buf[], Uint32 &define_count) const;

};
