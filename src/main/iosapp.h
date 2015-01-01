#pragma once

class Application
{
	enum {
		MAX_FRAME_RATE = 120,
	};
	typedef void (*FuncUpdate)();

private:
	Scene *m_scene;
	CameraNode *m_camera_node;

	Bool m_shutdown;

	Float m_render_rate;
	unsigned long m_frame_rate;
	unsigned long m_logic_count;
	unsigned long m_display_count;
	Float m_flush_time;
	Timer *m_timer;

	FuncUpdate m_update;

public:
	bool InitRenderer(Uint32 width, Uint32 height, void *GLKView);
    
    void InitScene();
    
	Scene* GetScene() { return m_scene; }
	void SetFrameRate(Uint32 frame_rate);
	void Release();
	void UpdateFrame();
	void ShutDown();
	void RegisterUpdate(FuncUpdate update);

	static Application& GetInstance();

private:
	Application() : m_scene(NULL), m_render_rate(0), m_frame_rate(30),
		m_logic_count(0), m_display_count(0), m_flush_time(0.0f),
		m_timer(NULL), m_update(NULL), m_shutdown(true) {}

    Application(const Application& other);
	Application& operator = (const Application& other);

	void Cleanup();
	void Logic();
	void Display();

};
