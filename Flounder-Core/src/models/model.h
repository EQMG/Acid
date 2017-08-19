#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../devices/display.h"
#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"
#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../physics/aabb.h"

#include "material.h"
#include "vertexdata.h"

namespace flounder
{
	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class model
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
		model(const std::string &file);

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="indices"> The model indices. </param>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="textures"> The model textures. </param>
		/// <param name="normals"> The model normals. </param>
		/// <param name="tangents"> The model tangents. </param>
		model(std::vector<int> *indices, std::vector<float> *vertices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *tangents);

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~model();
	private:
		/// <summary>
		/// Loads the model object from a OBJ file.
		/// </summary>
		void loadFromFile();

		void loadMaterials(const std::string &filepath, std::vector<material> *list);

		vertexdata *processDataVertex(vector3 vertex, std::vector<vertexdata*> *vertices, std::vector<int> *indices);

		vertexdata *dealWithAlreadyProcessedDataVertex(vertexdata *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<int> *indices, std::vector<vertexdata*> *vertices);

		void calculateTangents(vertexdata *v0, vertexdata *v1, vertexdata *v2, std::vector<vector2> *textures);

		void loadToVulkan();

		VkBuffer createBuffer(std::vector<float> *data);

		void memoryTypeFromProperties(uint32_t typeBits, VkFlags reqMask, uint32_t *typeIndex);

		void createAABB();
	};
}
