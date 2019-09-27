#include "Instance.hpp"

#include "Graphics/Graphics.hpp"
#include "Window.hpp"

#if !defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"
#endif

namespace acid {
const std::vector<const char *> Instance::ValidationLayers = {"VK_LAYER_KHRONOS_validation"}; // "VK_LAYER_RENDERDOC_Capture"
const std::vector<const char *> Instance::InstanceExtensions = {VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};
const std::vector<const char *> Instance::DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME}; // VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD_EXTENSION_NAME,
// VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME

VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	const char *type =
		(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		? "ERROR"
		: (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		? "WARNING"
		: "INFO";

	Log::Error(type, ": ", pCallbackData->pMessage, "\n");
	return VK_FALSE;
}

VkResult Instance::FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Instance::FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func)
		return func(instance, messenger, pAllocator);
}

void Instance::FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
	uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites) {
	auto func = reinterpret_cast<PFN_vkCmdPushDescriptorSetKHR>(vkGetDeviceProcAddr(device, "vkCmdPushDescriptorSetKHR"));
	if (func)
		func(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

uint32_t Instance::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements,
	const VkMemoryPropertyFlags &requiredProperties) {
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
	SetupLayers();
	SetupExtensions();
	CreateInstance();
	CreateDebugCallback();
}

Instance::~Instance() {
	FvkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

void Instance::SetupLayers() {
	uint32_t instanceLayerPropertyCount;
	vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
	std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
	vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

#if defined(ACID_DEBUG)
	LogVulkanLayers(instanceLayerProperties);
#endif

	// Sets up the layers.
#if defined(ACID_DEBUG) && !defined(ACID_BUILD_MACOS)
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
			continue;
		}

		m_instanceLayers.emplace_back(layerName);
	}
#endif

	for (const auto &layerName : DeviceExtensions) {
		m_deviceExtensions.emplace_back(layerName);
	}
}

void Instance::SetupExtensions() {
	// Sets up the extensions.
	auto [extensions, extensionsCount] = Window::Get()->GetInstanceExtensions();

	for (uint32_t i = 0; i < extensionsCount; i++) {
		m_instanceExtensions.emplace_back(extensions[i]);
	}

	for (const auto &instanceExtension : InstanceExtensions) {
		m_instanceExtensions.emplace_back(instanceExtension);
	}

#if defined(ACID_DEBUG) && !defined(ACID_BUILD_MACOS)
	m_instanceExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	m_instanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
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

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = m_instanceLayers.data();
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensions.data();
	instanceCreateInfo.pNext = nullptr;

#if defined(ACID_DEBUG) && !defined(ACID_BUILD_MACOS)
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback = &CallbackDebug;
	instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
#endif

	Graphics::CheckVk(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
}

void Instance::CreateDebugCallback() {
#if defined(ACID_DEBUG) && !defined(ACID_BUILD_MACOS)
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback = &CallbackDebug;

	Graphics::CheckVk(FvkCreateDebugUtilsMessengerEXT(m_instance, &debugUtilsMessengerCreateInfo, nullptr, &m_debugMessenger));
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
