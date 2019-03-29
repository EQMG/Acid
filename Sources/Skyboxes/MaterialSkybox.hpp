#pragma once

#include "Materials/Material.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Images/ImageCube.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a skybox material shader.
	/// </summary>
	class ACID_EXPORT MaterialSkybox :
		public Material
	{
	public:
		explicit MaterialSkybox(std::shared_ptr<ImageCube> cubemap = nullptr, const Colour &skyColour = Colour::White);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		const std::shared_ptr<ImageCube> &GetCubemap() const { return m_cubemap; }

		void SetCubemap(const std::shared_ptr<ImageCube> &cubemap) { m_cubemap = cubemap; }

		const Colour &GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { m_skyColour = skyColour; }

		const float &GetBlend() const { return m_blend; }

		void SetBlend(const float &blend) { m_blend = blend; }

		const Colour &GetFogColour() const { return m_fogColour; }

		void SetFogColour(const Colour &fogColour) { m_fogColour = fogColour; }

		const Vector2 &GetFogLimits() const { return m_fogLimits; }

		void SetFogLimits(const Vector2 &fogLimits) { m_fogLimits = fogLimits; }
	private:
		std::shared_ptr<ImageCube> m_cubemap;
		Colour m_skyColour;
		float m_blend;
		Colour m_fogColour;
		Vector2 m_fogLimits;
	};
}
