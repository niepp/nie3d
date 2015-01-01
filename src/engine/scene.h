#pragma once

class SceneNode;
class ENGINE_API Scene {
public:
	Scene(IRenderer *renderer);

	void AddNode(SceneNode *node);
	void RemoveNode(SceneNode *node);

	void Update();
	void Render();

	void RenderLight(LightNode *light);

	void SetMainCamera(CameraNode *camera);
	CameraNode* GetMainCamera();

	void AddLight(LightNode *light);
	void DelLight(LightNode *light);

private:
	IRenderTarget *m_scene_target;
	ITexture *m_scene_texture;
	IRenderTarget *m_screen_target;
	RenderContextQueue *m_rc_queue;
	SceneNodeVec m_nodes;
	LightVec m_lights;

	CameraNode *m_camera_node;
	RenderEnv m_render_env;
	UtilityRenderer *m_utility_renderer;

	ITexture *m_depth_texture;

	// gbuffer
	IRenderTarget *m_gbuffer_target;
	ITexture *m_gbuffer_texture;
	IShader *m_gbuffer_shader;

	// light buffer
	IRenderTarget *m_light_target;
	ITexture *m_light_texture;
	IShader *m_light_shader;

	// light shapes
	const Sphere *m_unit_sphere;
	IInputLayout *m_point_shape_layout;
	IVertexBuffer *m_point_shape_vb;
	IIndexBuffer *m_point_shape_ib;

	// post process
	QuadRender *m_quad_render;
	IShader *m_blit_shader;
	PostProcessDOF *m_dof;

};
