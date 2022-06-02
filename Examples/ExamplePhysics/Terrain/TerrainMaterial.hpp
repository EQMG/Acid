#pragma once

#include <Materials/Material.hpp>
#include <Graphics/Images/Image2d.hpp>

using namespace acid;

namespace test {
class TerrainMaterial : public Material::Registrar<TerrainMaterial> {
	inline static const bool Registered = Register("terrain");
public:
	explicit TerrainMaterial(std::shared_ptr<Image2d> imageR = nullptr, std::shared_ptr<Image2d> imageG = nullptr);

	void CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) override;
	void PushUniforms(UniformHandler &uniformObject, const Transform *transform) override;
	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	friend const Node &operator>>(const Node &node, TerrainMaterial &material);
	friend Node &operator<<(Node &node, const TerrainMaterial &material);

	const std::shared_ptr<Image2d> &GetImageR() const { return imageR; }
	void SetImageR(std::shared_ptr<Image2d> imageR) { this->imageR = std::move(imageR); }

	const std::shared_ptr<Image2d> &GetImageG() const { return imageG; }
	void SetImageG(std::shared_ptr<Image2d> imageG) { this->imageG = std::move(imageG); }

private:
	std::shared_ptr<Image2d> imageR;
	std::shared_ptr<Image2d> imageG;
};
}
