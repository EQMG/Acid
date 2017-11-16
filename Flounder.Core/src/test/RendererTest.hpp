#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "TestEntity.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeUboObject;
		static const DescriptorType typeSamplerDiffuse;
		static const DescriptorType typeSamplerSway;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
		std::vector<TestEntity*> m_testEntities;
	public:
		RendererTest(const int &subpass);

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
