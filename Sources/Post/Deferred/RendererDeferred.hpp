#pragma once

#include "Lights/Fog.hpp"
#include "Models/Model.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Textures/Cubemap.hpp"

namespace acid
{
	enum DeferredModel
	{
		DEFERRED_IBL = 0,
		DEFERRED_SIMPLE = 1
	};

	class ACID_EXPORT RendererDeferred :
		public IRenderer
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformScene;

		DeferredModel m_lightModel;

		Pipeline m_pipeline;
		std::shared_ptr<Model> m_model;

		std::shared_ptr<Texture> m_brdf;

		std::shared_ptr<Cubemap> m_skybox;
		std::shared_ptr<Cubemap> m_ibl;

		Fog m_fog;
	public:
		static const uint32_t MAX_LIGHTS;

		explicit RendererDeferred(const GraphicsStage &graphicsStage, const DeferredModel &lightModel);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Fog GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { m_fog = fog; }
	private:
		std::vector<PipelineDefine> GetDefines();

		static std::shared_ptr<Texture> ComputeBrdf(const uint32_t &size);

		static std::shared_ptr<Cubemap> ComputeIbl(const std::shared_ptr<acid::Cubemap> &source);
	};
}
