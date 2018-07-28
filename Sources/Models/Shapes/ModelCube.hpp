#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelCube :
		public Model
	{
	public:
		static std::shared_ptr<ModelCube> Resource(const float &width, const float &height, const float &depth)
		{
			auto resource = Resources::Get()->Get(ToFilename(width, height, depth));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ModelCube>(resource);
			}

			auto result = std::make_shared<ModelCube>(width, height, depth);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ModelCube> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			float width = static_cast<float>(atof(split[1].c_str()));
			float height = static_cast<float>(atof(split[2].c_str()));
			float depth = static_cast<float>(atof(split[3].c_str()));
			return Resource(width, height, depth);
		}

		ModelCube(const float &width, const float &height, const float &depth);

	private:
		static std::string ToFilename(const float &width, const float &height, const float &depth);
	};
}
