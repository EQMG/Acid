#include "Display.hpp"

#include <cassert>
#include <map>

namespace Flounder
{
	const std::vector<const char*> Display::VALIDATION_LAYERS = { "VK_LAYER_LUNARG_standard_validation" };
	const std::vector<const char*> Display::DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

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

		GlfwVulkan::ErrorVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Display::Get()->m_physicalDevice, Display::Get()->m_surface, &Display::Get()->m_surfaceCapabilities));
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
		m_window(nullptr),
		m_closed(false),
		m_focused(true),
		m_windowPosX(0),
		m_windowPosY(0),
#if FLOUNDER_VERBOSE
		m_validationLayers(true),
#else
		m_validationLayers(false),
#endif	m_instanceLayerList(std::vector<const char*>()),
		m_instanceExtensionList(std::vector<const char*>()),
		m_deviceExtensionList(std::vector<const char*>()),
		m_debugReport(VK_NULL_HANDLE),

		m_instance(VK_NULL_HANDLE),
		m_surface(VK_NULL_HANDLE),
		m_surfaceCapabilities({}),
		m_surfaceFormat({}),
		m_device(VK_NULL_HANDLE),
		m_queue(VK_NULL_HANDLE),

		m_physicalDevice(VK_NULL_HANDLE),
		m_physicalDeviceProperties({}),
		m_physicalDeviceFeatures({}),
		m_physicalDeviceMemoryProperties({}),
		m_graphicsFamilyIndex(0)
	{
		CreateGlfw();
		CreateVulkan();
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
		glfwDestroyWindow(m_window);

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
		glfwSetWindowSize(m_window, width, height);
	}

	void Display::SetTitle(const std::string &title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
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

				glfwSetWindowIcon(m_window, 1, icons);
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
			glfwSetWindowMonitor(m_window, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			printf("Display is going windowed.\n");
			m_windowPosX = (videoMode->width - m_windowWidth) / 2;
			m_windowPosY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_window, nullptr, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}

	void Display::CreateGlfw()
	{
		// Set the error error callback
		glfwSetErrorCallback(CallbackError);

		// Initialize the GLFW library.
		GlfwVulkan::ErrorGlfw(glfwInit());

		// Checks Vulkan support on GLFW.
		GlfwVulkan::ErrorGlfw(glfwVulkanSupported());

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
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : nullptr, nullptr);

		// Gets any window errors.
		if (m_window == nullptr)
		{
			glfwTerminate();
			throw std::runtime_error("Filed to create the GLFW window!");
		}

		// Centre the window position.
		m_windowPosX = (videoMode->width - m_windowWidth) / 2;
		m_windowPosY = (videoMode->height - m_windowHeight) / 2;
		glfwSetWindowPos(m_window, m_windowPosX, m_windowPosY);

		// Shows the Vulkan window.
		glfwShowWindow(m_window);

		// Sets the displays callbacks.
		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowCloseCallback(m_window, CallbackClose);
		glfwSetWindowFocusCallback(m_window, CallbackFocus);
		glfwSetWindowPosCallback(m_window, CallbackPosition);
		glfwSetWindowSizeCallback(m_window, CallbackSize);
		glfwSetFramebufferSizeCallback(m_window, CallbackFrame);
	}

	void Display::CreateVulkan()
	{
		// Creates Vulkan.
		SetupLayers();
		SetupExtensions();
		CreateInstance();
		CreateDebugCallback();
		CreatePhysicalDevice();
		CreateLogicalDevice();
		CreateSurface();
	}

	void Display::SetupLayers()
	{
		// Sets up the layers.
		if (m_validationLayers)
		{
			uint32_t instanceLayerPropertyCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

			LogVulkanLayers(instanceLayerProperties, "Instance", false);

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

				assert(layerFound && "Could not find a Vulkan validation layer!");
			}
		}

		m_instanceLayerList.push_back(VK_STANDARD_VALIDATION_LAYER_NAME);
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
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = m_title.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.pEngineName = "Flounder";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 50);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		if (m_validationLayers)
		{
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
			instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}

		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensionList.size());
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();

		GlfwVulkan::ErrorVk(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
	}

	void Display::CreateDebugCallback()
	{
		if (m_validationLayers)
		{
			VkDebugReportCallbackCreateInfoEXT debugCallBackCreateInfo = VkDebugReportCallbackCreateInfoEXT();
			debugCallBackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
			debugCallBackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
			debugCallBackCreateInfo.pfnCallback = VkCallbackDebug;

			GlfwVulkan::ErrorVk(FvkCreateDebugReportCallbackEXT(m_instance, &debugCallBackCreateInfo, nullptr, &m_debugReport));
		}
	}

	void Display::CreatePhysicalDevice()
	{
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data());

		m_physicalDevice = ChoosePhysicalDevice(physicalDevices);
		assert(m_physicalDevice && "Vulkan runtime error, failed to find a suitable gpu!");

		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

		LogVulkanDevice(m_physicalDeviceProperties, m_physicalDeviceFeatures, m_physicalDeviceMemoryProperties);

		uint32_t deviceQueueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &deviceQueueFamilyPropertyCount,
			deviceQueueFamilyProperties.data());

		bool foundQueueFamily = false;

		for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_graphicsFamilyIndex = i;
			}
		}

		assert(!foundQueueFamily && "Vulkan runtime error, failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT!");
	}

	VkPhysicalDevice Display::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices)
	{
		// Maps to hold devices and sort by rank.
		std::multimap<int, VkPhysicalDevice> rankedDevices;

		// Iterates through all devices and rate their suitability.
		for (auto device : devices)
		{
			int score = ScorePhysicalDevice(device);
			rankedDevices.insert(std::make_pair(score, device));
		}

		// Checks to make sure the best candidate scored higher than 0  rbegin points to last element of ranked devices(highest rated), first is its rating.
		if (rankedDevices.rbegin()->first > 0)
		{
			// Returns the second value of the highest rated device (its VkPhysicalDevice component).
			return rankedDevices.rbegin()->second;
		}

		return nullptr;
	}

	int Display::ScorePhysicalDevice(const VkPhysicalDevice & device)
	{
		int score = 0;

		/// Adjusts score based on queue families:
		// Finds an index of a queue family which contiains the necessary commands.
		/*QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(device, m_surface);

		// Returns a score of 0 if this device has no suitable family.
		if (!indices.IsComplete())
		{
			return 0;
		}*/

		// Checks if the requested extensions are supported.
		uint32_t extensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

		// Iterates through all extensions requested.
		for (const char* currentExtension : DEVICE_EXTENSIONS)
		{
			bool extensionFound = false;

			// Checks if the extension is in the available extensions.
			for (const auto& extension : extensionProperties)
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
		
		/// check if this device has an adequate swap chain
		/*bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

		if (!swapChainAdequate)
		{
			return 0;
		}*/

		// Obtain the device features and properties of the current device being rateds.
		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

		/// Adjusts score based on properties:
		// Adds a large score boost for discrete GPUs (dedicated graphics cards).
		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		// Gives a higher score to devices with a higher maximum texture size.
		score += physicalDeviceProperties.limits.maxImageDimension2D;

		/// Adjust score based on features:
		// Only allow a device if it supports geometry shaders.
		if (!physicalDeviceFeatures.geometryShader)
		{
			return 0;
		}

		return score;
	}

	void Display::CreateLogicalDevice()
	{
		float queuePriorities[] = { 1.0f };
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = m_graphicsFamilyIndex;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
		physicalDeviceFeatures.shaderClipDistance = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

		if (m_validationLayers)
		{
			deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayerList.size());
			deviceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}

		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensionList.size());
		deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionList.data();

		GlfwVulkan::ErrorVk(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));
		
		vkGetDeviceQueue(m_device, m_graphicsFamilyIndex, 0, &m_queue);
	}

	void Display::CreateSurface()
	{
		// Creates the Vulkan-GLFW surface.
		GlfwVulkan::ErrorVk(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface));

		GlfwVulkan::ErrorVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities));

		VkBool32 physicalDeviceSurfaceSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_graphicsFamilyIndex, m_surface, &physicalDeviceSurfaceSupport);
		assert(physicalDeviceSurfaceSupport && "Vulkan runtime error, failed to find a physical surface!");

		uint32_t physicalDeviceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> physicalDeviceFormats(physicalDeviceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceFormatCount,
			physicalDeviceFormats.data());

		m_surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		m_surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		if (physicalDeviceFormats[0].format != VK_FORMAT_UNDEFINED)
		{
			m_surfaceFormat = physicalDeviceFormats[0];
		}
	}

	void Display::LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties,
		const VkPhysicalDeviceFeatures &physicalDeviceFeatures, const VkPhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties)
	{
#if FLOUNDER_VERBOSE
		printf("-- Selected Device: '%s' --\n", physicalDeviceProperties.deviceName);
		printf("Type: %i\n", physicalDeviceProperties.deviceType);

		uint32_t supportedVersion[] = {
			VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_MINOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_PATCH(physicalDeviceProperties.apiVersion)
		};
		printf("Supports Version: %i.%i.%i\n", supportedVersion[0], supportedVersion[1], supportedVersion[2]);
		printf("-- Done --\n");
#endif
	}

	void Display::LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties, const std::string &type,
		const bool &showDescription)
	{
#if FLOUNDER_VERBOSE
		printf("-- Avalable Layers For: '%s' --\n", type.c_str());

		for (auto layer : layerProperties)
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
