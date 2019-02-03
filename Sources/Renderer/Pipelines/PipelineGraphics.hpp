#pragma once

#include <set>
#include <array>
#include <string>
#include <optional>
#include <vector>
#include <type_traits>
#include "Maths/Vector2.hpp"
#include "Serialized/Metadata.hpp"
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

		/// <summary>
		/// Gets the depth stencil used in a stage.
		/// </summary>
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>
		/// <returns> The depth stencil that is found. </returns>
		const DepthStencil *GetDepthStencil(const std::optional<uint32_t> &stage = {}) const;

		/// <summary>
		/// Gets a texture used in a stage by the index given to it in the renderpass.
		/// </summary>
		/// <param name="index"> The renderpass texture index. </param>
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>
		/// <returns> The texture that is found. </returns>
		const Texture *GetTexture(const uint32_t &index, const std::optional<uint32_t> &stage = {}) const;

		/// <summary>
		/// Gets the height of the render stage in pixels.
		/// </summary>
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>
		/// <returns> The height of the render stage. </returns>
		uint32_t GetWidth(const std::optional<uint32_t> &stage = {}) const;

		/// <summary>
		/// Gets the height of the render stage in pixels.
		/// </summary>
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>
		/// <returns> The height of the render stage. </returns>
		uint32_t GetHeight(const std::optional<uint32_t> &stage = {}) const;

		/// <summary>
		/// Gets the dimensions of the render stage in pixels.
		/// </summary>
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>
		/// <returns> The dimensions of the render stage. </returns>
		Vector2 GetDimensions(const std::optional<uint32_t> &stage = {}) const;

		/// <summary>
		/// Gets the aspect ratio between the render stages wid
		/// <param name="stage"> The stage to get values from, if not provided the pipelines stage will be used. </param>th and height.
		/// </summary>
		/// <returns> The aspect ratio. </returns>
		float GetAspectRatio(const std::optional<uint32_t> &stage = {}) const;

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
		explicit PipelineGraphicsCreate(const std::vector<std::string> &shaderStages = {}, const std::vector<Shader::VertexInput> &vertexInputs = {}, const PipelineGraphics::Mode &mode = PipelineGraphics::Mode::Polygon,
			const PipelineGraphics::Depth &depth = PipelineGraphics::Depth::ReadWrite, const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT,
			const bool &pushDescriptors = false, const std::vector<Shader::Define> &defines = {}) :
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

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("Shader Stages", m_shaderStages);
		//	metadata.GetChild("Vertex Inputs", m_vertexInputs);
			metadata.GetChild("Mode", m_mode);
			metadata.GetChild("Depth", m_depth);
			metadata.GetChild("Polygon Mode", m_polygonMode);
			metadata.GetChild("Cull Mode", m_cullMode);
			metadata.GetChild("Push Descriptors", m_pushDescriptors);
			metadata.GetChild("Defines", m_defines);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("Shader Stages", m_shaderStages);
		//	metadata.SetChild("Vertex Inputs", m_vertexInputs);
			metadata.SetChild("Mode", m_mode);
			metadata.SetChild("Depth", m_depth);
			metadata.SetChild("Polygon Mode", m_polygonMode);
			metadata.SetChild("Cull Mode", m_cullMode);
			metadata.SetChild("Push Descriptors", m_pushDescriptors);
			metadata.SetChild("Defines", m_defines);
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
