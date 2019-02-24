#pragma once

#include "Lights/Fog.hpp"
#include "Models/Model.hpp"
#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Textures/Cubemap.hpp"

namespace acid
{
	class ACID_EXPORT RendererDeferred :
		public RenderPipeline
	{
	public:
		enum class Type
		{
			Ibl, Simple
		};

		explicit RendererDeferred(const Pipeline::Stage &pipelineStage, const Type &type);

		void Render(const CommandBuffer &commandBuffer) override;

		const Fog &GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { m_fog = fog; }
	private:
		struct DeferredLight
		{
			Colour m_colour;
			Vector3 m_position;
			float m_radius{};
		};

		std::vector<Shader::Define> GetDefines();

		static std::shared_ptr<Texture> ComputeBrdf(const uint32_t &size);

		static std::shared_ptr<Cubemap> ComputeIbl(const std::shared_ptr<Cubemap> &source);

		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformScene;
		StorageHandler m_storageLights;

		Type m_type;

		PipelineGraphics m_pipeline;
		std::shared_ptr<Model> m_model;

		std::shared_ptr<Texture> m_brdf;

		std::shared_ptr<Cubemap> m_skybox;
		std::shared_ptr<Cubemap> m_ibl;

		Fog m_fog;
	};
}
