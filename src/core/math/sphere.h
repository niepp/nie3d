#pragma once

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <vector>

/*
	unit sphere with center at (0,0,0) and radius of 1.0
*/

struct Sphere {
	typedef std::vector<Vector3> Vector3Vec;
	typedef std::vector<Vector2> Vector2Vec;
	typedef std::vector<Color> ColorVec;

	Sphere(Uint32 seg) {
		GenSphere(seg, seg * 2);
	}

	void AssignVertex(Uint32 v_idx, Vector3 p);
	void GenSphere(Uint32 seg1, Uint32 seg2);

	Vector3Vec m_sphere_pos;
	Vector3Vec m_sphere_nor;
	Vector2Vec m_sphere_uv;
	ColorVec m_sphere_color;
	std::vector<Uint16> m_sphere_indices;

};

inline void Sphere::AssignVertex(Uint32 v_idx, Vector3 p)
{
	m_sphere_pos[v_idx].Set(p.x, p.y, p.z);
	m_sphere_nor[v_idx].Set(p.x, p.y, p.z);
	m_sphere_uv[v_idx].Set(p.x * 0.5f + 0.5f, p.y * 0.5f + 0.5f);
	m_sphere_color[v_idx] = Color::Red;
}

inline void Sphere::GenSphere(Uint32 seg1, Uint32 seg2)
{
	Uint32 v_count = (seg1 - 1) * seg2 + 2;
	m_sphere_pos.resize(v_count);
	m_sphere_nor.resize(v_count);
	m_sphere_uv.resize(v_count);
	m_sphere_color.resize(v_count);
	AssignVertex(0, Vector3(0, 0, 1.0f));
	for (Uint32 i = 0; i < seg1 - 1; ++i)
	{
		Float alpha = cPi * (i + 1) / seg1;
		Float radius = sin(alpha);
		Float z = cos(alpha);
		for (Uint32 j = 0; j < seg2; ++j)
		{
			Float beta = cTwoPi * j / seg2;
			Float x = radius * cos(beta);
			Float y = radius * sin(beta);
			Uint32 v_idx = i * seg2 + j + 1;
			AssignVertex(v_idx, Vector3(x, y, z));
		}
	}

	AssignVertex(v_count - 1, Vector3(0, 0, -1.0f));
	m_sphere_indices.resize((seg1 * 2 - 2) * 3 * seg2);

	Uint32 tri = 0;
	for (Uint32 j = 1; j < seg2 + 1; ++j)
	{
		m_sphere_indices[tri * 3 + 0] = 0;
		m_sphere_indices[tri * 3 + 1] = j;
		m_sphere_indices[tri * 3 + 2] = (j % seg2) + 1;
		++tri;
	}

	for (Uint32 i = 1; i < seg1 - 1; ++i)
	{
		for (Uint32 j = 1; j < seg2 + 1; ++j)
		{
			m_sphere_indices[tri * 3 + 0] = (i - 1) * seg2 + j;
			m_sphere_indices[tri * 3 + 1] = i * seg2 + j;
			m_sphere_indices[tri * 3 + 2] = i * seg2 + (j % seg2) + 1;
			++tri;
			m_sphere_indices[tri * 3 + 0] = (i - 1) * seg2 + (j % seg2) + 1;
			m_sphere_indices[tri * 3 + 1] = (i - 1) * seg2 + j;
			m_sphere_indices[tri * 3 + 2] = i * seg2 + (j % seg2) + 1;
			++tri;
		}
	}

	for (Uint32 j = 1; j < seg2 + 1; ++j)
	{
		m_sphere_indices[tri * 3 + 0] = (seg1 - 2) * seg2 + (j % seg2) + 1;
		m_sphere_indices[tri * 3 + 1] = (seg1 - 2) * seg2 + j;
		m_sphere_indices[tri * 3 + 2] = v_count - 1;
		++tri;
	}

}

#endif //__SPHERE_H__