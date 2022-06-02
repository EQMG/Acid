#pragma once

#include "Materials/Material.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Graphics/Images/ImageCube.hpp"

namespace acid {
/**
 * @brief Class that represents a skybox material shader.
 */
class ACID_SHIM_EXPORT SkyboxMaterial : public Material::Registrar<SkyboxMaterial> {
	inline static const bool Registered = Register("skybox");
public:
	explicit SkyboxMaterial(std::shared_ptr<ImageCube> image = nullptr, const Colour &baseColour = Colour::White);

	void CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) override;
	void PushUniforms(UniformHandler &uniformObject, const Transform *transform) override;
	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	const std::shared_ptr<ImageCube> &GetImage() const { return image; }
	void SetImage(const std::shared_ptr<ImageCube> &image) { this->image = image; }

	const Colour &GetBaseColour() const { return baseColour; }
	void SetBaseColour(const Colour &baseColour) { this->baseColour = baseColour; }

	float GetBlend() const { return blend; }
	void SetBlend(float blend) { this->blend = blend; }

	const Colour &GetFogColour() const { return fogColour; }
	void SetFogColour(const Colour &fogColour) { this->fogColour = fogColour; }

	const Vector2f &GetFogLimits() const { return fogLimits; }
	void SetFogLimits(const Vector2f &fogLimits) { this->fogLimits = fogLimits; }

	friend const Node &operator>>(const Node &node, SkyboxMaterial &material);
	friend Node &operator<<(Node &node, const SkyboxMaterial &material);

private:
	std::shared_ptr<ImageCube> image;
	Colour baseColour;
	float blend;
	Colour fogColour;
	Vector2f fogLimits;
};
}
