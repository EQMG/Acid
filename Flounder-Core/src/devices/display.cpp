#include "display.h"

namespace flounder
{
	const std::vector<const char*> display::VALIDATION_LAYERS = {"VK_LAYER_LUNARG_standard_validation"};

	void callbackError(int error, const char *description)
	{
		fprintf(stderr, "GLFW Error: %s\n", description);
	}

	void callbackClose(GLFWwindow *window)
	{
		display::get()->m_closed = false;
		framework::get()->requestClose(false);
	}

	void callbackFocus(GLFWwindow *window, int focused)
	{
		display::get()->m_focused = focused;
	}

	void callbackPosition(GLFWwindow *window, int xpos, int ypos)
	{
		if (!display::get()->m_fullscreen)
		{
			display::get()->m_windowPosX = xpos;
			display::get()->m_windowPosY = ypos;
		}
	}

	void callbackSize(GLFWwindow *window, int width, int height)
	{
		display::get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		if (display::get()->m_fullscreen)
		{
			display::get()->m_fullscreenWidth = width;
			display::get()->m_fullscreenHeight = height;
		}
		else
		{
			display::get()->m_windowWidth = width;
			display::get()->m_windowHeight = height;
		}
	}

	void callbackFrame(GLFWwindow *window, int width, int height)
	{
		display::get()->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		// TODO
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL vkCallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,   const char *layerPrefix,  const char *msg, void *userData)
	{
		std::cout << "Validation layer: " << msg << std::endl;
		return false;
	}

	VkResult display::fvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void display::fvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

	void display::vkErrorCheck(VkResult result)
	{
		if (result < 0)
		{
			switch (result)
			{
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
				break;
			case VK_ERROR_DEVICE_LOST:
				std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
				break;
			case VK_ERROR_MEMORY_MAP_FAILED:
				std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
				break;
			case VK_ERROR_FEATURE_NOT_PRESENT:
				std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
				break;
			case VK_ERROR_TOO_MANY_OBJECTS:
				std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
				break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
				break;
			case VK_ERROR_SURFACE_LOST_KHR:
				std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
				break;
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
				std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
				break;
			case VK_SUBOPTIMAL_KHR:
				std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				std::cout << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
				break;
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
				std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
				break;
			case VK_ERROR_VALIDATION_FAILED_EXT:
				std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
				break;
			default:
				break;
			}

			assert(0 && "Vulkan runtime error.");
		}
	}

	display::display() :
		imodule()
	{
		m_windowWidth = 1080;
		m_windowHeight = 720;
		m_fullscreenWidth = 0;
		m_fullscreenHeight = 0;
		m_aspectRatio = 1.5f;

		m_title = "Flounder C++";
		m_icon = "";
		m_fpsLimit = -1.0f;
		m_vsync = false;
		m_antialiasing = true;
		m_samples = 0;
		m_fullscreen = false;

		m_window = nullptr;
		m_closed = false;
		m_focused = true;
		m_windowPosX = 0;
		m_windowPosY = 0;

		m_validationLayers = true;

		m_instance = VK_NULL_HANDLE;
		m_surface = VK_NULL_HANDLE;
		m_physicalDevice = VK_NULL_HANDLE;
		m_physicalDeviceProperties = {};
		m_instanceLayerList = std::vector<const char*>();
		m_instanceExtensionList = std::vector<const char*>();
		m_deviceExtensionList = std::vector<const char*>();
		m_debugReport = VK_NULL_HANDLE;
		m_queueIndices = {};
		m_device = VK_NULL_HANDLE;

		// Set the error error callback
		glfwSetErrorCallback(callbackError);

		// Initialize the GLFW library.
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to init GLFW!");
		}

		// Configures the window.
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // The window will be resizable depending on if it's fullscreen.

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

		// Creates the OpenGL context.
		glfwMakeContextCurrent(m_window);

		// Enables VSync if requested.
		glfwSwapInterval(m_vsync ? 1 : 0);

		// Shows the OpenGl window.
		glfwShowWindow(m_window);

		// Sets the displays callbacks.
		glfwSetWindowCloseCallback(m_window, callbackClose);
		glfwSetWindowFocusCallback(m_window, callbackFocus);
		glfwSetWindowPosCallback(m_window, callbackPosition);
		glfwSetWindowSizeCallback(m_window, callbackSize);
		glfwSetFramebufferSizeCallback(m_window, callbackFrame);

		// Sets up the debug callbacks and extensions.
		if (m_validationLayers)
		{
			uint32_t layerCount = 0;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, instanceLayerProperties.data());

			for (const char *layerName : VALIDATION_LAYERS)
			{
				bool layerFound = false;

				for (const auto &layerProperties : instanceLayerProperties)
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

		unsigned int glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			m_instanceExtensionList.push_back(glfwExtensions[i]);
		}

