#pragma once

#include <functional>
#include "../Model.hpp"

namespace Flounder
{
	class Cube :
		public Model
	{
	public:
		static Cube *Resource(const float &width, const float &height, const float &depth)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(width, height, depth));

			if (resource != nullptr)
			{
				return dynamic_cast<Cube*>(resource);
			}

			Cube *result = new Cube(width, height, depth);
			Resources::Get()->Add(dynamic_cast<IResource*>(result));
			return result;
		}

		Cube(const float &width, const float &height, const float &depth);
	private:
		static std::string ToFilename(const float &width, const float &height, const float &depth);
	};
}
