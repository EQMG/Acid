#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeDisk :
		public Model
	{
	public:
		static ShapeDisk *Resource(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(innerRadius, outerRadius, slices, loops));

			if (resource != nullptr)
			{
				return dynamic_cast<ShapeDisk *>(resource);
			}

			ShapeDisk *result = new ShapeDisk(innerRadius, outerRadius, slices, loops);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static ShapeDisk *Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float innerRadius = static_cast<float>(atof(split[1].c_str()));
			float outerRadius = static_cast<float>(atof(split[2].c_str()));
			int slices = atoi(split[3].c_str());
			int loops = atoi(split[4].c_str());
			return Resource(innerRadius, outerRadius, slices, loops);
		}

		ShapeDisk(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops);

		~ShapeDisk();

	private:
		static std::string ToFilename(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops);
	};
}
