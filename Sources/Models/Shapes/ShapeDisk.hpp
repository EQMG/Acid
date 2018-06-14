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
		static std::shared_ptr<ShapeDisk> Resource(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops)
		{
			auto resource = Resources::Get()->Get(ToFilename(innerRadius, outerRadius, slices, loops));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ShapeDisk>(resource);
			}

			auto result = std::make_shared<ShapeDisk>(innerRadius, outerRadius, slices, loops);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ShapeDisk> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
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
