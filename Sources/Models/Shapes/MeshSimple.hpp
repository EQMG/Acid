#pragma once

#include <functional>
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT MeshSimple :
		public Model
	{
	private:
		float m_sideLength;
		float m_squareSize;
		int m_vertexCount;
		float m_textureScale;
	public:
		MeshSimple(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale);

	protected:
		void GenerateMesh();

		virtual Vector3 GetPosition(const float &x, const float &z);

		virtual Vector3 GetNormal(const Vector3 &position);

		virtual Vector3 GetColour(const Vector3 &position, const Vector3 &normal);
	};
}
