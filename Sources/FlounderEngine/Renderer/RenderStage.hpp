#pragma once

#include "../Devices/Display.hpp"
#include "Renderpass/Renderpass.hpp"
#include "Stencils/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "Swapchain/Framebuffers.hpp"

namespace Flounder
{
	class F_EXPORT RenderStage
	{
	public:
		RenderpassCreate *m_renderpassCreate;

		DepthStencil *m_depthStencil;
		Renderpass *m_renderpass;
		Framebuffers *m_framebuffers;

		std::vector<VkClearValue> m_clearValues;
		uint32_t m_imageAttachments;
		bool m_hasDepth;
		bool m_hasSwapchain;

		bool m_fitDisplaySize;

		RenderStage(RenderpassCreate *renderpassCreate);

		~RenderStage();

		void RebuildImages(Swapchain *swapchain, const VkExtent2D &extent2D, const VkExtent3D &extent3D);

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		bool IsOutOfDate(const VkExtent2D &extent2D) const;

		VkFramebuffer GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const { return m_framebuffers->GetFramebuffers()[activeSwapchainImage]; }
	};
}
