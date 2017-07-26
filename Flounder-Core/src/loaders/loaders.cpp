#include "loaders.h"

namespace flounder
{
	loaders::loaders() : 
		imodule()
	{
		m_loaded = new std::map<GLuint, std::vector<GLuint>*>();
	}

	loaders::~loaders()
	{
	}

	void loaders::init()
	{
	}

	void loaders::update()
	{
	}

	GLuint loaders::createVAO()
	{
		GLuint result;
		glGenVertexArrays(1, &result);
		glBindVertexArray(result);
		// m_loaded->insert(result, new std::vector<GLuint>());
		return result;
	}

	void loaders::unbindVAO()
	{
		glBindVertexArray(0);
	}

	GLuint loaders::createIndicesVBO(GLuint &vaoID, const std::vector<GLint> &indices)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
		return result;
	}

	GLuint loaders::storeDataInVBO(GLuint &vaoID, const std::vector<GLfloat> &data, const int &attributeNumber, const int &coordSize)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordSize, GL_FLOAT, false, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
	}
}
