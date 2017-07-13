#pragma once

#include <string>
#include <GL/glew.h>

#include "../loaders/loaders.h"

namespace flounder {
	/// <summary>
	/// Class that represents a loaded model.
	/// </summary>
	class model {
		/// <summary>
		/// A builder used to set model parameters for loading.
		/// </summary>
		class builder
		{
		private:
			model *m_model;
		public:
			/// <summary>
			/// Creates a new model builder.
			/// </summary>
			/// <param name="name"> The name. </param>
			builder();

			/// <summary>
			/// Deconstructor for the model builder.
			/// </summary>
			~builder();

			/// <summary>
			/// Sets the file to load the model from.
			/// </summary>
			/// <param name="name"> The file name. </param>
			/// <returns> This. </returns>
			builder *setFile(const std::string &file);

			/// <summary>
			/// Creates a model from the builder.
			/// </summary>
			/// <returns> The created model. </returns>
			model *create();
		};
	protected:
		builder *m_builder; 

		std::string m_file;

		float *m_vertices;
		float *m_textures;
		float *m_normals;
		float *m_tangents;
		int *m_indices;

		GLuint m_vaoID;
		GLuint m_vaoLength;

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="builder"> The models builder. </param>
		model(builder *builder);
	public:
		~model();

		/// <summary>
		/// Creates a new model builder that is used to configure a model.
		/// </summary>
		/// <returns> The model builder. </returns>
		static builder *newModel();

		GLuint getVaoID() { return m_vaoID; }

		GLuint getVaoLength() { return m_vaoLength; }
	private:
		void loadFromFile(const std::string &file);

		void loadToOpenGL();
	};
}