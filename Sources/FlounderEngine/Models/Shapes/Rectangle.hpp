#pragma once

#include <Helpers/FormatString.hpp>
#include "../Model.hpp"

namespace Flounder
{
	class F_EXPORT Rectangle :
		public Model
	{
	public:
		static Rectangle *Resource(const float &min, const float &max)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(min, max));

			if (resource != nullptr)
			{
				return dynamic_cast<Rectangle *>(resource);
			}

			Rectangle *result = new Rectangle(min, max);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static Rectangle *Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float width = static_cast<float>(atof(split.at(1).c_str()));
			float height = static_cast<float>(atof(split.at(2).c_str()));
			return Resource(width, height);
		}

		Rectangle(const float &width, const float &height);

	private:
		static std::string ToFilename(const float &min, const float &max);
	};
}
