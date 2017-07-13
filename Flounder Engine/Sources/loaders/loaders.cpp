#include "loaders.h"

namespace flounder {
	loaders::loaders()
		: imodule()
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
		// m_loaded->insert(result, new std::vector<GLuint>());
		return result;
	}

	GLuint loaders::createIndicesVBO(GLuint vaoID, int *indices)
	{
		if (indices == NULL) 
		{
			return NULL;
		}

		int size = sizeof(indices) / sizeof(*indices);

		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		return result;
	}

	GLuint loaders::storeDataInVBO(GLuint vaoID, float *data, int attributeNumber, int coordSize)
	{
		if (data == NULL) 
		{
			return NULL;
		}

		int size = sizeof(data) / sizeof(*data);

		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
	}
}