#if PLATFORM == PLATFORM_WIN32

#include <windows.h>

#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine/engine.h"
#include "engine/resource.h"
#include "engine/texture.h"
#include "engine/material.h"
#include "engine/meshresource.h"
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

#include "cameractrl.h"
#include "winapp.h"

CameraCtrl g_camera_ctrl;

extern ENGINE_API IRenderer *g_renderer;

extern "C"
{
	OGLRENDERER_API IRenderer* CreateRenderer(const RendererDesc &desc);
};

Vector3 g_pos;
Quaternion g_rot;
CameraNode *g_camera_node;


LRESULT CALLBACK OnKeyDown(WPARAM wParam)
{
	switch(wParam)
	{
	case 'A':
		g_camera_node->SetPosition(g_pos);
		g_camera_node->SetRotation(g_rot);
		g_camera_ctrl.InitCamera(g_pos, Vector3(0.0f, 1.0f, -0.5f), Vector3(0.0f, 0.0f, 1.0f));
		break;
	case 'D':
		break;
	case 'W':
		break;
	case 'S':
		break;
	case 'Q':
		break;
	case 'E':
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_F1:
		Config::Instance().enable_debug_draw = !Config::Instance().enable_debug_draw;
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	case WM_SIZE:
	case WM_EXITSIZEMOVE:
		g_renderer->GetMainTarget()->HandlePossibleSizeChange();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	g_camera_ctrl.MsgProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Application& Application::GetInstance()
{
	static Application app;
	return app;
}

bool Application::Init(HINSTANCE instance, const char *title, int width, int height)
{
	strcpy(m_title, title);
	const char class_name[] = "wndclass";

	// Register the window class
	WNDCLASSEX wc =	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		instance, NULL, NULL, NULL, NULL,
		class_name, NULL
	};

	RegisterClassEx(&wc);

	// calculate client size，设置窗口客户区大小
	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = width;
	clientSize.bottom = height;
	DWORD style = WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	//计算客户区矩形大小
	::AdjustWindowRect(&clientSize, style, FALSE);
	int w = clientSize.right - clientSize.left;
	int h = clientSize.bottom - clientSize.top;

	// Create the application's window
	m_hwnd = CreateWindow(class_name, title,
		style, 0, 0, w, h,
		NULL, NULL, instance, NULL);

	::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(m_hwnd);

	FileUtils::Instance()->SetResourceRoot("../../res/");

	RendererDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.adapter = 0;
	desc.interval = false;
	desc.bfullscreen = false;
	desc.width = width;
	desc.height = height;
	desc.colorbits = 32;
	desc.alphabits = 8;
	desc.depthbits = 24;
	desc.stencilbits = 0;
	desc.context_obj = (void*)m_hwnd;

	IRenderer *renderer = CreateRenderer(desc);
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
	Vector3 cam_pos = -look_forward * 15.0f;
	Vector3 look_up(0.0f, 0.0f, 1.0f);
	Matrix mat;
	Quaternion cam_rot;
	OrientToMatrix(look_forward, look_up, mat); 
	MatrixToQuaternion(mat, cam_rot);
	m_camera_node->SetPosition(cam_pos);
	m_camera_node->SetRotation(cam_rot);

	g_camera_ctrl.InitCamera(cam_pos, look_forward, look_up);

	m_scene->SetMainCamera(m_camera_node);

	Config::Instance().enable_debug_draw = false;

	g_camera_node = m_camera_node;
	g_pos = m_camera_node->GetPosition();
	g_rot = m_camera_node->GetRotation();

	return true;

}

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

void Application::DoFrame()
{
	Float interv = 1000.0f / m_frame_rate;
	Float elapse_time = m_timer->GetElapseMilliseconds();
	if (elapse_time < interv) // 逻辑帧时间未到
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
			Float render_rate = 1000.0f * m_display_count / m_flush_time;
			m_flush_time = 0.0f;
			m_display_count = 0;
			char str[64];
			::sprintf(str, "%s  fps: %.1f", m_title, render_rate);
			::SetWindowText(m_hwnd, str);
		}
	}
}

void Application::Logic()
{
	m_update();

	const Vector3 &pos = g_camera_ctrl.GetCameraPos();
	m_camera_node->SetPosition(pos);
	const Quaternion &rot = g_camera_ctrl.GetCameraRot();
	m_camera_node->SetRotation(rot);

	m_scene->Update();

	++m_logic_count;
}

void Application::Display()
{
	m_scene->Render();
	++m_display_count;
}

void Application::MainLoop()
{
    // Show the window
	::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(m_hwnd);

    // Enter the message loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while(msg.message != WM_QUIT)
    {
		if(::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
        }
        else {	
			DoFrame();
		}
    }
}

void Application::RegisterUpdate(FuncUpdate update)
{
	m_update = update;
}

#endif