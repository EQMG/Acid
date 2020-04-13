#pragma once

#include <array>

#include "Files/Node.hpp"
#include "Pipeline.hpp"
#include "Graphics/RenderStage.hpp"

namespace acid {
class ImageDepth;
class Image2d;

/**
 * @brief Class that represents a graphics pipeline.
 */
class ACID_EXPORT PipelineGraphics : public Pipeline {
public:
	enum class Mode {
		Polygon, MRT
	};

	enum class Depth {
		None = 0,
		Read = 1,
		Write = 2,
		ReadWrite = Read | Write
	};

	/**
	 * Creates a new pipeline.
	 * @param stage The graphics stage this pipeline will be run on.
	 * @param shaderStages The source files to load the pipeline shaders from.
	 * @param vertexInputs The vertex inputs that will be used as a shaders input.
	 * @param defines A list of defines added to the top of each shader.
	 * @param mode The mode this pipeline will run in.
	 * @param depth The depth read/write that will be used.
	 * @param topology The topology of the input assembly.
	 * @param polygonMode The polygon draw mode.
	 * @param cullMode The vertex cull mode.
	 * @param frontFace The direction to render faces.
	 * @param pushDescriptors If no actual descriptor sets are allocated but instead pushed.
	 */
	PipelineGraphics(Stage stage, std::vector<std::filesystem::path> shaderStages, std::vector<Shader::VertexInput> vertexInputs, std::vector<Shader::Define> defines = {},
		Mode mode = Mode::Polygon, Depth depth = Depth::ReadWrite, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, 
		VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT, VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE, bool pushDescriptors = false);
	~PipelineGraphics();

	/**
	 * Gets the depth stencil used in a stage.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The depth stencil that is found.
	 */
	const ImageDepth *GetDepthStencil(const std::optional<uint32_t> &stage = std::nullopt) const;

	/**
	 * Gets a image used in a stage by the index given to it in the renderpass.
	 * @param index The renderpass Image index.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The image that is found.
	 */
	const Image2d *GetImage(uint32_t index, const std::optional<uint32_t> &stage = std::nullopt) const;

	/**
	 * Gets the render stage viewport.
	 * @param stage The stage to get values from, if not provided the pipelines stage will be used.
	 * @return The the render stage viewport.
	 */
	RenderArea GetRenderArea(const std::optional<uint32_t> &stage = std::nullopt) const;

	const Stage &GetStage() const { return stage; }
	const std::vector<std::filesystem::path> &GetShaderStages() const { return shaderStages; }
	const std::vector<Shader::VertexInput> &GetVertexInputs() const { return vertexInputs; }
	const std::vector<Shader::Define> &GetDefines() const { return defines; }
	Mode GetMode() const { return mode; }
	Depth GetDepth() const { return depth; }
	VkPrimitiveTopology GetTopology() const { return topology; }
	VkPolygonMode GetPolygonMode() const { return polygonMode; }
	VkCullModeFlags GetCullMode() const { return cullMode; }
	VkFrontFace GetFrontFace() const { return frontFace; }
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
	void CreateAttributes();
	void CreatePipeline();
	void CreatePipelinePolygon();
	void CreatePipelineMrt();

	Stage stage;
	std::vector<std::filesystem::path> shaderStages;
	std::vector<Shader::VertexInput> vertexInputs;
	std::vector<Shader::Define> defines;
	Mode mode;
	Depth depth;
	VkPrimitiveTopology topology;
	VkPolygonMode polygonMode;
	VkCullModeFlags cullMode;
	VkFrontFace frontFace;
	bool pushDescriptors;

	std::unique_ptr<Shader> shader;

	std::vector<VkDynamicState> dynamicStates;

	std::vector<VkShaderModule> modules;
	std::vector<VkPipelineShaderStageCreateInfo> stages;

	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

