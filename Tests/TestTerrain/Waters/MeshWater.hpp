#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Colour.hpp>

using namespace fl;

namespace test
{
	class MeshWater :
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