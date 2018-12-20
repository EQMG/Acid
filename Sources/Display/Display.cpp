#include "Display.hpp"

#include <cstring>
#if defined(ACID_BUILD_WINDOWS)
#include <windows.h>
#endif
#include <GLFW/glfw3.h>
#include <SPIRV/GlslangToSpv.h>
#include "Files/Files.hpp"
#include "Textures/Texture.hpp"

#if !defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"
#endif

namespace acid
{
	static const std::vector<const char *> VALIDATION_LAYERS =
	{
		"VK_LAYER_LUNARG_standard_validation" //, "VK_LAYER_RENDERDOC_Capture"
	};
	static const std::vector<const char *> INSTANCE_EXTENSIONS =
	{
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
	};
	static const std::vector<const char *> DEVICE_EXTENSIONS =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void CallbackError(int32_t error, const char *description)
	{
		Display::CheckGlfw(error);
		Log::Error("GLFW error: %s, %i\n", description, error);
	}

	void CallbackMonitor(GLFWmonitor *monitor, int32_t event)
	{
		Log::Error("Monitor action: %s, %i\n", glfwGetMonitorName(monitor), event);
	}

	void CallbackClose(GLFWwindow *window)
	{
		Display::Get()->m_closed = false;
		Engine::Get()->RequestClose(false);
	}

	void CallbackFocus(GLFWwindow *window, int32_t focused)
	{
		Display::Get()->m_focused = static_cast<bool>(focused);
	}

