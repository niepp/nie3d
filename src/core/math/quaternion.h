#pragma once

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

// 描述旋转方位的单位四元数类，实现各种计算，多用于3D空间
// 中物体的平滑旋转计算，两个旋转量之间的插值最常用

struct Quaternion
{
	union
	{
		Float q[4];
		struct
		{
			Float x, y, z, w;
		};
	};

	Quaternion()
	{}

	Quaternion(const Float &_x, const Float &_y, const Float &_z, const Float &_w):
		x(_x), y(_y), z(_z), w(_w)
	{}

	Quaternion(const Vector3 &axis, const Float &angle)
	{
		// 约束：axis必须被归一化
		Float half_angle = 0.5f * angle;
		Float sin_value = sin(half_angle);
		
		x = axis.x * sin_value;
		y = axis.y * sin_value;
		z = axis.z * sin_value;
		w = cos(half_angle);
	}

	Quaternion(const Vector3 &euler)
	{
		SetEuler(euler.x, euler.y, euler.z);
	}

	Quaternion(Float pitch, Float roll, Float yaw)
	{
		SetEuler(pitch, roll, yaw);
	}

	void SetIdentity()
	{
		Set(0, 0, 0, 1);
	}

	void Init(Float _x = 0, Float _y = 0, Float _z = 0, Float _w = 0)
	{
		Set(_x, _y, _z, _w);
	}

