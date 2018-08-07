#pragma once

#include "Materials/IMaterial.hpp"
#include "Maths/Colour.hpp"
#include "Textures/Cubemap.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a skybox material shader.
	/// </summary>
	class ACID_EXPORT MaterialSkybox :
		public IMaterial
	{
	private:
		std::shared_ptr<Cubemap> m_cubemap;
		Colour m_skyColour;
		float m_blend;
		Colour m_fogColour;
		Vector2 m_fogLimits;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialSkybox(std::shared_ptr<Cubemap> cubemap = nullptr, const Colour &skyColour = Colour::WHITE);

		~MaterialSkybox();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Cubemap> GetCubemap() const { return m_cubemap; }

		void SetCubemap(std::shared_ptr<Cubemap> cubemap) { m_cubemap = cubemap; }

		void TrySetCubemap(const std::string &filename, const std::string &fileExt)
		{
			if (!filename.empty() && !fileExt.empty())
			{
				m_cubemap = Cubemap::Resource(filename, fileExt);
			}
		}

		Colour GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { m_skyColour = skyColour; }

		float GetBlend() const { return m_blend; }

		void SetBlend(const float &blend) { m_blend = blend; }

		Colour GetFogColour() const { return m_fogColour; }

		void SetFogColour(const Colour &fogColour) { m_fogColour = fogColour; }

		Vector2 GetFogLimits() const { return m_fogLimits; }

		void SetFogLimits(const Vector2 &fogLimits) { m_fogLimits = fogLimits; }

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
