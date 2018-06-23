#pragma once

#include "Lights/Fog.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Textures/Cubemap.hpp"

namespace fl
{
	class FL_EXPORT RendererDeferred :
		public IRenderer
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformScene;

		Pipeline m_pipeline;
		std::shared_ptr<Model> m_model;

		std::shared_ptr<Texture> m_brdflut;

		Fog m_fog;
	public:
		static const int MAX_LIGHTS;

		RendererDeferred(const GraphicsStage &graphicsStage);

		~RendererDeferred();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Fog GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { m_fog = fog; }
	};
}