	void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos)
	{
		if (!Display::Get()->m_fullscreen)
		{
			Display::Get()->m_positionX = static_cast<uint32_t>(xpos);
			Display::Get()->m_positionY = static_cast<uint32_t>(ypos);
		}
	}

	void CallbackSize(GLFWwindow *window, int32_t width, int32_t height)
	{
		if (width <= 0 || height <= 0)
		{
			return;
		}

		Display::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		if (Display::Get()->m_fullscreen)
		{
			Display::Get()->m_fullscreenWidth = static_cast<uint32_t>(width);
			Display::Get()->m_fullscreenHeight = static_cast<uint32_t>(height);
		}
		else
		{
			Display::Get()->m_windowWidth = static_cast<uint32_t>(width);
			Display::Get()->m_windowHeight = static_cast<uint32_t>(height);
		}

		Display::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Display::Get()->m_physicalDevice, Display::Get()->m_surface, &Display::Get()->m_surfaceCapabilities));
	}

	void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height)
	{
		Display::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	void CallbackIconify(GLFWwindow *window, int32_t iconified)
	{
		Display::Get()->m_iconified = iconified == GLFW_TRUE;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
	{
		Log::Error("%s\n", pMessage);
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
		m_windowWidth(1080),
		m_windowHeight(720),
		m_fullscreenWidth(0),
		m_fullscreenHeight(0),
		m_aspectRatio(1.5f),
		m_positionX(0),
		m_positionY(0),
		m_title("Acid Loading..."),
		m_iconPath(""),
		m_antialiasing(true),
		m_borderless(false),
		m_resizable(true),
		m_floating(false),
		m_fullscreen(false),
		m_closed(false),
		m_focused(true),
		m_iconified(false),
#if defined(ACID_VERBOSE) && !defined(ACID_BUILD_MACOS)
		m_validationLayers(true),
#else
		m_validationLayers(false),
#endif
		m_window(nullptr),
		m_instanceLayerList(std::vector<const char *>()),
		m_instanceExtensionList(std::vector<const char *>()),
		m_deviceExtensionList(std::vector<const char *>()),
		m_debugReportCallback(VK_NULL_HANDLE),
		m_instance(VK_NULL_HANDLE),
		m_surface(VK_NULL_HANDLE),
		m_surfaceCapabilities({}),
		m_surfaceFormat({}),
		m_logicalDevice(VK_NULL_HANDLE),
		m_msaaSamples(VK_SAMPLE_COUNT_1_BIT),
		m_physicalDevice(VK_NULL_HANDLE),
		m_physicalDeviceProperties({}),
		m_physicalDeviceFeatures({}),
		m_physicalDeviceMemoryProperties({}),
		m_supportedQueues(0),
		m_graphicsFamily(0),
		m_presentFamily(0),
		m_computeFamily(0),
		m_transferFamily(0),
		m_graphicsQueue(VK_NULL_HANDLE),
		m_presentQueue(VK_NULL_HANDLE),
		m_computeQueue(VK_NULL_HANDLE),
		m_transferQueue(VK_NULL_HANDLE)
	{
		CreateGlfw();
		SetupLayers();
		SetupExtensions();
		CreateInstance();
		CreateDebugCallback();
		CreatePhysicalDevice();
		CreateSurface();
		CreateQueueIndices();
		CreateLogicalDevice();

		glslang::InitializeProcess();
	}

	Display::~Display()
	{
		glslang::FinalizeProcess();

		// Waits for the device to finish before destroying.
		Display::CheckVk(vkDeviceWaitIdle(m_logicalDevice));

		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Destroys Vulkan.
		vkDestroyDevice(m_logicalDevice, nullptr);
		FvkDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		// Terminate GLFW.
		glfwTerminate();

		m_closed = true;
	}

	void Display::Update()
	{
		// Polls for window events.
		glfwPollEvents();
	}

	uint32_t Display::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties)
	{
		for (uint32_t i = 0; i < deviceMemoryProperties->memoryTypeCount; ++i)
		{
			if (memoryRequirements->memoryTypeBits & (1 << i))
			{
				if ((deviceMemoryProperties->memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
				{
					return i;
				}
			}
		}

		assert(false && "Vulkan runtime error, couldn't find proper memory type!");
		return UINT32_MAX;
	}

	void Display::SetDimensions(const uint32_t &width, const uint32_t &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		glfwSetWindowSize(m_window, width, height);
	}

	void Display::SetDimensions(const Vector2 &size)
	{
		SetDimensions(size.m_x == -1.0f ? GetWidth() : static_cast<uint32_t>(size.m_x),
			size.m_y == -1.0f ? GetHeight() : static_cast<uint32_t>(size.m_y));
	}

	void Display::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		glfwSetWindowPos(m_window, x, y);
	}

	void Display::SetPosition(const Vector2 &position)
	{
		SetPosition(position.m_x == -1.0f ? GetPositionX() : static_cast<uint32_t>(position.m_x),
			position.m_y == -1.0f ? GetPositionY() : static_cast<uint32_t>(position.m_y));
	}

	void Display::SetTitle(const std::string &title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	void Display::SetIcon(const std::string &filename)
	{
		m_iconPath = filename;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t components = 0;
		uint8_t *data = Texture::LoadPixels(m_iconPath, &width, &height, &components);

		if (data == nullptr)
		{
			return;
		}

		GLFWimage icons[1];
		icons[0].pixels = data;
		icons[0].width = width;
		icons[0].height = height;

		glfwSetWindowIcon(m_window, 1, icons);
		Texture::DeletePixels(data);
	}

	void Display::SetBorderless(const bool &borderless)
	{
		m_borderless = borderless;
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
	}

	void Display::SetResizable(const bool &resizable)
	{
		m_resizable = resizable;
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
	}

	void Display::SetFloating(const bool &floating)
	{
		m_floating = floating;
		glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);
	}

	void Display::SetFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;

		// int32_t monitorCount;
		// GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor(); // monitors[1];
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (fullscreen)
		{
#if defined(ACID_VERBOSE)
			printf("Display is going fullscreen\n");
#endif
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			glfwSetWindowMonitor(m_window, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
#if defined(ACID_VERBOSE)
			printf("Display is going windowed\n");
#endif
			m_positionX = (videoMode->width - m_windowWidth) / 2;
			m_positionY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_window, nullptr, m_positionX, m_positionY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}

	void Display::SetIconified(const bool &iconify)
	{
		if (!m_iconified && iconify)
		{
			glfwIconifyWindow(m_window);
		}
		else if (m_iconified && !iconify)
		{
			glfwRestoreWindow(m_window);
		}
	}

	std::string Display::StringifyResultGlfw(const int32_t &result)
	{
		switch (result)
		{
		case GLFW_TRUE:
			return "Success";
		case GLFW_NOT_INITIALIZED:
			return "GLFW has not been initialized";
		case GLFW_NO_CURRENT_CONTEXT:
			return "No context is current for this thread";
		case GLFW_INVALID_ENUM:
			return "One of the arguments to the function was an invalid enum value";
		case GLFW_INVALID_VALUE:
			return "One of the arguments to the function was an invalid value";
		case GLFW_OUT_OF_MEMORY:
			return "A memory allocation failed";
		case GLFW_API_UNAVAILABLE:
			return "GLFW could not find support for the requested API on the system";
		case GLFW_VERSION_UNAVAILABLE:
			return "The requested OpenGL or OpenGL ES version is not available";
		case GLFW_PLATFORM_ERROR:
			return "A platform-specific error occurred that does not match any of the more specific categories";
		case GLFW_FORMAT_UNAVAILABLE:
			return "The requested format is not supported or available";
		case GLFW_NO_WINDOW_CONTEXT:
			return "The specified window does not have an OpenGL or OpenGL ES context";
		default:
			return "ERROR: UNKNOWN GLFW ERROR";
		}
	}

	void Display::CheckGlfw(const int32_t &result)
	{
		if (result == GLFW_TRUE)
		{
			return;
		}

		std::string failure = StringifyResultGlfw(result);

		Log::Error("GLFW error: %s, %i\n", failure.c_str(), result);
		assert(false && "GLFW error!");
#if defined(ACID_BUILD_WINDOWS)
		MessageBox(nullptr, failure.c_str(), "GLFW Error", 0);
#endif
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

	void Display::CheckVk(const VkResult &result)
	{
		if (result >= 0)
		{
			return;
		}

		std::string failure = StringifyResultVk(result);

		Log::Error("Vulkan error: %s, %i\n", failure.c_str(), result);
		assert(false && "Vulkan error!");
#if defined(ACID_BUILD_WINDOWS)
		MessageBox(nullptr, failure.c_str(), "Vulkan Error", 0);
#endif
	}

	void Display::CreateGlfw()
	{
		// Set the error error callback
		glfwSetErrorCallback(CallbackError);

		// Initialize the GLFW library.
		if (glfwInit() == GLFW_FALSE)
		{
			Log::Error("GLFW error: Failed to initialize!\n");
			assert(false && "GLFW runtime error!");
		}

		// Checks Vulkan support on GLFW.
		if (glfwVulkanSupported() == GLFW_FALSE)
		{
			Log::Error("GLFW error: Failed to find Vulkan support!\n");
			assert(false && "GLFW runtime error!");
		}

		// Set the monitor callback
		glfwSetMonitorCallback(CallbackMonitor);

		// Configures the window.
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Disable context creation.

		// For new GLFW, and macOS.
		glfwWindowHint(GLFW_STENCIL_BITS, 8); // Fixes 16 bit stencil bits in macOS.
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // No stereo view!

		// Get the resolution of the primary monitor.
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (m_fullscreen)
		{
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			m_aspectRatio = static_cast<float>(videoMode->width) / static_cast<float>(videoMode->height);
		}

	//	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
	//	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
	//	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
	//	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

		// Create a windowed mode window and its context.
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : nullptr, nullptr);

		// Gets any window errors.
		if (m_window == nullptr)
		{
			glfwTerminate();
			assert(false && "Filed to create the GLFW window!");
		}

		// Window attributes that can change later.
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, m_borderless ? GLFW_FALSE : GLFW_TRUE);
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, m_resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_FLOATING, m_floating ? GLFW_TRUE : GLFW_FALSE);

		// Centre the window position.
		m_positionX = (videoMode->width - m_windowWidth) / 2;
		m_positionY = (videoMode->height - m_windowHeight) / 2;
		glfwSetWindowPos(m_window, m_positionX, m_positionY);

		// Shows the glfw window.
		glfwShowWindow(m_window);

		// Sets the displays callbacks.
		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowCloseCallback(m_window, CallbackClose);
		glfwSetWindowFocusCallback(m_window, CallbackFocus);
		glfwSetWindowPosCallback(m_window, CallbackPosition);
		glfwSetWindowSizeCallback(m_window, CallbackSize);
		glfwSetWindowIconifyCallback(m_window, CallbackIconify);
		glfwSetFramebufferSizeCallback(m_window, CallbackFrame);
	}

	void Display::SetupLayers()
	{
		uint32_t instanceLayerPropertyCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

#if defined(ACID_VERBOSE)
		LogVulkanLayers(instanceLayerProperties, "Instance", false);
#endif

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
					Log::Error("Vulkan validation layer not found: '%s'\n", layerName);
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
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			m_instanceExtensionList.emplace_back(glfwExtensions[i]);
		}

		for (auto &instanceExtension : INSTANCE_EXTENSIONS)
		{
			m_instanceExtensionList.emplace_back(instanceExtension);
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
		applicationInfo.pEngineName = "Acid";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 9, 4);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
		instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensionList.size());
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();
		CheckVk(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	}

	void Display::CreateDebugCallback()
	{
		if (m_validationLayers)
		{
			VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};
			debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debugReportCallbackCreateInfo.pNext = nullptr;
			debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debugReportCallbackCreateInfo.pfnCallback = &CallbackDebug;
			debugReportCallbackCreateInfo.pUserData = nullptr;
			CheckVk(FvkCreateDebugReportCallbackEXT(m_instance, &debugReportCallbackCreateInfo, nullptr, &m_debugReportCallback));
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
			assert(false && "Vulkan runtime error, failed to find a suitable gpu!");
		}

		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

		Log::Out("Selected Physical Device: '%s', %i\n", m_physicalDeviceProperties.deviceName, m_physicalDeviceProperties.deviceID);

		m_msaaSamples = GetMaxUsableSampleCount();
	}

	VkPhysicalDevice Display::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices)
	{
		// Maps to hold devices and sort by rank.
		std::multimap<int32_t, VkPhysicalDevice> rankedDevices;

		// Iterates through all devices and rate their suitability.
		for (auto &device : devices)
		{
			int32_t score = ScorePhysicalDevice(device);
			rankedDevices.emplace(score, device);
		}

		// Checks to make sure the best candidate scored higher than 0  rbegin points to last element of ranked devices(highest rated), first is its rating.
		if (rankedDevices.rbegin()->first > 0)
		{
			return rankedDevices.rbegin()->second;
		}

		return nullptr;
	}

	int32_t Display::ScorePhysicalDevice(const VkPhysicalDevice &device)
	{
		int32_t score = 0;

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

#if defined(ACID_VERBOSE)
		LogVulkanDevice(physicalDeviceProperties);
#endif

		// Adds a large score boost for discrete GPUs (dedicated graphics cards).
		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		// Gives a higher score to devices with a higher maximum texture size.
		score += physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}

	VkSampleCountFlagBits Display::GetMaxUsableSampleCount()
	{
		auto physicalDevice = Display::Get()->GetPhysicalDevice();

		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);

		std::vector<VkSampleCountFlagBits> sampleFlagBits = { VK_SAMPLE_COUNT_64_BIT, VK_SAMPLE_COUNT_32_BIT, VK_SAMPLE_COUNT_16_BIT, VK_SAMPLE_COUNT_8_BIT, VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_2_BIT };

		for (auto &sampleFlag : sampleFlagBits)
		{
			if (counts & sampleFlag)
			{
				return sampleFlag;
			}
		}

		return VK_SAMPLE_COUNT_1_BIT;
	}

	void Display::CreateSurface()
	{
		// Creates the WSI Vulkan surface.
		CheckVk(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface));

		CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities));

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

	void Display::CreateQueueIndices()
	{
		uint32_t deviceQueueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

		int32_t graphicsFamily = -1;
		int32_t presentFamily = -1;
		int32_t computeFamily = -1;
		int32_t transferFamily = -1;

		for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			// Check for graphics support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamily = i;
				m_graphicsFamily = i;
				m_supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
			}

			// Check for presentation support.
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

			if (deviceQueueFamilyProperties[i].queueCount > 0 && presentSupport)
			{
				presentFamily = i;
				m_presentFamily = i;
			}

			// Check for compute support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				computeFamily = i;
				m_computeFamily = i;
				m_supportedQueues |= VK_QUEUE_COMPUTE_BIT;
			}

			// Check for transfer support.
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				transferFamily = i;
				m_transferFamily = i;
				m_supportedQueues |= VK_QUEUE_TRANSFER_BIT;
			}

			if (graphicsFamily != -1 && presentFamily != -1 && computeFamily != -1 && transferFamily != -1)
			{
				break;
			}
		}

		if (graphicsFamily == -1)
		{
			assert(false && "Vulkan runtime error, failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT!");
		}
	}

	void Display::CreateLogicalDevice()
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
		float queuePriorities[] = {0.0f};

		if (m_supportedQueues & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
			graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueCreateInfo.queueFamilyIndex = m_graphicsFamily;
			graphicsQueueCreateInfo.queueCount = 1;
			graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.emplace_back(graphicsQueueCreateInfo);
		}
		else
		{
			m_graphicsFamily = VK_NULL_HANDLE;
		}

		if (m_supportedQueues & VK_QUEUE_COMPUTE_BIT && m_computeFamily != m_graphicsFamily)
		{
			VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
			computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueCreateInfo.queueFamilyIndex = m_computeFamily;
			computeQueueCreateInfo.queueCount = 1;
			computeQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.emplace_back(computeQueueCreateInfo);
		}
		else
		{
			m_computeFamily = m_graphicsFamily;
		}

		if (m_supportedQueues & VK_QUEUE_TRANSFER_BIT && m_transferFamily != m_graphicsFamily && m_transferFamily != m_computeFamily)
		{
			VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
			transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueCreateInfo.queueFamilyIndex = m_transferFamily;
			transferQueueCreateInfo.queueCount = 1;
			transferQueueCreateInfo.pQueuePriorities = queuePriorities;
			queueCreateInfos.emplace_back(transferQueueCreateInfo);
		}
		else
		{
			m_transferFamily = m_graphicsFamily;
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.sampleRateShading = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.wideLines = VK_TRUE;
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
		deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
		deviceFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
		deviceFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
		deviceFeatures.shaderClipDistance = VK_TRUE;
		deviceFeatures.shaderCullDistance = VK_TRUE;


		if (m_physicalDeviceFeatures.geometryShader)
		{
			deviceFeatures.geometryShader = VK_TRUE;
		}
		else
		{
			Log::Error("Selected GPU does not support geometry shaders!");
		}

		if (m_physicalDeviceFeatures.tessellationShader)
		{
			deviceFeatures.tessellationShader = VK_TRUE;
		}
		else
		{
			Log::Error("Selected GPU does not support tessellation shaders!");
		}

		if (m_physicalDeviceFeatures.multiViewport)
		{
			deviceFeatures.multiViewport = VK_TRUE;
		}
		else
		{
			Log::Error("Selected GPU does not support multi viewports!");
		}

		if (m_physicalDeviceFeatures.textureCompressionBC)
		{
			deviceFeatures.textureCompressionBC = VK_TRUE;
		}
		else if (m_physicalDeviceFeatures.textureCompressionASTC_LDR)
		{
			deviceFeatures.textureCompressionASTC_LDR = VK_TRUE;
		}
		else if (m_physicalDeviceFeatures.textureCompressionETC2)
		{
			deviceFeatures.textureCompressionETC2 = VK_TRUE;
		}

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
		deviceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensionList.size());
		deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionList.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		CheckVk(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice));

		vkGetDeviceQueue(m_logicalDevice, m_graphicsFamily, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, m_presentFamily, 0, &m_presentQueue);
		vkGetDeviceQueue(m_logicalDevice, m_computeFamily, 0, &m_computeQueue);
		vkGetDeviceQueue(m_logicalDevice, m_transferFamily, 0, &m_transferQueue);
	}

	void Display::LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties)
	{
		Log::Out("-- Physical Device: '%s' --\n", physicalDeviceProperties.deviceName);
		Log::Out("ID: %i\n", physicalDeviceProperties.deviceID);

		switch (static_cast<int>(physicalDeviceProperties.deviceType))
		{
		case 1:
			Log::Out("Type: Integrated\n");
			break;
		case 2:
			Log::Out("Type: Discrete\n");
			break;
		case 3:
			Log::Out("Type: Virtual\n");
			break;
		case 4:
			Log::Out("Type: CPU\n");
			break;
		default:
			Log::Out("Type: Other (%x)\n", physicalDeviceProperties.deviceType);
		}

		switch (physicalDeviceProperties.vendorID)
		{
		case 0x8086:
			Log::Out("Vendor: Intel\n");
			break;
		case 0x10DE:
			Log::Out("Vendor: NVIDIA\n");
			break;
		case 0x1002:
			Log::Out("Vendor: AMD\n");
			break;
		default:
			Log::Out("Vendor: Unknown (0x%x)\n", physicalDeviceProperties.vendorID);
		}

		uint32_t supportedVersion[] = {
			VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_MINOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_PATCH(physicalDeviceProperties.apiVersion)
		};
		Log::Out("Supports Version: %i.%i.%i\n", supportedVersion[0], supportedVersion[1], supportedVersion[2]);
		Log::Out("Header Version: %i\n", VK_HEADER_VERSION);
		Log::Out("-- Done --\n");
	}

	void Display::LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties, const std::string &type, const bool &showDescription)
	{
		Log::Out("-- Avalable Layers For: '%s' --\n", type.c_str());

		for (auto &layer : layerProperties)
		{
			if (showDescription)
			{
				Log::Out("\n	%s   | %s, ", layer.layerName, layer.description);
			}
			else
			{
				Log::Out("%s, ", layer.layerName);
			}
		}

		Log::Out("\n-- Done --\n");
	}
}
