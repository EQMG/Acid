#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <stdarg.h>

#include "../platform.h"
#include "../framework/framework.h"

namespace flounder
{
	/// <summary>
	/// A module used for loading and managing OpenGL VAO's and VBO's.
	/// </summary>
	class loaders :
		public imodule
	{
	private:
		std::map<GLuint, std::vector<GLuint>*> *m_loaded;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static loaders *get()
		{
			return static_cast<loaders*>(framework::get()->getInstance("loaders"));
		}

		loaders();

		~loaders();

		void update() override;

		/// <summary>
		/// Creates an empty VAO.
		/// </summary>
		/// <returns> The ID of the VAO. </returns>
		GLuint createVAO();

		void unbindVAO();

		/// <summary>
		/// Creates a new VBO with no data.
		/// </summary>
		/// <param name="floatCount"> The number of floats to be allotted.
		/// </param>
		/// <returns> The new buffer objects ID. </returns>
		int createEmptyVBO(const int &floatCount);

		/// <summary>
		/// Updates a FBO with a new set of data.
		/// </summary>
		/// <param name="vboID"> The FBO to update. </param>
		/// <param name="data"> The data to add into the FBO. </param>
		void updateVBO(const GLuint &vboID, const int &floatCount, const std::vector<GLfloat> &data);

		/// <summary>
		/// Creates an index buffer and binds it to a VAO.
		/// </summary>
		/// <param name="vaoID"> The ID of the VAO to which the index buffer should be bound. </param>
		/// <param name="indices"> The array of indices to be stored in the index buffer. </param>
		/// <returns> The ID of the index buffer VBO. </returns>
		GLuint createIndicesVBO(const GLuint &vaoID, const std::vector<GLint> &indices);

		/// <summary>
		/// Stores a float array of data into a FBO.
		/// </summary>
		/// <param name="vaoID"> The VAO to create a new FBO in. </param>
		/// <param name="data"> The data to store. </param>
		/// <param name="attributeNumber"> The attribute to create the FBO under. </param>
		/// <param name="coordSize"> The size of data being store. </param>
		/// <returns> The new FBO's ID. </returns>
		GLuint storeDataInVBO(const GLuint &vaoID, const std::vector<GLfloat> &data, const int &attributeNumber, const int &coordSize);

		/// <summary>
		/// Adds a instances attribute to a VBO.
		/// </summary>
		/// <param name="vaoID"> The VBO's VAO. </param>
		/// <param name="vboID"> The VBO. </param>
		/// <param name="attribute"> The attribute to add data to. </param>
		/// <param name="dataSize"> The size of data to add. </param>
		/// <param name="instancedDataLength"> The length of data to allocate. </param>
		/// <param name="offset"> The offset between data. </param>
		void addInstancedAttribute(const GLuint &vaoID, const GLuint &vboID, const GLuint &attribute, const GLuint &dataSize, const GLuint &instancedDataLength, const GLuint &offset);
	};
}
