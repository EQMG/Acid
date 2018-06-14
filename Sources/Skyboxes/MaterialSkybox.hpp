#pragma once

#include "Textures/Cubemap.hpp"
#include "Materials/IMaterial.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a skybox material shader.
	/// </summary>
	class FL_EXPORT MaterialSkybox :
		public IMaterial
	{
	private:
		std::shared_ptr<Cubemap> m_cubemap;
		bool m_enableFog;

		float m_blend;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialSkybox(std::shared_ptr<Cubemap> cubemap = nullptr, const bool &enableFog = true);

		~MaterialSkybox();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::string GetName() const override { return "MaterialSkybox"; };

		std::shared_ptr<Cubemap> GetCubemap() const { return m_cubemap; }

		void SetCubemap(std::shared_ptr<Cubemap> cubemap) { m_cubemap = cubemap; }

		void TrySetCubemap(const std::string &filename, const std::string &fileExt)
		{
			if (!filename.empty() && !fileExt.empty())
			{
				m_cubemap = Cubemap::Resource(filename, fileExt);
			}
		}

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
