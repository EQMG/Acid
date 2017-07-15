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
		glBindVertexArray(result);
		// m_loaded->insert(result, new std::vector<GLuint>());
		return result;
	}

	void loaders::unbindVAO()
	{
		glBindVertexArray(0);
	}

	/*GLuint loaders::storeInterleavedDataInVAO(GLuint vaoID, const std::vector<GLfloat>& data, const int & n_args, ...)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

		int total = 0;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			total += va_arg(ap, int);
		}

		va_end(ap);

		GLsizei vertexByteCount = sizeof(GLfloat) * total;
		total = 0;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int length = va_arg(ap, int);
			glVertexAttribPointer(i, length, GL_FLOAT, false, vertexByteCount, sizeof(GLfloat) * total);
			total += length;
		}

		va_end(ap);

		return result;
	}*/

	GLuint loaders::createIndicesVBO(GLuint vaoID, const std::vector<GLint> &indices)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
		return result;
	}

	GLuint loaders::storeDataInVBO(GLuint vaoID, const std::vector<GLfloat> &data, int attributeNumber, int coordSize)
	{
		GLuint result;
		glGenBuffers(1, &result);
		// m_loaded->at(vaoID).push_back(result);

		glBindBuffer(GL_ARRAY_BUFFER, result);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data() , GL_STATIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordSize, GL_FLOAT, false, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return result;
	}
}