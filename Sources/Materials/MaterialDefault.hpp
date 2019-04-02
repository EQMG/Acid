#pragma once

#include "Maths/Colour.hpp"
#include "Models/Model.hpp"
#include "Renderer/Images/Image2d.hpp"
#include "Material.hpp"

namespace acid
{
/**
 * @brief Class that represents the default material shader.
 */
class ACID_EXPORT MaterialDefault :
	public Material
{
public:
	explicit MaterialDefault(const Colour &baseDiffuse = Colour::White, std::shared_ptr<Image2d> diffuseTexture = nullptr, const float &metallic = 0.0f,
		const float &roughness = 0.0f, std::shared_ptr<Image2d> materialTexture = nullptr, std::shared_ptr<Image2d> normalTexture = nullptr, const bool &castsShadows = true,
		const bool &ignoreLighting = false, const bool &ignoreFog = false);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	void PushUniforms(UniformHandler &uniformObject) override;

	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	const Colour &GetBaseDiffuse() const { return m_baseDiffuse; }

	void SetBaseDiffuse(const Colour &baseDiffuse) { m_baseDiffuse = baseDiffuse; }

	const std::shared_ptr<Image2d> &GetDiffuseTexture() const { return m_diffuseTexture; }

	void SetDiffuseTexture(const std::shared_ptr<Image2d> &diffuseTexture) { m_diffuseTexture = diffuseTexture; }

	const float &GetMetallic() const { return m_metallic; }

	void SetMetallic(const float &metallic) { m_metallic = metallic; }

	const float &GetRoughness() const { return m_roughness; }

	void SetRoughness(const float &roughness) { m_roughness = roughness; }

	const std::shared_ptr<Image2d> &GetMaterialTexture() const { return m_materialTexture; }

	void SetMaterialTexture(const std::shared_ptr<Image2d> &materialTexture) { m_materialTexture = materialTexture; }

	const std::shared_ptr<Image2d> &GetNormalTexture() const { return m_normalTexture; }

	void SetNormalTexture(const std::shared_ptr<Image2d> &normalTexture) { m_normalTexture = normalTexture; }

	const bool &IsCastsShadows() const { return m_castsShadows; }

	void SetCastsShadows(const bool &castsShadows) { m_castsShadows = castsShadows; }

	const bool &IsIgnoringLighting() const { return m_ignoreLighting; }

	void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

	const bool &IsIgnoringFog() const { return m_ignoreFog; }

	void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

private:
	std::vector<Shader::Define> GetDefines() const;

	bool m_animated;
	Colour m_baseDiffuse;
	std::shared_ptr<Image2d> m_diffuseTexture;

	float m_metallic;
	float m_roughness;
	std::shared_ptr<Image2d> m_materialTexture;
	std::shared_ptr<Image2d> m_normalTexture;

	bool m_castsShadows;
	bool m_ignoreLighting;
	bool m_ignoreFog;
};
}
