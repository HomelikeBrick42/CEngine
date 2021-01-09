#pragma once

#include <vulkan/vulkan.h>

VkResult vkCreateDebuggerEXT(VkInstance instance, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debuggerMessenger);
void vkDestroyDebugerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debuggerMessenger, const VkAllocationCallbacks* allocator);
