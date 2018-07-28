#pragma once

#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ModelRectangle> Resource(const float &min, const float &max)
		{
			auto resource = Resources::Get()->Get(ToFilename(min, max));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ModelRectangle>(resource);
			}

			auto result = std::make_shared<ModelRectangle>(min, max);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ModelRectangle> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			float width = static_cast<float>(atof(split[1].c_str()));
			float height = static_cast<float>(atof(split[2].c_str()));
			return Resource(width, height);
		}

		ModelRectangle(const float &width, const float &height);

	private:
		static std::string ToFilename(const float &min, const float &max);
	};
}
