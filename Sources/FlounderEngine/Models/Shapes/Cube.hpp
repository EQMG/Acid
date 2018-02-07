#pragma once

#include <functional>
#include <Helpers/FormatString.hpp>
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
				return dynamic_cast<Cube *>(resource);
			}

			Cube *result = new Cube(width, height, depth);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static Cube *Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float width = atof(split.at(1).c_str());
			float height = atof(split.at(2).c_str());
			float depth = atof(split.at(3).c_str());
			return Resource(width, height, depth);
		}

		Cube(const float &width, const float &height, const float &depth);

	private:
		static std::string ToFilename(const float &width, const float &height, const float &depth);
	};
}
