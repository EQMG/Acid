#pragma once

#include <Maths/Vector2.hpp>
#include <Materials/IMaterial.hpp>
#include <Textures/Texture.hpp>

using namespace acid;

namespace test
{
	class MaterialTile :
		public IMaterial
	{
	private:
		std::shared_ptr<Texture> m_texture;
		uint32_t m_numberOfRows;
		uint32_t m_selectedRow;

		Vector2 m_atlasOffset;
		float m_alpha;

		std::shared_ptr<PipelineMaterial> m_material;
	public:
		explicit MaterialTile(const std::shared_ptr<Texture> &texture = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void PushUniforms(UniformHandler &uniformObject) override;

		void PushDescriptors(DescriptorsHandler &descriptorSet) override;

		std::shared_ptr<Texture> GetTexture() const { return m_texture; }

		void SetTexture(const std::shared_ptr<Texture> &texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}

		uint32_t GetNumberOfRows() const { return m_numberOfRows; }

		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		uint32_t GetSelectedRow() const { return m_selectedRow; }

		void SetSelectedRow(const uint32_t &selectedRow) { m_selectedRow = selectedRow; }

		Vector2 GetAtlasOffset() const { return m_atlasOffset; }

		float GetAlpha() const { return m_alpha; }

		void SetAlpha(const float &alpha) { m_alpha = alpha; }

		std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const override { return m_material; }
	};
}
