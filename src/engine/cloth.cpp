#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "resourcecache.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "cloth.h"

const Vector3 gravity = Vector3(0.0f, 0.0f, -9.81f);
const float DEFAULT_DAMPING = -0.0125f;
const float KsStretch = 50.75f, KdStretch = -0.25f;
const float KsShear = 50.75f, KdShear = -0.25f;
const float KsBend = 50.95f, KdBend = -0.25f;

Cloth::Cloth(Uint32 width, Uint32 height, Float grid_size, Float mass) : m_width(width),
	m_height(height), m_mass(mass), m_size(grid_size)
{
	m_particle_count = (m_width + 1) * (m_height + 1);
	m_particles.resize(m_particle_count);
	m_last_particles.resize(m_particle_count);
	m_force_per_vertex.resize(m_particle_count);

	//fill in positions
	Float offset = m_size * 0.5f;
	for(Uint32 j = 0; j <= m_height; ++j) {
		for(Uint32 i = 0; i <= m_width; ++i) {
			Uint32 count = j * (m_width + 1) + i;
			m_particles[count].Set(float(i) / m_width * m_size - offset, float(j) / m_height * m_size, float(j) / m_height * m_size - offset);
			m_last_particles[count] = m_particles[count];
		}
	}

/*
	0------1------2
	|\    /|\    /|
	| \  / | \  / |
	|  \/  |  \/  |
	|  /\  |  /\  |
	| /  \ | /  \ |
	|/    \|/    \|
	3------4------5
	|\    /|\    /|
	| \  / | \  / |
	|  \/  |  \/  |
	|  /\  |  /\  |
	| /  \ | /  \ |
	|/    \|/    \|
	6------7------8
	[_____________]
*/

	//setup springs
	// Horizontal Stretch Springs
	for (Uint32 v = 0; v <= m_height; ++v) {
		for (Uint32 u = 0; u < m_width; ++u) {
			Uint32 p = v * (m_width + 1) + u;
			AddSpring(p, p + 1, KsStretch, KdStretch, STRETCH_SPRING);
		}
	}

	// Vertical Stretch Springs
	for (Uint32 u = 0; u <= m_width; ++u) {
		for (Uint32 v = 0; v < m_height; ++v) {
			Uint32 p = u * (m_height + 1) + v;
			AddSpring(p, p + 1, KsStretch, KdStretch, STRETCH_SPRING);
		}
	}

	// Shearing Springs
	for (Uint32 v = 0; v < m_height; ++v) {
		for (Uint32 u = 0; u < m_width; ++u) {
			Uint32 p1 = v * (m_width + 1) + u;
			Uint32 p2 = (v + 1) * (m_width + 1) + u;
			AddSpring(p1, p2 + 1, KsShear, KdShear, SHEAR_SPRING);
			AddSpring(p2, p1 + 1, KsShear, KdShear, SHEAR_SPRING);
		}
	}

	// Bend Springs
	for (Uint32 v = 0; v <= m_height; ++v) {
		for (Uint32 u = 0; u < m_width - 1; ++u) {
			Uint32 p = v * (m_width + 1) + u;
			AddSpring(p, p + 2, KsBend, KdBend, BEND_SPRING);
		}
	}
	for (Uint32 u = 0; u <= m_width; ++u) {
		for (Uint32 v = 0; v < m_height - 1; ++v) {
			Uint32 p = u * (m_height + 1) + v;
			AddSpring(p, p + 2, KsBend, KdBend, BEND_SPRING);
		}
	}

	// render related
	InputElement elements0[] =
	{
		{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float},
		{0, 12, cInputUsageTexcoord, 0, cPixelFormatR32G32_Float},
	};

	m_layout = GetRenderer()->CreateInputLayout(elements0, ArraySize(elements0));
	m_vb = GetRenderer()->CreateVertexBuffer(cUsageDynamic, m_particle_count * sizeof(Vertex), sizeof(Vertex));
	Uint32 tri_count = m_width * m_height * 2;
	m_ib = GetRenderer()->CreateIndexBuffer(cUsageStatic, tri_count * 3, cIndex16);
	m_mtl = (Material*)ResourceCache::Instance()->Create("joker.mtl", cResTypeMaterial);

	m_vertex.resize(m_particle_count);
	for(Uint32 j = 0; j <= m_height; ++j) {
		for(Uint32 i = 0; i <= m_width; ++i) {
			Uint32 count = j * (m_width + 1) + i;
			m_vertex[count].pos = m_particles[count];
			m_vertex[count].u = float(i) / m_width;
			m_vertex[count].v = float(j) / m_height;
		}
	}
	m_vb->Update(&m_vertex[0], m_vertex.size() * sizeof(Vertex));

	Uint32 index = 0;
	std::vector<Uint16> indices_vec;
	indices_vec.resize(tri_count * 3);
	for (Uint32 v = 0; v < m_height; ++v) {
		for (Uint32 u = 0; u < m_width; ++u) {
			Uint32 p1 = v * (m_width + 1) + u;
			Uint32 p2 = (v + 1) * (m_width + 1) + u;
			indices_vec[index++] = p2;
			indices_vec[index++] = p1;
			indices_vec[index++] = p2 + 1;
			indices_vec[index++] = p1;
			indices_vec[index++] = p1 + 1;
			indices_vec[index++] = p2 + 1;
		}
	}
	m_ib->Update(&indices_vec[0], indices_vec.size() * sizeof(Uint16));

	//// sphere vertex
	//InputElement elements1[] = {
	//	{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float}
	//};

	//m_pos_layout = GetRenderer()->CreateInputLayout(elements1, ArraySize(elements1));
	//const Uint32 segment = 4;//32;
	//Uint32 sv_count = segment * (segment + 1);
	//m_sphere_vb = GetRenderer()->CreateVertexBuffer(cUsageStatic, sv_count * sizeof(Vector3));
	//std::vector<Vector3> sphere_vertex;
	//sphere_vertex.resize(sv_count);

	//const float da = cTwoPi / segment;
	//const Quaternion dq1(Vector3(0, 1, 0), da);
	//const Quaternion dq2(Vector3(0, 0, 1), da);
	//const Vector3 v(0, 1.0f, 0);
	//sv_count = 0;
	//Quaternion q1;
	//q1.SetIdentity();
	//for (Uint32 j = 0; j <= segment; ++j) {
	//	Quaternion q2 = q1;
	//	for (Uint32 i = 0; i < segment; ++i) {
	//		Vector3 p;
	//		q2.RotateVector(p, v);
	//		sphere_vertex[sv_count++] = p;
	//		q2 = dq2 * q2;
	//	}
	//	q1 = dq1 * q1;
	//}

	//m_sphere_vb->Update(&sphere_vertex[0], sphere_vertex.size() * sizeof(Vector3));
	//m_sphere_vertex_count = sv_count;
}

