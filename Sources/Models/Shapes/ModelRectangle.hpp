#pragma once

#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ModelRectangle> Resource(const float &min, const float &max);

		static std::shared_ptr<ModelRectangle> Resource(const std::string &data);

		ModelRectangle(const float &width, const float &height);
	private:
		static std::string ToName(const float &min, const float &max);
	};
}
