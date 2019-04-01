#pragma once

#include <vulkan/vulkan.h>
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Images/ImageDepth.hpp"
#include "Renderpass/Framebuffers.hpp"
#include "Renderpass/Renderpass.hpp"
#include "Renderpass/Swapchain.hpp"

namespace acid
{
/**
 * Class that represents an attachment in a renderpass.
 */
class ACID_EXPORT Attachment
{
public:
	enum class Type
	{
		Image, Depth, Swapchain
	};

	/**
	 * Creates a new attachment that represents a object in the render pipeline.
	 * @param binding The index the attachment is bound to in the renderpass.
	 * @param name The unique name given to the object for all renderpasses.
	 * @param type The attachment type this represents.
	 * @param multisampled If this attachment is multisampled.
	 * @param format The format that will be created (only applies to type ATTACHMENT_IMAGE).
	 * @param clearColour The colour to clear to before rendering to it.
	 */
	Attachment(const uint32_t &binding, std::string name, const Type &type, const bool &multisampled = false, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM,
		const Colour &clearColour = Colour::Black) :
		m_binding(binding),
		m_name(std::move(name)),
		m_type(type),
		m_multisampled(multisampled),
		m_format(format),
		m_clearColour(clearColour)
	{
	}

	const uint32_t &GetBinding() const { return m_binding; }

	const std::string &GetName() const { return m_name; }

	const Type &GetType() const { return m_type; }

	const bool &IsMultisampled() const { return m_multisampled; }

	const VkFormat &GetFormat() const { return m_format; }

	const Colour &GetClearColour() const { return m_clearColour; }

private:
	uint32_t m_binding;
	std::string m_name;
	Type m_type;
	bool m_multisampled;
	VkFormat m_format;
	Colour m_clearColour;
};

class ACID_EXPORT SubpassType
{
public:
	SubpassType(const uint32_t &binding, std::vector<uint32_t> attachmentBindings) :
		m_binding(binding),
		m_attachmentBindings(std::move(attachmentBindings))
	{
	}

	const uint32_t &GetBinding() const { return m_binding; }

	const std::vector<uint32_t> &GetAttachmentBindings() const { return m_attachmentBindings; }

private:
	uint32_t m_binding;
	std::vector<uint32_t> m_attachmentBindings;
};

class ACID_EXPORT Viewport
{
public:
	explicit Viewport(const Vector2 &scale = Vector2::One) :
		m_scale(scale),
		m_offset(0.0f, 0.0f)
	{
	}

	Viewport(const std::optional<uint32_t> &width, const std::optional<uint32_t> &height) :
		m_width(width),
		m_height(height),
		m_scale(1.0f, 1.0f),
		m_offset(0.0f, 0.0f)
	{
	}

	const std::optional<uint32_t> &GetWidth() const { return m_width; }

	void SetWidth(const std::optional<uint32_t> &width) { m_width = width; }

	const std::optional<uint32_t> &GetHeight() const { return m_height; }

	void SetHeight(const std::optional<uint32_t> &height) { m_height = height; }

	const Vector2 &GetScale() const { return m_scale; }

	void SetScale(const Vector2 &scale) { m_scale = scale; }

	const Vector2 &GetOffset() const { return m_offset; }

	void SetOffset(const Vector2 &offset) { m_offset = offset; }

private:
	std::optional<uint32_t> m_width;
	std::optional<uint32_t> m_height;
	Vector2 m_scale;
	Vector2 m_offset;
};

class ACID_EXPORT RenderStage
{
public:
	explicit RenderStage(std::vector<Attachment> images = {}, std::vector<SubpassType> subpasses = {}, const Viewport &viewport = Viewport());

	void Update();

	void Rebuild(const Swapchain &swapchain);

	std::optional<Attachment> GetAttachment(const std::string &name) const;

	std::optional<Attachment> GetAttachment(const uint32_t &binding) const;

	const std::vector<Attachment> &GetAttachments() const { return m_attachments; }

	const std::vector<SubpassType> &GetSubpasses() const { return m_subpasses; }

	Viewport &GetViewport() { return m_viewport; }

	void SetViewport(const Viewport &viewport) { m_viewport = viewport; }

	/**
	 * Gets the height of the render stage in pixels.
	 * @return The height of the render stage.
	 */
	const uint32_t &GetWidth() const { return m_width; }

	/**
	 * Gets the height of the render stage in pixels.
	 * @return The height of the render stage.
	 */
	const uint32_t &GetHeight() const { return m_height; }

	/**
	 * Gets the aspect ratio between the render stages width and height.
	 * @return The aspect ratio.
	 */
	const float &GetAspectRatio() const { return m_aspectRatio; }

	/**
	 * Gets if the width or height has changed between the last update and now.
	 * @return If the width or height has changed.
	 */
	const bool &IsOutOfDate() const { return m_outOfDate; }

	const Renderpass *GetRenderpass() const { return m_renderpass.get(); };

	const ImageDepth *GetDepthStencil() const { return m_depthStencil.get(); };

	const Framebuffers *GetFramebuffers() const { return m_framebuffers.get(); };

	const Descriptor *GetDescriptor(const std::string &name) const;

	const VkFramebuffer &GetActiveFramebuffer(const uint32_t &activeSwapchainImage) const;

	const std::vector<VkClearValue> &GetClearValues() const { return m_clearValues; }

	const uint32_t &GetAttachmentCount(const uint32_t &subpass) const { return m_subpassAttachmentCount[subpass]; }

	bool HasDepth() const { return m_depthAttachment.has_value(); }

	bool HasSwapchain() const { return m_swapchainAttachment.has_value(); }

	bool IsMultisampled(const uint32_t &subpass) const { return m_subpassMultisampled[subpass]; }

private:
	friend class Renderer;

	std::vector<Attachment> m_attachments;
	std::vector<SubpassType> m_subpasses;

	Viewport m_viewport;

	std::unique_ptr<Renderpass> m_renderpass;
	std::unique_ptr<ImageDepth> m_depthStencil;
	std::unique_ptr<Framebuffers> m_framebuffers;

	std::map<std::string, const Descriptor *> m_descriptors;

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