		m_instanceExtensionList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

		m_deviceExtensionList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		VkDebugReportCallbackCreateInfoEXT debugCallBackCreateInfo = {};
		debugCallBackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugCallBackCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT |			VK_DEBUG_REPORT_WARNING_BIT_EXT | 
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | 
			VK_DEBUG_REPORT_ERROR_BIT_EXT | 
			VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		debugCallBackCreateInfo.pfnCallback = vkCallbackDebug;

		// Sets up the instance.
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = m_title.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.pEngineName = "Flounder";
		applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = &debugCallBackCreateInfo;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		if (m_validationLayers)
		{
			instanceCreateInfo.enabledLayerCount = m_instanceLayerList.size();
			instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
		}

		instanceCreateInfo.enabledExtensionCount = m_instanceExtensionList.size();
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();

		vkErrorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));

		// Inits debuging.
		if (m_validationLayers)
		{
			vkErrorCheck(fvkCreateDebugReportCallbackEXT(m_instance, &debugCallBackCreateInfo, nullptr, &m_debugReport));
		}

		// Creates the Vulkan-GLFW surface.
		vkErrorCheck(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface));

		// Gets the physical GPU device.
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> deviceList(deviceCount);
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

			// Figgures out extensions support.
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(m_deviceExtensionList.begin(), m_deviceExtensionList.end());

			for (const auto& extension : availableExtensions) {
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
			
			// Application can't function without geometry shaders, without extensions or adequate swap chains.
			if (!deviceFeatures.geometryShader || !extensionsSupported || !swapChainAdequate)
			{
				score = 0;
			}

			candidates.insert(std::make_pair(score, device));
		}

		if (candidates.rbegin()->first > 0) 
		{
			m_physicalDevice = candidates.rbegin()->second;
			vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		}
		else 
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}

		// Finds the queue families from the devices.
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

		for (uint32_t i = 0; i < queueFamilyCount; i++)
		{
			VkQueueFamilyProperties queueFamily = queueFamilies[i];

			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
			{
				m_queueIndices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport) 
			{
				m_queueIndices.presentFamily = i;
			}

			if (m_queueIndices.isComplete()) 
			{
				break;
			}
		}

		if (!m_queueIndices.isComplete())
		{
			throw std::runtime_error("Failed to find Vulkan graphics queue bit!");
		}

		// Gets the GPU family queues.
		std::set<int> uniqueQueueFamilies = { m_queueIndices.graphicsFamily, m_queueIndices.presentFamily };
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float quePriorities[] = { 1.0f };

		for (int queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_queueIndices.graphicsFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = quePriorities;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Creates the Vulkan device object.
		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		if (m_validationLayers)
		{
			deviceCreateInfo.enabledLayerCount = m_instanceLayerList.size();
			deviceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		}
		else
		{
			deviceCreateInfo.enabledLayerCount = 0;
		}

		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		vkErrorCheck(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));
	}

	display::~display()
	{
		// Destroys Vulkan.
		vkDestroyDevice(m_device, nullptr);
		fvkDestroyDebugReportCallbackEXT(m_instance, m_debugReport, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Terminate GLFW.
		glfwTerminate();

		m_closed = false;
	}

	void display::update()
	{
		// Swap the colour buffers to the display.
		glfwSwapBuffers(m_window);

		// Polls for window events. The key callback will only be invoked during this call.
		glfwPollEvents();

		glfwSwapInterval(m_vsync ? 1 : 0);
		glfwWindowHint(GLFW_SAMPLES, m_samples);

		// Updates the aspect ratio.
		m_aspectRatio = static_cast<float>(getWidth()) / static_cast<float>(getHeight());
	}

	void display::screenshot()
	{
		// TODO
	}

	void display::setWindowSize(const int &width, const int &height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		glfwSetWindowSize(m_window, width, height);
	}

	void display::setTitle(const std::string &title)
	{
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	void display::setIcon(const std::string &icon)
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
				std::cout << "Unable to load texture: " << m_icon << std::endl;
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

	void display::setFullscreen(const bool &fullscreen)
	{
		if (m_fullscreen == fullscreen)
		{
			return;
		}

		m_fullscreen = fullscreen;

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

		std::cout << (fullscreen ? "Display is going fullscreen." : "Display is going windowed.") << std::endl;

		if (fullscreen)
		{
			m_fullscreenWidth = videoMode->width;
			m_fullscreenHeight = videoMode->height;
			glfwSetWindowMonitor(m_window, monitor, 0, 0, m_fullscreenWidth, m_fullscreenHeight, GLFW_DONT_CARE);
		}
		else
		{
			m_windowPosX = (videoMode->width - m_windowWidth) / 2;
			m_windowPosY = (videoMode->height - m_windowHeight) / 2;
			glfwSetWindowMonitor(m_window, nullptr, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}
}
