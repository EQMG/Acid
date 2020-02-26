#include "MainRenderer.hpp"
#include <Graphics/Graphics.hpp>
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace test
{
	/*
	 * Create something to render
	 * It will have a pipeline graphics and shaders
	 * The vertices are hardcoded into the shader for the first test
	 * We disable Depth tests
	 */
	class Tutorial1 : public Subrender
	{
	protected:
		/*
		 * The ony thing this Triangle has is a pipeline, which is created in the Constructor
		 */
		PipelineGraphics m_pipeline;

	public:
		explicit Tutorial1(const Pipeline::Stage &stage)
			: Subrender(stage),
			  m_pipeline(stage,
				  {"Tutorial/Shaders/tri1.vert", "Tutorial/Shaders/tri1.frag"},
				  {}, {},
				  PipelineGraphics::Mode::Polygon,
				  PipelineGraphics::Depth::None)
		{
		}

		/*
		 * To render this, we just Bind the pipeline and draw.  Acid will call this at the right time.
		 * The vertices are in the shader, we just need to tell Vulkan that there are 3 of them
		 * and 1 instance
		 */
		virtual void Render(const CommandBuffer &commandBuffer) override
		{
			m_pipeline.BindPipeline(commandBuffer);
			vkCmdDraw(commandBuffer, 3, 1, 0, 0);
		};
	};

	/*
	 * Application main renderer
	 * Create a single SubPass and define the only attachment to be the swapchain attachment
	 * The format doesnt matter since it is always matched to the Framebuffer/Window
	 * We set the clear color to Aqua and disable MSAA for now.
	 */
	MainRenderer::MainRenderer()
	{
		/*
		 * Define the attachments for the Pass
		 */
		std::vector<Attachment> renderpassAttachments1{
			{0, "swapchain", Attachment::Type::Swapchain, false, VK_FORMAT_UNDEFINED, Colour::Aqua},
		};


		/*
		 * Add the references to the attachments for the SubPass
		 * In our case, SubPass 1 will be using Attachment 1
		 */
		std::vector<SubpassType> renderpassSubpasses1 = {
			{0, {0}}
		};
		AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
	}

	void MainRenderer::Start()
	{
		/*
		 * Ok set our triangle to be rendered in Pass 1, SubPass 1 - Acid will ask us to render it
		 */
		AddSubrender<Tutorial1>({0, 0});
	}

	/*
	 * No updates need for this
	 */
	void MainRenderer::Update()
	{
	}
}
