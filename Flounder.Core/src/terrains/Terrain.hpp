#pragma once

#include <vector>
#include "../maths/Colour.hpp"
#include "../maths/Matrix4.hpp"
#include "../maths/Vector3.hpp"
#include "../physics/Aabb.hpp"
#include "../models/Model.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"

namespace Flounder
{
	class Terrain
	{
	private:
		UniformBuffer *m_uniformObject;

		Model *m_model;

		Vector3 *m_position;
		Vector3 *m_rotation;
		bool m_moved;

		Matrix4 *m_modelMatrix;
		Aabb *m_aabb;
	public:
		static const float SIDE_LENGTH;
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;

		Terrain(const Vector3 &position, const Vector3 &rotation);

		~Terrain();

		void Update();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);
	private:
		void GenerateMesh();

		void StoreQuad(std::vector<uint32_t> &indices, const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight);

		Vector3 CalculateNormal(const float &x, const float &z);

		Colour CalculateColour(const Vector3 &position, const Vector3 &normal);
	public:
		Model *GetModel() const { return m_model; }

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position);

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation);

		Matrix4 *GetModelMatrix() const { return m_modelMatrix; }

		Aabb *GetAabb() const { return m_aabb; }
	};
}
