#pragma once
#include <vector>

/*
	Mass-Spring system using Verlet integration
*/
class ENGINE_API Cloth : public SceneNode {
public:
	enum SpringType{
		STRETCH_SPRING = 0,
		SHEAR_SPRING,
		BEND_SPRING,
	};

	struct Spring {
		Uint32 p1, p2;
		Float rest_length;
		Float Ks, Kd;
		SpringType type;
	};

	struct Vertex{
		Vector3 pos;
		Float u, v;
	};

	Cloth(Uint32 width, Uint32 height, Float grid_size, Float mass);

	void AddSpring(Uint32 a, Uint32 b, Float ks, Float kd, SpringType type);

	void SetTransform(const Vector3 &pos, const Quaternion &rot); // pos and rot of the cloth

	void StepPhysics(Float dt);

	void IntegrateVerlet(Float deltaTime);

	inline Vector3 GetVerletVelocity(Vector3 x_i, Vector3 xi_last, Float dt) {
		return  (x_i - xi_last) / dt;
	}

	void ComputeForces(Float dt);
	void SphereCollision(const Vector3 &center, Float radius);

	virtual void Update();
	virtual void FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env);
	virtual void RenderDebug(UtilityRenderer *renderer);

private:
	Uint32 m_width, m_height; // spring number of cloth wide and high
	Uint32 m_particle_count; // m_particle_count = (m_width + 1) * (m_height + 1)
	Float m_size;
	Float m_mass;

	std::vector<Spring> m_springs;
	std::vector<Vector3> m_particles;
	std::vector<Vector3> m_last_particles;
	std::vector<Vector3> m_force_per_vertex;

	// for render
	std::vector<Vertex> m_vertex;
	IInputLayout *m_layout;
	IVertexBuffer *m_vb;
	IIndexBuffer *m_ib;
	Material *m_mtl;

	IInputLayout *m_pos_layout;
	IVertexBuffer *m_sphere_vb;
	Uint32 m_sphere_vertex_count;
	Vector3 m_sphere_center;
	Float m_sphere_radius;

};
