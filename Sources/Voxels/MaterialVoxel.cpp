#include "MaterialVoxel.hpp"

namespace fl
{
	MaterialVoxel::MaterialVoxel() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Voxels/Voxel.vert", "Resources/Shaders/Voxels/Voxel.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	MaterialVoxel::~MaterialVoxel()
	{
	}

	void MaterialVoxel::Update()
	{
	}

	void MaterialVoxel::Load(LoadedValue *value)
	{
	}

	void MaterialVoxel::Write(LoadedValue *destination)
	{
	}

	void MaterialVoxel::PushUniforms(UniformHandler *uniformObject)
	{
		uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
	}

	void MaterialVoxel::PushDescriptors(DescriptorsHandler *descriptorSet)
	{
	}
}
