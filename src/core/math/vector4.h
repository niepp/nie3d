#pragma once
#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <math.h>
#include <assert.h>

struct Vector4
{
	union
	{
		Float vec[4];
		struct
		{
			Float x, y, z, w;
		};
	};

	Vector4() : x(0), y(0), z(0), w(0)
	{}

	Vector4(const Float &val): 
		x(val), y(val), z(val), w(val)
	{}

	Vector4(const Float &_x, const Float &_y, const Float &_z, const Float &_w): 
		x(_x), y(_y), z(_z), w(_w)
	{}

	void Init(const Float &_x = 0, const Float &_y = 0, const Float &_z = 0, const Float &_w = 0)
	{
		Set(_x, _y, _z, _w);
	}

	void Set(const Float &_x, const Float &_y, const Float &_z, const Float &_w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Float LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}
	
	Float Length() const
	{
		return (Float)sqrt(LengthSqr());
	}
	
	const Vector4& Normalize(const Float &scale = 1)
	{
		Float len = Length();
		assert(len != 0);
		*this *= (scale / len);
		return *this;
	}

	const Vector4& NormFast2(const Float &scale = 1)
	{
		*this *= (InvSqrtFast(LengthSqr()) * scale);
		return *this;
	}

	Bool IsEqual(const Vector4 &vec, const Float &eps = cEpsilon) const
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
		diff = w - vec.w;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		return true;
	}
	
	void Intrp(const Vector4 &vec0, const Vector4 &vec1, const Float &u)
	{
		x = vec0.x + (vec1.x - vec0.x) * u;
		y = vec0.y + (vec1.y - vec0.y) * u;
		z = vec0.z + (vec1.z - vec0.z) * u;
		w = vec0.w + (vec1.w - vec0.w) * u;
	}

	Float Distance(const Vector4 &vec) const
	{
		return (*this - vec).Length();
	}

	Float DistanceSqr(const Vector4 &vec) const
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
		if (min > w)
		{
			min = w;
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
		if (max < w)
		{
			max = w;
		}
		return max;
	}

	Bool operator <(const Vector4 &other) const
	{
		return (x < other.x)||
			(x == other.x && y < other.y)||
			(x == other.x && y == other.y && z <other.z) ||
			(x == other.x && y == other.y && z == other.z && w <other.w);
	}

	Float& operator [](Int32 i)
	{
		return vec[i];
	}

	Bool operator ==(const Vector4 &vec) const
	{
		return IsEqual(vec);
	}

	Bool operator !=(const Vector4 &vec) const
	{
		return !(vec == *this);
	}

	Vector4 operator -() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	Vector4& operator =(const Vector4 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	Vector4& operator +=(const Vector4 &vec) 
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	} 

	Vector4& operator -=(const Vector4 &vec) 
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	Vector4& operator *=(const Vector4 &vec) 
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
		return *this;
	}

	Vector4& operator /=(const Vector4 &vec) 
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;
		return *this;
	}

	Vector4 operator +(const Vector4 &vec) const
	{
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	Vector4 operator -(const Vector4 &vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	Vector4 operator *(const Vector4 &vec) const
	{
		return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
	}

	Vector4 operator /(const Vector4 &vec) const
	{
		return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
	}

	Vector4& operator *=(const Float &scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}

	Vector4& operator /=(const Float &scale)
	{
		const Float reci = 1.0f / scale;
		x *= reci;
		y *= reci;
		z *= reci;
		w *= reci;
		return *this;
	}

	Vector4 operator *(const Float &scale) const
	{
		return Vector4(x * scale, y * scale, z * scale, w * scale);
	}

	Vector4 operator /(const Float &scale) const
	{
		const Float reci = 1.0f / scale;
		return Vector4(x * reci, y * reci, z * reci, w * reci);
	}

};

inline Float Dot(const Vector4 &vec1, const Vector4 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
}

#endif // __VECTOR4_H__
