#pragma once

#include <optional>
#include <map>
#include <vector>
#include <vulkan/vulkan.h>
#include "Renderpass/Renderpass.hpp"
#include "Swapchain/Framebuffers.hpp"
#include "Swapchain/Swapchain.hpp"
#include "Textures/DepthStencil.hpp"

namespace acid
{
	class ACID_EXPORT RenderStage
	{
	private:
		uint32_t m_stageIndex;
		RenderpassCreate m_renderpassCreate;

		std::unique_ptr<Renderpass> m_renderpass;
		std::unique_ptr<DepthStencil> m_depthStencil;
		std::unique_ptr<Framebuffers> m_framebuffers;

		std::map<std::string, Descriptor *> m_attachments;

		std::vector<VkClearValue> m_clearValues;
		std::vector<uint32_t> m_subpassAttachmentCount;
		std::optional<Attachment> m_depthAttachment;
		std::optional<Attachment> m_swapchainAttachment;
		std::vector<bool> m_subpassMultisampled;

		bool m_fitDisplaySize;

		uint32_t m_lastWidth;
		uint32_t m_lastHeight;
	public:
		RenderStage(const uint32_t &stageIndex, const RenderpassCreate &renderpassCreate);

		void Rebuild(const Swapchain &swapchain);

		uint32_t SubpassCount() const { return static_cast<uint32_t>(m_renderpassCreate.GetSubpasses().size()); };

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		bool IsOutOfDate(const VkExtent2D &extent2D);

		RenderpassCreate &GetRenderpassCreate() { return m_renderpassCreate; }

		const Renderpass *GetRenderpass() const { return m_renderpass.get(); };

		const DepthStencil *GetDepthStencil() const { return m_depthStencil.get(); };

		const Framebuffers *GetFramebuffers() const { return m_framebuffers.get(); };

		Descriptor *GetAttachment(const std::string &name) const;

		VkFramebuffer GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const;

		const std::vector<VkClearValue> &GetClearValues() const { return m_clearValues; }

		const uint32_t &GetAttachmentCount(const uint32_t &subpass) const { return m_subpassAttachmentCount[subpass]; }

		bool HasDepth() const { return m_depthAttachment.has_value(); }

		bool HasSwapchain() const { return m_swapchainAttachment.has_value(); }

		bool IsMultisampled(const uint32_t &subpass) const { return m_subpassMultisampled[subpass]; }

		const bool &FitDisplaySize() const { return m_fitDisplaySize; }
	};
}
