#pragma once

#include "Materials/Material.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Textures/Cubemap.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a skybox material shader.
	/// </summary>
	class ACID_EXPORT MaterialSkybox :
		public Material
	{
	private:
		std::shared_ptr<Cubemap> m_cubemap;
		Colour m_skyColour;
		float m_blend;
		Colour m_fogColour;
		Vector2 m_fogLimits;

		std::shared_ptr<PipelineMaterial> m_pipelineMaterial;
	public:
		explicit MaterialSkybox(const std::shared_ptr<Cubemap> &cubemap = nullptr, const Colour &skyColour = Colour::WHITE);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Cubemap> GetCubemap() const { return m_cubemap; }

		void SetCubemap(const std::shared_ptr<Cubemap> &cubemap) { m_cubemap = cubemap; }

		Colour GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { m_skyColour = skyColour; }

		float GetBlend() const { return m_blend; }

		void SetBlend(const float &blend) { m_blend = blend; }

		Colour GetFogColour() const { return m_fogColour; }

		void SetFogColour(const Colour &fogColour) { m_fogColour = fogColour; }

		Vector2 GetFogLimits() const { return m_fogLimits; }

		void SetFogLimits(const Vector2 &fogLimits) { m_fogLimits = fogLimits; }

		std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const override { return m_pipelineMaterial; }
	};
}
