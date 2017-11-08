#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../models/Model.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	class RendererDeferred :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeSamplerColour;
		static const DescriptorType typeSamplerNormal;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
		Texture *m_textureUndefined;
		Model *m_model;
	public:
		RendererDeferred();

		~RendererDeferred();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
