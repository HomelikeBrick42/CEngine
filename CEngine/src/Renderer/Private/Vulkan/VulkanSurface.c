#include "Renderer/Private/Vulkan/VulkanSurface.h"

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <vulkan/vulkan_win32.h>

#include "System/Window.h"
#include "System/Private/Win32WindowData.inl"

VkResult vkCreateSurfaceKHR(VkInstance instance, Window* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	surfaceCreateInfo.hinstance = ((Win32WindowData*)window->Data)->HInstance;
	surfaceCreateInfo.hwnd = ((Win32WindowData*)window->Data)->HWnd;

	return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, allocator, surface);
}

#else
	#error "Implement vulkan surface for this platform!"
#endif
