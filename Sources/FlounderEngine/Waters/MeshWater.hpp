#pragma once

#include "Models/Shapes/MeshSimple.hpp"
#include "Maths/Colour.hpp"

namespace fl
{
	class FL_EXPORT MeshWater :
		public MeshSimple
	{
	public:
		static const float SIDE_LENGTH;
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float TEXTURE_SCALE;
		static const Colour WATER_COLOUR;

		MeshWater();

		~MeshWater();

		Vector3 GetPosition(const float &x, const float &z) override;

		Vector3 GetNormal(const Vector3 &position) override;

		Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;
	};
}