#pragma once
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>

// 右手系Z-up, 行主序
struct Matrix
{
	static const Int32 N = 4;
	union
	{
		Float m[N][N];
		Float v[N * N];
		struct
		{
			Float m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};
	};

	Matrix() :
		m00(0), m01(0), m02(0), m03(0),
		m10(0), m11(0), m12(0), m13(0), 
		m20(0), m21(0), m22(0), m23(0), 
		m30(0), m31(0), m32(0), m33(0)
	{
	}

	Matrix(const Float &_m00, const Float &_m01, const Float &_m02, const Float &_m03, 
		const Float &_m10, const Float &_m11, const Float &_m12, const Float &_m13, 
		const Float &_m20, const Float &_m21, const Float &_m22, const Float &_m23, 
		const Float &_m30, const Float &_m31, const Float &_m32, const Float &_m33): 
		m00(_m00), m01(_m01), m02(_m02), m03(_m03), 
		m10(_m10), m11(_m11), m12(_m12), m13(_m13), 
		m20(_m20), m21(_m21), m22(_m22), m23(_m23), 
		m30(_m30), m31(_m31), m32(_m32), m33(_m33)
	{
	}

	void Set(const Float &_m00, const Float &_m01, const Float &_m02, const Float &_m03, 
		const Float &_m10, const Float &_m11, const Float &_m12, const Float &_m13,
		const Float &_m20, const Float &_m21, const Float &_m22, const Float &_m23,
		const Float &_m30, const Float &_m31, const Float &_m32, const Float &_m33);

	void SetRow(Uint32 row, const Vector3 &vec);
	void GetRow(Uint32 row, Vector3 &vec) const;
	void SetIdentity();
	void SetTranslation(const Vector3 &pos);
	void SetNoTranslation();
	void SetRotation(const Vector3 &euler);
	void SetNoRotation();
	void SetScale(const Vector3 &scale);
	void SetNoScale();

	// IsOrthogonal, Orthogonalize这两个接口应该只用于旋转矩阵
	Bool IsOrthogonal();
	void Orthogonalize();

	void GetTranslation(Vector3 &pos) const;
	void GetRotation(Quaternion &quat) const;
	void GetScale(Vector3 &scale) const;
	void GetRight(Vector3 &right) const;
	void GetUp(Vector3 &up) const;
	void GetForward(Vector3 &forward) const;
	void Compose(const Vector3 &pos, const Quaternion &quat, const Vector3 &scale);
	void Decompose(Vector3 &pos, Quaternion &quat, Vector3 &scale) const;
	void Transpose();
	void Inverse();
	void MakeMatViewR(const Vector3 &eye, const Vector3 &lookat, const Vector3 &up);
	void MakeMatPerspR(Float fov_y, Float aspect, Float min_z, Float max_z);
	void MakeMatOrthoR(Float w, Float h, Float min_z, Float max_z);
	Bool IsEqual(const Matrix &mat) const;

	Float* operator [](Int32 index);
	Bool operator ==(const Matrix &mat) const;
	Bool operator !=(const Matrix &mat) const;
	Matrix& operator +=(const Matrix &mat);
	Matrix& operator -=(const Matrix &mat);
	Matrix& operator *=(const Matrix &mat);

private:
	void Swap(Float &num1, Float &num2)
	{
		Float temp = num1;
		num1 = num2;
		num2 = temp;
	}

};

inline void QuaternionToMatrix(const Quaternion &rot, Matrix &mat)
{
	Float wx, wy, wz, x2, y2, z2, xy, yz, xz;

	wx = 2.0f * rot.w * rot.x;
	wy = 2.0f * rot.w * rot.y;
	wz = 2.0f * rot.w * rot.z;

	x2 = 2.0f * rot.x * rot.x;
	y2 = 2.0f * rot.y * rot.y;
	z2 = 2.0f * rot.z * rot.z;

	xy = 2.0f * rot.x * rot.y;
	yz = 2.0f * rot.y * rot.z;
	xz = 2.0f * rot.x * rot.z;

	mat.m00 = 1.0f - y2 - z2;
	mat.m01 = xy + wz;
	mat.m02 = xz - wy;

	mat.m10 = xy - wz;
	mat.m11 = 1.0f - x2 - z2;
	mat.m12 = yz + wx;

	mat.m20 = xz + wy;
	mat.m21 = yz - wx;
	mat.m22 = 1.0f - x2 - y2;

	mat.m03 = mat.m13 = mat.m23 = mat.m30 = mat.m31 = mat.m32 = 0;
	mat.m33 = 1.0f;

}

