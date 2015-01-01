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

#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "androidapp.h"

extern ENGINE_API IRenderer *g_renderer;

String JString2String(JNIEnv *env, jstring jstr)
{
	const char *chars = env->GetStringUTFChars(jstr, NULL);
	String path = chars;
	env->ReleaseStringUTFChars(jstr, chars);
	return path;
}

String GetApkPath(android_app *app)
{
	JavaVM *jvm = app->activity->vm;
	JNIEnv* env = NULL;
	int state = jvm->AttachCurrentThread(&env, 0);
	if(state < 0) {
		__android_log_print(ANDROID_LOG_ERROR, "error", "getApkPath Failed!");
		return "";
	}

	jclass clazz = env->GetObjectClass(app->activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "getPackageResourcePath", "()Ljava/lang/String;");
	jobject result = env->CallObjectMethod(app->activity->clazz, methodID);
	String path = JString2String(env, (jstring)result);
	jvm->DetachCurrentThread();
	return path;
}

Application& Application::GetInstance()
{
	static Application app;
	return app;
}

bool Application::InitRenderer()
{
	m_shutdown = false;

	String apkPath = GetApkPath(m_android_app);
	FileUtils::Instance()->SetResourceRoot(apkPath.AsCharPtr());

	LogUtils::Instance()->LogInfo("InitRenderer apkPath = %s", apkPath.AsCharPtr());

	int width = 0, height = 0;
	width = ANativeWindow_getWidth(m_android_app->window);
	height = ANativeWindow_getHeight(m_android_app->window);

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
	desc.context_obj = (void*)m_android_app->window;

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

void Application::Resume()
{
	int width = 0, height = 0;
	width = ANativeWindow_getWidth(m_android_app->window);
	height = ANativeWindow_getHeight(m_android_app->window);

	LogUtils::Instance()->LogInfo("android_app window = %p", m_android_app->window);

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
	desc.context_obj = (void*)m_android_app->window;

	ResumeRenderer(g_renderer, desc);

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
	if (m_scene == NULL) {
		return;
	}

	Float interv = 1000.0f / m_frame_rate;
	Float elapse_time = m_timer->GetElapseMilliseconds();
	if (elapse_time < interv) // Âß¼­Ö¡Ê±¼äÎ´µ½
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
	m_update();
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
	while(true)
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL) {
				source->process(m_android_app, source);
			}
			if (m_android_app->destroyRequested != 0) {	
				LogUtils::Instance()->LogInfo("App destroy!");
				return;
			}
		}
		
		if (!m_shutdown) {
			DoFrame();
		}
	}
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
