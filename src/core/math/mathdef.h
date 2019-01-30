#pragma once

#ifndef __MATH_DEF_H__
#define __MATH_DEF_H__

#include <cmath>

const Float cEpsilon = 1e-4f;
const Float cPi = 3.141592653589793f;
const Float cTwoPi = 6.283185307f;
const Float cHalfPi = 1.570796327f;
const Float cQuaterPi = 0.785398163f;
const Float cReciprocalPi = 0.318309886f;
const Float cReciprocalTwoPi = 0.159154943f;
const Float cLog2 = 0.693147180559945f;
const Float cReciprocalLog2 = 1.442695040f;

inline Float InvSqrtFast(Float number)
{
	Int32 i;
	Float x2, y;
	const Float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(Int32*)&y;  // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1); // what the fuck?
	y = *(Float*)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed
	return y;
}

inline Float DegreeToRadian(Float degree)
{
	// the const Float is PI / 180
	return degree * 0.01745329f;
}

inline Float RadianToDegree(Float radian)
{
	// the const Float is 180 / PI
	return radian * 57.2957795f;
}

inline Int32 NextPower2(Int32 x)
{
	Int32 r = 1;
	while (x > r) r = r << 1;
	return r;
}

inline Float Log2(Float x)
{
	return ::logf(x) / cLog2;
}

template<typename T>
inline T Abs(T val)
{
	return (val < 0 ? -val : val);
}

template<typename T>
inline T Max(T a, T b)
{
	return (a > b) ? a : b;
}

template<typename T>
inline T Min(T a, T b)
{
	return (a < b) ? a : b;
}

template<typename T>
inline T Sqrt(T a)
{
	return (T)1.0 / InvSqrtFast(a);
}

template<typename T>
inline T Sin(T a)
{
	return std::sin(a);
}

template<typename T>
inline T Cos(T a)
{
	return std::cos(a);
}

template<typename T>
inline T Asin(T a)
{
	return std::asin(a);
}

template<typename T>
inline T Acos(T a)
{
	return std::acos(a);
}

template<typename T>
inline T Tan(T a)
{
	return std::tan(a);
}

template<typename T>
inline T Atan2(T a, T b)
{
	return std::atan2(a, b);
}

#endif // __MATH_DEF_H__
