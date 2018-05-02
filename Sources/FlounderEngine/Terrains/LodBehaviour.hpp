#pragma once

#include "Scenes/Objects/Behaviour.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT LodBehaviour :
		public Behaviour
	{
	private:
		float m_radius;
		Transform *m_transform;
		std::vector<Model *> m_modelLods;
		unsigned int m_currentLod;
	public:
		LodBehaviour(const float &radius = 0.0f, const Transform &transform = Transform());

		~LodBehaviour();

		void Update() override;

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) { *m_transform = transform; }

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }
	private:
		void CreateLod(const unsigned int &lod);

		static int CalculateVertexCount(const int &terrainLength, const float &squareSize);
	};
}
