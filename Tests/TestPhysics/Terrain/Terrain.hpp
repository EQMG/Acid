#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>
#include <Models/Model.hpp>
#include <Maths/Noise/Noise.hpp>
#include "MeshTerrain.hpp"

using namespace acid;

namespace test
{
class Terrain :
	public Component
{
public:
	explicit Terrain(const float &sideLength = 200.0f, const float &squareSize = 2.0f);

	void Start() override;

	void Update() override;

	friend const Metadata &operator>>(const Metadata &metadata, Terrain &terrain);

	friend Metadata &operator<<(Metadata &metadata, const Terrain &terrain);

private:
	static uint32_t CalculateVertexCount(const float &sideLength, const float &squareSize);

	static float CalculateTextureScale(const float &sideLength);

	std::vector<float> GenerateHeightmap(const uint32_t &vertexCount);

	Noise m_noise;
	std::vector<float> m_heightmap;

	float m_sideLength;
	float m_squareSize;

	float m_minHeight;
	float m_maxHeight;
};
}
