#pragma once

#include <Materials/Material.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialTerrain :
		public Material
	{
	private:
		std::shared_ptr<Texture> m_textureR;
		std::shared_ptr<Texture> m_textureG;
	public:
		explicit MaterialTerrain(const std::shared_ptr<Texture> &textureR = nullptr, const std::shared_ptr<Texture> &textureG = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		const std::shared_ptr<Texture> &GetTextureR() const { return m_textureR; }

		void SetTextureR(const std::shared_ptr<Texture> &textureR) { m_textureR = textureR; }

		const std::shared_ptr<Texture> &GetTextureG() const { return m_textureG; }

		void SetTextureG(const std::shared_ptr<Texture> &textureG) { m_textureG = textureG; }
	};
}
