#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogldevice.h"
#include "oglrenderer/oglgeobuffer.h"
#include "oglrenderer/oglinputlayout.h"
#include "oglrenderer/ogltexture.h"
#include "oglrenderer/oglmaterialtemplate.h"
#include "oglrenderer/oglwindowtarget.h"
#include "oglrenderer/oglrendertarget.h"
#include "oglrenderer/oglrenderer.h"

#include "engine/engine.h"
#include "engine/resource.h"
#include "engine/texture.h"
#include "engine/material.h"
#include "engine/meshresource.h"
#include "engine/resourcecache.h"
#include "engine/rendercontext.h"
#include "engine/utilityrenderer.h"
#include "engine/scenenode.h"
#include "engine/cameranode.h"
#include "engine/lightnode.h"
#include "engine/rendernode.h"
#include "engine/postprocess.h"
#include "engine/postprocessdof.h"
#include "engine/scene.h"
#include "engine/cloth.h"
#include "engine/gridnode.h"
#include "engine/meshnode.h"

#include "iosapp.h"

extern ENGINE_API IRenderer *g_renderer;

Application& Application::GetInstance()
{
	static Application app;
	return app;
}

bool Application::InitRenderer(Uint32 width, Uint32 height, void *GLKView)
{
	m_shutdown = false;

	RendererDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.adapter = 0;
	desc.brefrast = false;
	desc.interval = false;
	desc.bfullscreen = false;
	desc.width = width;
	desc.height = height;
	desc.colorbits = 32;
	desc.alphabits = 8;
	desc.depthbits = 24;
	desc.stencilbits = 0;
	desc.hwnd = NULL;
	desc.pContextObj = GLKView;

	IRenderer *renderer = CreateRenderer(desc);

	LogUtils::Instance()->LogInfo("CreateRenderer renderer = %p, w x h = %d x %d", renderer, width, height);

	if (renderer == NULL) {
		return false;
	}

	g_renderer = renderer;

	m_timer = new Timer;

	m_scene = new Scene(g_renderer);

	m_camera_node = new CameraNode;
	m_camera_node->SetAspect(1.0f * width / height);

	Vector3 look_forward(0.0f, 1.0f, -0.5f);
	look_forward.Normalize();
	Vector3 pos = -look_forward * 15.0f;
	Vector3 look_up(0.0f, 0.0f, 1.0f);
	Matrix mat;
	Quaternion rot;
	OrientToMatrix(look_forward, look_up, mat); 
	MatrixToQuaternion(mat, rot);
	m_camera_node->SetPosition(pos);
	m_camera_node->SetRotation(rot);

	m_scene->SetMainCamera(m_camera_node);

	Config::Instance().enable_debug_draw = false;

	return true;

}


const int nCount = 10;
const int nLightCount = 10;
PointLight* pointLights[nLightCount/2][nLightCount];

void Application::InitScene()
{
    
    Color colors[] = {
        Color::Blue,
        Color::Green,
        Color::Red,
    };
        
    const int colorCount = 3;
    for (int i = 0; i < nLightCount/2; ++i)
    {
        for (int j = 0; j < nLightCount; ++j)
        {
            pointLights[i][j] = new PointLight(2.0f, 1.0f, 1.0f);
            pointLights[i][j]->SetLightColor(colors[((i % colorCount) * (j % colorCount) ) % colorCount]);
            pointLights[i][j]->SetPosition(Vector3((i - (nLightCount/2 - 1) * 0.5f) * 2.0f, (j - (nLightCount - 1) * 0.5f) * 2.0f, 1.0f));
            GetScene()->AddLight(pointLights[i][j]);
        }
    }
        
    MeshResource *sphere_mesh = (MeshResource*)ResourceCache::Instance()->Create("sphere.mesh", cResTypeMesh);
    
    for (int i = 0; i < nCount/2; ++i)
    {
        for (int j = 0; j < nCount; ++j)
        {
            MeshNode *sphereNode = new MeshNode(sphere_mesh);
            sphereNode->SetPosition(Vector3((i - (nCount/2 - 1) * 0.5f) * 2.0f, (j - (nCount - 1) * 0.5f) * 2.0f, 0));
            GetScene()->AddNode(sphereNode);
        }
    }

}

/*
void Application::InitScene()
{
    PointLight *pt = new PointLight(2.0f, 1.0f, 1.0f);
    pt->SetLightColor(Color::White);
    pt->SetPosition(Vector3(0.0f, 0.0f, 1.0f));
    GetScene()->AddLight(pt);

    MeshResource *sphere_mesh = (MeshResource*)ResourceCache::Instance()->Create("sphere.mesh", cResTypeMesh);
    MeshNode *sphereNode = new MeshNode(sphere_mesh);
    sphereNode->SetPosition(Vector3(0,0,0));
    GetScene()->AddNode(sphereNode);
}
*/

void Application::Release()
{
	Cleanup();
}

void Application::SetFrameRate(Uint32 frame_rate)
{
	m_frame_rate = frame_rate;
	if(m_frame_rate > MAX_FRAME_RATE) {
		m_frame_rate = MAX_FRAME_RATE;
	}
}

void Application::Cleanup()
{

}

void Application::UpdateFrame()
{
	if (m_scene == NULL) {
		return;
	}

	Float interv = 1000.0f / m_frame_rate;
	Float elapse_time = m_timer->GetElapseMilliseconds();
	if (elapse_time < interv)
	{
		Display();
	}
	else
	{
		m_timer->Reset();
		Logic();
		Display();

		m_flush_time += elapse_time;
		if (m_flush_time > 500.0f)
		{
			m_render_rate = 1000.0f * m_display_count / m_flush_time;
			m_flush_time = 0.0f;
			m_display_count = 0;
		}
	}
}

void Application::Logic()
{
    
    Float angle = DegreeToRadian(1.125f);
	Float cosT = cos(angle);
	Float sinT = sin(angle);
	for (int i = 0; i < nLightCount/2; ++i)
	{
		for (int j = 0; j < nLightCount; ++j)
		{
			const Vector3 &pos = pointLights[i][j]->GetPosition();
			Vector3 p = pos;
			p.x = pos.x * cosT - pos.y * sinT;
			p.y = pos.x * sinT + pos.y * cosT;
			pointLights[i][j]->SetPosition(p);
		}
	}
    
    if(m_update) m_update();
	m_scene->Update();
	++m_logic_count;
}

void Application::Display()
{
	m_scene->Render();
	++m_display_count;
}

void Application::ShutDown()
{
	m_shutdown = true;
	((OGLRenderer*)g_renderer)->Destroy();
}

void Application::RegisterUpdate(FuncUpdate update)
{
	m_update = update;
}
