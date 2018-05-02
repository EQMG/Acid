#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeCube :
		public Model
	{
	public:
		static ShapeCube *Resource(const float &width, const float &height, const float &depth)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(width, height, depth));

			if (resource != nullptr)
			{
				return dynamic_cast<ShapeCube *>(resource);
			}

			ShapeCube *result = new ShapeCube(width, height, depth);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static ShapeCube *Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float width = static_cast<float>(atof(split[1].c_str()));
			float height = static_cast<float>(atof(split[2].c_str()));
			float depth = static_cast<float>(atof(split[3].c_str()));
			return Resource(width, height, depth);
		}

		ShapeCube(const float &width, const float &height, const float &depth);

	private:
		static std::string ToFilename(const float &width, const float &height, const float &depth);
	};
}
