#include "MaterialTile.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialTile::MaterialTile(const std::shared_ptr<Texture> &texture) :
		m_texture(texture),
		m_numberOfRows(1),
		m_selectedRow(0),
		m_atlasOffset(Vector2()),
		m_alpha(1.0f),
		m_material(nullptr)
	{
	}

	void MaterialTile::Start()
	{
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Tiles/Tile.vert", "Shaders/Tiles/Tile.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));
	}

	void MaterialTile::Update()
	{
		int32_t numberOfRows = m_texture != nullptr ? m_numberOfRows : 1;
		int32_t column = m_selectedRow % numberOfRows;
		int32_t row = m_selectedRow / numberOfRows;
		m_atlasOffset = Vector2(static_cast<float>(column) / static_cast<float>(numberOfRows), static_cast<float>(row) / static_cast<float>(numberOfRows));
	}

	void MaterialTile::Decode(const Metadata &metadata)
	{
		TrySetTexture(metadata.GetChild<std::string>("Texture"));
	}

	void MaterialTile::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Texture", m_texture == nullptr ? "" : m_texture->GetFilename());
	}

	void MaterialTile::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("atlasOffset", m_atlasOffset);
		uniformObject.Push("atlasRows", static_cast<float>(m_numberOfRows));
		uniformObject.Push("alpha", m_alpha);
	}

	void MaterialTile::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("sampler", m_texture);
	}
}
