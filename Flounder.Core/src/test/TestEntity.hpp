#pragma once

#include "../models/Model.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	class TestEntity
	{
	public:
		UniformBuffer *m_uniformObject;

		Model *m_model;
		Texture *m_diffuse;
		Texture *m_swapMap;

		Vector3 m_position;
		Vector3 m_rotation;

		TestEntity(const Vector3 &position, const Vector3 &rotation, Model *model, Texture *diffuse, Texture *swapMap);

		~TestEntity();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);
	};
}
