#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelDisk :
		public Model
	{
	public:
		static std::shared_ptr<ModelDisk> Resource(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);

		static std::shared_ptr<ModelDisk> Resource(const std::string &data);

		ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);
	private:
		static std::string ToFilename(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);
	};
}
