#pragma once

enum VariantType
{
	cVarNull,
	cVarBool,
	cVarInt,
	cVarFloat,
	cVarFloat2,
	cVarFloat3,
	cVarFloat4,
	cVarMatrix,
	cVarColor,
	cVarString,
	cVarObject,

	cVarTypeCount,
};

class CORE_API Variant
{
public:
	Variant();
	Variant(const Variant &rhs);
	Variant(Bool b);
	Variant(Int32 i);
	Variant(Float f);
	Variant(const Vector2 &v);
	Variant(const Vector3 &v);
	Variant(const Vector4 &v);
	Variant(const Matrix &m);
	Variant(const Color &c);
	Variant(const char *s);
	Variant(RefObject *obj);
	~Variant();

	Variant& operator=(const Variant &rhs);
	void Set(VariantType type, const char *pvalue = NULL);
	void SetBool(Bool b);
	void SetInt(Int32 i);
	void SetFloat(Float f);
	void SetFloat2(const Vector2 &v);
	void SetFloat3(const Vector3 &v);
	void SetFloat4(const Vector4 &v);
	void SetMatrix(const Matrix &m);
	void SetColor(const Color &c);
	void SetString(const char *s);
	void SetObjectPtr(RefObject *obj);

	VariantType GetType() const;
	Bool IsValid()const;
	Bool GetBool() const;
	Int32 GetInt() const;
	Float GetFloat() const;
	const Vector2& GetFloat2() const;
	const Vector3& GetFloat3() const;
	const Vector4& GetFloat4() const;
	const Matrix& GetMatrix() const;
	const Color& GetColor() const;
	const char* GetString() const;
	RefObject* GetObjectPtr() const;

	void Clear();

private:
	void SetRawValue(const char *pvalue);

public:
	static const char* TypeToName(VariantType type);
	static VariantType NameToType(const char *pname);

private:
	VariantType m_type;
	union
	{
		Bool m_b;
		Int32 m_i;
		Float m_f;
		Float m_vec[16];
		char m_str[256];
		RefObject* m_obj;
	};
};

inline Variant::Variant()
	: m_type(cVarNull)
{
	m_b = false;
	m_i = 0;
	m_f = 0;
	memset(m_str, 0, sizeof(m_str));
	m_obj = NULL;
}

inline Variant::Variant(const Variant &rhs)
	: m_type(rhs.m_type)
{
	m_type = rhs.m_type;
	if (m_type == cVarString)
	{
		::strcpy(m_str, rhs.m_str);
	}
	else if (m_type == cVarObject)
	{
		m_obj = rhs.m_obj;
		if (m_obj)
		{
			m_obj->IncRefCount();
		}
	}
	else
	{
		memcpy(m_vec, rhs.m_vec, sizeof(m_vec));
	}

}

inline Variant::Variant(Bool b)
	: m_type(cVarBool)
	, m_b(b)
{
}

inline Variant::Variant(Int32 i)
	: m_type(cVarInt)
	, m_i(i)
{
}

inline Variant::Variant(Float f)
	: m_type(cVarFloat)
	, m_f(f)
{
}

inline Variant::Variant(const Vector2 &v)
: m_type(cVarFloat2)
{
	m_vec[0] = v.x;
	m_vec[1] = v.y;
}

inline Variant::Variant(const Vector3 &v)
	: m_type(cVarFloat3)
{
	m_vec[0] = v.x;
	m_vec[1] = v.y;
	m_vec[2] = v.z;
}

inline Variant::Variant(const Vector4 &v)
	: m_type(cVarFloat4)
{
	m_vec[0] = v.x;
	m_vec[1] = v.y;
	m_vec[2] = v.z;
	m_vec[3] = v.w;
}

inline Variant::Variant(const Matrix &m)
	: m_type(cVarMatrix)
{
	for (Uint32 i = 0; i < 16; ++i)
	{
		m_vec[i] = m.v[i];
	}
}

inline Variant::Variant(const Color &c)
	: m_type(cVarColor)
{
	m_vec[0] = c.r;
	m_vec[1] = c.g;
	m_vec[2] = c.b;
	m_vec[3] = c.a;
}

inline Variant::Variant(const char *s)
	: m_type(cVarString)
{
	Uint32 len = strlen(s);
	assert(len < 64);
	::strncpy(m_str, s, Max<Uint32>(len, 63));
}

inline Variant::Variant(RefObject *obj)
	: m_type(cVarObject)
{
	m_obj = obj;
	if (m_obj != NULL)
	{
		m_obj->IncRefCount();
	}
}

inline Variant::~Variant()
{
	Clear();
}

inline Variant& Variant::operator=(const Variant &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	Clear();
	m_type = rhs.m_type;
	if (m_type == cVarString)
	{
		::strcpy(m_str, rhs.m_str);
	}
	else if (m_type == cVarObject)
	{
		m_obj = rhs.m_obj;
		if (m_obj)
		{
			m_obj->IncRefCount();
		}
	}
	else
	{
		memcpy(m_vec, rhs.m_vec, sizeof(m_vec));
	}

	return *this;
}

