#include "IPostPipeline.hpp"

namespace fl
{
	IPostPipeline::IPostPipeline(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage)
	{
	}

	IPostPipeline::~IPostPipeline()
	{
	}
}
