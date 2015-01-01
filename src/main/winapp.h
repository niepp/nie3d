#pragma once
#if PLATFORM == PLATFORM_WIN32

class Application
{
	enum {
		MAX_FRAME_RATE = 120,
	};
	typedef void (*FuncUpdate)();

private:
	Scene *m_scene;
	CameraNode *m_camera_node;

	HWND m_hwnd;
	char m_title[256];

	unsigned long m_frame_rate;
	unsigned long m_logic_count;
	unsigned long m_display_count;
	Float m_flush_time;
	Timer *m_timer;

	FuncUpdate m_update;

public:
	bool Init(HINSTANCE instance, const char *title, int width, int height);
	Scene* GetScene() { return m_scene; }
	void SetFrameRate(Uint32 frame_rate);
	void Release();
	void MainLoop();
	void RegisterUpdate(FuncUpdate update);

	static Application& GetInstance();

private:
	Application() : m_hwnd(0), m_frame_rate(30),
		m_logic_count(0), m_display_count(0), m_flush_time(0.0f),
		m_timer(NULL), m_update(NULL) {}

	Application(const Application& other);
	Application& operator = (const Application& other);

	void Cleanup();
	void DoFrame();
	void Logic();
	void Display();
	
};

#endif