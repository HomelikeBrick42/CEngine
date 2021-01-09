#pragma once

#include "System/Defines.h"
#include "vulkan/vulkan.h"

typedef struct VulkanRendererData
{
	VkInstance Instance;
#if _DEBUG
	VkDebugUtilsMessengerEXT DebugMessenger;
#endif
	VkSurfaceKHR Surface;
	VkPhysicalDevice PhysicalDevice;
	u32 GraphicsQueueFamilyIndex;
	VkDevice Device;
	VkQueue GraphicsQueue;
} VulkanRendererData;
