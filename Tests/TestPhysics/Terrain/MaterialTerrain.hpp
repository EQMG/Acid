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
		std::shared_ptr<PipelineMaterial> m_material;

		std::shared_ptr<Texture> m_textureR;
	public:
		MaterialTerrain(std::shared_ptr<Texture> textureR = nullptr);

		~MaterialTerrain();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Texture> GetTextureR() const { return m_textureR; }

		void SetTextureR(std::shared_ptr<Texture> textureR) { m_textureR = textureR; }

		void TrySetTextureR(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_textureR = Texture::Resource(filename);
			}
		}

		std::shared_ptr<PipelineMaterial> GetMaterial() const override { return m_material; }
	};
}
