#pragma once

#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"
#include "../models/Model.hpp"
#include "../renderer/pipelines/Pipeline.hpp"

namespace Flounder
{
	class TestEntity
	{
	public:
		UniformBuffer m_uniformObject;
		Model m_model;
		Texture m_diffuse;
		Texture m_swapMap;

		TestEntity();

		~TestEntity();

		void Create();

		void Cleanup();

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);
	};
}
