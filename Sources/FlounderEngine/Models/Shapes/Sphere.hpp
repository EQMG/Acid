#pragma once

#include <functional>
#include <Helpers/FormatString.hpp>
#include "../Model.hpp"

namespace Flounder
{
	class Sphere :
		public Model
	{
	public:
		static Sphere *Resource(const int &latitudeBands, const int &longitudeBands, const float &radius)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(latitudeBands, longitudeBands, radius));

			if (resource != nullptr)
			{
				return dynamic_cast<Sphere *>(resource);
			}

			Sphere *result = new Sphere(latitudeBands, longitudeBands, radius);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		static Sphere *Resource(const std::string &filename)
		{
			auto split = FormatString::Split(filename, "_");
			int latitudeBands = atoi(split.at(1).c_str());
			int longitudeBands = atoi(split.at(2).c_str());
			float radius = atof(split.at(3).c_str());
			return Resource(latitudeBands, longitudeBands, radius);
		}

		Sphere(const int &latitudeBands, const int &longitudeBands, const float &radius);

		~Sphere();

	private:
		static std::string ToFilename(const int &latitudeBands, const int &longitudeBands, const float &radius);
	};
}
