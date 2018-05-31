#pragma once

#include <functional>
#include "Helpers/FormatString.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT ShapeSphere :
		public Model
	{
	public:
		static std::shared_ptr<ShapeSphere> Resource(const unsigned int &latitudeBands, const unsigned int &longitudeBands, const float &radius)
		{
			auto resource = Resources::Get()->Get(ToFilename(latitudeBands, longitudeBands, radius));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<ShapeSphere>(resource);
			}

			auto result = std::make_shared<ShapeSphere>(latitudeBands, longitudeBands, radius);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		static std::shared_ptr<ShapeSphere> Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			int latitudeBands = atoi(split[1].c_str());
			int longitudeBands = atoi(split[2].c_str());
			float radius = static_cast<float>(atof(split[3].c_str()));
			return Resource(latitudeBands, longitudeBands, radius);
		}

		ShapeSphere(const unsigned int &latitudeBands, const unsigned int &longitudeBands, const float &radius);

		~ShapeSphere();

	private:
		static std::string ToFilename(const unsigned int &latitudeBands, const unsigned int &longitudeBands, const float &radius);
	};
}
