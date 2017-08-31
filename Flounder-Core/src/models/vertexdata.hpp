#pragma once

#include <vector>
#include "../maths/vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// A class that represents a single model vertex, used when loading OBJ files.
	/// </summary>
	class vertexdata
	{
	private:
		static const int NO_INDEX;

		vector3 m_position;

		int m_textureIndex;
		int m_normalIndex;
		vertexdata *m_duplicateVertex;

		int m_index;
		float m_length;

		std::vector<vector3*> m_tangents;
		vector3 m_averagedTangent;

	public:
		vertexdata(const int index, vector3 position);

		~vertexdata();

		vector3 getPosition() const { return m_position; }

		int getTextureIndex() const { return m_textureIndex; }

		void setTextureIndex(int textureIndex) { m_textureIndex = textureIndex; }

		int getNormalIndex() const { return m_normalIndex; }

		void setNormalIndex(const int normalIndex) { m_normalIndex = normalIndex; }

		vertexdata *getDuplicateVertex() const { return m_duplicateVertex; }

		void setDuplicateVertex(vertexdata *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		int getIndex() const { return m_index; }

		float getLength() const { return m_length; }

		void addTangent(vector3 *tangent);

		void averageTangents();

		vector3 getAverageTangent() const { return m_averagedTangent; }

		bool isSet();

		bool hasSameTextureAndNormal(const int textureIndexOther, const int normalIndexOther);
	};
}
