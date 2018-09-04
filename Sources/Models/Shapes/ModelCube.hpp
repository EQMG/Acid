#pragma once

#include <functional>
#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCube :
		public Model
	{
	public:
		static std::shared_ptr<ModelCube> Resource(const float &width, const float &height, const float &depth);

		static std::shared_ptr<ModelCube> Resource(const std::string &data);

		ModelCube(const float &width, const float &height, const float &depth);

		~ModelCube();
	private:
		static std::string ToFilename(const float &width, const float &height, const float &depth);
	};
}
