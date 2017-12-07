#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Models/Model.hpp"

namespace Flounder
{
	class RendererDeferred :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeUboLights;
		static const DescriptorType typeSamplerDepth;
		static const DescriptorType typeSamplerColour;
		static const DescriptorType typeSamplerNormal;
		static const DescriptorType typeSamplerMaterial;
		static const DescriptorType typeSamplerShadows;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		UniformBuffer *m_uniformLights;
		Pipeline *m_pipeline;
		Model *m_model;
	public:
		RendererDeferred(const int &subpass);

		~RendererDeferred();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
