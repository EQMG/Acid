#pragma once

#include "Graphics/Commands/CommandBuffer.hpp"
#include "Maths/Vector2.hpp"
#include "Pipeline.hpp"

namespace acid {
/**
 * @brief Class that represents a compute compute pipeline.
 */
class ACID_EXPORT PipelineCompute : public Pipeline {
public:
	/**
	 * Creates a new compute pipeline.
	 * @param shaderStage The shader file that will be loaded.
	 * @param defines A list of defines added to the top of each shader.
	 * @param pushDescriptors If no actual descriptor sets are allocated but instead pushed.
	 */
	explicit PipelineCompute(std::filesystem::path shaderStage, std::vector<Shader::Define> defines = {}, bool pushDescriptors = false);
	~PipelineCompute();

	void CmdRender(const CommandBuffer &commandBuffer, const Vector2ui &extent) const;

	const std::filesystem::path &GetShaderStage() const { return shaderStage; }
	const std::vector<Shader::Define> &GetDefines() const { return defines; }
	bool IsPushDescriptors() const override { return pushDescriptors; }
	const Shader *GetShader() const override { return shader.get(); }
	const VkDescriptorSetLayout &GetDescriptorSetLayout() const override { return descriptorSetLayout; }
	const VkDescriptorPool &GetDescriptorPool() const override { return descriptorPool; }
	const VkPipeline &GetPipeline() const override { return pipeline; }
	const VkPipelineLayout &GetPipelineLayout() const override { return pipelineLayout; }
	const VkPipelineBindPoint &GetPipelineBindPoint() const override { return pipelineBindPoint; }

private:
	void CreateShaderProgram();
	void CreateDescriptorLayout();
	void CreateDescriptorPool();
	void CreatePipelineLayout();
	void CreatePipelineCompute();

	std::filesystem::path shaderStage;
	std::vector<Shader::Define> defines;
	bool pushDescriptors;

	std::unique_ptr<Shader> shader;

	VkShaderModule shaderModule = VK_NULL_HANDLE;
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};

	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

	VkPipeline pipeline = VK_NULL_HANDLE;
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipelineBindPoint pipelineBindPoint;
};
}
