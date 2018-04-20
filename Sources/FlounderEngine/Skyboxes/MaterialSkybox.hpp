#pragma once

#include "Maths/Colour.hpp"
#include "Models/Model.hpp"
#include "Textures/Cubemap.hpp"
#include "Materials/IMaterial.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents the default material shader.
	/// </summary>
	class F_EXPORT MaterialSkybox :
		public IMaterial
	{
	private:
		Cubemap *m_cubemap;
		bool m_enableFog;

		float m_blend;

		PipelineMaterial *m_material;
	public:
		MaterialSkybox(Cubemap *cubemap = nullptr, const bool &enableFog = true);

		~MaterialSkybox();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler *uniformObject) override;

		void PushDescriptors(DescriptorsHandler *descriptorSet) override;

		std::string GetName() const override { return "MaterialSkybox"; };

		std::vector<Define> GetDefines();

		Cubemap *GetCubemap() const { return m_cubemap; }

		void SetCubemap(Cubemap *cubemap) { m_cubemap = cubemap; }

		void TrySetCubemap(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_cubemap = Cubemap::Resource(filename, ".png");
			}
		}

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }

		PipelineMaterial *GetMaterial() const override { return m_material; }
	};
}
