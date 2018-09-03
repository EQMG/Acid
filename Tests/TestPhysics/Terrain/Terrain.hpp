#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>
#include <Noise/Noise.hpp>
#include <Models/Model.hpp>
#include "MeshTerrain.hpp"

using namespace acid;

namespace test
{
	class Terrain :
		public IComponent
	{
	private:
		Noise m_noise;

		float m_sideLength;
		float m_squareSize;

		float m_minHeight;
		float m_maxHeight;
	public:
		Terrain(const float &sideLength = 200.0f, const float &squareSize = 2.0f);

		~Terrain();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;
	private:
		uint32_t CalculateVertexCount(const float &sideLength, const float &squareSize);

		float CalculateTextureScale(const float &sideLength);

		std::vector<float> GenerateHeightmap(const uint32_t &vertexCount);
	};
}
