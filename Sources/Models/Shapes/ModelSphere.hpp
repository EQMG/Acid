#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelSphere :
		public Model
	{
	public:
		static std::shared_ptr<ModelSphere> Resource(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius)
		{
			auto resource = Resources::Get()->Get(ToFilename(latitudeBands, longitudeBands, radius));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ModelSphere>(resource);
			}

			auto result = std::make_shared<ModelSphere>(latitudeBands, longitudeBands, radius);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ModelSphere> Resource(const std::string &data)
		{
			auto split = FormatString::Split(data, "_");
			int latitudeBands = atoi(split[1].c_str());
			int longitudeBands = atoi(split[2].c_str());
			float radius = static_cast<float>(atof(split[3].c_str()));
			return Resource(latitudeBands, longitudeBands, radius);
		}

		ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);

		~ModelSphere();

	private:
		static std::string ToFilename(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius);
	};
}
