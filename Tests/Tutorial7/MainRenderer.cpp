#include "MainRenderer.hpp"

#include <Graphics/Graphics.hpp>
#include <utility>

#include "Devices/Window.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Materials/Material.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Models/Obj/ObjModel.hpp"
#include "Meshes/MeshesSubrender.hpp"

namespace test {
/*
 * Application main renderer
 * Now our Renderer is just depth and swapchain
 */
MainRenderer::MainRenderer() {
	/*
	 * Define the attachments for the Pass
	 * We are adding a depth buffer to the list of attachments
	 */
	std::vector<Attachment> renderpassAttachments1{
		{0, "depth", Attachment::Type::Depth, false},
		{1, "swapchain", Attachment::Type::Swapchain, false, VK_FORMAT_UNDEFINED, Colour::Aqua},
	};

	/*
	 * Add the references to the attachments for the SubPass
	 * In our case, SubPass 1 will be using Attachment 1
	 * We have added the depth buffer to the list of attachments that the SubPass will
	 * since our pipeline uses it now
	 */
	std::vector<SubpassType> renderpassSubpasses1 = {{0, {0, 1}}};
	AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
}

void MainRenderer::Start() {
	/*
	 * We will be using the ECS system, this will render any Mesh Components in Pass1 Subpass 1
	 */
	AddSubrender<MeshesSubrender>({0, 0});
}

/*
 * No updates need for this
 */
void MainRenderer::Update() {}
}
