#pragma once
#include <vector>
#include <map>

class ENGINE_API UtilityRenderer {
public:
	struct ColorVertex
	{
		Vector3 pos;
		Color color;
	};

	typedef std::vector<ColorVertex> VertexVec;

	UtilityRenderer(IRenderer *renderer);
	~UtilityRenderer();

	void DrawLine(const Vector3 &pos0, const Vector3 &pos1, Color color = Color::Red, Bool zTestEnabled = true);
	void DrawSphere(const Vector3 &center, Float radius, Color color = Color::Red, Bool zTestEnabled = true);
	void DoDraw(const RenderEnv *env);

private:
	IShader *m_shader;
	IInputLayout *m_layout;

	IVertexBuffer *m_vb;
	IVertexBuffer *m_vb_test_depth;

	VertexVec m_vertex;
	VertexVec m_vertex_test_depth;

	// triangle
	const Sphere *m_unit_sphere;

	VertexVec m_sphere_vectex;
	VertexVec m_sphere_vectex_test_depth;
	std::vector<Uint16> m_sphere_indices;
	std::vector<Uint16> m_sphere_indices_test_depth;
	
	IVertexBuffer *m_sphere_vb;
	IVertexBuffer *m_sphere_vb_test_depth;
	IIndexBuffer *m_sphere_ib;
	IIndexBuffer *m_sphere_ib_test_depth;

};