	VkPipeline pipeline = VK_NULL_HANDLE;
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipelineBindPoint pipelineBindPoint;

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	VkPipelineRasterizationStateCreateInfo rasterizationState = {};
	std::array<VkPipelineColorBlendAttachmentState, 1> blendAttachmentStates;
	VkPipelineColorBlendStateCreateInfo colourBlendState = {};
	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkPipelineMultisampleStateCreateInfo multisampleState = {};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	VkPipelineTessellationStateCreateInfo tessellationState = {};
};

class ACID_EXPORT PipelineGraphicsCreate {
public:
	PipelineGraphicsCreate(std::vector<std::filesystem::path> shaderStages = {}, std::vector<Shader::VertexInput> vertexInputs = {}, std::vector<Shader::Define> defines = {},
		PipelineGraphics::Mode mode = PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth depth = PipelineGraphics::Depth::ReadWrite,
		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL,
		VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT, VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE, bool pushDescriptors = false) :
		shaderStages(std::move(shaderStages)),
		vertexInputs(std::move(vertexInputs)),
		defines(std::move(defines)),
		mode(mode),
		depth(depth),
		topology(topology),
		polygonMode(polygonMode),
		cullMode(cullMode),
		frontFace(frontFace),
		pushDescriptors(pushDescriptors) {
	}

	/**
	 * Creates a new pipeline.
	 * @param pipelineStage The pipelines graphics stage.
	 * @return The created graphics pipeline.
	 */
	PipelineGraphics *Create(const Pipeline::Stage &pipelineStage) const {
		return new PipelineGraphics(pipelineStage, shaderStages, vertexInputs, defines, mode, depth, topology, polygonMode, cullMode, frontFace,
			pushDescriptors);
	}

	friend const Node &operator>>(const Node &node, PipelineGraphicsCreate &pipelineCreate) {
		node["shaderStages"].Get(pipelineCreate.shaderStages);
		node["vertexInputs"].Get(pipelineCreate.vertexInputs);
		node["defines"].Get(pipelineCreate.defines);
		node["mode"].Get(pipelineCreate.mode);
		node["depth"].Get(pipelineCreate.depth);
		node["polygonMode"].Get(pipelineCreate.polygonMode);
		node["cullMode"].Get(pipelineCreate.cullMode);
		node["frontFace"].Get(pipelineCreate.frontFace);
		node["pushDescriptors"].Get(pipelineCreate.pushDescriptors);
		return node;
	}

	friend Node &operator<<(Node &node, const PipelineGraphicsCreate &pipelineCreate) {
		node["shaderStages"].Set(pipelineCreate.shaderStages);
		node["vertexInputs"].Set(pipelineCreate.vertexInputs);
		node["defines"].Set(pipelineCreate.defines);
		node["mode"].Set(pipelineCreate.mode);
		node["depth"].Set(pipelineCreate.depth);
		node["polygonMode"].Set(pipelineCreate.polygonMode);
		node["cullMode"].Set(pipelineCreate.cullMode);
		node["frontFace"].Set(pipelineCreate.frontFace);
		node["pushDescriptors"].Set(pipelineCreate.pushDescriptors);
		return node;
	}

	const std::vector<std::filesystem::path> &GetShaderStages() const { return shaderStages; }
	const std::vector<Shader::VertexInput> &GetVertexInputs() const { return vertexInputs; }
	const std::vector<Shader::Define> &GetDefines() const { return defines; }
	PipelineGraphics::Mode GetMode() const { return mode; }
	PipelineGraphics::Depth GetDepth() const { return depth; }
	VkPrimitiveTopology GetTopology() const { return topology; }
	VkPolygonMode GetPolygonMode() const { return polygonMode; }
	VkCullModeFlags GetCullMode() const { return cullMode; }
	VkFrontFace GetFrontFace() const { return frontFace; }
	bool GetPushDescriptors() const { return pushDescriptors; }

private:
	std::vector<std::filesystem::path> shaderStages;
	std::vector<Shader::VertexInput> vertexInputs;
	std::vector<Shader::Define> defines;

	PipelineGraphics::Mode mode;
	PipelineGraphics::Depth depth;
	VkPrimitiveTopology topology;
	VkPolygonMode polygonMode;
	VkCullModeFlags cullMode;
	VkFrontFace frontFace;
	bool pushDescriptors;
};
}
