#include "MeshRender.hpp"

#include "Materials/Material.hpp"
#include "Physics/Rigidbody.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
void MeshRender::Start()
{
}

void MeshRender::Update()
{
	auto material{GetParent()->GetComponent<Material>()};

	if (material == nullptr)
	{
		return;
	}

	// Updates uniforms.
	material->PushUniforms(m_uniformObject);
}

bool MeshRender::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage)
{
	// Checks if the mesh is in view.
	if (auto rigidbody{GetParent()->GetComponent<Rigidbody>()}; rigidbody != nullptr)
	{
		if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
		{
			return false;
		}
	}

	// Gets required components.
	auto material{GetParent()->GetComponent<Material>()};
	auto mesh{GetParent()->GetComponent<Mesh>()};

	if (material == nullptr || mesh == nullptr)
	{
		return false;
	}

	auto meshModel{mesh->GetModel()};
	auto materialPipeline{material->GetPipelineMaterial()};

	if (meshModel == nullptr || materialPipeline == nullptr || materialPipeline->GetStage() != pipelineStage)
	{
		return false;
	}

	// Binds the material pipeline.

	if (!materialPipeline->BindPipeline(commandBuffer))
	{
		return false;
	}

	auto &pipeline{*materialPipeline->GetPipeline()};

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	material->PushDescriptors(m_descriptorSet);

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return meshModel->CmdRender(commandBuffer);
}

bool MeshRender::operator<(const MeshRender &other) const
{
	auto camera{Scenes::Get()->GetCamera()};

	auto thisDistance2{(camera->GetPosition() - GetParent()->GetWorldTransform().GetPosition()).LengthSquared()};
	auto otherDistance2{(camera->GetPosition() - other.GetParent()->GetWorldTransform().GetPosition()).LengthSquared()};

	return thisDistance2 > otherDistance2;
}

const Metadata &operator>>(const Metadata &metadata, MeshRender &meshRender)
{
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const MeshRender &meshRender)
{
	return metadata;
}
}
