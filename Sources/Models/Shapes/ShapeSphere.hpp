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
		static ShapeSphere *Resource(const unsigned int &latitudeBands, const unsigned int &longitudeBands, const float &radius)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(latitudeBands, longitudeBands, radius));

			if (resource != nullptr)
			{
				return dynamic_cast<ShapeSphere *>(resource);
			}

			ShapeSphere *result = new ShapeSphere(latitudeBands, longitudeBands, radius);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static ShapeSphere *Resource(const std::string &filename)
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
