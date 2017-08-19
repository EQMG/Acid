#pragma once

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

#ifdef FLOUNDER_PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

#include "../framework/framework.h"

namespace flounder
{
	struct VkQueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	struct VkSwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the display.
	/// </summary>
	class display :
		public imodule
	{
	private:
		static const std::vector<const char*> VALIDATION_LAYERS;

		int m_windowWidth;
		int m_windowHeight;
		int m_fullscreenWidth;
		int m_fullscreenHeight;
		float m_aspectRatio;

		std::string m_title;
		std::string m_icon;
		float m_fpsLimit;
		bool m_antialiasing;
		bool m_fullscreen;

		GLFWwindow *m_window;
		bool m_closed;
		bool m_focused;
		int m_windowPosX;
		int m_windowPosY;

		bool m_validationLayers;

		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice;
		VkPhysicalDeviceProperties m_physicalDeviceProperties;
		VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
		std::vector<const char*> m_instanceLayerList;
		std::vector<const char*> m_instanceExtensionList;
		std::vector<const char*> m_deviceExtensionList;
		VkDebugReportCallbackEXT m_debugReport;
		VkDevice m_device;

		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		friend void callbackError(int error, const char *description);

		friend void callbackClose(GLFWwindow *window);

		friend void callbackFocus(GLFWwindow *window, int focused);

		friend void callbackPosition(GLFWwindow *window, int xpos, int ypos);

		friend void callbackSize(GLFWwindow *window, int width, int height);

		friend void callbackFrame(GLFWwindow *window, int width, int height);

		friend VKAPI_ATTR VkBool32 VKAPI_CALL vkCallbackDebug(VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char *layerPrefix,
			const char *msg,
			void *userData);

		VkResult fvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);

		void fvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);
	public:
		static void vkErrorCheck(VkResult result);

		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline display *get()
		{
			return static_cast<display*>(framework::get()->getInstance("display"));
		}

		/// <summary>
		/// Creates a new display module.
		/// </summary>
		display();

		/// <summary>
		/// Deconstructor for the display module.
		/// </summary>
		~display();

		void update() override;

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into the screenshots folder a png image.
		/// </summary>
		void screenshot();

		/// <summary>
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		inline int getWidth() { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

		/// <summary>
		/// Gets the non-fullscreen width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		inline int getWindowWidth() const { return m_windowWidth; }

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		inline int getHeight() { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

		/// <summary>
		/// Gets the non-fullscreen height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		inline int getWindowHeight() const { return m_windowHeight; }

		/// <summary>
		/// Gets the aspect ratio between the displays width and height.
		/// </summary>
		/// <returns> The aspect ratio. </returns>
		inline float getAspectRatio() const { return m_aspectRatio; }

		/// <summary>
		/// Sets window size to a new size.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		/// <param name="height"> The new height in pixels. </param>
		void setWindowSize(const int &width, const int &height);

		/// <summary>
		/// Gets the window's title.
		/// </summary>
		/// <returns> The window's title. </returns>
		inline std::string getTitle() const { return m_title; }

		/// <summary>
		/// Sets window title
		/// </summary>
		/// <param name="title"> The new title. </param>
		void setTitle(const std::string &title);

		/// <summary>
		/// Gets the window's icon file.
		/// </summary>
		/// <returns> The window's icon file. </returns>
		inline std::string getIcon() const { return m_icon; }

		/// <summary>
		/// Sets window icon image.
		/// </summary>
		/// <param name="title"> The new icon file. </param>
		void setIcon(const std::string &icon);

		/// <summary>
		/// Gets the fps limit.
		/// </summary>
		/// <returns> The fps limit. </returns>
		inline float getFpsLimit() const { return m_fpsLimit; }

		/// <summary>
		/// Sets the fps limit. -1 disables limits.
		/// </summary>
		/// <param name="fpsLimit"> The new fps limit. </param>
		inline void setFpsLimit(const float &fpsLimit) { m_fpsLimit = fpsLimit; }

		/// <summary>
		/// Gets if the display requests antialiased images.
		/// </summary>
		/// <returns> If using antialiased images. </returns>
		inline bool isAntialiasing() const { return m_antialiasing; }

		/// <summary>
		/// Requests the display to antialias.
		/// </summary>
		/// <param name="antialiasing"> If the display should antialias. </param>
		inline void setAntialiasing(const bool &antialiasing) { m_antialiasing = antialiasing; }

		/// <summary>
		/// Gets weather the display is fullscreen or not.
		/// </summary>
		/// <returns> Fullscreen or windowed. </returns>
		inline bool isFullscreen() const { return m_fullscreen; }

		/// <summary>
		/// Sets the display to be fullscreen or windowed.
		/// </summary>
		/// <param name="fullscreen"> Weather or not to be fullscreen. </param>
		void setFullscreen(const bool &fullscreen);

		/// <summary>
		/// Gets the current GLFW window.
		/// </summary>
		/// <returns> The current GLFW window. </returns>
		inline GLFWwindow *getWindow() const { return m_window; }

		/// <summary>
		/// Gets if the GLFW display is closed.
		/// </summary>
		/// <returns> If the GLFW display is closed. </returns>
		inline bool isClosed() const { return m_closed; }

		/// <summary>
		/// Gets if the GLFW display is selected.
		/// </summary>
		/// <returns> If the GLFW display is selected. </returns>
		inline bool isFocused() const { return m_focused; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows x position. </returns>
		inline int getWindowXPos() const { return m_windowPosX; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows Y position. </returns>
		inline int getWindowYPos() const { return m_windowPosY; }

		/// <summary>
		/// Gets the current Vulkan instance.
		/// </summary>
		/// <returns> The current Vulkan instance. </returns>
		inline VkInstance getVkInstance() const { return m_instance; }

		/// <summary>
		/// Gets the current Vulkan surface.
		/// </summary>
		/// <returns> The current Vulkan surface. </returns>
		inline VkSurfaceKHR getVkSurface() const { return m_surface; }

		/// <summary>
		/// Gets the current Vulkan physical device (gpu).
		/// </summary>
		/// <returns> The current Vulkan physical device (gpu). </returns>
		inline VkPhysicalDevice getVkPhysicalDevice() const { return m_physicalDevice; }

		inline VkPhysicalDeviceProperties getVkPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

		inline VkPhysicalDeviceFeatures getVkPhysicalDeviceFeatures() const { return m_physicalDeviceFeatures; }

		inline VkPhysicalDeviceMemoryProperties getVkPhysicalDeviceMemoryProperties() const { return m_physicalDeviceMemoryProperties; }

		/// <summary>
		/// Gets the current Vulkan device.
		/// </summary>
		/// <returns> The current Vulkan device. </returns>
		inline VkDevice getVkDevice() const { return m_device; }

		inline VkQueue getVkGraphicsQueue() const { return m_graphicsQueue; }

		inline VkQueue getVkPresentQueue() const { return m_presentQueue; }

		inline VkQueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		uint32_t memoryTypeIndex(uint32_t typeBits, VkFlags properties);
	private:
		void createWindow();

		void setupLayers();

		void setupExtensions();

		void createInstance();

		void setupDebugCallback();

		void createSurface();

		void pickPhysicalDevice();

		void createLogicalDevice();

		bool isDeviceSuitable(VkPhysicalDevice device);
	};
}