inline void Variant::Clear()
{
	if (m_type == cVarObject)
	{
		m_obj->DecRefCount();
		m_obj = NULL;
	}
	
}

inline void Variant::SetBool(Bool b)
{
	Clear();
	m_type = cVarBool;
	m_b = b;
}

inline void Variant::SetInt(Int32 i)
{
	Clear();
	m_type = cVarInt;
	m_i = i;
}

inline void Variant::SetFloat(Float f)
{
	Clear();
	m_type = cVarFloat;
	m_f = f;
}

inline void Variant::SetFloat2(const Vector2 &v)
{
	Clear();
	m_type = cVarFloat2;
	m_vec[0] = v.x;
	m_vec[1] = v.y;
}

inline void Variant::SetFloat3(const Vector3 &v)
{
	Clear();
	m_type = cVarFloat3;
	m_vec[0] = v.x;
	m_vec[1] = v.y;
	m_vec[2] = v.z;
}

inline void Variant::SetFloat4(const Vector4 &v)
{
	Clear();
	m_type = cVarFloat4;
	m_vec[0] = v.x;
	m_vec[1] = v.y;
	m_vec[2] = v.z;
	m_vec[3] = v.w;
}

inline void Variant::SetMatrix(const Matrix &m)
{
	Clear();
	m_type = cVarMatrix;
	for (Uint32 i = 0; i < 16; ++i)
	{
		m_vec[i] = m.v[i];
	}
}

inline void Variant::SetColor(const Color &c)
{
	Clear();
	m_type = cVarColor;
	m_vec[0] = c.r;
	m_vec[1] = c.g;
	m_vec[2] = c.b;
	m_vec[3] = c.a;
}

inline void Variant::SetString(const char *s)
{
	Clear();
	m_type = cVarString;
	Uint32 len = strlen(s);
	assert(len < 64);
	::strncpy(m_str, s, Max<Uint32>(len, 63));
}

inline void Variant::SetObjectPtr(RefObject *obj)
{
	if (m_type == cVarObject)
	{
		if (m_obj == obj)
		{
			return;
		}
		m_obj->DecRefCount();
	}
	m_type = cVarObject;
	m_obj = obj;
	if (m_obj != NULL)
	{
		m_obj->IncRefCount();
	}
}

inline void Variant::Set(VariantType type, const char *pvalue)
{
	Clear();
	m_type = type;
	SetRawValue(pvalue);
}

inline VariantType Variant::GetType() const
{
	return m_type;
}

inline Bool Variant::IsValid()const
{
	return m_type != cVarNull;
}

inline Bool Variant::GetBool() const
{
	return m_b;
}

inline Int32 Variant::GetInt() const
{
	return m_i;
}

inline Float Variant::GetFloat() const
{
	return m_f;
}

inline const Vector2& Variant::GetFloat2() const
{
	return *(Vector2*)m_vec;
}

inline const Vector3& Variant::GetFloat3() const
{
	return *(Vector3*)m_vec;
}

inline const Vector4& Variant::GetFloat4() const
{
	return *(Vector4*)m_vec;
}

inline const Matrix& Variant::GetMatrix() const
{
	return *(Matrix*)m_vec;
}

inline const Color& Variant::GetColor() const
{
	return *(Color*)m_vec;
}

inline const char* Variant::GetString() const
{
	return m_str;
}

inline RefObject* Variant::GetObjectPtr() const
{
	return m_obj;
}

inline void Variant::SetRawValue(const char *pvalue)
{
	Clear();
	switch (m_type)
	{
	case cVarBool:
		if (strcmp(pvalue, "true") == 0) {
			m_b = true;
		}
		else{
			m_b = false;
		}
		break;
	case cVarInt:
		m_i = atoi(pvalue);
		break;
	case cVarFloat:
		m_f = (float)atof(pvalue);
		break;
	case cVarFloat2:
		sscanf(pvalue, "%f%f", &m_vec[0], &m_vec[1]);
		break;
	case cVarFloat3:
		sscanf(pvalue, "%f%f%f", &m_vec[0], &m_vec[1], &m_vec[2]);
		break;
	case cVarFloat4:
	case cVarColor:
		sscanf(pvalue, "%f%f%f%f", &m_vec[0], &m_vec[1], &m_vec[2], &m_vec[3]);
		break;
	case cVarMatrix:
		sscanf(pvalue, "%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f", &m_vec[0], &m_vec[1], &m_vec[2], &m_vec[3],
			&m_vec[4], &m_vec[5], &m_vec[6], &m_vec[7], &m_vec[8], &m_vec[9], &m_vec[10], &m_vec[11],
			&m_vec[12], &m_vec[13], &m_vec[14], &m_vec[15]);
		break;
	case cVarString:
		{
			Uint32 len = strlen(pvalue);
			assert(len < 64);
			::strncpy(m_str, pvalue, Max<Uint32>(len, 63));
		}
		break;
	case cVarNull:
		assert(0);
		break;
	default:
		break;
	}
}