void Cloth::AddSpring(Uint32 a, Uint32 b, Float ks, Float kd, SpringType type)
{
	Spring spring;
	spring.p1   = a;
	spring.p2   = b;
	spring.Ks   = ks;
	spring.Kd   = kd;
	spring.type = type;
	Vector3 deltaP = m_particles[a] - m_particles[b];
	spring.rest_length = deltaP.Length();
	m_springs.push_back(spring);
}

void Cloth::SetTransform(const Vector3 &pos, const Quaternion &rot)
{
	m_pos = pos;
	m_rot = rot;
	for(Uint32 i = 0; i < m_particle_count; ++i) {
		Vector3 p(0, 0, 0);
		rot.RotateVector(p, m_particles[i]); // Ðý×ª
		p += pos; // Æ½ÒÆ
		m_particles[i] = p;
	}
}

void Cloth::StepPhysics(Float dt)
{
	ComputeForces(dt);
	IntegrateVerlet(dt);
	SphereCollision(Vector3(0,0,0), 1.0f);
}

void Cloth::IntegrateVerlet(Float deltaTime)
{
	Float deltaTime2Mass = (deltaTime * deltaTime) / m_mass;
	for(Uint32 i = 0; i < m_particle_count; ++i) {
		Vector3 buffer = m_particles[i];
		m_particles[i] = m_particles[i] + (m_particles[i] - m_last_particles[i]) + m_force_per_vertex[i] * deltaTime2Mass;
		m_last_particles[i] = buffer;
	}
}

