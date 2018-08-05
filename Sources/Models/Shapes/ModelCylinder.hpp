#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCylinder :
		public Model
	{
	public:
		static std::shared_ptr<ModelCylinder> Resource(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks)
		{
			auto resource = Resources::Get()->Get(ToFilename(radiusBase, radiusTop, height, slices, stacks));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ModelCylinder>(resource);
			}

			auto result = std::make_shared<ModelCylinder>(radiusBase, radiusTop, height, slices, stacks);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ModelCylinder> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			float radiusBase = static_cast<float>(atof(split[1].c_str()));
			float radiusTop = static_cast<float>(atof(split[2].c_str()));
			float height = static_cast<float>(atof(split[3].c_str()));
			int slices = atoi(split[4].c_str());
			int stacks = atoi(split[5].c_str());
			return Resource(radiusBase, radiusTop, height, slices, stacks);
		}

		ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);

		~ModelCylinder();

	private:
		static std::string ToFilename(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks);
	};
}
