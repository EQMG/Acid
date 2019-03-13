#include "Renderpass.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
Renderpass::Renderpass(const RenderpassCreate& renderpassCreate, const VkFormat& depthFormat, const VkFormat& surfaceFormat, const VkSampleCountFlagBits& samples) : m_renderpass(VK_NULL_HANDLE)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	// Creates
	// the
	// renderpasses
	// attachment
	// descriptions,
	std::vector<VkAttachmentDescription> attachments = {};

	for(const auto& image : renderpassCreate.GetImages())
		{
			auto imageSamples = image.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;
			VkAttachmentDescription attachment = {};
			attachment.samples = imageSamples;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear at beginning of the render pass.
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // // The image can be read from so it's important to store the attachment results
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // We don't care about initial layout of the attachment.

			switch(image.GetType())
				{
					case Attachment::Type::Image:
						attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						attachment.format = image.GetFormat();
						break;
					case Attachment::Type::Depth:
						attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
						attachment.format = depthFormat;
						break;
					case Attachment::Type::Swapchain:
						attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
						attachment.format = surfaceFormat;
						break;
				}

			attachments.emplace_back(attachment);
		}

	// Creates
	// each
	// subpass
	// and
	// its
	// dependencies.
	std::vector<std::unique_ptr<SubpassDescription>> subpasses = {};
	std::vector<VkSubpassDependency> dependencies = {};

	for(const auto& subpassType : renderpassCreate.GetSubpasses())
		{
			// Attachments.
			std::vector<VkAttachmentReference> subpassColourAttachments = {};

			std::optional<uint32_t> depthAttachment = {};

			for(const auto& attachmentBinding : subpassType.GetAttachmentBindings())
				{
					auto attachment = renderpassCreate.GetAttachment(attachmentBinding);

					if(!attachment)
						{
							Log::Error("Filed to find a renderpass attachment bound to: %i\n", attachmentBinding);
							continue;
						}

					if(attachment->GetType() == Attachment::Type::Depth)
						{
							depthAttachment = attachment->GetBinding();
							continue;
						}

					VkAttachmentReference attachmentReference = {};
					attachmentReference.attachment = attachment->GetBinding();
					attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					subpassColourAttachments.emplace_back(attachmentReference);
				}

			// Subpass description.
			subpasses.emplace_back(std::make_unique<SubpassDescription>(VK_PIPELINE_BIND_POINT_GRAPHICS, subpassColourAttachments, depthAttachment));

			// Subpass dependencies.
			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			if(subpassType.GetBinding() == renderpassCreate.GetSubpasses().size())
				{
					subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
					subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
					subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				}
			else
				{
					subpassDependency.dstSubpass = subpassType.GetBinding();
				}

			if(subpassType.GetBinding() == 0)
				{
					subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
					subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
					subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
					subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				}
			else
				{
					subpassDependency.srcSubpass = subpassType.GetBinding() - 1;
				}

			dependencies.emplace_back(subpassDependency);
		}

	std::vector<VkSubpassDescription> subpassDescriptions = {};
	subpassDescriptions.reserve(subpasses.size());

	for(const auto& subpass : subpasses)
		{
			subpassDescriptions.emplace_back(subpass->GetSubpassDescription());
		}

	// Creates
	// the
	// render
	// pass.
	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassCreateInfo.pAttachments = attachments.data();
	renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
	renderPassCreateInfo.pSubpasses = subpassDescriptions.data();
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassCreateInfo.pDependencies = dependencies.data();
	Renderer::CheckVk(vkCreateRenderPass(logicalDevice->GetLogicalDevice(), &renderPassCreateInfo, nullptr, &m_renderpass));
}

Renderpass::~Renderpass()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkDestroyRenderPass(logicalDevice->GetLogicalDevice(), m_renderpass, nullptr);
}
}
