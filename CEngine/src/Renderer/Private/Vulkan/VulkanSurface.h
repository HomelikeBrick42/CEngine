#pragma once

#include "System/Defines.h"

DECLARE_TYPE(Window);

#include <vulkan/vulkan.h>

#if PLATFORM_WINDOWS
	#define VK_KHR_PLATFORM_SURFACE_EXTENTION_NAME "VK_KHR_win32_surface"
#else
	#error "Implement vulkan surface extention for this platform!"
#endif

VkResult vkCreateSurfaceKHR(VkInstance instance, Window* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
