#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelSphere :
		public Model
	{
	public:
		static std::shared_ptr<ModelSphere> Create(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

		static std::shared_ptr<ModelSphere> Create(const std::string &data);

		ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);
	private:
		static std::string ToName(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);
	};
}
