#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Engine/Exports.hpp"
#include "PipelineCreate.hpp"

namespace acid
{
	class ACID_EXPORT LayoutSet
	{
	public:
		VertexInput GetVertexInput(const uint32_t &set = 0);
	};
}
