#pragma once

#include "Files/Files.hpp"
#include "Renderer/Commands/CommandBuffer.hpp"
#include "Shader.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
/// <summary>
/// Class
/// that
/// represents
/// is
/// used
/// to
/// represent
/// a
/// Vulkan
/// pipeline.
/// </summary>
class ACID_EXPORT Pipeline
{
  public:
	/// <summary>
	/// Represents
	/// position
	/// in
	/// the
	/// render
	/// structure,
	/// first
	/// value
	/// being
	/// the
	/// renderpass
	/// and
	/// second
	/// for
	/// subpass.
	/// </summary>
	using Stage = std::pair<uint32_t, uint32_t>;

	Pipeline() = default;

	virtual ~Pipeline() = default;

	void BindPipeline(const CommandBuffer& commandBuffer) const
	{
		vkCmdBindPipeline(commandBuffer.GetCommandBuffer(), GetPipelineBindPoint(), GetPipeline());
	}

	virtual const Shader* GetShaderProgram() const = 0;

	virtual const bool& IsPushDescriptors() const = 0;

	virtual const VkDescriptorSetLayout& GetDescriptorSetLayout() const = 0;

	virtual const VkDescriptorPool& GetDescriptorPool() const = 0;

	virtual const VkPipeline& GetPipeline() const = 0;

	virtual const VkPipelineLayout& GetPipelineLayout() const = 0;

	virtual const VkPipelineBindPoint& GetPipelineBindPoint() const = 0;
};
}
