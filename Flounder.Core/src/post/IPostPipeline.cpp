#include "IPostPipeline.hpp"

namespace Flounder
{
	IPostPipeline::IPostPipeline()
	{
	}

	IPostPipeline::~IPostPipeline()
	{
	}

	void IPostPipeline::RenderPipeline(const int n_args, ...)
	{
		va_list args;
		va_start(args, n_args);
		RenderPipeline(n_args, args);
		va_end(args);
	}
}
