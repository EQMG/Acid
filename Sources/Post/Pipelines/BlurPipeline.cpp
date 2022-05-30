#include "BlurPipeline.hpp"

#include "Devices/Windows.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
BlurPipeline::BlurPipeline(const Pipeline::Stage &pipelineStage, float blur, const BlurFilter::Type &blurType, bool toScreen, float inputScale,
	float outputScale) :
	PostPipeline(pipelineStage),
	filterBlurVertical(pipelineStage, {0.0f, blur}, blurType),
	filterBlurHorizontal(pipelineStage, {blur, 0.0f}, blurType),
	toScreen(toScreen),
	inputScale(inputScale),
	outputScale(outputScale),
	blur(blur) {
}

void BlurPipeline::Render(const CommandBuffer &commandBuffer) {
	if (!toScreen) {
		if (auto size = Windows::Get()->GetWindow(0)->GetSize(); size != lastSize) {
			auto newSize = outputScale * size;
			output = std::make_unique<Image2d>(newSize, VK_FORMAT_R8G8B8A8_UNORM);

			filterBlurVertical.SetAttachment("writeColour", output.get());
			filterBlurHorizontal.SetAttachment("writeColour", output.get());

			lastSize = size;
		}
	}

	// Input might be scaled for faster blur.
	//Image2d::CopyImage(dynamic_cast<Image2d *>(Renderer::Get()->GetAttachment("resolved"))->GetImage(), halfRes->GetImage(), halfRes->GetDeviceMemory(), width / 2, height / 2, false, 0, 1);

	filterBlurVertical.SetDirection({0.0f, blur});
	filterBlurHorizontal.SetDirection({blur, 0.0f});

	filterBlurVertical.Render(commandBuffer);
	filterBlurHorizontal.Render(commandBuffer);
}
}
