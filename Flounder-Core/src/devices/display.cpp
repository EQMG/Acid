#include "display.h"

namespace flounder
{
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

	VKAPI_ATTR VkBool32 VKAPI_CALL vkCallbackDebug(VkDebugReportFlagsEXT messageFlags, VkDebugReportObjectTypeEXT objectType, uint64_t sourceObject, size_t location, int32_t messageCode, const char *layerPrefix, const char *message, void *userdata)
	{
		std::cout << message << std::endl;
		return false;
	}

	display::display() :
		imodule()
	{
		m_glfwMajor = 3;
		m_glfwMinor = 3;

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

		m_window = NULL;
#ifdef FLOUNDER_API_VULKAN
		m_instance = NULL;
		m_gpu = NULL;
		m_graphicsFamilyIndex = 0;
		m_gpuProperties = {};
		m_instanceLayerList = std::vector<const char*>();
		m_instanceExtensionList = std::vector<const char*>();
		m_deviceExtensionList = std::vector<const char*>();
		m_debugReport = {};
		m_device = NULL;
#endif
		m_closed = false;
		m_focused = true;
		m_windowPosX = 0;
		m_windowPosY = 0;

		// Set the error error callback
		glfwSetErrorCallback(callbackError);

		// Initialize the GLFW library.
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to init GLFW!");
		}

		// Configures the window.
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // The window will stay hidden until after creation.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // The window will be resizable depending on if it's fullscreen.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glfwMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glfwMinor);

		// For new GLFW, and macOS.
		if (m_glfwMajor >= 3 && m_glfwMinor >= 2)
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
		}

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
		m_window = glfwCreateWindow(m_fullscreen ? m_fullscreenWidth : m_windowWidth, m_fullscreen ? m_fullscreenHeight : m_windowHeight, m_title.c_str(), m_fullscreen ? monitor : NULL, NULL);
		m_closed = false;
		m_focused = true;

		// Gets any window errors.
		if (m_window == NULL)
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
		m_instanceLayerList.push_back("VK_LAYER_LUNARG_standard_validation");

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++) 
		{
			m_instanceExtensionList.push_back(glfwExtensions[i]);
		}

		m_instanceExtensionList.push_back("VK_EXT_DEBUG_REPORT_EXTENSION_NAME");

		VkDebugReportCallbackCreateInfoEXT debugCallBackCreateInfo{};
		debugCallBackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugCallBackCreateInfo.pfnCallback = vkCallbackDebug;
		debugCallBackCreateInfo.flags = 
			// VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
			VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_ERROR_BIT_EXT |
			// VK_DEBUG_REPORT_DEBUG_BIT_EXT |
			0;

		// Sets up the instance.
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = m_title.c_str();
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Flounder";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;
		instanceCreateInfo.enabledLayerCount = m_instanceLayerList.size();
		instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerList.data();
		instanceCreateInfo.enabledExtensionCount = m_instanceExtensionList.size();
		instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionList.data();
		instanceCreateInfo.pNext = &debugCallBackCreateInfo;

		if (vkCreateInstance(&instanceCreateInfo, NULL, &m_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance!");
		}

		// Inits debuging.
		fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
		fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");

		if (fvkCreateDebugReportCallbackEXT == NULL || fvkDestroyDebugReportCallbackEXT == NULL)
		{
			throw std::runtime_error("Failed to find Vulkan callback extensions!");
		}

		fvkCreateDebugReportCallbackEXT(m_instance, &debugCallBackCreateInfo, NULL, &m_debugReport);

		// Gets the physical GPU device.
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, NULL);
		std::vector<VkPhysicalDevice> gpuList(gpuCount);
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, gpuList.data());
		m_gpu = gpuList[0];
		vkGetPhysicalDeviceProperties(m_gpu, &m_gpuProperties);

		// Gets the families from the GPU,
		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &familyCount, NULL);
		std::vector<VkQueueFamilyProperties> familyPropertyList(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &familyCount, familyPropertyList.data());

		bool foundGraphics = false;

		for (uint32_t i = 0; i < familyCount; i++)
		{
			if (familyPropertyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_graphicsFamilyIndex = i;
				foundGraphics = true;
			}
		}

		if (!foundGraphics)
		{
			throw std::runtime_error("Failed to find Vulkan graphics queue bit!");
		}

		// Gets the GPU family queue.
		float quePriorities[]{ 1.0f };
		VkDeviceQueueCreateInfo deviceQueueInfo{};
		deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueInfo.queueFamilyIndex = m_graphicsFamilyIndex;
		deviceQueueInfo.queueCount = 1;
		deviceQueueInfo.pQueuePriorities = quePriorities;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueInfo;
		deviceCreateInfo.enabledExtensionCount = m_deviceExtensionList.size();
		deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionList.data();

		if (vkCreateDevice(m_gpu, &deviceCreateInfo, NULL, &m_device) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan device!");
		}
	}

	display::~display()
	{
		// Free the window callbacks and destroy the window.
		glfwDestroyWindow(m_window);

		// Terminate GLFW.
		glfwTerminate();

		// Destroys vulkan.
		fvkDestroyDebugReportCallbackEXT(m_instance, m_debugReport, NULL);
		vkDestroyDevice(m_device, NULL);
		vkDestroyInstance(m_instance, NULL);

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

			if (data == NULL)
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
			glfwSetWindowMonitor(m_window, NULL, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, GLFW_DONT_CARE);
		}
	}
}
