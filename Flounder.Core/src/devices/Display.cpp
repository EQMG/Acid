#include "Display.hpp"

namespace Flounder
{
	const std::vector<const char*> Display::VALIDATION_LAYERS = {"VK_LAYER_LUNARG_standard_validation"};

	void CallbackError(int error, const char *description)
	{
		fprintf(stderr, "GLFW Error: %s\n", description);
	}

	void CallbackClose(GLFWwindow *window)
	{
		Display::Get()->m_closed = false;
		Engine::Get()->RequestClose(false);
	}

	void CallbackFocus(GLFWwindow *window, int focused)
	{
		Display::Get()->m_focused = focused;
	}

	void CallbackPosition(GLFWwindow *window, int xpos, int ypos)
	{
		if (!Display::Get()->m_fullscreen)
		{
			Display::Get()->m_windowPosX = xpos;
			Display::Get()->m_windowPosY = ypos;
		}
	}

	void CallbackSize(GLFWwindow *window, int width, int height)
	{
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
	}

	void CallbackFrame(GLFWwindow *window, int width, int height)
	{
		Display::Get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		// TODO
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VkCallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, void *userData)
	{
		printf("%s\n", msg);
		return false;
	}

	VkResult Display::FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
	{
		auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void Display::FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
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
		m_title("Flounder C++"),
		m_icon(""),
		m_fpsLimit(-1.0f),
		m_antialiasing(true),
		m_fullscreen(false),
		m_glfwWindow(nullptr),
		m_closed(false),
		m_focused(true),
		m_windowPosX(0),
		m_windowPosY(0),
#if FLOUNDER_VERBOSE
		m_validationLayers(true),
#else
		m_validationLayers(false),
#endif
		m_instance(VK_NULL_HANDLE),
		m_surface(VK_NULL_HANDLE),
		m_physicalDevice(VK_NULL_HANDLE),
		m_physicalDeviceProperties(VkPhysicalDeviceProperties()),
		m_physicalDeviceFeatures(VkPhysicalDeviceFeatures()),
		m_physicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties()),
		m_instanceLayerList(std::vector<const char*>()),
		m_instanceExtensionList(std::vector<const char*>()),
		m_deviceExtensionList(std::vector<const char*>()),
		m_debugReport(VK_NULL_HANDLE),
		m_device(VK_NULL_HANDLE),
		m_graphicsQueue(VK_NULL_HANDLE),
		m_transferQueue(VK_NULL_HANDLE)
	{
		CreateGlfwWindow();
		SetupLayers();
		SetupExtensions();
		CreateInstance();
		SetupDebugCallback();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
	}

	Display::~Display()
	{
		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(m_device);

		// Destroys Vulkan.
		vkDestroyDevice(m_device, nullptr);
		FvkDestroyDebugReportCallbackEXT(m_instance, m_debugReport, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_glfwWindow);

		// Terminate GLFW.
		glfwTerminate();

		m_closed = false;
	}

	void Display::Update()
	{
		// Polls for window events. The key callback will only be invoked during this call.
		glfwPollEvents();

		// Updates the aspect ratio.
		m_aspectRatio = static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
	}

	void Display::Screenshot()
	{
		// TODO
	}

	void Display::SetWindowSize(const int &width, const int &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		glfwSetWindowSize(m_glfwWindow, width, height);
	}

	void Display::SetTitle(const std::string &title)
	{
		m_title = title;
		glfwSetWindowTitle(m_glfwWindow, m_title.c_str());
	}

	void Display::SetIcon(const std::string &icon)
	{
		// Creates a window icon for this GLFW display.
		m_icon = icon;

		if (!m_icon.empty())
		{
			int width = 0;
			int height = 0;
			int components = 0;
			stbi_uc *data = stbi_load(m_icon.c_str(), &width, &height, &components, 4);

			if (data == nullptr)
			{
				printf("Unable to load texture: '%s'.\n", m_icon.c_str());
			}
			else
			{
				GLFWimage icons[1];
				icons[0].pixels = data;
				icons[0].width = width;
				icons[0].height = height;

				glfwSetWindowIcon(m_glfwWindow, 1, icons);
			}

			stbi_image_free(data);
		}
	}

	void Display::SetFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		if (fullscreen)
		{
			printf("Display is going fullscreen.\n");
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			glfwSetWindowMonitor(m_glfwWindow, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			printf("Display is going windowed.\n");
			m_windowPosX = (videoMode->width - m_windowWidth) / 2;
			m_windowPosY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_glfwWindow, nullptr, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}

	uint32_t Display::MemoryTypeIndex(uint32_t typeBits, VkFlags properties)
	{
		for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			typeBits >>= 1;
		}

		return 0;
	}

	void Display::CreateGlfwWindow()
	{
		// Set the error error callback
		glfwSetErrorCallback(CallbackError);

		// Initialize the GLFW library.
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to init GLFW!");
		}

		// Checks Vulkan support on GLFW.
		if (!glfwVulkanSupported())
		{
			throw std::runtime_error("GLFW does not support Vulkan compute!");
		}

		// Configures the window.
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // The window will be resizable depending on if it's fullscreen.

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

