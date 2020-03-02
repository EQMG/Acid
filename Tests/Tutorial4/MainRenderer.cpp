#include "MainRenderer.hpp"

#include <Graphics/Graphics.hpp>
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Maths/Matrix4.hpp"
#include "Devices/Window.hpp"

namespace test {
class TutorialVertex4 {
public:
	/*
	 * We need to describe how these are input to the shader
	 *
	 * from the shader:
	 *	layout(location = 0) in vec2 inPosition;
	 *	layout(location = 1) in vec3 inColor;
	 *
	 * So with the 2 data types interleaved we will create 1 binding with both types and map them to
	 * the correct locations, describing size/offset and total stride
	 *
	 */
	static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			{baseBinding, sizeof(TutorialVertex4), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(TutorialVertex4, position)},
			{1, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(TutorialVertex4, color)}
		};
		return {bindingDescriptions, attributeDescriptions};
	}

	Vector2f position;
	Vector3f color;
};

/*
 * Create something to render
 * It will have a pipeline graphics and shaders
 * We disable Depth tests
 */
class TutorialSubrender4 : public Subrender {
public:
	/*
	 * To follow the Vulkan tutorial, we have flipped the Y in the projection, and here
	 * we are also reversing the triangle
	 * cull mode as per https://vulkan-tutorial.com/Uniform_buffers/Descriptor_layout_and_buffer
	 */
	explicit TutorialSubrender4(const Pipeline::Stage &stage) :
		Subrender(stage),
		pipeline(stage,
			{"Tutorial/Shaders/tri4.vert", "Tutorial/Shaders/tri4.frag"},
			{TutorialVertex4::GetVertexInput()}, {}, PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE),
		descriptorSet(pipeline) {
	}

	/*
	 * To render this, we just Bind the pipeline and draw.  Acid will call this at the right time.
	 */
	void Render(const CommandBuffer &commandBuffer) override {
		// Set the pipeline
		pipeline.BindPipeline(commandBuffer);

		// Now to calculate the transform of the model
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		ubo.model = Matrix4().Rotate(time * Maths::Radians(90.0f), Vector3f(0.0f, 0.0f, 1.0f));

		// These have to match the order in the shader.
		uniformScene.Push("model", ubo.model);
		uniformScene.Push("view", ubo.view);
		uniformScene.Push("proj", ubo.proj);

		// This has to match the uniform setting in the shader
		descriptorSet.Push("UniformBufferObject", uniformScene);

		// Set it to the pipeline
		descriptorSet.Update(pipeline);
		descriptorSet.BindDescriptor(commandBuffer, pipeline);

		// Draw the model, this time it knows it has a indexbuffer and does the indexed draw
		// It will also pass the UBO to the shader
		model->CmdRender(commandBuffer);
	}

	void SetModel(std::unique_ptr<Model> &&model) {
		this->model = std::move(model);

		startTime = std::chrono::high_resolution_clock::now();

		// Lets set up the camera viewpoint
		// for now the models transform will be the Unit transform
		ubo.view = Matrix4::LookAt(Vector3f(2.0f, 2.0f, 2.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
		ubo.proj = Matrix4::PerspectiveMatrix(Maths::Radians(45.0f), Window::Get()->GetAspectRatio(), 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;
		ubo.model = Matrix4();
	}
private:
	PipelineGraphics pipeline;
	std::unique_ptr<Model> model;
	UniformHandler uniformScene;

	struct UniformBufferObject {
		Matrix4 model;
		Matrix4 view;
		Matrix4 proj;
	} ubo;

	std::chrono::high_resolution_clock::time_point startTime;
	DescriptorsHandler descriptorSet;
};

/*
 * Application main renderer
 * Create a single SubPass and define the only attachment to be the swapchain attachment
 * The format doesnt matter since it is always matched to the Framebuffer/Window
 * We set the clear color to Aqua and disable MSAA for now.
 */
MainRenderer::MainRenderer() {
	// Define the attachments for the Pass
	std::vector<Attachment> renderpassAttachments1{
		{0, "swapchain", Attachment::Type::Swapchain, false, VK_FORMAT_UNDEFINED, Colour::Aqua},
	};

	// Add the references to the attachments for the SubPass
	// In our case, SubPass 1 will be using Attachment 1
	std::vector<SubpassType> renderpassSubpasses1 = {{0, {0}}};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
}

void MainRenderer::Start() {
	// Lets create a set of vertices and matching colors, a square this time.
	const std::vector<TutorialVertex4> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	// Create a set of indices.
	const std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	// Create a model from that set of Vertices and indices.
	auto model = std::make_unique<Model>(vertices, indices);

	// Ok set our square to be rendered in Pass 1, SubPass 1 - Acid will ask us to draw it
	// Also provide how the buffers are mapped to the shader.
	auto square = AddSubrender<TutorialSubrender4>({0, 0});

	// Set the model
	square->SetModel(std::move(model));
}

void MainRenderer::Update() {
}
}
