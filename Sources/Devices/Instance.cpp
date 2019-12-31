#include "Instance.hpp"

#include "Graphics/Graphics.hpp"
#include "Window.hpp"

namespace acid {
#if USE_DEBUG_MESSENGER
const std::vector<const char *> Instance::ValidationLayers = {"VK_LAYER_KHRONOS_validation"}; // "VK_LAYER_RENDERDOC_Capture"
#else
#ifndef VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"
#endif

const std::vector<const char *> Instance::ValidationLayers = {"VK_LAYER_LUNARG_standard_validation"};
#endif

#if USE_DEBUG_MESSENGER
VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		Log::Warning(pCallbackData->pMessage, '\n');
	else if (messageSeverity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		Log::Info(pCallbackData->pMessage, '\n');
	else // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
		Log::Error(pCallbackData->pMessage, '\n');

	return VK_FALSE;
}

VkResult Instance::FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
	auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	if (func)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Instance::FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator) {
	auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	if (func)
		return func(instance, messenger, pAllocator);
}
#else
VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData) {
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		Log::Warning(pMessage, '\n');
	else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
		Log::Info(pMessage, '\n');
	else //  VK_DEBUG_REPORT_ERROR_BIT_EXT
		Log::Error(pMessage, '\n');

	return VK_FALSE;
}

	
VkResult Instance::FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
	VkDebugReportCallbackEXT *pCallback) {
	auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	if (func)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Instance::FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator) {
	auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
	if (func)
		func(instance, callback, pAllocator);
}
#endif

void Instance::FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
	uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites) {
	auto func = reinterpret_cast<PFN_vkCmdPushDescriptorSetKHR>(vkGetDeviceProcAddr(device, "vkCmdPushDescriptorSetKHR"));
	if (func)
		func(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

uint32_t Instance::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements,
	VkMemoryPropertyFlags requiredProperties) {
	for (uint32_t i = 0; i < deviceMemoryProperties->memoryTypeCount; ++i) {
		if (memoryRequirements->memoryTypeBits & (1 << i)) {
			if ((deviceMemoryProperties->memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties) {
				return i;
			}
		}
	}

	throw std::runtime_error("Couldn't find a proper memory type");
}

Instance::Instance() {
#if defined(ACID_DEBUG)
	m_enableValidationLayers = true;
#endif
	
	CreateInstance();
	CreateDebugMessenger();
}

Instance::~Instance() {
#if USE_DEBUG_MESSENGER
	FvkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
#else
	FvkDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
#endif
	vkDestroyInstance(m_instance, nullptr);
}

bool Instance::CheckValidationLayerSupport() const {
	uint32_t instanceLayerPropertyCount;
	vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
	std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
	vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

#if defined(ACID_DEBUG)
	LogVulkanLayers(instanceLayerProperties);
#endif

	for (const auto &layerName : ValidationLayers) {
		bool layerFound = false;

		for (const auto &layerProperties : instanceLayerProperties) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			Log::Error("Vulkan validation layer not found: ", std::quoted(layerName), '\n');
			return false;
		}
	}
	
	return true;
}

std::vector<const char *> Instance::GetExtensions() const {
	// Sets up the extensions.
	auto [glfwExtensions, glfwExtensionsCount] = Window::Get()->GetInstanceExtensions();

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

	if (m_enableValidationLayers)
		extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	//extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	return extensions;
}

void Instance::CreateInstance() {
	const auto &engineVersion = Engine::Get()->GetVersion();
	//const auto &appVersion = Engine::Get()->GetApp()->GetVersion();
	//const auto &appName = Engine::Get()->GetApp()->GetName();

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//applicationInfo.pApplicationName = appName.c_str();
	//applicationInfo.applicationVersion = VK_MAKE_VERSION(appVersion.m_major, appVersion.m_minor, appVersion.m_patch);
	applicationInfo.pEngineName = "Acid";
	applicationInfo.engineVersion = VK_MAKE_VERSION(engineVersion.m_major, engineVersion.m_minor, engineVersion.m_patch);
	applicationInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

	if (m_enableValidationLayers && !CheckValidationLayerSupport()) {
		Log::Error("Validation layers requested, but not available!\n");
		m_enableValidationLayers = false;
	}

	auto extensions = GetExtensions();
	
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

#if USE_DEBUG_MESSENGER
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
#endif
	if (m_enableValidationLayers) {
#if USE_DEBUG_MESSENGER
		debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugUtilsMessengerCreateInfo.pfnUserCallback = &CallbackDebug;
		instanceCreateInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugUtilsMessengerCreateInfo);
#endif

		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}

	Graphics::CheckVk(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
}

void Instance::CreateDebugMessenger() {
	if (!m_enableValidationLayers) return;

#if USE_DEBUG_MESSENGER
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback = &CallbackDebug;
	Graphics::CheckVk(FvkCreateDebugUtilsMessengerEXT(m_instance, &debugUtilsMessengerCreateInfo, nullptr, &m_debugMessenger));
#else
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};
	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.pNext = nullptr;
	debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	debugReportCallbackCreateInfo.pfnCallback = &CallbackDebug;
	debugReportCallbackCreateInfo.pUserData = nullptr;
	auto debugReportResult = FvkCreateDebugReportCallbackEXT(m_instance, &debugReportCallbackCreateInfo, nullptr, &m_debugReportCallback);
	if (debugReportResult == VK_ERROR_EXTENSION_NOT_PRESENT) {
		m_enableValidationLayers = false;
		Log::Error("Extension vkCreateDebugReportCallbackEXT not present!\n");
	} else {
		Graphics::CheckVk(debugReportResult);
	}
#endif
}

void Instance::LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties) {
	Log::Out("Instance Layers: ");

	for (const auto &layer : layerProperties) {
		Log::Out(layer.layerName, ", ");
	}

	Log::Out("\n\n");
}
}
