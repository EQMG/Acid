#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../devices/Display.hpp"
#include "../helpers/HelperFile.hpp"
#include "../helpers/HelperString.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../physics/Aabb.hpp"

#include "Material.hpp"
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

		std::vector<int> *m_indices;
		std::vector<float> *m_vertices;
		std::vector<float> *m_textures;
		std::vector<float> *m_normals;
		std::vector<float> *m_tangents;

		aabb *m_aabb;

		VkBuffer m_indicesBuffer;
		VkBuffer m_verticesBuffer;
		VkBuffer m_texturesBuffer;
		VkBuffer m_normalsBuffer;
		VkBuffer m_tangentsBuffer;
		VkDeviceMemory m_memory;
		VkDescriptorBufferInfo m_bufferInfo;
	public:
		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="name"> The file name. </param>
		Model(const std::string &file);

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="indices"> The model indices. </param>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="textures"> The model textures. </param>
		/// <param name="normals"> The model normals. </param>
		/// <param name="tangents"> The model tangents. </param>
		Model(const std::vector<int> &indices = std::vector<int>(), const std::vector<float> &vertices = std::vector<float>(), const std::vector<float> &textures = std::vector<float>(), const std::vector<float> &normals = std::vector<float>(), const std::vector<float> &tangents = std::vector<float>());

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~Model();
	private:
		/// <summary>
		/// Loads the model object from a OBJ file.
		/// </summary>
		void LoadFromFile();

		void LoadMaterials(const std::string &filepath, std::vector<Material> *list);

		VertexData *ProcessDataVertex(Vector3 vertex, std::vector<VertexData*> *vertices, std::vector<int> *indices);

		VertexData *DealWithAlreadyProcessedDataVertex(VertexData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<int> *indices, std::vector<VertexData*> *vertices);

		void CalculateTangents(VertexData *v0, VertexData *v1, VertexData *v2, std::vector<Vector2> *textures);

		void LoadToVulkan();

		VkBuffer CreateBuffer(std::vector<float> *data);

		void MemoryTypeFromProperties(uint32_t typeBits, VkFlags reqMask, uint32_t *typeIndex);

		void CreateAabb();
	};
}
