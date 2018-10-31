#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"

struct GLFWwindow;
struct GLFWmonitor;

namespace acid
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the display.
	/// </summary>
	class ACID_EXPORT Display :
		public IModule
	{
	private:
		uint32_t m_windowWidth;
		uint32_t m_windowHeight;
		uint32_t m_fullscreenWidth;
		uint32_t m_fullscreenHeight;
		float m_aspectRatio;

		uint32_t m_positionX;
		uint32_t m_positionY;

		std::string m_title;
		std::string m_iconPath;
		bool m_antialiasing;
		bool m_fullscreen;

		bool m_closed;
		bool m_focused;
		bool m_iconified;

		bool m_validationLayers;

		GLFWwindow *m_window;

		std::vector<const char *> m_instanceLayerList;
		std::vector<const char *> m_instanceExtensionList;
		std::vector<const char *> m_deviceExtensionList;

		VkDebugReportCallbackEXT m_debugReportCallback;

		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkDevice m_logicalDevice;

		VkSampleCountFlagBits m_msaaSamples;

		VkPhysicalDevice m_physicalDevice;
		VkPhysicalDeviceProperties m_physicalDeviceProperties;
		VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;

		VkQueueFlags m_supportedQueues;
		uint32_t m_graphicsFamily;
		uint32_t m_presentFamily;
		uint32_t m_computeFamily;
		uint32_t m_transferFamily;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		VkQueue m_computeQueue;
		VkQueue m_transferQueue;

		friend void CallbackError(int32_t error, const char *description);

		friend void CallbackMonitor(GLFWmonitor* monitor, int32_t event);

		friend void CallbackClose(GLFWwindow *window);

		friend void CallbackFocus(GLFWwindow *window, int32_t focused);

		friend void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos);

		friend void CallbackSize(GLFWwindow *window, int32_t width, int32_t height);

		friend void CallbackIconify(GLFWwindow *window, int32_t iconified);

		friend void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height);

		friend VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData);

		friend VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);

		friend void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Display *Get() { return Engine::Get()->GetModule<Display>(); }

		Display();

		~Display();

		void Update() override;

		uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties);

		/// <summary>
		/// Gets the width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		uint32_t GetWidth() { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

		/// <summary>
		/// Gets the non-fullscreen width of the display in pixels.
		/// </summary>
		/// <returns> The width of the display. </returns>
		uint32_t GetWindowWidth() const { return m_windowWidth; }

		/// <summary>
		/// Sets the width of the display in pixels.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		void SetWidth(const uint32_t &width) { SetWindowSize(width, GetHeight()); }

		/// <summary>
		/// Gets the height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		uint32_t GetHeight() { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

		/// <summary>
		/// Gets the non-fullscreen height of the display in pixels.
		/// </summary>
		/// <returns> The height of the display. </returns>
		uint32_t GetWindowHeight() const { return m_windowHeight; }

		/// <summary>
		/// Sets the height of the display in pixels.
		/// </summary>
		/// <param name="height"> The new height in pixels. </param>
		void SetHeight(const uint32_t &height) { SetWindowSize(GetWidth(), height); }

		/// <summary>
		/// Sets window size to a new size.
		/// </summary>
		/// <param name="width"> The new width in pixels. </param>
		/// <param name="height"> The new height in pixels. </param>
		void SetWindowSize(const uint32_t &width, const uint32_t &height);

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
		std::string GetIcon() const { return m_iconPath; }

		/// <summary>
		/// Sets window icon image.
		/// </summary>
		/// <param name="filename"> The new icon file. </param>
		void SetIcon(const std::string &filename);

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

		ACID_HIDDEN static std::string StringifyResultGlfw(const int32_t &result);

		ACID_HIDDEN static void CheckGlfw(const int32_t &result);

		static std::string StringifyResultVk(const VkResult &result);

		static void CheckVk(const VkResult &result);

		/// <summary>
		/// Gets if the display is closed.
		/// </summary>
		/// <returns> If the display is closed. </returns>
		bool IsClosed() const { return m_closed; }

		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		bool IsFocused() const { return m_focused; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows x position. </returns>
		uint32_t GetWindowXPos() const { return m_positionX; }

		/// <summary>
		/// Gets the windows Y position of the display in pixels.
		/// </summary>
		/// <returns> The windows Y position. </returns>
		uint32_t GetWindowYPos() const { return m_positionY; }

		/// <summary>
		/// Gets the windows is minimized.
		/// </summary>
		/// <returns> If the window is minimized. </returns>
		bool IsIconified() const { return m_iconified; }

		ACID_HIDDEN GLFWwindow *GetWindow() const { return m_window; }

		VkInstance GetInstance() const { return m_instance; }

		VkSurfaceKHR GetSurface() const { return m_surface; }

		VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() const { return m_surfaceCapabilities; }

		VkSurfaceFormatKHR GetSurfaceFormat() const { return m_surfaceFormat; }

		VkDevice GetLogicalDevice() const { return m_logicalDevice; }

		VkSampleCountFlagBits GetMsaaSamples() const { return m_msaaSamples; }

		VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }

		VkPhysicalDeviceProperties GetPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

		VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures() const { return m_physicalDeviceFeatures; }

		VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties() const { return m_physicalDeviceMemoryProperties; }

		VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }

		VkQueue GetPresentQueue() const { return m_presentQueue; }

		VkQueue GetComputeQueue() const { return m_computeQueue; }

		VkQueue GetTransferQueue() const { return m_transferQueue; }

		uint32_t GetGraphicsFamily() const { return m_graphicsFamily; }

		uint32_t GetPresentFamily() const { return m_presentFamily; }

		uint32_t GetComputeFamily() const { return m_computeFamily; }

		uint32_t GetTransferFamily() const { return m_transferFamily; }
	private:
		void CreateGlfw();

		void SetupLayers();

		void SetupExtensions();

		void CreateInstance();

		void CreateDebugCallback();

		void CreatePhysicalDevice();

		VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices);

		int32_t ScorePhysicalDevice(const VkPhysicalDevice &device);

		VkSampleCountFlagBits GetMaxUsableSampleCount();

		void CreateSurface();

		void CreateQueueIndices();

		void CreateLogicalDevice();

		static void LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties);

		static void LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties, const std::string &type, const bool &showDescription);
	};
}
