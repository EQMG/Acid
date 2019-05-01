#pragma once

#include <Materials/Material.hpp>
#include <Renderer/Images/Image2d.hpp>

using namespace acid;

namespace test
{
class MaterialTerrain :
	public Material
{
public:
	explicit MaterialTerrain(std::shared_ptr<Image2d> imageR = nullptr, std::shared_ptr<Image2d> imageG = nullptr);

	void Start() override;

	void Update() override;

	void PushUniforms(UniformHandler &uniformObject) override;

	void PushDescriptors(DescriptorsHandler &descriptorSet) override;

	const std::shared_ptr<Image2d> &GetImageR() const { return m_imageR; }

	void SetImageR(const std::shared_ptr<Image2d> &imageR) { m_imageR = imageR; }

	const std::shared_ptr<Image2d> &GetImageG() const { return m_imageG; }

	void SetImageG(const std::shared_ptr<Image2d> &imageG) { m_imageG = imageG; }

	friend const Metadata &operator>>(const Metadata &metadata, MaterialTerrain &material);

	friend Metadata &operator<<(Metadata &metadata, const MaterialTerrain &material);

private:
	std::shared_ptr<Image2d> m_imageR;
	std::shared_ptr<Image2d> m_imageG;
};
}
