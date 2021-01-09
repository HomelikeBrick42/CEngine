#pragma once

#include "System/Defines.h"

DECLARE_TYPE(Window);

typedef enum RendererAPI
{
	RENDERER_API_VULKAN
} RendererAPI;

DECLARE_TYPE(Renderer);

typedef void(*RendererDestroyFunc)(Renderer* renderer);

typedef struct Renderer
{
	RendererAPI API;
	Window* Window;
	const RendererDestroyFunc _Destroy;
	void* Data;
} Renderer;

Renderer* RendererCreate(Window* window, RendererAPI api);
#define RendererDestroy(renderer) renderer->_Destroy(renderer)
