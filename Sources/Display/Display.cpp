#include "Display.hpp"

#ifdef FL_BUILD_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif
#include <SPIRV/GlslangToSpv.h>
#include "Files/Files.hpp"
#include "Textures/Texture.hpp"

namespace fl
{
	static const std::vector<const char *> VALIDATION_LAYERS = {
		"VK_LAYER_LUNARG_standard_validation" // , "VK_LAYER_RENDERDOC_Capture"
	};
	static const std::vector<const char *> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void CallbackPosition(WsiShell shell, uint32_t x, uint32_t y)
	{
		if (Display::Get()->m_fullscreen)
		{
			return;
		}

		Display::Get()->m_positionX = x;
		Display::Get()->m_positionY = y;
	}

	void CallbackSize(WsiShell shell, uint32_t width, uint32_t height, bool fullscreen)
	{
		if (width <= 0 || height <= 0)
		{
			return;
		}

		Display::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		if (Display::Get()->m_fullscreen)
		{
			Display::Get()->m_fullscreenWidth = width;
			Display::Get()->m_fullscreenHeight = height;
		}
		else
		{
			Display::Get()->m_windowWidth = width;
			Display::Get()->m_windowHeight = height;
		}

		Display::ErrorVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Display::Get()->m_physicalDevice, Display::Get()->m_surface, &Display::Get()->m_surfaceCapabilities));
	}

	void CallbackFocus(WsiShell shell, bool focused)
	{
		Display::Get()->m_focused = focused;
	}

	void CallbackIconify(WsiShell shell, bool iconified)
	{
		Display::Get()->m_iconified = iconified;
	}

	void CallbackClose(WsiShell shell)
	{
		Display::Get()->m_closed = true;
		Engine::Get()->RequestClose(false);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
	{
		fprintf(stderr, "%s\n", pMessage);
		return static_cast<VkBool32>(false);
	}

	VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
	{
		auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));

		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

	Display::Display() :
		IModule(),
		m_windowWidth(1080),
		m_windowHeight(720),
		m_fullscreenWidth(0),
		m_fullscreenHeight(0),
		m_aspectRatio(1.5f),
		m_positionX(0),
		m_positionY(0),
		m_title("Flounder Loading..."),
		m_iconPath(""),
		m_antialiasing(true),
		m_fullscreen(false),
		m_closed(false),
		m_focused(true),
		m_iconified(false),
#if defined(FL_VERBOSE) && !defined(FL_BUILD_MACOS)
		m_validationLayers(true),
#else
		m_validationLayers(false),
