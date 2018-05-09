#pragma once

#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeRectangle :
		public Model
	{
	public:
		static ShapeRectangle *Resource(const float &min, const float &max)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(min, max));

			if (resource != nullptr)
			{
				return dynamic_cast<ShapeRectangle *>(resource);
			}

			ShapeRectangle *result = new ShapeRectangle(min, max);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static ShapeRectangle *Resource(const std::string &filename)
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
