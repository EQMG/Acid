#pragma once

#include "Pipeline.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a compute Vulkan pipeline.
	/// </summary>
	class FL_EXPORT Compute :
		public Pipeline
	{
	private:
	public:
		Compute(const std::string &shader, const VertexInput &vertexInput);

		~Compute();
	};
}
