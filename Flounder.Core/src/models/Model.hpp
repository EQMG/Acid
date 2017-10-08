#pragma once

#include <string>
#include <vector>

#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../physics/Aabb.hpp"
#include "../renderer/buffers/VertexBuffer.hpp"
#include "../renderer/buffers/IndexBuffer.hpp"

#include "Vertex.hpp"
#include "VertexData.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class Model
	{
	private:
		std::string m_file;

		std::vector<Vertex> m_vertices;
		std::vector<uint16_t> m_indices;

		Aabb *m_aabb;

		VertexBuffer *m_vertexBuffer;
		IndexBuffer *m_indexBuffer;
	public:
		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="name"> The file name. </param>
		Model(const std::string &file);

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="indices"> The model indices. </param>
		Model(const std::vector<Vertex> &vertices, const std::vector<uint16_t> &indices);

		/// <summary>
		/// Creates a new model without indices.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		Model(const std::vector<Vertex> &vertices);

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~Model();

		Aabb *GetAabb() const { return m_aabb; }

		VertexBuffer *GetVertexBuffer() const { return m_vertexBuffer; }

		IndexBuffer *GetIndexBuffer() const { return m_indexBuffer; }
	private:
		/// <summary>
		/// Loads the model object from a OBJ file.
		/// </summary>
		void LoadFromFile();

		VertexData *ProcessDataVertex(Vector3 vertex, std::vector<VertexData*> *vertices, std::vector<uint16_t> *indices);

		VertexData *DealWithAlreadyProcessedDataVertex(VertexData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint16_t> *indices, std::vector<VertexData*> *vertices);

		void CalculateTangents(VertexData *v0, VertexData *v1, VertexData *v2, std::vector<Vector2> *textures);

		void CreateAabb();
	};
}
