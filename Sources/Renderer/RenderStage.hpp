#pragma once

#include "Renderpass/Framebuffers.hpp"
#include "Renderpass/Renderpass.hpp"
#include "Renderpass/Swapchain.hpp"
#include "Textures/DepthStencil.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
class ACID_EXPORT RenderStage
{
  public:
	RenderStage(RenderpassCreate renderpassCreate);

	void Update();

	void Rebuild(const Swapchain& swapchain);

	uint32_t SubpassCount() const
	{
		return static_cast<uint32_t>(m_renderpassCreate.GetSubpasses().size());
	};

	/// <summary>
	/// Gets
	/// the
	/// height
	/// of
	/// the
	/// render
	/// stage
	/// in
	/// pixels.
	/// </summary>
	/// <returns>
	/// The
	/// height
	/// of
	/// the
	/// render
	/// stage.
	/// </returns>
	const uint32_t& GetWidth() const
	{
		return m_width;
	}

	/// <summary>
	/// Gets
	/// the
	/// height
	/// of
	/// the
	/// render
	/// stage
	/// in
	/// pixels.
	/// </summary>
	/// <returns>
	/// The
	/// height
	/// of
	/// the
	/// render
	/// stage.
	/// </returns>
	const uint32_t& GetHeight() const
	{
		return m_height;
	}

	/// <summary>
	/// Gets
	/// the
	/// aspect
	/// ratio
	/// between
	/// the
	/// render
	/// stages
	/// width
	/// and
	/// height.
	/// </summary>
	/// <returns>
	/// The
	/// aspect
	/// ratio.
	/// </returns>
	const float& GetAspectRatio() const
	{
		return m_aspectRatio;
	}

	/// <summary>
	/// Gets
	/// if
	/// the
	/// width
	/// or
	/// height
	/// has
	/// changed
	/// between
	/// the
	/// last
	/// update
	/// and
	/// now.
	/// </summary>
	/// <returns>
	/// If
	/// the
	/// width
	/// or
	/// height
	/// has
	/// changed.
	/// </returns>
	const bool& IsOutOfDate() const
	{
		return m_outOfDate;
	}

	RenderpassCreate& GetRenderpassCreate()
	{
		return m_renderpassCreate;
	}

	const Renderpass* GetRenderpass() const
	{
		return m_renderpass.get();
	};

	const DepthStencil* GetDepthStencil() const
	{
		return m_depthStencil.get();
	};

	const Framebuffers* GetFramebuffers() const
	{
		return m_framebuffers.get();
	};

	const Descriptor* GetAttachment(const std::string& name) const;

	const VkFramebuffer& GetActiveFramebuffer(const uint32_t& activeSwapchainImage) const;

	const std::vector<VkClearValue>& GetClearValues() const
	{
		return m_clearValues;
	}

	const uint32_t& GetAttachmentCount(const uint32_t& subpass) const
	{
		return m_subpassAttachmentCount[subpass];
	}

	bool HasDepth() const
	{
		return m_depthAttachment.has_value();
	}

	bool HasSwapchain() const
	{
		return m_swapchainAttachment.has_value();
	}

	bool IsMultisampled(const uint32_t& subpass) const
	{
		return m_subpassMultisampled[subpass];
	}

  private:
	friend class Renderer;

	RenderpassCreate m_renderpassCreate;

	std::unique_ptr<Renderpass> m_renderpass;
	std::unique_ptr<DepthStencil> m_depthStencil;
	std::unique_ptr<Framebuffers> m_framebuffers;

	std::map<std::string, const Descriptor*> m_attachments;

	std::vector<VkClearValue> m_clearValues;
	std::vector<uint32_t> m_subpassAttachmentCount;
	std::optional<Attachment> m_depthAttachment;
	std::optional<Attachment> m_swapchainAttachment;
	std::vector<bool> m_subpassMultisampled;

	uint32_t m_width;
	uint32_t m_height;
	float m_aspectRatio;
	bool m_outOfDate;
};
}
