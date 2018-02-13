#pragma once

#include "../Maths/Colour.hpp"
#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector3.hpp"
#include "../Models/Model.hpp"
#include "../Physics/Aabb.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT Water
	{
	private:
		UniformBuffer *m_uniformObject;

		Model *m_model;

		Colour *m_colour;

		Transform *m_transform;
		Vector3 *m_offset;
		bool m_moved;

		Matrix4 *m_modelMatrix;
		Aabb *m_aabb;
	public:
		static const float SIDE_LENGTH;
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float TEXTURE_SCALE;

		static const Colour WATER_COLOUR;

		Water(const Transform &transform);

		~Water();

		void Update();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

	private:
		void GenerateMesh();

	public:
		Model *GetModel() const { return m_model; }

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) const { m_colour->Set(colour); }

		Transform *GetTransform() const { return m_transform; }

		void SetPosition(const Transform &transform);

		Vector3 *GetOffset() const { return m_offset; }

		Matrix4 *GetModelMatrix() const { return m_modelMatrix; }

		Aabb *GetAabb() const { return m_aabb; }
	};
}
