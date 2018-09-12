#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCylinder :
		public Model
	{
	public:
		static std::shared_ptr<ModelCylinder> Resource(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);

		static std::shared_ptr<ModelCylinder> Resource(const std::string &data);

		ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);
	private:
		static std::string ToFilename(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);
	};
}
