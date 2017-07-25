#include "ipostpipeline.h"

namespace flounder
{
	ipostpipeline::ipostpipeline()
	{
	}

	ipostpipeline::~ipostpipeline()
	{
	}

	void ipostpipeline::renderPipelineV(const int n_args, ...)
	{
		va_list args;
		va_start(args, n_args);
		renderPipeline(n_args, args);
		va_end(args);
	}
}
