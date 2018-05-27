#pragma once

#include <Objects/Behaviour.hpp>
#include <Objects/GameObject.hpp>
#include <Models/Model.hpp>

using namespace fl;

namespace test
{
	class LodBehaviour :
		public Behaviour
	{
	private:
		std::vector<Model *> m_modelLods;
		unsigned int m_currentLod;
	public:
		LodBehaviour();

		~LodBehaviour();

		void Update() override;
	private:
		void CreateLod(const unsigned int &lod);

		static int CalculateVertexCount(const int &terrainLength, const float &squareSize);
	};
}
