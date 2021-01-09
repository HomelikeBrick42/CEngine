#pragma once

#include "System/Defines.h"

DECLARE_TYPE(Window);

typedef void(*WindowCloseCallback)	(Window* window);
typedef void(*WindowResizeCallback)	(Window* window, u32 width, u32 height);

typedef struct Window
{
	WindowCloseCallback CloseCallback;
	WindowResizeCallback ResizeCallback;
	void* Data;
} Window;

Window* WindowCreate(u32 width, u32 height, const char* title);
void WindowDestroy(Window* window);
void WindowShow(Window* window);
void WindowHide(Window* window);
void WindowUpdate(Window* window);