inline void MatrixToQuaternion(const Matrix &mat, Quaternion &rot)
{
	Float tr, s;	
	tr = mat.m00 + mat.m11 + mat.m22;

	if (tr < cEpsilon)
	{
		Int32 i, j, k;
		static Int32 next_index[3] = {1, 2, 0};

		i = 0;
		if (mat.m11 > mat.m00)
		{
			i = 1;
		}
		if (mat.m22 > mat.m[i][i])
		{
			i = 2;
		}

		j = next_index[i];
		k = next_index[j];

		s = sqrt(mat.m[i][i] - mat.m[j][j] - mat.m[k][k] + 1.0f);

		rot.q[i] = s * 0.5f;
		s = 0.5f / s;
		rot.w = (mat.m[j][k] - mat.m[k][j]) * s;
		rot.q[j] = (mat.m[j][i] + mat.m[i][j]) * s;
		rot.q[k] = (mat.m[k][i] + mat.m[i][k]) * s;
	}
	else
	{
		s = sqrt(tr + 1.0f);

		rot.w = s * 0.5f;
		s = 0.5f / s;

		rot.x = (mat.m12 - mat.m21) * s;
		rot.y = (mat.m20 - mat.m02) * s;
		rot.z = (mat.m01 - mat.m10) * s;	
	}
	rot.Normalize();
}

inline void EulerToMatrix(Float pitch, Float roll, Float yaw, Matrix &mat)
{
	// euler : 
	Float xc = cos(pitch);
	Float xs = sin(pitch);
	Float yc = cos(roll);
	Float ys = sin(roll);
	Float zc = cos(yaw);
	Float zs = sin(yaw);

	mat.m00 = yc * zc;
	mat.m10 = -xc * zs + zc * xs * ys;
	mat.m20 = xs * zs + xc * zc * ys;

	mat.m01 = yc * zs;
	mat.m11 = xc * zc + xs * ys * zs;
	mat.m21 = -xs * zc + xc * ys * zs;

	mat.m02 = -ys;
	mat.m12 = xs * yc;
	mat.m22 = xc * yc;

	mat.m03 = mat.m13 = mat.m23 = 0;
	mat.m30 = mat.m31 = mat.m32 = 0;
	mat.m33 = 1.0f;
}

inline void MatrixToEuler(const Matrix &mat, Float &pitch, Float &roll, Float &yaw)
{
	// 注意：欧拉轴顺序决定了结果角度的范围，
	// 处于中间的轴对应的旋转角在(-PI / 2, PI / 2]之间，
	// 另外两个旋转角在(-PI, PI]之间。
	// 例如：euler_order = EULER_ORDER_YZX 时有
	// -PI < x_angle <= PI
	// -PI < y_angle <= PI
	// -PI / 2 < z_angle <= PI / 2
	Float ys;
	ys = -mat.m02;
	if (ys <= -1.0f)
	{
		roll = -cHalfPi;
	}
	else if (ys >= 1.0f)
	{
		roll = cHalfPi;
	}
	else
	{
		roll = asin(ys);
	}
	if (ys > 1.0f - cEpsilon)
	{
		yaw = 0;
		pitch = atan2(-mat.m21, mat.m11);
	}
	else
	{
		yaw = atan2(mat.m01, mat.m00);
		pitch = atan2(mat.m12, mat.m22);
	}
}

inline void MatrixToOrient(const Matrix &mat, Vector3 &right, Vector3 &up, Vector3 &forward)
{
	right.Set(mat.m00, mat.m01, mat.m02);
	up.Set(mat.m10, mat.m11, mat.m12);
	forward.Set(mat.m20, mat.m21, mat.m22);
	right.Normalize();
	up.Normalize();
	forward.Normalize();
}

inline void OrientToMatrix(const Vector3 &forward, const Vector3 &up, Matrix &mat)
{
	Vector3 f, u;
	f = forward;
	u = up;
	f.Normalize();
	u.Normalize();

	Float dot = Dot(f, u);
	if (dot > (1.0f - cEpsilon) || dot < (-1.0f + cEpsilon))
	{
		// 前向量和上向量平行时重新生成上向量
		Vector3 f_abs(f);
		f_abs.Set(Abs(f.x), Abs(f.y), Abs(f.z));
		u.Set(1.0f, 0, 0);
		Float min = f_abs.x;
		if (min > f_abs.y) {
			u.Set(0, 1.0f, 0);
			min = f_abs.y;
		}
		if (min > f_abs.z) {
			u.Set(0, 0, 1.0f);
		}
	}

	Vector3 right = Cross(u, f);
	right.Normalize();
	Vector3 true_up = Cross(f, right);
	mat.SetRow(0, right);
	mat.SetRow(1, true_up);
	mat.SetRow(2, f);

}