#endif
		m_shell(VK_NULL_HANDLE),
		m_instanceLayerList(std::vector<const char *>()),
		m_instanceExtensionList(std::vector<const char *>()),
		m_deviceExtensionList(std::vector<const char *>()),
		m_debugReportCallback(VK_NULL_HANDLE),
		m_instance(VK_NULL_HANDLE),
		m_surface(VK_NULL_HANDLE),
		m_surfaceCapabilities({}),
		m_surfaceFormat({}),
		m_logicalDevice(VK_NULL_HANDLE),
		m_queue(VK_NULL_HANDLE),
		m_physicalDevice(VK_NULL_HANDLE),
		m_physicalDeviceProperties({}),
		m_physicalDeviceFeatures({}),
		m_physicalDeviceMemoryProperties({}),
		m_graphicsFamilyIndex(0)
	{
		CreateWsi();
		SetupLayers();
		SetupExtensions();
		CreateInstance();
		CreateDebugCallback();
		CreatePhysicalDevice();
		CreateLogicalDevice();
		CreateSurface();

		glslang::InitializeProcess();
	}

	Display::~Display()
	{
		glslang::FinalizeProcess();

		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(m_logicalDevice);

		// Destroys Vulkan.
		vkDestroyDevice(m_logicalDevice, nullptr);
		FvkDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);
		wsiDestroyShell(m_shell);

		m_closed = false;
	}

	void Display::Update()
	{
		// Polls for shell events. The key callback will only be invoked during this call.
		wsiPollEvents(m_shell);

		// Updates the aspect ratio.
		m_aspectRatio = static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
	}

	void Display::SetWindowSize(const uint32_t &width, const uint32_t &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		wsiCmdSetSize(m_shell, width, height);
	}

	void Display::SetTitle(const std::string &title)
	{
		m_title = title;
		wsiCmdSetTitle(m_shell, m_title.c_str());
	}

	void Display::SetIcon(const std::string &filename)
	{
		// Loads a window icon for this GLFW display.
		m_iconPath = Files::Get()->SearchFile(filename);

		if (m_iconPath.empty())
		{
			return;
		}

		int width = 0;
		int height = 0;
		int components = 0;
		auto data = Texture::LoadPixels(m_iconPath.c_str(), &width, &height, &components);

		if (data == nullptr)
		{
			fprintf(stderr, "Unable to load texture: '%s'\n", m_iconPath.c_str());
			return;
		}

		WsiIcon icon = {};
		icon.pixels = data;
		icon.width = width;
		icon.height = height;

		wsiCmdSetIcon(m_shell, icon);
		Texture::DeletePixels(data);
	}

	void Display::SetFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;

		/*uint32_t instanceMonitorCount;
		wsiEnumerateMonitors(&instanceMonitorCount, nullptr);
		std::vector<WsiMonitor> monitors(instanceMonitorCount);
		wsiEnumerateMonitors(&instanceMonitorCount, monitors.data());

		WsiMonitorProperties monitorProperties;
		wsiGetMonitorProperties(monitors[0], &monitorProperties);

		if (fullscreen)
		{
#if FL_VERBOSE
			printf("Display is going fullscreen\n");
#endif
			m_fullscreenWidth = monitorProperties.width;
			m_fullscreenHeight = monitorProperties.height;
			wsiCmdSetPosition(m_shell, 0, 0);
			wsiCmdSetSize(m_shell, m_fullscreenWidth, m_fullscreenHeight);
			wsiCmdSetFullscreen(m_shell, monitors[0], true);
		}
		else
		{
#if FL_VERBOSE
			printf("Display is going windowed\n");
#endif
			m_positionX = (monitorProperties.width - m_windowWidth) / 2;
			m_positionY = (monitorProperties.height - m_windowHeight) / 2;
			wsiCmdSetFullscreen(m_shell, monitors[0], false);
			wsiCmdSetSize(m_shell, m_windowWidth, m_windowHeight);
			wsiCmdSetPosition(m_shell, m_positionX, m_positionY);
		}*/
	}

	std::string Display::StringifyResultVk(const VkResult &result)
	{
		switch (result)
		{
		case VK_SUCCESS:
			return "Success";
		case VK_NOT_READY:
			return "A fence or query has not yet completed";
		case VK_TIMEOUT:
			return "A wait operation has not completed in the specified time";
		case VK_EVENT_SET:
			return "An event is signaled";
		case VK_EVENT_RESET:
			return "An event is unsignaled";
		case VK_INCOMPLETE:
			return "A return array was too small for the result";
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return "A host memory allocation has failed";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return "A device memory allocation has failed";
		case VK_ERROR_INITIALIZATION_FAILED:
			return "Initialization of an object could not be completed for implementation-specific reasons";
		case VK_ERROR_DEVICE_LOST:
			return "The logical or physical device has been lost";
		case VK_ERROR_MEMORY_MAP_FAILED:
			return "Mapping of a memory object has failed";
		case VK_ERROR_LAYER_NOT_PRESENT:
			return "A requested layer is not present or could not be loaded";
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return "A requested extension is not supported";
		case VK_ERROR_FEATURE_NOT_PRESENT:
			return "A requested feature is not supported";
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
		case VK_ERROR_TOO_MANY_OBJECTS:
			return "Too many objects of the type have already been created";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return "A requested format is not supported on this device";
		case VK_ERROR_SURFACE_LOST_KHR:
			return "A surface is no longer available";
		case VK_SUBOPTIMAL_KHR:
			return "A swapchain no longer matches the surface properties exactly, but can still be used";
		case VK_ERROR_OUT_OF_DATE_KHR:
			return "A surface has changed in such a way that it is no longer compatible with the swapchain";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return "The display used by a swapchain does not use the same presentable image layout";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
		case VK_ERROR_VALIDATION_FAILED_EXT:
			return "A validation layer found an error";
		default:
			return "ERROR: UNKNOWN VULKAN ERROR";
		}
	}

	void Display::ErrorVk(const VkResult &result)
	{
		if (result >= 0)
		{
			return;
		}

		std::string failure = StringifyResultVk(result);

		fprintf(stderr, "Vulkan error: %s, %i\n", failure.c_str(), result);
#ifdef FL_BUILD_WINDOWS
		MessageBox(nullptr, failure.c_str(), "Vulkan Error", 0);
#endif
		throw std::runtime_error("Vulkan runtime error.");
	}

	void Display::CreateWsi()
	{
		/*uint32_t instanceMonitorCount;
		wsiEnumerateMonitors(&instanceMonitorCount, nullptr);
		std::vector<WsiMonitor> monitors(instanceMonitorCount);
		wsiEnumerateMonitors(&instanceMonitorCount, monitors.data());

		WsiMonitorProperties monitorProperties;
		wsiGetMonitorProperties(monitors[0], &monitorProperties);*/

		WsiCallbacks callbacks = {};
		callbacks.position = CallbackPosition;
		callbacks.size = CallbackSize;
		callbacks.focus = CallbackFocus;
		callbacks.iconify = CallbackIconify;
		callbacks.close = CallbackClose;
//		callbacks.cursorPosition = CallbackCursorPosition;
//		callbacks.cursorEnter = CallbackCursorEnter;
//		callbacks.cursorScroll = CallbackCursorScroll;
//		callbacks.key = CallbackKey;
//		callbacks.mouse = CallbackMouseButton;
//		callbacks.touch = CallbackTouch;
//		callbacks.joystickConnect = CallbackJoystickConnect;
//		callbacks.joystickButton = CallbackJoystickButton;
//		callbacks.joystickAxis = CallbackJoystickAxis;

		WsiShellCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.pCallbacks = &callbacks;
		instanceCreateInfo.pIcon = nullptr;
		instanceCreateInfo.width = 1080;
		instanceCreateInfo.height = 720;
//		instanceCreateInfo.x = (monitorProperties.width - instanceCreateInfo.width) / 2;
//		instanceCreateInfo.y = (monitorProperties.height - instanceCreateInfo.height) / 2;
		instanceCreateInfo.resizable = true;
		instanceCreateInfo.title = m_title.c_str();

		ErrorVk(wsiCreateShell(&instanceCreateInfo, &m_shell));
	}

	void Display::SetupLayers()
	{
		uint32_t instanceLayerPropertyCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

		LogVulkanLayers(instanceLayerProperties, "Instance", false);

		// Sets up the layers.
		if (m_validationLayers)
		{
			for (auto &layerName : VALIDATION_LAYERS)
			{
				bool layerFound = false;

				for (auto &layerProperties : instanceLayerProperties)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					fprintf(stderr, "Vulkan validation layer not found: '%s'\n", layerName);
				}

				m_instanceLayerList.emplace_back(layerName);
			}
		}

		for (auto &layerName : DEVICE_EXTENSIONS)
		{
			m_deviceExtensionList.emplace_back(layerName);
		}
	}

	void Display::SetupExtensions()
	{
		// Sets up the extensions.
		uint32_t instanceExtensionCount;
		wsiEnumerateShellExtensions(m_shell, &instanceExtensionCount, nullptr);
		std::vector<const char *> instanceExtensions(instanceExtensionCount);
		wsiEnumerateShellExtensions(m_shell, &instanceExtensionCount, instanceExtensions.data());

		for (auto &extension : instanceExtensions)
		{
			m_instanceExtensionList.emplace_back(extension);
		}

		if (m_validationLayers)
		{
			m_instanceExtensionList.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			m_instanceExtensionList.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
	}

	void Display::CreateInstance()
	{
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = m_title.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.pEngineName = "Flounder";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 9, 3);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
		instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensionList.size());
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();

		ErrorVk(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	}

	void Display::CreateDebugCallback()
	{
		if (m_validationLayers)
		{
			VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};
			debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debugReportCallbackCreateInfo.pNext = nullptr;
			debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
				VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debugReportCallbackCreateInfo.pfnCallback = &CallbackDebug;
			debugReportCallbackCreateInfo.pUserData = nullptr;

			ErrorVk(FvkCreateDebugReportCallbackEXT(m_instance, &debugReportCallbackCreateInfo, nullptr, &m_debugReportCallback));
		}
	}

	void Display::CreatePhysicalDevice()
	{
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data());

		m_physicalDevice = ChoosePhysicalDevice(physicalDevices);

		if (m_physicalDevice == nullptr)
		{
			throw std::runtime_error("Vulkan runtime error, failed to find a suitable gpu!");
		}

		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

		LogVulkanDevice(m_physicalDeviceProperties, m_physicalDeviceFeatures, m_physicalDeviceMemoryProperties);

		uint32_t deviceQueueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

		bool foundQueueFamily = false;

		for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_graphicsFamilyIndex = i;
				foundQueueFamily = true;
			}
		}

		if (!foundQueueFamily)
		{
			throw std::runtime_error("Vulkan runtime error, failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT!");
		}
	}

	VkPhysicalDevice Display::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices)
	{
		// Maps to hold devices and sort by rank.
		std::multimap<int, VkPhysicalDevice> rankedDevices;

		// Iterates through all devices and rate their suitability.
		for (auto &device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			int score = ScorePhysicalDevice(device);
			rankedDevices.emplace(score, device);
		}

		// Checks to make sure the best candidate scored higher than 0  rbegin points to last element of ranked devices(highest rated), first is its rating.
		if (rankedDevices.rbegin()->first > 0)
		{
			return rankedDevices.rbegin()->second;
		}

		return nullptr;
	}

	int Display::ScorePhysicalDevice(const VkPhysicalDevice &device)
	{
		int score = 0;

		// Checks if the requested extensions are supported.
		uint32_t extensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

		// Iterates through all extensions requested.
		for (const char *currentExtension : DEVICE_EXTENSIONS)
		{
			bool extensionFound = false;

			// Checks if the extension is in the available extensions.
			for (auto &extension : extensionProperties)
			{
				if (strcmp(currentExtension, extension.extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			// Returns a score of 0 if this device is missing a required extension.
			if (!extensionFound)
			{
				return 0;
			}
		}

		// Obtain the device features and properties of the current device being rateds.
		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

		// Adds a large score boost for discrete GPUs (dedicated graphics cards).
		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		// Gives a higher score to devices with a higher maximum texture size.
		score += physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}

	void Display::CreateLogicalDevice()
	{
		float queuePriorities[] = {1.0f};
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = m_graphicsFamilyIndex;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
		physicalDeviceFeatures.shaderClipDistance = VK_TRUE;
		physicalDeviceFeatures.shaderCullDistance = VK_TRUE;
		physicalDeviceFeatures.fillModeNonSolid = VK_TRUE;
		physicalDeviceFeatures.fragmentStoresAndAtomics = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
		deviceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensionList.size());
		deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionList.data();

		ErrorVk(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice));

		vkGetDeviceQueue(m_logicalDevice, m_graphicsFamilyIndex, 0, &m_queue);
	}

	void Display::CreateSurface()
	{
		// Creates the WSI Vulkan surface.
		ErrorVk(wsiCreateSurface(m_shell, m_instance, nullptr, &m_surface));

		ErrorVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities));

		VkBool32 physicalDeviceSurfaceSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_graphicsFamilyIndex, m_surface, &physicalDeviceSurfaceSupport);

		if (!physicalDeviceSurfaceSupport)
		{
			throw std::runtime_error("Vulkan runtime error, failed to find a physical surface!");
		}

		uint32_t physicalDeviceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> physicalDeviceFormats(physicalDeviceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceFormatCount, physicalDeviceFormats.data());

		m_surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		m_surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		if (physicalDeviceFormats[0].format != VK_FORMAT_UNDEFINED)
		{
			m_surfaceFormat = physicalDeviceFormats[0];
		}
	}

	void Display::LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties, const VkPhysicalDeviceFeatures &physicalDeviceFeatures, const VkPhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties)
	{
#if FL_VERBOSE
		printf("-- Selected Device: '%s' --\n", physicalDeviceProperties.deviceName);

		switch (static_cast<int>(physicalDeviceProperties.deviceType))
		{
		case 1:
			printf("Type: Integrated\n");
			break;
		case 2:
			printf("Type: Discrete\n");
			break;
		case 3:
			printf("Type: Virtual\n");
			break;
		case 4:
			printf("Type: CPU\n");
			break;
		default:
			printf("Type: Other (%x)\n", physicalDeviceProperties.deviceType);
		}

		switch (physicalDeviceProperties.vendorID)
		{
		case 0x8086:
			printf("Vendor: Intel\n");
			break;
		case 0x10DE:
			printf("Vendor: NVIDIA\n");
			break;
		case 0x1002:
			printf("Vendor: AMD\n");
			break;
		default:
			printf("Vendor: Unknown (0x%x)\n", physicalDeviceProperties.vendorID);
		}

		uint32_t supportedVersion[] = {
			VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_MINOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_PATCH(physicalDeviceProperties.apiVersion)
		};
		printf("Supports Version: %i.%i.%i\n", supportedVersion[0], supportedVersion[1], supportedVersion[2]);
		printf("Header Version: %i\n", VK_HEADER_VERSION);
		printf("-- Done --\n");
#endif
	}

	void Display::LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties, const std::string &type, const bool &showDescription)
	{
#if FL_VERBOSE
		printf("-- Avalable Layers For: '%s' --\n", type.c_str());

		for (auto &layer : layerProperties)
		{
			if (showDescription)
			{
				printf("\n    %s   | %s, ", layer.layerName, layer.description);
			}
			else
			{
				printf("%s, ", layer.layerName);
			}
		}

		printf("\n-- Done --\n");
#endif
	}
}
