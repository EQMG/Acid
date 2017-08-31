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

#include "../engine/Engine.hpp"

namespace Flounder
{
	struct VkQueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() const { return graphicsFamily >= 0 && presentFamily >= 0; }
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
	class Display :
		public IModule
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

		GLFWwindow *m_glfwWindow;
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

		friend void CallbackError(int error, const char *description);

		friend void CallbackClose(GLFWwindow *window);

		friend void CallbackFocus(GLFWwindow *window, int focused);

		friend void CallbackPosition(GLFWwindow *window, int xpos, int ypos);

		friend void CallbackSize(GLFWwindow *window, int width, int height);

		friend void CallbackFrame(GLFWwindow *window, int width, int height);

		friend VKAPI_ATTR VkBool32 VKAPI_CALL VkCallbackDebug(VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char *layerPrefix,
			const char *msg,
			void *userData);

		VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);

		void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);
	public:
		static void vkErrorCheck(VkResult result);

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Display *Get()
		{
			return static_cast<Display*>(Engine::Get()->GetModule("display"));
		}

		/// <summary>
		/// Creates a new display module.
		/// </summary>
		Display();

		/// <summary>
		/// Deconstructor for the display module.
		/// </summary>
		~Display();

		void Update() override;

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into the screenshots folder a png image.
		/// </summary>
		void Screenshot();

		/// <summary>
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWidth() { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

		/// <summary>
		/// Gets the non-fullscreen width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWindowWidth() const { return m_windowWidth; }

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int GetHeight() { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

		/// <summary>
		/// Gets the non-fullscreen height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int GetWindowHeight() const { return m_windowHeight; }

		/// <summary>
		/// Gets the aspect ratio between the displays width and height.
		/// </summary>
		/// <returns> The aspect ratio. </returns>
		float GetAspectRatio() const { return m_aspectRatio; }

		/// <summary>
		/// Sets window size to a new size.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		/// <param name="height"> The new height in pixels. </param>
		void SetWindowSize(const int &width, const int &height);

		/// <summary>
		/// Gets the window's title.
		/// </summary>
		/// <returns> The window's title. </returns>
		std::string GetTitle() const { return m_title; }

		/// <summary>
		/// Sets window title
		/// </summary>
		/// <param name="title"> The new title. </param>
		void SetTitle(const std::string &title);

		/// <summary>
		/// Gets the window's icon file.
		/// </summary>
		/// <returns> The window's icon file. </returns>
		std::string GetIcon() const { return m_icon; }

		/// <summary>
		/// Sets window icon image.
		/// </summary>
		/// <param name="title"> The new icon file. </param>
		void SetIcon(const std::string &icon);

		/// <summary>
		/// Gets the fps limit.
		/// </summary>
		/// <returns> The fps limit. </returns>
		float GetFpsLimit() const { return m_fpsLimit; }

		/// <summary>
		/// Sets the fps limit. -1 disables limits.
		/// </summary>
		/// <param name="fpsLimit"> The new fps limit. </param>
		void SetFpsLimit(const float &fpsLimit) { m_fpsLimit = fpsLimit; }

		/// <summary>
		/// Gets if the display requests antialiased images.
		/// </summary>
		/// <returns> If using antialiased images. </returns>
		bool IsAntialiasing() const { return m_antialiasing; }

		/// <summary>
		/// Requests the display to antialias.
		/// </summary>
		/// <param name="antialiasing"> If the display should antialias. </param>
		void SetAntialiasing(const bool &antialiasing) { m_antialiasing = antialiasing; }

		/// <summary>
		/// Gets weather the display is fullscreen or not.
		/// </summary>
		/// <returns> Fullscreen or windowed. </returns>
		bool IsFullscreen() const { return m_fullscreen; }

		/// <summary>
		/// Sets the display to be fullscreen or windowed.
		/// </summary>
		/// <param name="fullscreen"> Weather or not to be fullscreen. </param>
		void SetFullscreen(const bool &fullscreen);

		/// <summary>
		/// Gets the current GLFW window.
		/// </summary>
		/// <returns> The current GLFW window. </returns>
		GLFWwindow *GetGlfwWindow() const { return m_glfwWindow; }

		/// <summary>
		/// Gets if the GLFW display is closed.
		/// </summary>
		/// <returns> If the GLFW display is closed. </returns>
		bool IsClosed() const { return m_closed; }

		/// <summary>
		/// Gets if the GLFW display is selected.
		/// </summary>
		/// <returns> If the GLFW display is selected. </returns>
		bool IsFocused() const { return m_focused; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows x position. </returns>
		int GetWindowXPos() const { return m_windowPosX; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows Y position. </returns>
		int GetWindowYPos() const { return m_windowPosY; }

		/// <summary>
		/// Gets the current Vulkan instance.
		/// </summary>
		/// <returns> The current Vulkan instance. </returns>
		VkInstance GetVkInstance() const { return m_instance; }

		/// <summary>
		/// Gets the current Vulkan surface.
		/// </summary>
		/// <returns> The current Vulkan surface. </returns>
		VkSurfaceKHR GetVkSurface() const { return m_surface; }

		/// <summary>
		/// Gets the current Vulkan physical device (gpu).
		/// </summary>
		/// <returns> The current Vulkan physical device (gpu). </returns>
		VkPhysicalDevice GetVkPhysicalDevice() const { return m_physicalDevice; }

		VkPhysicalDeviceProperties GetVkPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

		VkPhysicalDeviceFeatures GetVkPhysicalDeviceFeatures() const { return m_physicalDeviceFeatures; }

		VkPhysicalDeviceMemoryProperties GetVkPhysicalDeviceMemoryProperties() const { return m_physicalDeviceMemoryProperties; }

		/// <summary>
		/// Gets the current Vulkan device.
		/// </summary>
		/// <returns> The current Vulkan device. </returns>
		VkDevice GetVkDevice() const { return m_device; }

		VkQueue GetVkGraphicsQueue() const { return m_graphicsQueue; }

		VkQueue GetVkPresentQueue() const { return m_presentQueue; }

		inline VkQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		uint32_t MemoryTypeIndex(uint32_t typeBits, VkFlags properties);
	private:
		void CreateGlfwWindow();

		void SetupLayers();

		void SetupExtensions();

		void CreateInstance();

		void SetupDebugCallback();

		void CreateSurface();

		void PickPhysicalDevice();

		void CreateLogicalDevice();

		bool IsDeviceSuitable(VkPhysicalDevice device);
	};
}
