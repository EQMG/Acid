#pragma once

#include <string>
#include <vector>
#include "Files/Files.hpp"
#include "Maths/Vector2.hpp"
#include "Physics/ColliderAabb.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "VertexModel.hpp"
#include "VertexModelData.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class FL_EXPORT Model :
		public IResource
	{
	private:
		std::string m_filename;

		VertexBuffer *m_vertexBuffer;
		IndexBuffer *m_indexBuffer;

		ColliderAabb *m_aabb;
	public:
		static std::shared_ptr<Model> Resource(const std::string &filename)
		{
			std::string realFilename = Files::Get()->SearchFile(filename);
			auto resource = Resources::Get()->Get(realFilename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<Model>(resource);
			}

			auto result = std::make_shared<Model>(realFilename);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		/// <summary>
		/// Creates a new empty model.
		/// </summary>
		Model();

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		Model(const std::string &filename);

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="indices"> The model indices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

		/// <summary>
		/// Creates a new model without indices.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<IVertex *> &vertices, const std::string &name = "");

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~Model();

		void CmdRender(const CommandBuffer &commandBuffer, const unsigned int &instances = 1);

		std::string GetFilename() override { return m_filename; }

		ColliderAabb *GetAabb() const { return m_aabb; }

		VertexBuffer *GetVertexBuffer() const { return m_vertexBuffer; }

		IndexBuffer *GetIndexBuffer() const { return m_indexBuffer; }

	protected:
		void Set(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

	private:
		/// <summary>
		/// Loads the model object from a OBJ file.
		/// </summary>
		void LoadFromFile(const std::string &filename, std::vector<IVertex *> *vertices, std::vector<uint32_t> *indices);

		VertexModelData *ProcessDataVertex(const Vector3 &vertex, std::vector<VertexModelData *> *vertices, std::vector<uint32_t> *indices);

		VertexModelData *DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexModelData *> *vertices);

		void CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> *uvs);

		static ColliderAabb CalculateAabb(const std::vector<IVertex *> &vertices);
	};
}
