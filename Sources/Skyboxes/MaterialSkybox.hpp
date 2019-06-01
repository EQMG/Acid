#pragma once

#include "Materials/Material.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Graphics/Images/ImageCube.hpp"

namespace acid
{
/**
 * @brief class  that represents a skybox material shader.
 */
class ACID_EXPORT MaterialSkybox :
	public Material
{
public:
	explicit MaterialSkybox(std::shared_ptr<ImageCube> image = nullptr, const Colour &baseColour = Colour::White);

	void Start() override;

	void Update() override;

	void PushUniforms(UniformHandler &uniformObject) override;

	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	const std::shared_ptr<ImageCube> &GetImage() const { return m_image; }

	void SetImage(const std::shared_ptr<ImageCube> &image) { m_image = image; }

	const Colour &GetBaseColour() const { return m_baseColour; }

	void SetBaseColour(const Colour &baseColour) { m_baseColour = baseColour; }

	const float &GetBlend() const { return m_blend; }

	void SetBlend(const float &blend) { m_blend = blend; }

	const Colour &GetFogColour() const { return m_fogColour; }

	void SetFogColour(const Colour &fogColour) { m_fogColour = fogColour; }

	const Vector2f &GetFogLimits() const { return m_fogLimits; }

	void SetFogLimits(const Vector2f &fogLimits) { m_fogLimits = fogLimits; }

	friend const Metadata &operator>>(const Metadata &metadata, MaterialSkybox &material);

	friend Metadata &operator<<(Metadata &metadata, const MaterialSkybox &material);

private:
	std::shared_ptr<ImageCube> m_image;
	Colour m_baseColour;
	float m_blend;
	Colour m_fogColour;
	Vector2f m_fogLimits;
};
}
