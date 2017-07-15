#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"
#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../physics/aabb.h"
#include "../loaders/loaders.h"

#include "vertexdata.h"

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
			/// Sets the values to load to the model.
			/// </summary>
			/// <param name="indices"> The model indices. </param>
			/// <param name="vertices"> The model vertices. </param>
			/// <param name="textures"> The model textures. </param>
			/// <param name="normals"> The model normals. </param>
			/// <param name="tangents"> The model tangents. </param>
			/// <returns> This. </returns>
			builder* setDirectly(std::vector<int> *indices, std::vector<float> *vertices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *tangents);

			/// <summary>
			/// Creates a model from the builder.
			/// </summary>
			/// <returns> The created model. </returns>
			model *create();
		};
	protected:
		builder *m_builder; 

		std::string m_file;

		std::vector<int> *m_indices;
		std::vector<float> *m_vertices;
		std::vector<float> *m_textures;
		std::vector<float> *m_normals;
		std::vector<float> *m_tangents;

		aabb *m_aabb;

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

		vertexdata *processDataVertex(vector3 vertex, std::vector<vertexdata*> *vertices, std::vector<int> *indices);

		vertexdata *dealWithAlreadyProcessedDataVertex(vertexdata *previousVertex, int newTextureIndex, int newNormalIndex, std::vector<int> *indices, std::vector<vertexdata*> *vertices);

		void calculateTangents(vertexdata *v0, vertexdata *v1, vertexdata *v2, std::vector<vector2> *textures);

		void loadToOpenGL();

		void createAABB();
	};
}