inline Matrix operator +(const Matrix &mat1, const Matrix &mat2)
{
	Matrix mat;
	for (Int32 i = 0; i < Matrix::N * Matrix::N; ++i)
	{
		mat.v[i] = mat1.v[i] + mat2.v[i];
	}
	return mat;
}

inline Matrix operator -(const Matrix &mat1, const Matrix &mat2)
{
	Matrix mat;
	for (Int32 i = 0; i < Matrix::N * Matrix::N; ++i)
	{
		mat.v[i] = mat1.v[i] - mat2.v[i];
	}
	return mat;
}

inline Matrix operator *(const Matrix &mat1, const Matrix &mat2)
{
	Matrix mat;
	for (Int32 i = 0; i < Matrix::N; ++i)
	{
		for (Int32 j = 0; j < Matrix::N; ++j)
		{
			Float sum = 0;
			for (Int32 k = 0; k < Matrix::N; ++k)
			{
				sum += mat1.m[i][k] * mat2.m[k][j];
			}
			mat.m[i][j] = sum;
		}
	}

	return mat;
}

inline Vector3 operator *(Vector3 &vec, const Matrix &mat)
{
	Vector3 prod;
	prod.x = vec.x * mat.m00 + vec.y * mat.m10 + vec.z * mat.m20 + mat.m30;
	prod.y = vec.x * mat.m01 + vec.y * mat.m11 + vec.z * mat.m21 + mat.m31;
	prod.z = vec.x * mat.m02 + vec.y * mat.m12 + vec.z * mat.m22 + mat.m32;
	return prod;
}

//////////////////////////////////////////////////////////////////////////
inline void Matrix::Set(const Float &_m00, const Float &_m01, const Float &_m02, const Float &_m03, 
		 const Float &_m10, const Float &_m11, const Float &_m12, const Float &_m13, 
		 const Float &_m20, const Float &_m21, const Float &_m22, const Float &_m23, 
		 const Float &_m30, const Float &_m31, const Float &_m32, const Float &_m33)
{
	m00 = _m00, m01 = _m01, m02 = _m02, m03 = _m03;
	m10 = _m10, m11 = _m11, m12 = _m12, m13 = _m13;
	m20 = _m20, m21 = _m21, m22 = _m22, m23 = _m23;
	m30 = _m30, m31 = _m31, m32 = _m32, m33 = _m33;
}

inline void Matrix::SetRow(Uint32 row, const Vector3 &vec)
{
	m[row][0] = vec.x;
	m[row][1] = vec.y;
	m[row][2] = vec.z;
}

inline void Matrix::GetRow(Uint32 row, Vector3 &vec) const
{
	vec.Set(m[row][0], m[row][1], m[row][2]);
}

inline void Matrix::SetIdentity()
{
	Set(1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1.0f);
}

inline void Matrix::SetTranslation(const Vector3 &pos)
{
	m30 = pos.x;
	m31 = pos.y;
	m32 = pos.z;
}

inline void Matrix::SetNoTranslation()
{
	m30 = m31 = m32 = 0;
	m33 = 1;
}

inline void Matrix::SetRotation(const Vector3 &euler)
{
	EulerToMatrix(euler.x, euler.y, euler.z, *this);
}

inline void Matrix::SetNoRotation()
{
	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);

	m00 = vec0.Length();
	m01 = 0;
	m02 = 0;

	m10 = 0;
	m11 = vec1.Length();
	m12 = 0;

	m20 = 0;
	m21 = 0;
	m22 = vec2.Length();

}

inline void Matrix::SetScale(const Vector3 &scale)
{
	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);

	vec0.Normalize();
	vec1.Normalize();
	vec2.Normalize();

	vec0 *= scale.x;
	vec1 *= scale.y;
	vec2 *= scale.z;

	SetRow(0, vec0);
	SetRow(1, vec1);
	SetRow(2, vec2);

}

inline void Matrix::SetNoScale()
{
	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);
	vec0.Normalize();
	vec1.Normalize();
	vec2.Normalize();
	SetRow(0, vec0);
	SetRow(1, vec1);
	SetRow(2, vec2);
}

