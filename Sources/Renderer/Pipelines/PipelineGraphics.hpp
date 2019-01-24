#pragma once

#include <array>
#include <string>
#include <vector>
#include "Pipeline.hpp"

namespace acid
{
	class DepthStencil;
	class Texture;

	/// <summary>
	/// Class that represents a Vulkan graphics pipeline.
	/// </summary>
	class ACID_EXPORT PipelineGraphics :
		public Pipeline
	{
	public:
		enum class Mode
		{
			Polygon, Mrt
		};

		enum class Depth
		{
			None = 0,
			Read = 1,
			Write = 2,
			ReadWrite = Read | Write
		};

		/// <summary>
		/// Creates a new pipeline.
		/// </summary>
		/// <param name="stage"> The graphics stage this pipeline will be run on. </param>
		/// <param name="shaderStages"> The source files to load the pipeline shaders from. </param>
		/// <param name="vertexInputs"> The vertex inputs that will be used as a shaders input. </param>
		/// <param name="pipelineMode"> The mode this pipeline will run in. </param>
		/// <param name="depthMode"> The depth read/write that will be used. </param>
		/// <param name="polygonMode"> The polygon draw mode. </param>
		/// <param name="cullMode"> The vertex cull mode. </param>
		/// <param name="pushDescriptors"> If no actual descriptor sets are allocated but instead pushed. </param>
		/// <param name="defines"> A list of defines added to the top of each shader. </param>
		PipelineGraphics(const Pipeline::Stage &stage, const std::vector<std::string> &shaderStages, const std::vector<Shader::VertexInput> &vertexInputs, const Mode &mode = Mode::Polygon, const Depth &depthMode = Depth::ReadWrite,
			const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const bool &pushDescriptors = false, const std::vector<Shader::Define> &defines = {});

		~PipelineGraphics();

		const DepthStencil *GetDepthStencil(const int32_t &stage = -1) const;

		const Texture *GetTexture(const uint32_t &index, const int32_t &stage = -1) const;

		uint32_t GetWidth(const int32_t &stage = -1) const;

		uint32_t GetHeight(const int32_t &stage = -1) const;

		const Pipeline::Stage &GetStage() const { return m_stage; }

		const std::vector<std::string> &GetShaderStages() const { return m_shaderStages; }

		const std::vector<Shader::VertexInput> &GetVertexInputs() const { return m_vertexInputs; }

		const Mode &GetMode() const { return m_mode; }

		const Depth &GetDepth() const { return m_depth; }

		const VkPolygonMode &GetPolygonMode() const { return m_polygonMode; }

		const VkCullModeFlags &GetCullMode() const { return m_cullMode; }

		const bool &IsPushDescriptors() const override { return m_pushDescriptors; }

		const std::vector<Shader::Define> &GetDefines() const { return m_defines; }

		const Shader *GetShaderProgram() const override { return m_shader.get(); }

		const VkDescriptorSetLayout &GetDescriptorSetLayout() const override { return m_descriptorSetLayout; }

		const VkDescriptorPool &GetDescriptorPool() const override { return m_descriptorPool; }

		const VkPipeline &GetPipeline() const override { return m_pipeline; }

		const VkPipelineLayout &GetPipelineLayout() const override { return m_pipelineLayout; }

		const VkPipelineBindPoint &GetPipelineBindPoint() const override { return m_pipelineBindPoint; }
	private:
		void CreateShaderProgram();

		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreatePipelineLayout();

		void CreateAttributes();

		void CreatePipeline();

		void CreatePipelinePolygon();

		void CreatePipelineMrt();

		Pipeline::Stage m_stage;
		std::vector<std::string> m_shaderStages;
		std::vector<Shader::VertexInput> m_vertexInputs;
		Mode m_mode;
		Depth m_depth;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;
		bool m_pushDescriptors;
		std::vector<Shader::Define> m_defines;

		std::unique_ptr<Shader> m_shader;

		std::vector<VkDynamicState> m_dynamicStates;

		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		VkPipelineBindPoint m_pipelineBindPoint;

		VkPipelineVertexInputStateCreateInfo m_vertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		std::array<VkPipelineColorBlendAttachmentState, 1> m_blendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo m_colourBlendState;
		VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineMultisampleStateCreateInfo m_multisampleState;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
		VkPipelineTessellationStateCreateInfo m_tessellationState;
	};

	class ACID_EXPORT PipelineGraphicsCreate
	{
	public:
		PipelineGraphicsCreate(const std::vector<std::string> &shaderStages, const std::vector<Shader::VertexInput> &vertexInputs, const PipelineGraphics::Mode &mode = PipelineGraphics::Mode::Polygon, const PipelineGraphics::Depth &depth = PipelineGraphics::Depth::ReadWrite,
		    const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const bool &pushDescriptors = false, const std::vector<Shader::Define> &defines = {}) :
			m_shaderStages(shaderStages),
			m_vertexInputs(vertexInputs),
			m_mode(mode),
			m_depth(depth),
			m_polygonMode(polygonMode),
			m_cullMode(cullMode),
			m_pushDescriptors(pushDescriptors),
			m_defines(defines)
		{
		}

		/// <summary>
		/// Creates a new pipeline.
		/// </summary>
		/// <param name="pipelineStage"> The pipelines graphics stage. </param>
		PipelineGraphics *Create(const Pipeline::Stage &pipelineStage)
		{
			return new PipelineGraphics(pipelineStage, m_shaderStages, m_vertexInputs, m_mode, m_depth,
				m_polygonMode, m_cullMode, m_pushDescriptors, m_defines);
		}

		const std::vector<std::string> &GetShaderStages() const { return m_shaderStages; }

		const std::vector<Shader::VertexInput> &GetVertexInputs() const { return m_vertexInputs; }

		const PipelineGraphics::Mode &GetMode() const { return m_mode; }

		const PipelineGraphics::Depth &GetDepth() const { return m_depth; }

		const VkPolygonMode &GetPolygonMode() const { return m_polygonMode; }

		const VkCullModeFlags &GetCullMode() const { return m_cullMode; }

		const bool &GetPushDescriptors() const { return m_pushDescriptors; }

		const std::vector<Shader::Define> &GetDefines() const { return m_defines; }
	private:
		std::vector<std::string> m_shaderStages;
		std::vector<Shader::VertexInput> m_vertexInputs;

		PipelineGraphics::Mode m_mode;
		PipelineGraphics::Depth m_depth;
		VkPolygonMode m_polygonMode;
		VkCullModeFlags m_cullMode;
		bool m_pushDescriptors;

		std::vector<Shader::Define> m_defines;
	};
}
