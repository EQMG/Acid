#include "Gui.hpp"

#include "Graphics/Graphics.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Models/Vertex2d.hpp"

namespace acid {
static const std::vector<Vertex2d> VERTICES = {
	{{0.0f, 0.0f}, {0.0f, 0.0f}},
	{{1.0f, 0.0f}, {1.0f, 0.0f}},
	{{1.0f, 1.0f}, {1.0f, 1.0f}},
	{{0.0f, 1.0f}, {0.0f, 1.0f}}
};
static const std::vector<uint32_t> INDICES = {
	0, 1, 2,
	2, 3, 0
};

Gui::Gui() :
	model(std::make_unique<Model>(VERTICES, INDICES)),
	colourDriver(std::make_unique<ConstantDriver<Colour>>(Colour::White)) {
}

void Gui::UpdateObject() {
	auto numberOfRows = image ? this->numberOfRows : 1;
	auto column = selectedRow % numberOfRows;
	auto row = selectedRow / numberOfRows;
	atlasOffset = Vector2f(static_cast<float>(column), static_cast<float>(row)) / static_cast<float>(numberOfRows);

	colourDriver->Update(Engine::Get()->GetDelta());

	// Updates uniforms.
	uniformObject.Push("modelView", GetModelView());
	uniformObject.Push("alpha", GetScreenAlpha());

	uniformObject.Push("aspectRatio", static_cast<float>(GetScreenSize().x) / static_cast<float>(GetScreenSize().y));

	uniformObject.Push("colourOffset", colourDriver->Get());
	uniformObject.Push("atlasOffset", atlasOffset);
	uniformObject.Push("atlasScale", atlasScale);
	uniformObject.Push("atlasRows", static_cast<float>(numberOfRows));
	uniformObject.Push("ninePatches", ninePatches);
}

bool Gui::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	// Gets if this should be rendered.
	if (!image || !IsEnabled())
		return false;

	// Updates descriptors.
	descriptorSet.Push("UniformObject", uniformObject);
	descriptorSet.Push("samplerColour", image);

	if (!descriptorSet.Update(pipeline))
		return false;

	auto scissor = GetScissor();
	VkRect2D scissorRect = {};
	scissorRect.offset.x = scissor ? static_cast<int32_t>(scissor->x) : 0;
	scissorRect.offset.y = scissor ? static_cast<int32_t>(scissor->y) : 0;
	scissorRect.extent.width = scissor ? static_cast<int32_t>(scissor->z) : Window::Get()->GetSize().x;
	scissorRect.extent.height = scissor ? static_cast<int32_t>(scissor->w) : Window::Get()->GetSize().y;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

	// Draws the object.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return model->CmdRender(commandBuffer);
}
}