// IsOrthogonal, Orthogonalize这两个接口应该只用于旋转矩阵
inline Bool Matrix::IsOrthogonal()
{
	Float limit = 0.01f;
	if (((m11 * m21 + m12 * m22 + m13 * m23) < limit) &&
		((m11 * m31 + m12 * m32 + m13 * m33) < limit) &&
		((m31 * m21 + m32 * m22 + m33 * m23) < limit) &&
		(Abs(m11 * m11 + m12 * m12 + m13 * m13 - 1.0f) < limit) &&
		(Abs(m21 * m21 + m22 * m22 + m23 * m23 - 1.0f) < limit) &&
		(Abs(m31 * m31 + m32 * m32 + m33 * m33 - 1.0f) < limit))
		return true;
	else return false;
}

inline void Matrix::Orthogonalize()
{
	// Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
	// M = [m0|m1|m2]', then orthonormal output matrix is Q = [q0|q1|q2]',
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B. M' indicates transpose of Matrix<Float> M.

	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);

	// compute q0
	vec0.Normalize();

	// compute q1
	Float dot0 = Dot(vec0, vec1);
	vec1 -= vec0 * dot0;
	vec1.Normalize();

	// compute q2
	Float dot1 = Dot(vec1, vec2);	
	dot0 = Dot(vec0, vec2);
	vec2 -= vec0 * dot0 + vec1 * dot1;
	vec2.Normalize();

	SetRow(0, vec0);
	SetRow(1, vec1);
	SetRow(2, vec2);

}

inline void Matrix::GetTranslation(Vector3 &pos) const
{
	pos.Set(m30, m31, m32);
}

inline void Matrix::GetRotation(Quaternion &quat) const
{
	Vector3 pos, scale;
	Decompose(pos, quat, scale);
}

inline void Matrix::GetScale(Vector3 &scale) const
{
	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);
	scale.Set(vec0.Length(), vec1.Length(), vec2.Length());
}

inline void Matrix::GetRight(Vector3 &right) const
{
	right.Set(m00, m01, m02);
	right.Normalize();
}

inline void Matrix::GetUp(Vector3 &up) const
{
	up.Set(m10, m11, m12);
	up.Normalize();
}

inline void Matrix::GetForward(Vector3 &forward) const
{
	forward.Set(m20, m21, m22);
	forward.Normalize();
}

inline void Matrix::Compose(const Vector3 &pos, const Quaternion &quat,
			 const Vector3 &scale)
{
	QuaternionToMatrix(quat, *this);
	SetScale(scale);
	SetTranslation(pos);
}

inline void Matrix::Decompose(Vector3 &pos, Quaternion &quat, Vector3 &scale) const
{
	GetTranslation(pos);

	Vector3 vec0, vec1, vec2;
	GetRow(0, vec0);
	GetRow(1, vec1);
	GetRow(2, vec2);
	scale.Set(vec0.Length(), vec1.Length(), vec2.Length());

	vec0 /= scale.x;
	vec1 /= scale.y;
	vec2 /= scale.z;

	Matrix rot_mat;
	rot_mat.SetRow(0, vec0);
	rot_mat.SetRow(1, vec1);
	rot_mat.SetRow(2, vec2);
	rot_mat.Orthogonalize();
	MatrixToQuaternion(rot_mat, quat);

}

inline void Matrix::Transpose()
{
	Swap(m01, m10);
	Swap(m02, m20);
	Swap(m03, m30);
	Swap(m12, m21);
	Swap(m13, m31);
	Swap(m23, m32);
}

