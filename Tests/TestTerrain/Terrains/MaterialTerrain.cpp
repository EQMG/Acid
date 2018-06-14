#include "MaterialTerrain.hpp"

namespace test
{
	MaterialTerrain::MaterialTerrain() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	MaterialTerrain::~MaterialTerrain()
	{
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Load(LoadedValue *value)
	{
	}

	void MaterialTerrain::Write(LoadedValue *destination)
	{
	}

	void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
	}

	void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
