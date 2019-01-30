#pragma once
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cmath>
#include <cassert>

struct Vector3
{
	union
	{
		Float vec[3];
		struct
		{
			Float x, y, z;
		};
	};

	Vector3() : x(0), y(0), z(0)
	{}

	Vector3(const Float &val): 
		x(val), y(val), z(val)
	{}

	Vector3(const Float &_x, const Float &_y, const Float &_z): 
		x(_x), y(_y), z(_z)
	{}

	void Init(const Float &_x = 0, const Float &_y = 0, const Float &_z = 0)
	{
		Set(_x, _y, _z);
	}

	void Set(const Float &_x, const Float &_y, const Float &_z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Float LengthSqr() const
	{
		return x * x + y * y + z * z;
	}
	
	Float Length() const
	{
		return (Float)sqrt(LengthSqr());
	}
	
	const Vector3& Normalize(const Float &scale = 1)
	{
		Float len = Length();
		assert(len != 0);
		*this *= (scale / len);
		return *this;
	}

	const Vector3& NormFast2(const Float &scale = 1)
	{
		*this *= (InvSqrtFast(LengthSqr()) * scale);
		return *this;
	}

	Bool IsEqual(const Vector3 &vec, const Float &eps = cEpsilon) const
	{
		Float diff = x - vec.x;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		diff = y - vec.y;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		diff = z - vec.z;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		return true;
	}

	Bool GreaterAll(const Vector3 &vec) const // x, y, z都大于vec
	{
		return x > vec.x && y > vec.y && z > vec.z;
	}

	Bool Greater(const Vector3 &vec) const
	{
		return x > vec.x || y > vec.y || z > vec.z;
	}

	Bool LessAll(const Vector3 &vec) const // x, y, z都小于vec
	{
		return x < vec.x && y < vec.y && z < vec.z;
	}

	Bool Less(const Vector3 &vec) const
	{
		return x < vec.x || y < vec.y || z < vec.z;
	}
	
	void Intrp(const Vector3 &vec0, const Vector3 &vec1, const Float &u)
	{
		x = vec0.x + (vec1.x - vec0.x) * u;
		y = vec0.y + (vec1.y - vec0.y) * u;
		z = vec0.z + (vec1.z - vec0.z) * u;
	}

	Float Distance(const Vector3 &vec) const
	{
		return (*this - vec).Length();
	}

	Float DistanceSqr(const Vector3 &vec) const
	{
		return (*this - vec).LengthSqr();
	}

	Float Min() const
	{
		Float min = x;
		if (min > y)
		{
			min = y;
		}
		if (min > z)
		{
			min = z;
		}
		return min;
	}

	Float Max() const
	{
		Float max = x;
		if (max < y)
		{
			max = y;
		}
		if (max < z)
		{
			max = z;
		}
		return max;
	}

	Bool operator <(const Vector3 &other) const
	{
		return (x < other.x)||
			(x == other.x && y < other.y)||
			(x == other.x && y == other.y && z <other.z);
	}

	Float& operator [](Int32 i)
	{
		return vec[i];
	}

	Bool operator ==(const Vector3 &vec) const
	{
		return IsEqual(vec);
	}

	Bool operator !=(const Vector3 &vec) const
	{
		return !(vec == *this);
	}

	Vector3 operator -() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3& operator =(const Vector3 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	Vector3& operator +=(const Vector3 &vec) 
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	} 

	Vector3& operator -=(const Vector3 &vec) 
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator *=(const Vector3 &vec) 
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	Vector3& operator /=(const Vector3 &vec) 
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		return *this;
	}

	Vector3 operator +(const Vector3 &vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 operator -(const Vector3 &vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator *(const Vector3 &vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	Vector3 operator /(const Vector3 &vec) const
	{
		return Vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	Vector3& operator *=(const Float &scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3& operator /=(const Float &scale)
	{
		Float reci = 1.0f / scale;
		x *= reci;
		y *= reci;
		z *= reci;
		return *this;
	}

	Vector3 operator *(const Float &scale) const
	{
		return Vector3(x * scale, y * scale, z * scale);
	}

	Vector3 operator /(const Float &scale) const
	{
		const Float r = 1 / scale;
		return Vector3(x * r, y * r, z * r);
	}

};

inline Float Dot(const Vector3 &vec1, const Vector3 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 prod;
	prod.Set(vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x);
	return prod;
}

// 求两个向量夹角的余弦
inline Float Cos2Vec(const Vector3 &a, const Vector3 &b)
{
	return Dot(a, b) * InvSqrtFast(a.LengthSqr() * b.LengthSqr());
}

// 求两个向量夹角正弦的平方
inline Float Sin2VecSqr(const Vector3 &a, const Vector3 &b)
{
	Vector3 cross = Cross(a, b);
	return cross.LengthSqr() / (a.LengthSqr() * b.LengthSqr());
}

// 求两个向量夹角的正弦
inline Float Sin2Vec(const Vector3 &a, const Vector3 &b)
{
	return Sqrt(Sin2VecSqr(a, b));
}

#endif // __VECTOR3_H__
