#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"

class GLFWwindow;

namespace fl
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the display.
	/// </summary>
	class FL_EXPORT Display :
		public IModule
	{
	private:
		int m_windowWidth;
		int m_windowHeight;
		int m_fullscreenWidth;
		int m_fullscreenHeight;
		float m_aspectRatio;

		std::string m_title;
		std::string m_icon;
		bool m_antialiasing;
		bool m_fullscreen;

		GLFWwindow *m_window;
		bool m_closed;
		bool m_focused;
		int m_windowPosX;
		int m_windowPosY;
		bool m_iconified;

		bool m_validationLayers;

		std::vector<const char *> m_instanceLayerList;
		std::vector<const char *> m_instanceExtensionList;
		std::vector<const char *> m_deviceExtensionList;
		VkDebugReportCallbackEXT m_debugReport;

		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkDevice m_logicalDevice;
		VkQueue m_queue;

		VkPhysicalDevice m_physicalDevice;
		VkPhysicalDeviceProperties m_physicalDeviceProperties;
		VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
		uint32_t m_graphicsFamilyIndex;

		friend void CallbackError(int error, const char *description);

		friend void CallbackClose(GLFWwindow *window);

		friend void CallbackFocus(GLFWwindow *window, int focused);

		friend void CallbackPosition(GLFWwindow *window, int xpos, int ypos);

		friend void CallbackSize(GLFWwindow *window, int width, int height);

		friend void CallbackFrame(GLFWwindow *window, int width, int height);

		friend void CallbackIconify(GLFWwindow *window, int iconified);

		friend VKAPI_ATTR VkBool32 VKAPI_CALL VkCallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, void *userData);

		VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);

		void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Display *Get()
		{
			return reinterpret_cast<Display *>(Engine::Get()->GetModule("display"));
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
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWidth() { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

		/// <summary>
		/// Sets the width of the display in pixels.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		void SetWidth(const int &width) { SetWindowSize(width, GetHeight()); }

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		int GetHeight() { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

		/// <summary>
		/// Sets the height of the display in pixels.
		/// </summary>
		/// <param name="height"> The new height in pixels. </param>
		void SetHeight(const int &height) { SetWindowSize(GetWidth(), height); }

		/// <summary>
		/// Sets window size to a new size.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		/// <param name="height"> The new height in pixels. </param>
		void SetWindowSize(const int &width, const int &height);

		/// <summary>
		/// Gets the non-fullscreen width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		int GetWindowWidth() const { return m_windowWidth; }

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

		FL_HIDDEN static std::string StringifyResultVk(const VkResult &result);

		FL_HIDDEN static void ErrorVk(const VkResult &result);

		FL_HIDDEN static std::string StringifyResultGlfw(const int &result);

		FL_HIDDEN static void ErrorGlfw(const int &result);

		/// <summary>
		/// Gets the current GLFW window.
		/// </summary>
		/// <returns> The current GLFW window. </returns>
		FL_HIDDEN GLFWwindow *GetGlfwWindow() const { return m_window; }

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
		/// Gets the windows is minimized.
		/// </summary>
		/// <returns> If the window is minimized. </returns>
		bool IsIconified() const { return m_iconified; }

		FL_HIDDEN VkInstance GetVkInstance() const { return m_instance; }

		FL_HIDDEN VkSurfaceKHR GetVkSurface() const { return m_surface; }

		FL_HIDDEN VkSurfaceCapabilitiesKHR GetVkSurfaceCapabilities() const { return m_surfaceCapabilities; }

		FL_HIDDEN VkSurfaceFormatKHR GetVkSurfaceFormat() const { return m_surfaceFormat; }

		FL_HIDDEN VkDevice GetVkLogicalDevice() const { return m_logicalDevice; }

		FL_HIDDEN VkQueue GetVkQueue() const { return m_queue; }

		FL_HIDDEN VkPhysicalDevice GetVkPhysicalDevice() const { return m_physicalDevice; }

		FL_HIDDEN VkPhysicalDeviceProperties GetVkPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

		FL_HIDDEN VkPhysicalDeviceFeatures GetVkPhysicalDeviceFeatures() const { return m_physicalDeviceFeatures; }

		FL_HIDDEN VkPhysicalDeviceMemoryProperties GetVkPhysicalDeviceMemoryProperties() const { return m_physicalDeviceMemoryProperties; }

		FL_HIDDEN uint32_t GetVkGraphicsFamilyIndex() const { return m_graphicsFamilyIndex; }
	private:
		void CreateGlfw();

		void CreateVulkan();

		void SetupLayers();

		void SetupExtensions();

		void CreateInstance();

		void CreateDebugCallback();

		void CreatePhysicalDevice();

		VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices);

		int ScorePhysicalDevice(const VkPhysicalDevice &device);

		void CreateLogicalDevice();

		void CreateSurface();

		static void LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties, const VkPhysicalDeviceFeatures &physicalDeviceFeatures, const VkPhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties);

		static void LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties, const std::string &type, const bool &showDescription);
	};
}
