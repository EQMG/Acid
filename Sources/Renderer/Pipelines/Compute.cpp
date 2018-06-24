#include "Compute.hpp"

namespace fl
{
	Compute::Compute(const std::string &shader, const VertexInput &vertexInput) :
		Pipeline({0, 0}, PipelineCreate({shader}, vertexInput, PIPELINE_MODE_COMPUTE, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_NONE), {})
	{
	}

	Compute::~Compute()
	{
	}
}
