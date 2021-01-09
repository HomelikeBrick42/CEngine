#include "Renderer/Renderer.h"
#include "System/Window.h"

Renderer* VulkanRendererCreate(Window* window);

Renderer* RendererCreate(Window* window, RendererAPI api)
{
	switch (api)
	{
	case RENDERER_API_VULKAN:
		return VulkanRendererCreate(window);
	default:
		return NULL;
	}
}
