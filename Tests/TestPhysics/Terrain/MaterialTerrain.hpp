#pragma once

#include <Materials/Material.hpp>
#include <Images/Image2d.hpp>

using namespace acid;

namespace test
{
	class MaterialTerrain :
		public Material
	{
	public:
		explicit MaterialTerrain(std::shared_ptr<Image2d> textureR = nullptr, std::shared_ptr<Image2d> textureG = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		const std::shared_ptr<Image2d> &GetTextureR() const { return m_textureR; }

		void SetTextureR(const std::shared_ptr<Image2d> &textureR) { m_textureR = textureR; }

		const std::shared_ptr<Image2d> &GetTextureG() const { return m_textureG; }

		void SetTextureG(const std::shared_ptr<Image2d> &textureG) { m_textureG = textureG; }
	private:
		std::shared_ptr<Image2d> m_textureR;
		std::shared_ptr<Image2d> m_textureG;
	};
}
