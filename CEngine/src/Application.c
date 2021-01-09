#include "Application.h"
#include "Renderer/Renderer.h"
#include "System/Window.h"
#include "System/Logger.h"
#include "System/Defines.h"

extern b8 s_Running;
typedef struct ApplicationData
{
	Window* Window;
	Renderer* Renderer;
} ApplicationData;
static ApplicationData s_Data = { 0 };

static void OnWindowClose(Window* window)
{
	LOG_DEBUG("WindowCloseCallback %p", window);
	s_Running = FALSE;
}

static void OnWindowResize(Window* window, u32 width, u32 height)
{
	LOG_DEBUG("WindowResizeCallback %p: %d, %d", window, width, height);
}

void ApplicationInitialize()
{
	s_Data.Window = WindowCreate(640, 480, "Window");
	s_Data.Window->CloseCallback = OnWindowClose;
	s_Data.Window->ResizeCallback = OnWindowResize;
	WindowShow(s_Data.Window);

	s_Data.Renderer = RendererCreate(s_Data.Window, RENDERER_API_VULKAN);
}

void ApplicationUpdate(f64 dt)
{
	WindowUpdate(s_Data.Window);
}

void ApplicationShutdown()
{
	WindowHide(s_Data.Window);
	RendererDestroy(s_Data.Renderer);
	WindowDestroy(s_Data.Window);
}
