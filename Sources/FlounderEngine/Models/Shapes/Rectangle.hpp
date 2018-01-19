#pragma once

#include "../Model.hpp"

namespace Flounder
{
	class Rectangle :
		public Model
	{
	public:
		static Rectangle *Resource(const float &min, const float &max)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(min, max));

			if (resource != nullptr)
			{
				return dynamic_cast<Rectangle*>(resource);
			}

			Rectangle *result = new Rectangle(min, max);
			Resources::Get()->Add(dynamic_cast<IResource*>(result));
			return result;
		}

		Rectangle(const float &width, const float &height);
	private:
		static std::string ToFilename(const float &min, const float &max);
	};
}
