#pragma once

#include "Maths/Colour.hpp"
#include "Models/Model.hpp"
#include "Textures/Texture.hpp"
#include "IMaterial.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents the default material shader.
	/// </summary>
	class ACID_EXPORT MaterialDefault :
		public IMaterial
	{
	private:
		bool m_animated;
		Colour m_baseDiffuse;
		std::shared_ptr<Texture> m_diffuseTexture;

		float m_metallic;
		float m_roughness;
		std::shared_ptr<Texture> m_materialTexture;
		std::shared_ptr<Texture> m_normalTexture;

		bool m_castsShadows;
		bool m_ignoreLighting;
		bool m_ignoreFog;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialDefault(const Colour &baseDiffuse = Colour::WHITE, const std::shared_ptr<Texture> &diffuseTexture = nullptr,
						const float &metallic = 0.0f, const float &roughness = 0.0f, const std::shared_ptr<Texture> &materialTexture = nullptr, const std::shared_ptr<Texture> &normalTexture = nullptr,
						const bool &castsShadows = true, const bool &ignoreLighting = false, const bool &ignoreFog = false);

		~MaterialDefault();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::vector<PipelineDefine> GetDefines();

		Colour GetBaseDiffuse() const { return m_baseDiffuse; }

		void SetBaseDiffuse(const Colour &baseDiffuse) { m_baseDiffuse = baseDiffuse; }

		std::shared_ptr<Texture> GetDiffuseTexture() const { return m_diffuseTexture; }

		void SetDiffuseTexture(const std::shared_ptr<Texture> &diffuseTexture) { m_diffuseTexture = diffuseTexture; }

		void TrySetDiffuseTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_diffuseTexture = Texture::Resource(filename);
			}
		}

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		std::shared_ptr<Texture> GetMaterialTexture() const { return m_materialTexture; }

		void SetMaterialTexture(const std::shared_ptr<Texture> &materialTexture) { m_materialTexture = materialTexture; }

		void TrySetMaterialTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_materialTexture = Texture::Resource(filename);
			}
		}

		std::shared_ptr<Texture> GetNormalTexture() const { return m_normalTexture; }

		void SetNormalTexture(const std::shared_ptr<Texture> &normalTexture) { m_normalTexture = normalTexture; }

		void TrySetNormalTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_normalTexture = Texture::Resource(filename);
			}
		}

		bool CastsShadows() const { return m_castsShadows; }

		void SetCastsShadows(const bool &castsShadows) { m_castsShadows = castsShadows; }

		bool IsIgnoringLighting() const { return m_ignoreLighting; }

		void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

		bool IsIgnoringFog() const { return m_ignoreFog; }

		void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
