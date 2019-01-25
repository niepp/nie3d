#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

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

#if PLATFORM == PLATFORM_WIN32
#include <iostream>
#include "cameractrl.h"
#include "winapp.h"
#elif PLATFORM == PLATFORM_ANDROID
#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "androidapp.h"
#endif

const int nCount = 10;
const int nLightCount = 10;
PointLight* pointLights[nLightCount/2][nLightCount];
bool pause = false;

//
//void InitScene1()
//{
//	m_scene = new Scene(g_renderer);
//
//	m_camera_node = new CameraNode;
//	m_scene->SetMainCamera(m_camera_node);
//
//	PointLight *pointLight = new PointLight(5.0f, 1.0f, 1.0f);
//	pointLight->SetLightColor(Color::White);
//	pointLight->SetPosition(Vector3(-3.0f, 0, 1.0f));
//	m_scene->AddLight(pointLight);
//
//	Cloth *cloth = new Cloth(12, 12, 1.0f, 0.08f);
//	m_scene->AddNode(cloth);
//
//	m_grid_node = new GridNode;
//	m_scene->AddNode(m_grid_node);
//
//	Vector3 look_forward(0.0f, 1.0f, -0.5f);
//	look_forward.Normalize();
//	Vector3 cam_pos = -look_forward * 15.0f;
//	Vector3 look_up(0.0f, 0.0f, 1.0f);
//
//	g_camera_ctrl.InitCamera(cam_pos, look_forward, look_up);
//}

void InitScene()
{
	Application &app = Application::GetInstance();

	Color colors[] = {
		Color::Red,
		Color::Green,
		Color::Blue,
	};

	const int colorCount = 3;
	for (int i = 0; i < nLightCount/2; ++i)
	{
		for (int j = 0; j < nLightCount; ++j)
		{
			pointLights[i][j] = new PointLight(2.0f, 1.0f, 1.0f);
			pointLights[i][j]->SetLightColor(colors[((i % colorCount) * (j % colorCount) ) % colorCount]);
			pointLights[i][j]->SetPosition(Vector3((i - (nLightCount/2 - 1) * 0.5f) * 2.0f, (j - (nLightCount - 1) * 0.5f) * 2.0f, 1.0f));
			app.GetScene()->AddLight(pointLights[i][j]);
		}
	}

	MeshResource *sphere_mesh = (MeshResource*)ResourceCache::Instance()->Create("sphere.mesh", cResTypeMesh);

	for (int i = 0; i < nCount/2; ++i)
	{
		for (int j = 0; j < nCount; ++j)
		{
			MeshNode *sphereNode = new MeshNode(sphere_mesh);
			sphereNode->SetPosition(Vector3((i - (nCount/2 - 1) * 0.5f) * 2.0f, (j - (nCount - 1) * 0.5f) * 2.0f, 0));
			app.GetScene()->AddNode(sphereNode);
		}
	}

}

void InitScene3()
{
	Application &app = Application::GetInstance();

	PointLight *pointLight = new PointLight(15.0f, 1.0f, 1.0f);
	pointLight->SetLightColor(Color::Blue);
	pointLight->SetPosition(Vector3(0.0f, 0.0f, 8.0f));
	app.GetScene()->AddLight(pointLight);

	MeshResource *sphere_mesh = (MeshResource*)ResourceCache::Instance()->Create("sphere.mesh", cResTypeMesh);
	MeshNode *sphereNode = new MeshNode(sphere_mesh);
	sphereNode->SetPosition(Vector3(0.0f, 0.0f, 0));
	sphereNode->SetScale(Vector3(4.5f, 4.5f, 4.5f));
	app.GetScene()->AddNode(sphereNode);
}

void Update()
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
}

#if PLATFORM == PLATFORM_WIN32
int main(void)
{
	// Change to executable's folder
	{
		char buffer[1024];
		GetModuleFileName(NULL, &buffer[0], sizeof(buffer));
		std::string rawfilename(buffer);
		size_t pos = rawfilename.rfind('\\');
		std::string path = rawfilename.substr(0, pos + 1);
		SetCurrentDirectory(path.c_str());		
	}

	Application &app = Application::GetInstance();
	app.RegisterUpdate(Update);

	if (!app.Init(GetModuleHandle(NULL), "base demo!", 800, 600))
	{
		::MessageBox(NULL, "³õÊ¼»¯Ê§°Ü£¡", "´íÎó", MB_OK);
		return -1;
	}

	InitScene();

	app.SetFrameRate(30);
	app.MainLoop();
	app.Release();

	return 0;
}

#elif PLATFORM == PLATFORM_ANDROID

static void on_app_cmd(struct android_app *app, int32_t cmd)
{
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			if (app->window != NULL) {
				if(Application::GetInstance().InitRenderer()) {
					InitScene();
				}
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			Application::GetInstance().ShutDown();
			LogUtils::Instance()->LogInfo("app_cmd_term_window!");
			break;
		default:
			break;
	}
}

static int32_t on_handle_input(struct android_app* app, AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		return 1;
	}
	return 0;
}


void android_main(struct android_app* state)
{
	// Make sure glue isn't stripped.
	app_dummy();

	state->onAppCmd = on_app_cmd;
	state->onInputEvent = on_handle_input;

	Application &app = Application::GetInstance();
	app.InitAndroid(state);
	app.RegisterUpdate(Update);
	app.SetFrameRate(30);
	app.MainLoop();
	app.Release();
}

#endif

