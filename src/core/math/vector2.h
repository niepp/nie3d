#pragma once
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <math.h>
#include <assert.h>

struct Vector2
{
	union
	{
		Float vec[2];
		struct
		{
			Float x, y;
		};
	};

	Vector2() : x(0), y(0)
	{}

	Vector2(const Float &val): 
		x(val), y(val)
	{}

	Vector2(const Float &_x, const Float &_y): 
		x(_x), y(_y)
	{}

	void Init(const Float &_x = 0, const Float &_y = 0)
	{
		Set(_x, _y);
	}

	void Set(const Float &_x, const Float &_y)
	{
		x = _x;
		y = _y;
	}

	Bool IsEqual(const Vector2 &vec, const Float &eps = cEpsilon) const
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
		return true;
	}

	Bool GreaterAll(const Vector2 &vec) const // x, y都大于vec
	{
		return x > vec.x && y > vec.y;
	}

	Bool Greater(const Vector2 &vec) const
	{
		return x > vec.x || y > vec.y;
	}

	Bool LessAll(const Vector2 &vec) const // x, y都小于vec
	{
		return x < vec.x && y < vec.y;
	}

	Bool Less(const Vector2 &vec) const
	{
		return x < vec.x || y < vec.y;
	}
	
	void Intrp(const Vector2 &vec0, const Vector2 &vec1, const Float &u)
	{
		x = vec0.x + (vec1.x - vec0.x) * u;
		y = vec0.y + (vec1.y - vec0.y) * u;
	}

	Float Min() const
	{
		Float min = x;
		if (min > y)
		{
			min = y;
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
		return max;
	}

	Bool operator <(const Vector2 &other) const
	{
		return (x < other.x)||
			(x == other.x && y < other.y);
	}

	Float& operator [](Int32 i)
	{
		return vec[i];
	}

	Bool operator ==(const Vector2 &vec) const
	{
		return IsEqual(vec);
	}

	Bool operator !=(const Vector2 &vec) const
	{
		return !(vec == *this);
	}

	Vector2 operator -() const
	{
		return Vector2(-x, -y);
	}

	Vector2& operator =(const Vector2 &vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	Vector2& operator +=(const Vector2 &vec) 
	{
		x += vec.x;
		y += vec.y;
		return *this;
	} 

	Vector2& operator -=(const Vector2 &vec) 
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2& operator *=(const Vector2 &vec) 
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	Vector2& operator /=(const Vector2 &vec) 
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2 operator +(const Vector2 &vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator -(const Vector2 &vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 operator *(const Vector2 &vec) const
	{
		return Vector2(x * vec.x, y * vec.y);
	}

	Vector2 operator /(const Vector2 &vec) const
	{
		return Vector2(x / vec.x, y / vec.y);
	}

	Vector2& operator *=(const Float &scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2& operator /=(const Float &scale)
	{
		Float reci = 1.0f / scale;
		x *= reci;
		y *= reci;
		return *this;
	}

	Vector2 operator *(const Float &scale) const
	{
		return Vector2(x * scale, y * scale);
	}

	Vector2 operator /(const Float &scale) const
	{
		const Float r = 1 / scale;
		return Vector2(x * r, y * r);
	}

};

#endif // __VECTOR2_H__