void Cloth::ComputeForces(Float dt)
{
	for(Uint32 i = 0; i < m_particle_count; ++i) {
		m_force_per_vertex[i] = Vector3(0);
		Vector3 velocity = GetVerletVelocity(m_particles[i], m_last_particles[i], dt);
		//add gravity force
		if(i != 0 && i != m_width) {
			m_force_per_vertex[i] += gravity * m_mass;
		}
		//add force due to damping of velocity
		m_force_per_vertex[i] += velocity * DEFAULT_DAMPING;
	}

	for(Uint32 i = 0; i < m_springs.size(); ++i) {
		Vector3 p1 = m_particles[m_springs[i].p1];
		Vector3 p1Last = m_last_particles[m_springs[i].p1];
		Vector3 p2 = m_particles[m_springs[i].p2];
		Vector3 p2Last = m_last_particles[m_springs[i].p2];

		Vector3 v1 = GetVerletVelocity(p1, p1Last, dt);
		Vector3 v2 = GetVerletVelocity(p2, p2Last, dt);

		Vector3 deltaP = p1 - p2;
		Vector3 deltaV = v1 - v2;
		Float dist = deltaP.Length();
		Vector3 dirP = deltaP;
		dirP.Normalize();

		Float Fs = -m_springs[i].Ks * (dist - m_springs[i].rest_length);
		Float Fd = m_springs[i].Kd * Dot(deltaV, dirP);
		Vector3 springForce = dirP * (Fs + Fd);

		if(m_springs[i].p1 != 0 && m_springs[i].p1 != m_width) {
			m_force_per_vertex[m_springs[i].p1] += springForce;
		}
		if(m_springs[i].p2 != 0 && m_springs[i].p2 != m_width) {
			m_force_per_vertex[m_springs[i].p2] -= springForce;
		}
	}
}

void Cloth::SphereCollision(const Vector3 &center, Float radius)
{
	m_sphere_center = center;
	m_sphere_radius = radius;
	for(Uint32 i = 0; i < m_particle_count; ++i) {
		Vector3 delta0 = m_particles[i] - center;
		float distance = delta0.Length();
		if (distance < radius) {
			delta0 = delta0 * (radius - distance) / distance;
			m_particles[i] += delta0;
			m_last_particles[i] = m_particles[i];
		}
	}
}

void Cloth::Update()
{
	StepPhysics(1/60.0f);

	for(Uint32 j = 0; j <= m_height; ++j) {
		for(Uint32 i = 0; i <= m_width; ++i) {
			Uint32 count = j * (m_width + 1) + i;
			m_vertex[count].pos = m_particles[count];
		}
	}

	m_vb->Update(&m_vertex[0], m_vertex.size() * sizeof(Vertex));

}

void Cloth::FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env)
{
	RenderContext *rc = new RenderContext;
	Matrix to_world = GetLocalToWorldMatrix();
	Matrix to_local = GetWorldToLocalMatrix();
	rc->world_to_local = to_local;
	rc->local_to_world = to_world;
	rc->mvp = to_world * env->vp;
	rc->layout = m_layout;
	rc->vb = m_vb;
	rc->ib = m_ib;
	rc->mtl = m_mtl;
	rc->prim_type = cTriangleList;
	rc->prim_vert_count = m_width * m_height * 6;
	rc->start_index = 0;
	rc->rs.cull_mode = cCullNone;
//	rc->rs.fill_mode = cFillWireframe;
	rc_queue->Append(rc);
}

void Cloth::RenderDebug(UtilityRenderer *renderer)
{

}