		// Create a windowed mode window and its OpenGL context.
		m_glfwWindow = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : nullptr, nullptr);

		// Gets any window errors.
		if (m_glfwWindow == nullptr)
		{
			glfwTerminate();
			throw std::runtime_error("Filed to create the GLFW window!");
		}

		// Centre the window position.
		m_windowPosX = (videoMode->width - m_windowWidth) / 2;
		m_windowPosY = (videoMode->height - m_windowHeight) / 2;
		glfwSetWindowPos(m_glfwWindow, m_windowPosX, m_windowPosY);

		// Shows the Vulkan window.
		glfwShowWindow(m_glfwWindow);

		// Sets the displays callbacks.
		glfwSetWindowUserPointer(m_glfwWindow, this);
		glfwSetWindowCloseCallback(m_glfwWindow, CallbackClose);
		glfwSetWindowFocusCallback(m_glfwWindow, CallbackFocus);
		glfwSetWindowPosCallback(m_glfwWindow, CallbackPosition);
		glfwSetWindowSizeCallback(m_glfwWindow, CallbackSize);
		glfwSetFramebufferSizeCallback(m_glfwWindow, CallbackFrame);
	}

	void Display::SetupLayers()
	{
		// Sets up the layers.
		if (m_validationLayers)
		{
			uint32_t layerCount = 0;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> instanceLayerProperties(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, instanceLayerProperties.data());

			for (auto layerName : VALIDATION_LAYERS)
			{
				bool layerFound = false;

				for (auto layerProperties : instanceLayerProperties)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					throw std::runtime_error("Could not find Vulkan validation layer: " + std::string(layerName));
				}
			}
		}

		m_instanceLayerList.push_back("VK_LAYER_LUNARG_standard_validation");
	}

	void Display::SetupExtensions()
	{
		// Sets up the extensions.
		unsigned int glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			m_instanceExtensionList.push_back(glfwExtensions[i]);
		}

		m_instanceExtensionList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

		m_deviceExtensionList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	void Display::CreateInstance()
	{
		// Sets up the instance.
		VkApplicationInfo applicationInfo = VkApplicationInfo();
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = m_title.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.pEngineName = "Flounder";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo = VkInstanceCreateInfo();
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		if (m_validationLayers)
		{
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
			instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
		}

		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensionList.size());
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();

		GlfwVulkan::ErrorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	}

	void Display::SetupDebugCallback()
	{
		// Sets up the debug callbacks.
		VkDebugReportCallbackCreateInfoEXT debugCallBackCreateInfo = VkDebugReportCallbackCreateInfoEXT();
		debugCallBackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugCallBackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		/*debugCallBackCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
			VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_ERROR_BIT_EXT |
			VK_DEBUG_REPORT_DEBUG_BIT_EXT;*/
		debugCallBackCreateInfo.pfnCallback = VkCallbackDebug;

		// Inits debuging.
		if (m_validationLayers)
		{
			GlfwVulkan::ErrorCheck(FvkCreateDebugReportCallbackEXT(m_instance, &debugCallBackCreateInfo, nullptr, &m_debugReport));
		}
	}

	void Display::CreateSurface()
	{
		// Creates the Vulkan-GLFW surface.
		GlfwVulkan::ErrorCheck(glfwCreateWindowSurface(m_instance, m_glfwWindow, nullptr, &m_surface));
	}

	void Display::PickPhysicalDevice()
	{
		// Gets the physical GPU device.
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> deviceList = std::vector<VkPhysicalDevice>(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, deviceList.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		for (uint32_t i = 0; i < deviceCount; i++)
		{
			VkPhysicalDevice device = deviceList[i];

			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			int score = 0;

			// Discrete GPUs have a significant performance advantage.
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				score += 1000;
			}

			// Maximum possible size of textures affects graphics quality.
			score += deviceProperties.limits.maxImageDimension2D;

			// Application can't function without geometry shaders, without extensions or adequate swap chains.
			if (!IsDeviceSuitable(device) || !deviceFeatures.geometryShader)
			{
				score = 0;
			}

			candidates.insert(std::make_pair(score, device));
		}

		if (candidates.rbegin()->first > 0)
		{
			m_physicalDevice = candidates.rbegin()->second;
			vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
			vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);
		}
		else
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	void Display::CreateLogicalDevice()
	{
		// Finds the indice queues.
		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(&m_physicalDevice, &m_surface);

		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.transferFamily };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
		float quePriorities[] = {1.0f};

		for (auto queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = quePriorities;
			queueCreateInfoList.push_back(queueCreateInfo);
		}

		// Creates the Vulkan device object.
		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = (uint32_t) queueCreateInfoList.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfoList.data();

		if (m_validationLayers)
		{
			deviceCreateInfo.enabledLayerCount = (uint32_t) m_instanceLayerList.size();
			deviceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}
		else
		{
			deviceCreateInfo.enabledLayerCount = 0;
		}

		deviceCreateInfo.enabledExtensionCount = (uint32_t) m_deviceExtensionList.size();
		deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionList.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		GlfwVulkan::ErrorCheck(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));

		vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, indices.transferFamily, 0, &m_transferQueue);
	}

	bool Display::IsDeviceSuitable(VkPhysicalDevice device)
	{
		// Finds the devices indicie queues.
		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(&device, &m_surface);

		// Figgures out extensions support.
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_deviceExtensionList.begin(), m_deviceExtensionList.end());

		for (auto extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		bool extensionsSupported = requiredExtensions.empty();

		// Figgures out swap chain support.
		VkSwapChainSupportDetails swapChainSupport = {};

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &swapChainSupport.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			swapChainSupport.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, swapChainSupport.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			swapChainSupport.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, swapChainSupport.presentModes.data());
		}

		bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

		return indices.IsComplete() && extensionsSupported && swapChainAdequate;
	}
}
