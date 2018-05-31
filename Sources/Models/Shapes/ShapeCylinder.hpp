#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeCylinder :
		public Model
	{
	public:
		static std::shared_ptr<ShapeCylinder> Resource(const float &radiusBase, const float &radiusTop, const float &height, const unsigned int &slices, const unsigned int &stacks, const float &y0)
		{
			auto resource = Resources::Get()->Get(ToFilename(radiusBase, radiusTop, height, slices, stacks, y0));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ShapeCylinder>(resource);
			}

			auto result = std::make_shared<ShapeCylinder>(radiusBase, radiusTop, height, slices, stacks, y0);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ShapeCylinder> Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			float radiusBase = static_cast<float>(atof(split[1].c_str()));
			float radiusTop = static_cast<float>(atof(split[2].c_str()));
			float height = static_cast<float>(atof(split[3].c_str()));
			int slices = atoi(split[4].c_str());
			int stacks = atoi(split[5].c_str());
			float y0 = static_cast<float>(atof(split[6].c_str()));
			return Resource(radiusBase, radiusTop, height, slices, stacks, y0);
		}

		ShapeCylinder(const float &radiusBase, const float &radiusTop, const float &height, const unsigned int &slices, const unsigned int &stacks, const float &y0);

		~ShapeCylinder();

	private:
		static std::string ToFilename(const float &radiusBase, const float &radiusTop, const float &height, const unsigned int &slices, const unsigned int &stacks, const float &y0);
	};
}
