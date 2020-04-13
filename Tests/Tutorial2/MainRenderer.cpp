#include "MainRenderer.hpp"
#include <Graphics/Graphics.hpp>
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Models/Model.hpp"

namespace test {
class TutorialVertex2 {
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
			{baseBinding, sizeof(TutorialVertex2), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(TutorialVertex2, position)},
			{1, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(TutorialVertex2, color)}
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
class TutorialSubrender2 : public Subrender {
public:
	explicit TutorialSubrender2(const Pipeline::Stage &stage) :
		Subrender(stage),
		pipeline(stage, {"Tutorial/Shaders/tri2.vert", "Tutorial/Shaders/tri2.frag"},
			{TutorialVertex2::GetVertexInput()}, {}, PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None) {
	}

	/*
	 * To render this, we just Bind the pipeline and draw.  Acid will call this at the right time.
	 */
	void Render(const CommandBuffer &commandBuffer) override {
		// Set the pipeline.
		pipeline.BindPipeline(commandBuffer);

		// Draw the model.
		model->CmdRender(commandBuffer);
	}

	void SetModel(std::unique_ptr<Model> &&model) { this->model = std::move(model); }

private:
	PipelineGraphics pipeline;
	std::unique_ptr<Model> model;
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
	// Lets create a set of vertices and matching colors
	const std::vector<TutorialVertex2> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	// Create a model from that set of Vertices, we are not doing index drawing yet, so just vertices
	auto model = std::make_unique<Model>(vertices);

	// Ok set our triangle to be rendered in Pass 1, SubPass 1 - Acid will ask us to draw it
	auto tri = AddSubrender<TutorialSubrender2>({0, 0});

	// Set the model.
	tri->SetModel(std::move(model));
}

void MainRenderer::Update() {
}
}
