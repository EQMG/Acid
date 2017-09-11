#pragma once

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"

#include "../test/VertexBuffer.hpp"

#include "swapchain/Swapchain.hpp"

#include "IManagerRender.hpp"

namespace Flounder
{
	class Renderer :
		public IModule
	{
	private:
		struct TestUbo
		{
			Colour colour;
		};

		IManagerRender *m_managerRender;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;

		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;

		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;

		Swapchain *m_swapChain;
		Shader *m_shaderTest;
		VertexBuffer *m_vertexBuffer;

		int lastWidth;
		int lastHeight;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get()
		{
			return static_cast<Renderer*>(Engine::Get()->GetModule("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		Renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~Renderer();

		void Update() override;

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		IManagerRender *GetManager() const { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(IManagerRender *managerRender) { m_managerRender = managerRender; }

		std::vector<VkCommandBuffer> GetVkCommandBuffers() const { return m_commandBuffers; }
	private:
		void CreateRenderPass();

		void CreateGraphicsPipeline();

		void CreateCommandPool();

		void CreateCommandBuffers();

		void CreateSemaphores();

		void UpdateUniformBuffer();

		void DrawFrame();

		VkSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	};
}
