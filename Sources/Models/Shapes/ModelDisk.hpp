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
		static std::shared_ptr<ModelDisk> Create(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);

		static std::shared_ptr<ModelDisk> Create(const std::string &data);

		ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);
	private:
		static std::string ToName(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops);
	};
}
