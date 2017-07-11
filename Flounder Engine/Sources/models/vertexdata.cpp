#include "vertexdata.h"

namespace flounder {
	const int vertexdata::NO_INDEX = -1;

	vertexdata::vertexdata(int index, vector3 *position)
	{
		m_position = position;

		m_textureIndex = NO_INDEX;
		m_normalIndex = NO_INDEX;
		
		m_duplicateVertex = NULL;

		m_index = index;
		m_length = position->length();

		m_tangents = new std::vector<vector3*>();
		m_averagedTangent = new vector3();
	}

	vertexdata::~vertexdata()
	{
		delete m_position;

		delete m_duplicateVertex;

		delete m_tangents;
		delete m_averagedTangent;
	}

	void vertexdata::addTangent(vector3 *tangent)
	{
		m_tangents->push_back(tangent);
	}

	void vertexdata::averageTangents()
	{
		if (m_tangents->empty())
		{
			return;
		}

		for (std::vector<vector3*>::iterator it = m_tangents->begin(); it < m_tangents->end(); it++)
		{
			vector3::add(m_averagedTangent, *it, m_averagedTangent);
		}

		if (m_averagedTangent->length() > 0.0f)
		{
			m_averagedTangent->normalize();
		}
	}
}
