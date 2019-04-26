#pragma once

#include "Maths/Vector2.hpp"
#include "Serialized/Metadata.hpp"
#include "Pipeline.hpp"
#include "Renderer/RenderStage.hpp"

namespace acid
{
class ImageDepth;
class Image2d;

/**
 * @brief Class that represents a graphics pipeline.
 */
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
		None = 0, Read = 1, Write = 2, ReadWrite = Read | Write
	};

	/**
	 * Creates a new pipeline.
	 * @param stage The graphics stage this pipeline will be run on.
	 * @param shaderStages The source files to load the pipeline shaders from.
	 * @param vertexInputs The vertex inputs that will be used as a shaders input.
	 * @param defines A list of defines added to the top of each shader.
	 * @param mode The mode this pipeline will run in.
	 * @param depthMode The depth read/write that will be used.
	 * @param topology The topology of the input assembly.
	 * @param polygonMode The polygon draw mode.
	 * @param cullMode The vertex cull mode.
	 * @param frontFace The direction to render faces.
	 * @param pushDescriptors If no actual descriptor sets are allocated but instead pushed.
	 */
	PipelineGraphics(Stage stage, std::vector<std::string> shaderStages, std::vector<Shader::VertexInput> vertexInputs, std::vector<Shader::Define> defines = {},
		const Mode &mode = Mode::Polygon, const Depth &depthMode = Depth::ReadWrite, const VkPrimitiveTopology &topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL, const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const VkFrontFace &frontFace = VK_FRONT_FACE_CLOCKWISE,
		const bool &pushDescriptors = false);

	~PipelineGraphics();

	/**
	 * Gets the depth stencil used in a stage.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The depth stencil that is found.
	 */
	const ImageDepth *GetDepthStencil(const std::optional<uint32_t> &stage = {}) const;

	/**
	 * Gets a image used in a stage by the index given to it in the renderpass.
	 * @param index The renderpass Image index.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The image that is found.
	 */
	const Image2d *GetImage(const uint32_t &index, const std::optional<uint32_t> &stage = {}) const;

	/**
	 * Gets the render stage viewport.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The the render stage viewport.
	 */
	RenderArea GetRenderArea(const std::optional<uint32_t> &stage = {}) const;

	const Stage &GetStage() const { return m_stage; }

	const std::vector<std::string> &GetShaderStages() const { return m_shaderStages; }

	const std::vector<Shader::VertexInput> &GetVertexInputs() const { return m_vertexInputs; }

	const std::vector<Shader::Define> &GetDefines() const { return m_defines; }

	const Mode &GetMode() const { return m_mode; }

	const Depth &GetDepth() const { return m_depth; }

	const VkPrimitiveTopology &GetTopology() const { return m_topology; }

	const VkPolygonMode &GetPolygonMode() const { return m_polygonMode; }

	const VkCullModeFlags &GetCullMode() const { return m_cullMode; }

	const VkFrontFace &GetFrontFace() const { return m_frontFace; }

	const bool &IsPushDescriptors() const override { return m_pushDescriptors; }

	const Shader *GetShader() const override { return m_shader.get(); }

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

	Stage m_stage;
	std::vector<std::string> m_shaderStages;
	std::vector<Shader::VertexInput> m_vertexInputs;
	std::vector<Shader::Define> m_defines;
	Mode m_mode;
	Depth m_depth;
	VkPrimitiveTopology m_topology;
	VkPolygonMode m_polygonMode;
	VkCullModeFlags m_cullMode;
	VkFrontFace m_frontFace;
	bool m_pushDescriptors;

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
	explicit PipelineGraphicsCreate(std::vector<std::string> shaderStages = {}, std::vector<Shader::VertexInput> vertexInputs = {}, std::vector<Shader::Define> defines = {},
		const PipelineGraphics::Mode &mode = PipelineGraphics::Mode::Polygon, const PipelineGraphics::Depth &depth = PipelineGraphics::Depth::ReadWrite,
		const VkPrimitiveTopology &topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, const VkPolygonMode &polygonMode = VK_POLYGON_MODE_FILL,
		const VkCullModeFlags &cullMode = VK_CULL_MODE_BACK_BIT, const VkFrontFace &frontFace = VK_FRONT_FACE_CLOCKWISE, const bool &pushDescriptors = false) :
		m_shaderStages(std::move(shaderStages)),
		m_vertexInputs(std::move(vertexInputs)),
		m_defines(std::move(defines)),
		m_mode(mode),
		m_depth(depth),
		m_topology(topology),
		m_polygonMode(polygonMode),
		m_cullMode(cullMode),
		m_frontFace(frontFace),
		m_pushDescriptors(pushDescriptors)
	{
	}

	/**
	 * Creates a new pipeline.
	 * @param pipelineStage The pipelines graphics stage.
	 * @return The created graphics pipeline.
	 */
	PipelineGraphics *Create(const Pipeline::Stage &pipelineStage) const
	{
		return new PipelineGraphics(pipelineStage, m_shaderStages, m_vertexInputs, m_defines, m_mode, m_depth, m_topology, m_polygonMode, m_cullMode, m_frontFace,
			m_pushDescriptors);
	}

	friend const Metadata& operator>>(const Metadata& metadata, PipelineGraphicsCreate& pipelineCreate)
	{
		metadata.GetChild("Shader Stages", pipelineCreate.m_shaderStages);
		//metadata.GetChild("Vertex Inputs", pipelineCreate.m_vertexInputs);
		metadata.GetChild("Defines", pipelineCreate.m_defines);
		metadata.GetChild("Mode", pipelineCreate.m_mode);
		metadata.GetChild("Depth", pipelineCreate.m_depth);
		metadata.GetChild("Polygon Mode", pipelineCreate.m_polygonMode);
		metadata.GetChild("Cull Mode", pipelineCreate.m_cullMode);
		metadata.GetChild("Front Face", pipelineCreate.m_frontFace);
		metadata.GetChild("Push Descriptors", pipelineCreate.m_pushDescriptors);
		return metadata;

	}

	friend Metadata& operator<<(Metadata& metadata, const PipelineGraphicsCreate& pipelineCreate)
	{
		metadata.SetChild("Shader Stages", pipelineCreate.m_shaderStages);
		//metadata.SetChild("Vertex Inputs", pipelineCreate.m_vertexInputs);
		metadata.SetChild("Defines", pipelineCreate.m_defines);
		metadata.SetChild("Mode", pipelineCreate.m_mode);
		metadata.SetChild("Depth", pipelineCreate.m_depth);
		metadata.SetChild("Polygon Mode", pipelineCreate.m_polygonMode);
		metadata.SetChild("Cull Mode", pipelineCreate.m_cullMode);
		metadata.SetChild("Front Face", pipelineCreate.m_frontFace);
		metadata.SetChild("Push Descriptors", pipelineCreate.m_pushDescriptors);
		return metadata;

	}

	const std::vector<std::string> &GetShaderStages() const { return m_shaderStages; }

	const std::vector<Shader::VertexInput> &GetVertexInputs() const { return m_vertexInputs; }

	const std::vector<Shader::Define> &GetDefines() const { return m_defines; }

	const PipelineGraphics::Mode &GetMode() const { return m_mode; }

	const PipelineGraphics::Depth &GetDepth() const { return m_depth; }

	const VkPrimitiveTopology &GetTopology() const { return m_topology; }

	const VkPolygonMode &GetPolygonMode() const { return m_polygonMode; }

	const VkCullModeFlags &GetCullMode() const { return m_cullMode; }

	const VkFrontFace &GetFrontFace() const { return m_frontFace; }

	const bool &GetPushDescriptors() const { return m_pushDescriptors; }

private:
	std::vector<std::string> m_shaderStages;
	std::vector<Shader::VertexInput> m_vertexInputs;
	std::vector<Shader::Define> m_defines;

	PipelineGraphics::Mode m_mode;
	PipelineGraphics::Depth m_depth;
	VkPrimitiveTopology m_topology;
	VkPolygonMode m_polygonMode;
	VkCullModeFlags m_cullMode;
	VkFrontFace m_frontFace;
	bool m_pushDescriptors;
};
}
