#pragma once

#include "Maths/Colour.hpp"
#include "Models/Model.hpp"
#include "Graphics/Images/Image2d.hpp"
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
	explicit MaterialDefault(const Colour &baseDiffuse = Colour::White, std::shared_ptr<Image2d> imageDiffuse = nullptr, float metallic = 0.0f,
		float roughness = 0.0f, std::shared_ptr<Image2d> imageMaterial = nullptr, std::shared_ptr<Image2d> imageNormal = nullptr, bool castsShadows = true,
		bool ignoreLighting = false, bool ignoreFog = false);

	void Start() override;

	void Update() override;

	void PushUniforms(UniformHandler &uniformObject) override;

	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	const Colour &GetBaseDiffuse() const { return m_baseDiffuse; }

	void SetBaseDiffuse(const Colour &baseDiffuse) { m_baseDiffuse = baseDiffuse; }

	const std::shared_ptr<Image2d> &GetImageDiffuse() const { return m_imageDiffuse; }

	void SetImageDiffuse(const std::shared_ptr<Image2d> &imageDiffuse) { m_imageDiffuse = imageDiffuse; }

	float GetMetallic() const { return m_metallic; }

	void SetMetallic(float metallic) { m_metallic = metallic; }

	float GetRoughness() const { return m_roughness; }

	void SetRoughness(float roughness) { m_roughness = roughness; }

	const std::shared_ptr<Image2d> &GetImageMaterial() const { return m_imageMaterial; }

	void SetImageMaterial(const std::shared_ptr<Image2d> &imageMaterial) { m_imageMaterial = imageMaterial; }

	const std::shared_ptr<Image2d> &GetImageNormal() const { return m_imageNormal; }

	void SetImageNormal(const std::shared_ptr<Image2d> &imageNormal) { m_imageNormal = imageNormal; }

	bool IsCastsShadows() const { return m_castsShadows; }

	void SetCastsShadows(bool castsShadows) { m_castsShadows = castsShadows; }

	bool IsIgnoringLighting() const { return m_ignoreLighting; }

	void SetIgnoreLighting(bool ignoreLighting) { m_ignoreLighting = ignoreLighting; }

	bool IsIgnoringFog() const { return m_ignoreFog; }

	void SetIgnoreFog(bool ignoreFog) { m_ignoreFog = ignoreFog; }

	friend const Node &operator>>(const Node &node, MaterialDefault &material);

	friend Node &operator<<(Node &node, const MaterialDefault &material);

private:
	std::vector<Shader::Define> GetDefines() const;

	bool m_animated = false;
	Colour m_baseDiffuse;
	std::shared_ptr<Image2d> m_imageDiffuse;

	float m_metallic;
	float m_roughness;
	std::shared_ptr<Image2d> m_imageMaterial;
	std::shared_ptr<Image2d> m_imageNormal;

	bool m_castsShadows;
	bool m_ignoreLighting;
	bool m_ignoreFog;
};
}
