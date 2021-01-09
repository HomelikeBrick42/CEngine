#include "Renderer/Renderer.h"
#include "Renderer/Private/Vulkan/VulkanDebugger.h"
#include "Renderer/Private/Vulkan/VulkanSurface.h"
#include "System/Window.h"
#include "System/Logger.h"
#include "System/Defines.h"

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <string.h>

#define VK_CHECK(expr) \
	if ((expr) != VK_SUCCESS) \
	{ \
		LOG_FATAL("Vulkan assertion failed Expression: '%s', Function: '%s', Line: %d", #expr, __FUNCTION__, __LINE__); \
		__debugbreak(); \
	}

#if _DEBUG

static const u32 InstanceExtentionCount = 3;
static const char* InstanceExtentions[3] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_PLATFORM_SURFACE_EXTENTION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

static const u32 InstanceLayerCount = 1;
static const char* InstanceLayers[1] = {
	"VK_LAYER_KHRONOS_validation"
};

static const u32 DeviceExtentionCount = 1;
static const char* DeviceExtentions[1] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#else

static const u32 InstanceExtentionCount = 2;
static const char* InstanceExtentions[2] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_PLATFORM_SURFACE_EXTENTION_NAME
};

static const u32 InstanceLayerCount = 0;
static const char** InstanceLayers = NULL;

static const u32 DeviceExtentionCount = 1;
static const char* DeviceExtentions[1] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#endif

#include "Renderer/Private/Vulkan/VulkanRendererData.inl"

void VulkanRendererDestroy(Renderer* renderer)
{
	VulkanRendererData* data = (VulkanRendererData*)renderer->Data;
	if (data->Device)
	{
		VK_CHECK(vkDeviceWaitIdle(data->Device));
		vkDestroyDevice(data->Device, NULL);
	}
	if (data->Surface)
		vkDestroySurfaceKHR(data->Instance, data->Surface, NULL);
#if _DEBUG
	if (data->DebugMessenger)
		vkDestroyDebugerEXT(data->Instance, data->DebugMessenger, NULL);
#endif
	if (data->Instance)
		vkDestroyInstance(data->Instance, NULL);
	free(renderer);
}

Renderer* VulkanRendererCreate(Window* window)
{
	Renderer* renderer = (Renderer*)malloc(sizeof(Renderer) + sizeof(VulkanRendererData));
	renderer->API		= RENDERER_API_VULKAN;
	renderer->Window	= window;
	*(RendererDestroyFunc*)&renderer->_Destroy = VulkanRendererDestroy;

	renderer->Data = ((char*)renderer) + sizeof(Renderer);
	VulkanRendererData* data = (VulkanRendererData*)renderer->Data;
	data->Instance					= NULL;
	data->Surface					= NULL;
	data->PhysicalDevice			= NULL;
	data->GraphicsQueueFamilyIndex	= -1;
	data->Device					= NULL;
	data->GraphicsQueue				= NULL;

	u32 requiredAPIVersion = VK_API_VERSION_1_2;

	// Create Instance
	{
		// Check Version
		{
			u32 instanceVersion = 0;
			VK_CHECK(vkEnumerateInstanceVersion(&instanceVersion));
			if (instanceVersion < requiredAPIVersion) // @Bug: Test this
			{
				LOG_FATAL("%s", "Unsupported vulkan version!");
				free(renderer);
				return NULL;
			}
		}

		VkApplicationInfo applicationInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		applicationInfo.apiVersion = requiredAPIVersion;
		applicationInfo.pEngineName = "CEngine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pApplicationName = "CEngine App";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		// Has Required Extentions
		{
			u32 availableExtentionCount = 0;
			VK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &availableExtentionCount, NULL));
			VkExtensionProperties* availableExtentions =
				(VkExtensionProperties*)malloc(availableExtentionCount * sizeof(VkExtensionProperties));
			VK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &availableExtentionCount, availableExtentions));

			b8 hasRequiredExtentions = TRUE;
			for (u32 i = 0; i < InstanceExtentionCount; i++)
			{
				b8 hasExtention = FALSE;
				for (u32 j = 0; j < availableExtentionCount; j++)
				{
					if (strcmp(InstanceExtentions[i], availableExtentions[j].extensionName) == 0)
					{
						hasExtention = TRUE;
						break;
					}
				}
				if (!hasExtention)
				{
					hasRequiredExtentions = FALSE;
					break;
				}
			}
			free(availableExtentions);

			if (!hasRequiredExtentions)
			{
				LOG_FATAL("%s", "Failed to find required vulkan instance extentions!");
				free(renderer);
				return NULL;
			}
		}

		instanceCreateInfo.enabledExtensionCount = InstanceExtentionCount;
		instanceCreateInfo.ppEnabledExtensionNames = InstanceExtentions;

		// Has Required Layers
		{
			u32 availableLayerCount = 0;
			VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, NULL));
			VkLayerProperties* availableLayers =
				(VkLayerProperties*)malloc(availableLayerCount * sizeof(VkLayerProperties));
			VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

			b8 hasRequiredLayers = TRUE;
			for (u32 i = 0; i < InstanceLayerCount; i++)
			{
				b8 hasLayer = FALSE;
				for (u32 j = 0; j < availableLayerCount; j++)
				{
					if (strcmp(InstanceLayers[i], availableLayers[j].layerName) == 0)
					{
						hasLayer = TRUE;
						break;
					}
				}
				if (!hasLayer)
				{
					hasRequiredLayers = FALSE;
					break;
				}
			}
			free(availableLayers);

			if (!hasRequiredLayers)
			{
				LOG_FATAL("%s", "Failed to find required vulkan instance layers!");
				free(renderer);
				return NULL;
			}
		}

		instanceCreateInfo.enabledLayerCount = InstanceLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = InstanceLayers;

		VK_CHECK(vkCreateInstance(&instanceCreateInfo, NULL, &data->Instance));

		if (!data->Instance)
		{
			LOG_FATAL("%s", "Unable to create vulkan instance!");
			free(renderer);
			return NULL;
		}
	}

