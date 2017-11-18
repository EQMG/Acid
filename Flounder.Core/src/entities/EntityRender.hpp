#pragma once

#include <vector>
#include "../platforms/Platform.hpp"
#include "../models/Model.hpp"
#include "UbosEntities.hpp"

namespace Flounder
{
	struct EntityRender
	{
		VkDescriptorSet descriptorSet;
		UbosEntities::UboObject uboObject;
		std::vector<VkWriteDescriptorSet> descriptorWrites;
		Model *model;
	};
}