	void Set(Float _x, Float _y, Float _z, Float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void GetAxisAngle(Vector3 &axis, Float &angle)
	{
		// 约束：四元数必须被归一化
		Float half_angle = acos(w);
		angle = half_angle * 2;
		Float sin_value = sin(half_angle);
		axis.Set(x / sin_value, y / sin_value, z / sin_value);
	}

	void SetEuler(Float pitch, Float roll, Float yaw)
	{
		Float  sinRoll, sinPitch, sinYaw, cosRoll, cosPitch, cosYaw;

		sinYaw = sin(yaw * 0.5f);
		cosYaw = cos(yaw * 0.5f);

		sinPitch = sin(pitch * 0.5f);
		cosPitch = cos(pitch * 0.5f);

		sinRoll = sin(roll * 0.5f);
		cosRoll = cos(roll * 0.5f);

		x = cosRoll * sinPitch * cosYaw + sinRoll *cosPitch * sinYaw;
		y = cosRoll * cosPitch * sinYaw - sinRoll *sinPitch * cosYaw;
		z = sinRoll * cosPitch * cosYaw - cosRoll *sinPitch * sinYaw;
		w = cosRoll * cosPitch * cosYaw + sinRoll *sinPitch * sinYaw;

		Normalize();

	}

	Bool operator ==(const Quaternion &rot) const
	{
		return IsEqual(rot);
	}

	Bool operator !=(const Quaternion &rot) const
	{
		return !(*this == rot);
	}

	Bool IsEqual(const Quaternion &rot, const Float &eps = cEpsilon) const
	{
		Float diff = x - rot.x;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		diff = y - rot.y;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		diff = z - rot.z;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		diff = w - rot.w;
		if (diff > eps || diff < -eps)
		{
			return false;
		}
		return true;
	}

	void Negative()
	{
		for (Int32 i = 0; i < 4; ++i)
		{
			q[i] = -q[i];
		}
	}

	Quaternion& operator *=(const Quaternion &b)
	{
		// [s1*s2 - v1*v2, s1*v2 + s2*v1 + Cross(v1, v2)]
		Quaternion a = *this;
		Mul(a, b, *this);
		return *this;
	}

	Quaternion& operator +=(const Quaternion &rot)
	{
		x += rot.x;
		y += rot.y;
		z += rot.z;
		w += rot.w;
		return *this;
	}

	Quaternion& operator -=(const Quaternion &rot)
	{
		x -= rot.x;
		y -= rot.y;
		z -= rot.z;
		w -= rot.w;
		return *this;
	}

	Quaternion& operator *=(const Float &scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}

	Quaternion& operator /=(const Float &scale)
	{
		Float reci = 1.0f / scale;
		x *= reci;
		y *= reci;
		z *= reci;
		w *= reci;
		return *this;
	}

	Quaternion operator +(const Quaternion &rot) const
	{
		return Quaternion(x + rot.x, y + rot.y, z + rot.z, w + rot.w);
	}

	Quaternion operator -(const Quaternion &rot) const
	{
		return Quaternion(x - rot.x, y - rot.y, z - rot.z, w - rot.w);
	}

	Quaternion operator *(const Quaternion &rot) const
	{
		Quaternion ret;
		Mul(*this, rot, ret);
		return ret;
	}

	Quaternion operator *(const Float &scale) const
	{
		return Quaternion(x * scale, y * scale, z * scale, w * scale);
	}

	Quaternion operator /(const Float &scale) const
	{
		const Float r = (Float)(1.0 / scale);
		return Quaternion(x * r, y * r, z * r, w * r);
	}

	friend 	Quaternion operator *(const Float &scale, const Quaternion &rot)
	{
		return Quaternion(rot.x * scale, rot.y * scale, rot.z * scale, rot.w * scale);
	}

	friend 	Quaternion operator /(const Float &scale, const Quaternion &rot)
	{
		rot.Inverse();
		return Quaternion(rot.x * scale, rot.y * scale, rot.z * scale, rot.w * scale);
	}

	void Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void Normalize()
	{
		Float len_reci = 1.0f / Length();
		w *= len_reci;
		x *= len_reci;
		y *= len_reci;
		z *= len_reci;
	}
	
	Float SqrLength() const
	{
		return x * x + y * y + z * z + w * w;
	}
	
	Float Length() const
	{
		return sqrt(SqrLength());
	}

	Bool IsIdentity() const
	{
		return (x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f);
	}

	void Mul(const Quaternion &a, const Quaternion &b, Quaternion &prod) const
	{
		prod.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		prod.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		prod.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		prod.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	}

	void Intrp(const Quaternion &q1, const Quaternion &q2, const Float &t, Bool shortest_path = true);

	void RotateVector(Vector3 &vec_dest, const Vector3 &vec_src) const
	{
		Quaternion prod;				
		const Quaternion &q = *this; 
		const Vector3 &v = vec_src;

		// evaluate quat * vector
		// bw == 0.0f
		prod.x = q.w * v.x/*+ ax * bw*/+ q.y * v.z - q.z * v.y;
		prod.y = q.w * v.y - q.x * v.z/*+ ay * bw*/+ q.z * v.x;
		prod.z = q.w * v.z + q.x * v.y - q.y * v.x/*+ az * bw*/;
		prod.w =/*aw * bw*/- q.x * v.x - q.y * v.y - q.z * v.z;

		Quaternion q_(*this);
		q_.Conjugate(); // conjugate

		// evaluate vector * conj
		vec_dest.x = prod.w * q_.x + prod.x * q_.w + prod.y * q_.z - prod.z * q_.y;
		vec_dest.y = prod.w * q_.y - prod.x * q_.z + prod.y * q_.w + prod.z * q_.x;
		vec_dest.z = prod.w * q_.z + prod.x * q_.y - prod.y * q_.x + prod.z * q_.w;
	}

	void GetForward(Vector3 &vec) const
	{
		vec.x = (2.0f * (x * z + w * y));
		vec.y = (2.0f * (y * z - w * x));
		vec.z = (1.0f - 2.0f * (x * x + y * y));
	}

	void GetUp(Vector3 &vec) const
	{
		vec.x = (2.0f * (x * y - w * z));
		vec.y = (1.0f - 2.0f * (x * x + z * z));
		vec.z = (2.0f * (y * z + w * x));
	}

	void GetRight(Vector3 &vec) const
	{
		vec.x = 1.0f - 2.0f * (y * y + z * z);
		vec.y = 2.0f * (x * y + w * z);
		vec.z = 2.0f * (x * z - w * y);
	}
	
	Quaternion Pow(const Float &t) const
	{
		// As q = cos(A) + sin(A) * ( a*i + b*j + c*k ), then
		// q^t = cos(t*A) + sin(t*A) * ( a*i + b*j + c*k )

		Float angle = acos(w);
		Float sin_value = sin(angle);
		angle *= t;
		Quaternion result;
		result.w = cos(angle);
		
		Float coeff = sin(angle) / sin_value;
		result.x = coeff * x;
		result.y = coeff * y;
		result.z = coeff * z;
		return result;
	}

	Quaternion Exp() const
	{
		// As q = A * ( a*i + b*j + c*k ), then
		// exp(q) = cos(A) + sin(A) * ( a*i + b*j + c*k ).

		Float angle = sqrt(x * x + y * y + z * z);
		Float sin_value = sin(angle);

		Quaternion result;
		result.w = cos(angle);

		if ( Abs(sin_value) >= cEpsilon )
		{
			Float coeff = sin_value / angle;
			result.x = coeff * x;
			result.y = coeff * y;
			result.z = coeff * z;
		}
		else
		{
			result.x = x;
			result.y = y;
			result.z = z;
		}
		return result;
	}

	Quaternion Log() const
	{
		// As q = cos(A) + sin(A) * ( a*i + b*j + c*k ), then
		// log(q) = A * ( a*i + b*j + c*k ).

		Quaternion result;
		result.w = 0.0f;

		if (Abs(w) < 1.0f)
		{
			Float angle = acos(w);
			Float sin_value = sin(angle);
			if (Abs(sin_value) >= cEpsilon)
			{
				Float coeff = (Float)(angle / sin_value);
				result.x = coeff * x;
				result.y = coeff * y;
				result.z = coeff * z;
				return result;
			}
		}

		result.x = x;
		result.y = y;
		result.z = z;

		return result;
	}

	Quaternion Inverse() const
	{
		// 约束：四元数必须被归一化
		return Quaternion(-x, -y, -z, w);
	}

	void Squad(const Quaternion &q1, const Quaternion &q2,
				const Quaternion &a1, const Quaternion &a2, const Float &t)
	{
		Float w = 2.0f * t * (1.0f - t);
		Quaternion rot_q, rot_a;
		rot_q.Intrp(q1, q2, t);
		rot_a.Intrp(a1, a2, t, false);
		Intrp(rot_q, rot_a, w, false);
	}

};

inline Float Dot(const Quaternion &q1, const Quaternion &q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

inline void Quaternion::Intrp(const Quaternion &q1, const Quaternion &q2, const Float &t, Bool shortest_path)
{
	Quaternion rot_q2;
	Float omega, cos_om, sin_om_reci;

	cos_om = Dot(q1, q2);
	rot_q2 = q2;

	// 调整正负
	if (cos_om < 0.0 && shortest_path)
	{
		cos_om = -cos_om;
		rot_q2.Negative();
	}

	// 计算插值运算的系数
	Float k1, k2;
	if (cos_om > 1.0f - cEpsilon) 
	{ 
		// q1和q2很接近时做线性插值
		k1 = 1.0f - t;
		k2 = t;
		*this = k1 * q1 + k2 * rot_q2;
		// 线性插值,结果需要重新归一化
		Normalize();
	}
	else
	{ 
		// 四元数插值
		omega = acos(cos_om);
		sin_om_reci = 1.0f / sin(omega);
		k1 = sin(1.0f - t) * omega * sin_om_reci;
		k2 = sin(t * omega) * sin_om_reci;
		*this = k1 * q1 + k2 * rot_q2;
	}

}

#endif // __QUATERNION_H__
