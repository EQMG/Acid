#pragma once

#include "../Engine/Engine.hpp"
#include "../Devices/Display.hpp"
#include "Renderer/Pass/Renderpass.hpp"
#include "Stencils/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "Swapchain/Framebuffers.hpp"
#include "IManagerRender.hpp"

namespace Flounder
{
	class F_EXPORT Pass
	{
	public:
		RenderpassCreate *m_renderpassCreate;
		DepthStencil *m_depthStencil;
		Renderpass *m_renderpass;
		Framebuffers *m_framebuffers;

		std::vector<VkClearValue> m_clearValues;
		uint32_t m_imageAttachments;

		Pass(RenderpassCreate *renderpassCreate) :
			m_renderpassCreate(renderpassCreate),
			m_depthStencil(nullptr),
			m_renderpass(nullptr),
			m_framebuffers(nullptr),
			m_clearValues(std::vector<VkClearValue>()),
			m_imageAttachments(0)
		{
			for (auto image : renderpassCreate->images)
			{
				VkClearValue clearValue = {};

				switch (image.m_type)
				{
				case TypeImage:
					clearValue.color = {*image.m_clearColour.m_elements};
					m_imageAttachments++;
					break;
				case TypeDepth:
					clearValue.depthStencil = {1.0f, 0};
					break;
				case TypeSwapchain:
					clearValue.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
					break;
				}

				m_clearValues.push_back(clearValue);
			}
		}

		~Pass()
		{
			delete m_renderpassCreate;
			delete m_depthStencil;
			delete m_renderpass;
			delete m_framebuffers;
		}

		uint32_t GetWidth()
		{
			if (m_renderpassCreate->m_width != 0)
			{
				return m_renderpassCreate->m_width;
			}

			return static_cast<uint32_t>(Display::Get()->GetWidth());
		}

		uint32_t GetHeight()
		{
			if (m_renderpassCreate->m_height != 0)
			{
				return m_renderpassCreate->m_height;
			}

			return static_cast<uint32_t>(Display::Get()->GetHeight());
		}

		bool IsOutOfDate(const VkExtent2D &extent2D)
		{
			return GetWidth() != extent2D.width || GetHeight() != extent2D.height;
		}

		void Rebuild(Swapchain *swapchain, const VkExtent2D &extent2D, const VkExtent3D &extent3D)
		{
			delete m_depthStencil;

			m_depthStencil = new DepthStencil(extent3D);

			delete m_framebuffers;
			m_framebuffers = new Framebuffers(m_renderpass->GetRenderpass(), m_depthStencil->GetImageView(), *swapchain, extent2D);
		}

		VkFramebuffer GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const { return m_framebuffers->GetFramebuffers()[activeSwapchainImage]; }
	};

	class F_EXPORT Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		std::vector<Pass *> m_passes;

		Swapchain *m_swapchain;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchainImage;

		VkPipelineCache m_pipelineCache;

		VkSemaphore m_semaphore;
		VkCommandPool m_commandPool;
		VkCommandBuffer m_commandBuffer;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get()
		{
			return reinterpret_cast<Renderer *>(Engine::Get()->GetModule("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		Renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~Renderer();

	public:
		void Update() override;

		F_HIDDEN void CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates);

		/// <summary>
		/// Starts a renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		/// <param name="i"> The index of the render pass being rendered. </param>
		/// <returns> VK_SUCCESS on success. </returns>
		VkResult StartRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i);

		/// <summary>
		/// Ends the renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void EndRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i);

		/// <summary>
		/// Starts the next render subpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void NextSubpass(const VkCommandBuffer &commandBuffer);

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

		std::vector<Pass *> GetPasses() const { return m_passes; }

		Pass *GetPass(const int &i) const { return m_passes.at(i); }

		Swapchain *GetSwapchain() const { return m_swapchain; }

		VkCommandPool GetCommandPool() const { return m_commandPool; }

		VkCommandBuffer GetCommandBuffer() const { return m_commandBuffer; }

		uint32_t GetActiveSwapchainImage() const { return m_activeSwapchainImage; }

		VkPipelineCache GetPipelineCache() const { return m_pipelineCache; }

	private:
		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreatePass(const int &i);
	};
}
