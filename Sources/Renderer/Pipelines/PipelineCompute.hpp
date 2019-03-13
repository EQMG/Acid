#pragma once

#include "Pipeline.hpp"
#include "Renderer/Commands/CommandBuffer.hpp"

namespace acid
{
/// <summary>
/// Class
/// that
/// represents
/// a
/// compute
/// Vulkan
/// compute
/// pipeline.
/// </summary>
class ACID_EXPORT PipelineCompute : public Pipeline
{
  public:
	/// <summary>
	/// Creates
	/// a
	/// new
	/// compute
	/// pipeline.
	/// </summary>
	/// <param
	/// name="shaderStage">
	/// The
	/// shader
	/// file
	/// that
	/// will
	/// be
	/// loaded.
	/// </param>
	/// <param
	/// name="defines">
	/// A
	/// list
	/// of
	/// defines
	/// added
	/// to
	/// the
	/// top
	/// of
	/// each
	/// shader.
	/// </param>
	/// <param
	/// name="pushDescriptors">
	/// If
	/// no
	/// actual
	/// descriptor
	/// sets
	/// are
	/// allocated
	/// but
	/// instead
	/// pushed.
	/// </param>
	explicit PipelineCompute(std::string shaderStage, std::vector<Shader::Define> defines = {}, const bool& pushDescriptors = false);

	~PipelineCompute();

	const std::string& GetShaderStage() const
	{
		return m_shaderStage;
	}

	const std::vector<Shader::Define>& GetDefines() const
	{
		return m_defines;
	}

	const bool& IsPushDescriptors() const override
	{
		return m_pushDescriptors;
	}

	bool CmdRender(const CommandBuffer& commandBuffer, const uint32_t& width, const uint32_t& height) const;

	const Shader* GetShaderProgram() const override
	{
		return m_shader.get();
	}

	const VkDescriptorSetLayout& GetDescriptorSetLayout() const override
	{
		return m_descriptorSetLayout;
	}

	const VkDescriptorPool& GetDescriptorPool() const override
	{
		return m_descriptorPool;
	}

	const VkPipeline& GetPipeline() const override
	{
		return m_pipeline;
	}

	const VkPipelineLayout& GetPipelineLayout() const override
	{
		return m_pipelineLayout;
	}

	const VkPipelineBindPoint& GetPipelineBindPoint() const override
	{
		return m_pipelineBindPoint;
	}

  private:
	void CreateShaderProgram();

	void CreateDescriptorLayout();

	void CreateDescriptorPool();

	void CreatePipelineLayout();

	void CreatePipelineCompute();

	std::string m_shaderStage;
	std::vector<Shader::Define> m_defines;
	bool m_pushDescriptors;

	std::unique_ptr<Shader> m_shader;

	VkShaderModule m_shaderModule;
	VkPipelineShaderStageCreateInfo m_shaderStageCreateInfo;

	VkDescriptorSetLayout m_descriptorSetLayout;
	VkDescriptorPool m_descriptorPool;

	VkPipeline m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	VkPipelineBindPoint m_pipelineBindPoint;
};
}
