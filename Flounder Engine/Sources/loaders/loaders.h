#pragma once

#include <GL\glew.h>
#include <vector>
#include <map>

#include "../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A module used for loading and managing OpenGL VAO's and VBO's.
	/// </summary>
	class loaders : public imodule
	{
	private:
		std::map<GLuint, std::vector<GLuint>*> *m_loaded;
	public:
		static loaders *get()
		{
			return (loaders*)framework::get()->getInstance("loaders");
		}

		loaders();

		~loaders();

		void init();

		void update();

		/// <summary>
		/// Creates an empty VAO.
		/// </summary>
		/// <returns> The ID of the VAO. </returns>
		GLuint createVAO();

		/// <summary>
		/// Creates an index buffer and binds it to a VAO.
		/// </summary>
		/// <param name="vaoID"> The ID of the VAO to which the index buffer should be bound. </param>
		/// <param name="indices"> The array of indices to be stored in the index buffer.
		/// </param>
		/// <returns> The ID of the index buffer VBO. </returns>
		GLuint createIndicesVBO(GLuint vaoID, int *indices);

		/// <summary>
		/// Stores a float array of data into a FBO.
		/// </summary>
		/// <param name="vaoID"> The VAO to create a new FBO in. </param>
		/// <param name="data"> The data to store. </param>
		/// <param name="attributeNumber"> The attribute to create the FBO under. </param>
		/// <param name="coordSize"> The size of data being store.
		/// </param>
		/// <returns> The new FBO's ID. </returns>
		GLuint storeDataInVBO(GLuint vaoID, float *data, int attributeNumber, int coordSize);
	};
}