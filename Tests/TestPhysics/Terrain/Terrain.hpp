#pragma once

#include <Objects/Component.hpp>
#include <Objects/Entity.hpp>
#include <Noise/Noise.hpp>
#include <Models/Model.hpp>
#include "MeshTerrain.hpp"

using namespace acid;

namespace test
{
	class Terrain :
		public Component
	{
	private:
		Noise m_noise;
		std::vector<float> m_heightmap;

		float m_sideLength;
		float m_squareSize;

		float m_minHeight;
		float m_maxHeight;
	public:
		explicit Terrain(const float &sideLength = 200.0f, const float &squareSize = 2.0f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	private:
		uint32_t CalculateVertexCount(const float &sideLength, const float &squareSize);

		float CalculateTextureScale(const float &sideLength);

		std::vector<float> GenerateHeightmap(const uint32_t &vertexCount);
	};
}
