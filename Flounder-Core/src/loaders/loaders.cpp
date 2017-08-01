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

	int loaders::createEmptyVBO(const int &floatCount)
	{
		GLuint result;
		glGenBuffers(1, &result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(GLfloat), (GLvoid*) NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
	}

	void loaders::updateVBO(const GLuint &vboID, const int &floatCount, const std::vector<GLfloat> &data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(GLfloat), data.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint loaders::createIndicesVBO(const GLuint &vaoID, const std::vector<GLint> &indices)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
		return result;
	}

	GLuint loaders::storeDataInVBO(const GLuint &vaoID, const std::vector<GLfloat> &data, const int &attributeNumber, const int &coordSize)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordSize, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
	}

	void loaders::addInstancedAttribute(const GLuint &vaoID, const GLuint &vboID, const GLuint &attribute, const GLuint &dataSize, const GLuint &instancedDataLength, const GLuint &offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBindVertexArray(vaoID);

		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(GLfloat), (GLvoid*) (offset * sizeof(GLfloat)));
		glVertexAttribDivisor(attribute, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
