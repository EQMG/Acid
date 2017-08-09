#include "loaders.h"

namespace flounder
{
	loaders::loaders() :
		imodule()
	{
#if 0
		m_loaded = new std::map<GLuint, std::vector<GLuint>*>();
#endif
	}

	loaders::~loaders()
	{
	}

	void loaders::update()
	{
	}

	int loaders::createVAO()
	{
		return 0;
#if 0
		GLuint result;
		glGenVertexArrays(1, &result);
		glBindVertexArray(result);
		// m_loaded->insert(result, new std::vector<GLuint>());
		return result;
#endif
	}

	void loaders::unbindVAO()
	{
#if 0
		glBindVertexArray(0);
#endif
	}

	int loaders::createEmptyVBO(const int &floatCount)
	{
		return 0;
#if 0
		GLuint result;
		glGenBuffers(1, &result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(GLfloat), (GLvoid*) NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
#endif
	}

	void loaders::updateVBO(const int &vboID, const int &floatCount, const std::vector<float> &data)
	{
#if 0
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(GLfloat), data.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}

	int loaders::createIndicesVBO(const int &vaoID, const std::vector<int> &indices)
	{
		return 0;
#if 0
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
		return result;
#endif
	}

	int loaders::storeDataInVBO(const int &vaoID, const std::vector<float> &data, const int &attributeNumber, const int &coordSize)
	{
		return 0;
#if 0
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordSize, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
#endif
	}

	void loaders::addInstancedAttribute(const int &vaoID, const int &vboID, const int &attribute, const int &dataSize, const int &instancedDataLength, const int &offset)
	{
#if 0
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBindVertexArray(vaoID);

		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(GLfloat), (GLvoid*) (offset * sizeof(GLfloat)));
		glVertexAttribDivisor(attribute, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#endif
	}
}
