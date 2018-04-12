#include "MeshTerrain.hpp"

#include "../Maths/Maths.hpp"
#include "../Maths/Colour.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const std::array<Colour, 4> BIOME_COLOURS = {
		Colour("#ffcc00"), Colour("#E17B35"), Colour("#C35D35"), Colour("#683130")
	};
	const float SPREAD = 0.76f;
	const float HALF_SPREAD = SPREAD / 2.0f;
	const float AMPLITUDE = 20.0f;
	const float PART = 1.0f / (BIOME_COLOURS.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, Transform *transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_transform(transform),
		m_worldMatrix(new Matrix4())
	{
		m_worldMatrix = m_transform->GetWorldMatrix(m_worldMatrix);
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
		delete m_worldMatrix;
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		Vector4 position = Vector4(x, 0.0f, z, 1.0f);
		Matrix4::Multiply(*m_worldMatrix, position, &position);
		position.Set(GetSphereCoords(position));

		//if (position.m_x != 0.0f)
		//{
		//Vector3 polar = Vector3::CartesianToPolar(position); // Terrains::Get()->GetNoise()->GetValue(Maths::NormalizeAngle(Maths::Degrees(polar.m_y)), Maths::NormalizeAngle(Maths::Degrees(polar.m_z)));
		float height = Maths::Clamp(Terrains::Get()->GetNoise()->GetValue(3.0f * position.m_x, 3.0f * position.m_y, 3.0f * position.m_z), 0.2f, 2.0f);
		position *= height;
		//}

		return position;
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		Vector4 normal = Vector4(Terrains::Get()->GetNormal(position.m_x + m_transform->GetPosition()->m_x, position.m_z + m_transform->GetPosition()->m_z), 1.0f);
		Matrix4::Multiply(*m_worldMatrix, normal, &normal);

		return normal;
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		float value = (position.m_y + AMPLITUDE) / (AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - HALF_SPREAD) * (1.0f / SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / PART));
		float blend = (value - (firstBiome * PART)) / PART;
		Colour colour = Colour();
		Colour::Interpolate(BIOME_COLOURS.at(firstBiome), BIOME_COLOURS.at(firstBiome + 1), blend, &colour);
		return colour;
	}

	Vector3 MeshTerrain::GetSphereCoords(const Vector3 &position)
	{
		if (m_radius == 0.0f)
		{
			return position;
		}

		Vector3 cube = position / m_radius;
		float dx = cube.m_x * cube.m_x;
		float dy = cube.m_y * cube.m_y;
		float dz = cube.m_z * cube.m_z;
		float sx = cube.m_x * std::sqrt(1.0f - (dy / 2.0f) - (dz / 2.0f) + (dy * dz / 3.0f));
		float sy = cube.m_y * std::sqrt(1.0f - (dz / 2.0f) - (dx / 2.0f) + (dz * dx / 3.0f));
		float sz = cube.m_z * std::sqrt(1.0f - (dx / 2.0f) - (dy / 2.0f) + (dx * dy / 3.0f));
		return Vector3(sx * m_radius, sy * m_radius, sz * m_radius);
	}
}
