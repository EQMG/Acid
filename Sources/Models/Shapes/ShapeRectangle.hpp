#pragma once

#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ShapeRectangle> Resource(const float &min, const float &max)
		{
			auto resource = Resources::Get()->Get(ToFilename(min, max));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ShapeRectangle>(resource);
			}

			auto result = std::make_shared<ShapeRectangle>(min, max);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ShapeRectangle> Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float width = static_cast<float>(atof(split[1].c_str()));
			float height = static_cast<float>(atof(split[2].c_str()));
			return Resource(width, height);
		}

		ShapeRectangle(const float &width, const float &height);

	private:
		static std::string ToFilename(const float &min, const float &max);
	};
}
