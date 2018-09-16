#pragma once

#include <Maths/Colour.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialTerrain :
		public IMaterial
	{
	private:
		std::shared_ptr<Texture> m_textureR;
		std::shared_ptr<Texture> m_textureG;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		MaterialTerrain(const std::shared_ptr<Texture> &textureR = nullptr, const std::shared_ptr<Texture> &textureG = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Texture> GetTextureR() const { return m_textureR; }

		void SetTextureR(const std::shared_ptr<Texture> &textureR) { m_textureR = textureR; }

		void TrySetTextureR(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_textureR = Texture::Resource(filename);
			}
		}

		std::shared_ptr<Texture> GetTextureG() const { return m_textureG; }

		void SetTextureG(const std::shared_ptr<Texture> &textureG) { m_textureG = textureG; }

		void TrySetTextureG(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_textureG = Texture::Resource(filename);
			}
		}

		std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const override { return m_material; }
	};
}
