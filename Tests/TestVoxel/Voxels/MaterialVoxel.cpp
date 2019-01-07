#include "MaterialVoxel.hpp"

#include <Scenes/Entity.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialVoxel::MaterialVoxel()
	{
	}
	
	void MaterialVoxel::Start()
	{
		m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineCreate({"Shaders/Voxels/Voxel.vert", "Shaders/Voxels/Voxel.frag"}, {VertexModel::GetVertexInput()},
		    PIPELINE_MODE_MRT, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));
	}

	void MaterialVoxel::Update()
	{
	}

	void MaterialVoxel::Decode(const Metadata &metadata)
	{
	}

	void MaterialVoxel::Encode(Metadata &metadata) const
	{
	}

	void MaterialVoxel::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	}

	void MaterialVoxel::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