inline void Matrix::Inverse() 
{
	float d12 = m20 * m31 - m30 * m21;
	float d13 = m20 * m32 - m30 * m22;
	float d23 = m21 * m32 - m31 * m22;
	float d24 = m21 * m33 - m31 * m23;
	float d34 = m22 * m33 - m32 * m23;
	float d41 = m23 * m30 - m33 * m20;

	Matrix r;

	r.m00 = (m11 * d34 - m12 * d24 + m13 * d23);
	r.m10 = -(m10 * d34 + m12 * d41 + m13 * d13);
	r.m20 = (m10 * d24 + m11 * d41 + m13 * d12);
	r.m30 = -(m10 * d23 - m11 * d13 + m12 * d12);

	// Com.mpute determinant as early as possible using these cofactors.
	float det = m00 * r.m00 + m01 * r.m10 + m02 * r.m20 + m03 * r.m30;

	float invDet = float(1) / det;
	// Com.mpute rest of inverse.
	r.m00 *= invDet;
	r.m10 *= invDet;
	r.m20 *= invDet;
	r.m30 *= invDet;

	r.m01 = -(m01 * d34 - m02 * d24 + m03 * d23) * invDet;
	r.m11 = (m00 * d34 + m02 * d41 + m03 * d13) * invDet;
	r.m21 = -(m00 * d24 + m01 * d41 + m03 * d12) * invDet;
	r.m31 = (m00 * d23 - m01 * d13 + m02 * d12) * invDet;

	// Pre-compute 2x2 dets for first two rows when computing
	// cofactors of last two rows.
	d12 = m00 * m11 - m10 * m01;
	d13 = m00 * m12 - m10 * m02;
	d23 = m01 * m12 - m11 * m02;
	d24 = m01 * m13 - m11 * m03;
	d34 = m02 * m13 - m12 * m03;
	d41 = m03 * m10 - m13 * m00;

	r.m02 = (m31 * d34 - m32 * d24 + m33 * d23) * invDet;
	r.m12 = -(m30 * d34 + m32 * d41 + m33 * d13) * invDet;
	r.m22 = (m30 * d24 + m31 * d41 + m33 * d12) * invDet;
	r.m32 = -(m30 * d23 - m31 * d13 + m32 * d12) * invDet;
	r.m03 = -(m21 * d34 - m22 * d24 + m23 * d23) * invDet;
	r.m13 = (m20 * d34 + m22 * d41 + m23 * d13) * invDet;
	r.m23 = -(m20 * d24 + m21 * d41 + m23 * d12) * invDet;
	r.m33 = (m20 * d23 - m21 * d13 + m22 * d12) * invDet;

	*this = r;

}

inline void Matrix::MakeMatViewR(const Vector3 &eye, const Vector3 &lookat, const Vector3 &up)
{
	Vector3 zaxis = eye - lookat;
	zaxis.Normalize();
	Vector3	xaxis = Cross(up, zaxis);
	xaxis.Normalize();
	Vector3	yaxis = Cross(zaxis, xaxis);
	this->Set(
		xaxis.x,           yaxis.x,           zaxis.x,          0,
		xaxis.y,           yaxis.y,           zaxis.y,          0,
		xaxis.z,           yaxis.z,           zaxis.z,          0,
	   -Dot(xaxis, eye),   -Dot(yaxis, eye),   -Dot(zaxis, eye),   1);
}

inline void Matrix::MakeMatPerspR(Float fov_y, Float aspect, Float min_z, Float max_z)
{
	m11 = 1.0f / tan(fov_y / 2.0f);
	m00 = m11 / aspect;
	m22 = (min_z + max_z) / (min_z - max_z);
	m32 = 2 * min_z * max_z / (min_z - max_z);
	m23 = -1.0f;
}

inline void Matrix::MakeMatOrthoR(Float w, Float h, Float min_z, Float max_z)
{
	m00 = 2.0f / w;
	m11 = 2.0f / h;
	m22 = 2.0f / (min_z - max_z);
	m32 = (min_z + max_z) / (min_z - max_z);
	m33 = 1.0f;
}

inline Bool Matrix::IsEqual(const Matrix &mat) const
{
	for (Int32 i = 0; i < N * N; ++i)
	{
		if (Abs(v[i] - mat.v[i]) > cEpsilon)
		{
			return false;
		}
	}
	return true;
}

inline Float* Matrix::operator [](Int32 index)
{
	return (Float*)m[index];
}

inline Bool Matrix::operator ==(const Matrix &mat) const
{
	return IsEqual(mat);			
}

inline Bool Matrix::operator !=(const Matrix &mat) const
{
	return !(*this == mat);
}

inline Matrix& Matrix::operator +=(const Matrix &mat)
{
	for (Int32 i = 0; i < N * N; ++i)
	{
		v[i] += mat.v[i];
	}
	return *this;
}

inline Matrix& Matrix::operator -=(const Matrix &mat)
{
	for (Int32 i = 0; i < N * N; ++i)
	{
		v[i] -= mat.v[i];
	}
	return *this;
}

inline Matrix& Matrix::operator *=(const Matrix &mat)
{
	Matrix p;
	for (Int32 i = 0; i < N; ++i)
	{
		for (Int32 j = 0; j < N; ++j)
		{
			Float sum = 0;
			for (Int32 k = 0; k < N; ++k)
			{
				sum += m[i][k] * mat.m[k][j];
			}
			p.m[i][j] = sum;
		}
	}

	*this = p;
	return *this;
}

#endif // __MATRIX_H__
