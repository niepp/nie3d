#pragma once
#include <vector>
#include <map>

struct RenderEnv {
	Vector4 eye_pos;
	Matrix view;
	Matrix proj;
	Matrix vp; // view * proj
	Matrix inv_vp; // vp's inverse
	ITexture *light_tex;

	RenderEnv() : light_tex(NULL) {}

};

struct LightParam{
	Vector4 lightPositionRange;
	Vector4 lightColorAtten;
};

struct GBufferConstant{
	Vector4 normalSpecularParam; // x : specular Glossy;
};

struct RenderContext
{
//	IVertexArrayObject *vao;
	IInputLayout *layout;
	IVertexBuffer *vb;
	IIndexBuffer *ib;
	PrimitiveType prim_type;
	Uint32 start_index;
	Uint32 prim_vert_count;
	Material *mtl;
	Matrix mvp;
	Matrix local_to_world;
	Matrix world_to_local;
	RenderState rs;

	LightParam light_param;
	GBufferConstant gbufer_constant;

	RenderContext() : layout(NULL)
		, vb(NULL), ib(NULL), mtl(NULL) { }

};

class RenderContextQueue {
public:
	RenderContextQueue(IRenderer *renderer) : m_renderer(renderer) { }
	void Append(const RenderContext *rc);
	void Clear();
	void DoDraw(RenderEnv *render_env);
	void DrawWithShader(IShader *shader, RenderEnv *render_env);
private:
	IRenderer *m_renderer;
	typedef std::vector<const RenderContext*> RenderContextVec;
	RenderContextVec m_rc_vec;
	
};

