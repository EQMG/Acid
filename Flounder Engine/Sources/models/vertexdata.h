#pragma once

#include <vector>
#include "../maths/vector3.h"

namespace flounder {
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

		inline vector3 getPosition() { return m_position; }

		inline int getTextureIndex() { return m_textureIndex; }

		inline void setTextureIndex(int textureIndex) { m_textureIndex = textureIndex; }

		inline int getNormalIndex() { return m_normalIndex; }

		inline void setNormalIndex(const int normalIndex) { m_normalIndex = normalIndex; }

		inline vertexdata *getDuplicateVertex() { return m_duplicateVertex; }

		inline void setDuplicateVertex(vertexdata *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		inline int getIndex() { return m_index; }

		inline float getLength() { return m_length; }

		void addTangent(vector3 *tangent);

		void averageTangents();

		inline vector3 getAverageTangent() { return m_averagedTangent; }

		bool isSet();

		bool hasSameTextureAndNormal(const int textureIndexOther, const int normalIndexOther);
	};
}