#if _DEBUG
	// Create Debugger
	{
		VK_CHECK(vkCreateDebuggerEXT(data->Instance, NULL, &data->DebugMessenger));

		if (!data->DebugMessenger)
		{
			LOG_FATAL("%s", "Unable to create vulkan debugger!");
			free(renderer);
			return NULL;
		}
	}
#endif

	// Create Surface
	{
		VK_CHECK(vkCreateSurfaceKHR(data->Instance, renderer->Window, NULL, &data->Surface));

		if (!data->Surface)
		{
			LOG_FATAL("%s", "Unable to create vulkan surface!");
			free(renderer);
			return NULL;
		}
	}

	// Select PhysicalDevice
	{
		u32 physicalDeviceCount = 0;
		VK_CHECK(vkEnumeratePhysicalDevices(data->Instance, &physicalDeviceCount, NULL));
		VkPhysicalDevice* physicalDevices =
			(VkPhysicalDevice*)malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
		VK_CHECK(vkEnumeratePhysicalDevices(data->Instance, &physicalDeviceCount, physicalDevices));

		for (u32 i = 0; i < physicalDeviceCount; i++)
		{
			// Has Required Extentions
			{
				u32 availableExtentionCount = 0;
				VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevices[i], NULL, &availableExtentionCount, NULL));
				VkExtensionProperties* availableExtentions =
					(VkExtensionProperties*)malloc(availableExtentionCount * sizeof(VkExtensionProperties));
				VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevices[i], NULL, &availableExtentionCount, availableExtentions));

				b8 hasRequiredExtentions = TRUE;
				for (u32 i = 0; i < DeviceExtentionCount; i++)
				{
					b8 hasExtention = FALSE;
					for (u32 j = 0; j < availableExtentionCount; j++)
					{
						if (strcmp(DeviceExtentions[i], availableExtentions[j].extensionName) == 0)
						{
							hasExtention = TRUE;
							break;
						}
					}
					if (!hasExtention)
					{
						hasRequiredExtentions = FALSE;
						break;
					}
				}
				free(availableExtentions);

				if (!hasRequiredExtentions)
					continue;
			}

			u32 graphicsQueueFamilyIndex = -1;
			// Get Queue Family Indices
			{
				u32 queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, NULL);
				VkQueueFamilyProperties* queueFamilies =
					(VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies);

				VkQueueFlags requiredFlags = VK_QUEUE_GRAPHICS_BIT;

				for (u32 i = 0; i < queueFamilyCount; i++)
				{
					if (queueFamilies[i].queueCount > 0)
					{
						if ((queueFamilies[i].queueFlags & requiredFlags) == requiredFlags)
						{
							graphicsQueueFamilyIndex = i;
							break;
						}
					}
				}

				free(queueFamilies);
			}

			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceProperties);

			if (physicalDeviceProperties.apiVersion >= requiredAPIVersion &&
				graphicsQueueFamilyIndex != (u32)-1)
			{
				data->PhysicalDevice = physicalDevices[i];
				data->GraphicsQueueFamilyIndex = graphicsQueueFamilyIndex;
				if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
					break;
			}
		}

		free(physicalDevices);

		if (!data->PhysicalDevice)
		{
			LOG_FATAL("%s", "Unable to find suitable vulkan physical device!");
			free(renderer);
			return NULL;
		}
	}

	// Create Device
	{
		float queueFamilyPriorities[1] = { 1.0f };

		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		graphicsQueueCreateInfo.queueFamilyIndex = data->GraphicsQueueFamilyIndex;
		graphicsQueueCreateInfo.pQueuePriorities = queueFamilyPriorities;
		graphicsQueueCreateInfo.queueCount = 1;

		VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
		deviceCreateInfo.enabledExtensionCount = DeviceExtentionCount;
		deviceCreateInfo.ppEnabledExtensionNames = DeviceExtentions;
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledLayerNames = NULL;

		VK_CHECK(vkCreateDevice(data->PhysicalDevice, &deviceCreateInfo, NULL, &data->Device));

		if (!data->Device)
		{
			LOG_FATAL("%s", "Unable to create vulkan device!");
			free(renderer);
			return NULL;
		}
	}

	vkGetDeviceQueue(data->Device, data->GraphicsQueueFamilyIndex, 0, &data->GraphicsQueue);
	
	return renderer;
}
