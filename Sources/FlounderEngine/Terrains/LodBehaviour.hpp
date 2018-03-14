#pragma once

#include "../Objects/Behaviour.hpp"
#include "../Objects/GameObject.hpp"
#include "../Models/Model.hpp"

namespace Flounder
{
	class F_EXPORT LodBehaviour :
		public Behaviour
	{
	private:
		std::vector<Model *> m_modelLods;
		int m_currentLod;
	public:
		LodBehaviour();

		~LodBehaviour();

		void Update() override;
	private:
		void CreateLod(const unsigned int &lod);

		static int CalculateVertexCount(const int &terrainLength, const float &squareSize);
	};
}
