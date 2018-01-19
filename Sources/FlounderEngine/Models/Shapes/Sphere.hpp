#pragma once

#include <functional>
#include "../Model.hpp"

namespace Flounder
{
	class Sphere :
		public Model
	{
	private:
		int m_latitudeBands;
		int m_longitudeBands;
		float m_radius;
	public:
		static Sphere *Resource(const int &latitudeBands, const int &longitudeBands, const float &radius)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(latitudeBands, longitudeBands, radius));

			if (resource != nullptr)
			{
				return dynamic_cast<Sphere*>(resource);
			}

			Sphere *result = new Sphere(latitudeBands, longitudeBands, radius);
			Resources::Get()->Add(dynamic_cast<IResource*>(result));
			return result;
		}

		Sphere(const int &latitudeBands, const int &longitudeBands, const float &radius);

		~Sphere();
	private:
		static std::string ToFilename(const int &latitudeBands, const int &longitudeBands, const float &radius);
	};
}
