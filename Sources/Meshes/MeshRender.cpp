#include "MeshRender.hpp"

#include "Materials/IMaterial.hpp"
#include "Physics/Rigidbody.hpp"
#include "Objects/GameObject.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	MeshRender::MeshRender() :
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler())
	{
	}

	void MeshRender::Start()
	{
	}

	void MeshRender::Update()
	{
		auto material = GetGameObject()->GetComponent<IMaterial>();

		if (material == nullptr)
		{
			return;
		}

		// Updates uniforms.
		material->PushUniforms(m_uniformObject);
	}

	bool MeshRender::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const GraphicsStage &graphicsStage)
	{
		// Checks if the mesh is in view.
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody != nullptr)
		{
			if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return false;
			}
		}

		// Gets required components.
		auto material = GetGameObject()->GetComponent<IMaterial>();
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (material == nullptr || mesh == nullptr)
		{
			return false;
		}

		auto meshModel = mesh->GetModel();
		auto materialPipeline = material->GetMaterialPipeline();

		if (meshModel == nullptr || materialPipeline->GetGraphicsStage() != graphicsStage)
		{
			return false;
		}

		// Binds the material pipeline.
		bool bindSuccess = materialPipeline->BindPipeline(commandBuffer);

		if (!bindSuccess)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		material->PushDescriptors(m_descriptorSet);
		bool updateSuccess = m_descriptorSet.Update(*materialPipeline->GetPipeline());

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		meshModel->CmdRender(commandBuffer);
		return true;
	}

	void MeshRender::Decode(const Metadata &metadata)
	{
	}

	void MeshRender::Encode(Metadata &metadata) const
	{
	}

	bool MeshRender::operator<(const MeshRender &other) const
	{
		auto camera = Scenes::Get()->GetCamera();

		float thisDistance2 = (camera->GetPosition() - GetGameObject()->GetWorldTransform().GetPosition()).LengthSquared();
		float otherDistance2 = (camera->GetPosition() - other.GetGameObject()->GetWorldTransform().GetPosition()).LengthSquared();

		return thisDistance2 > otherDistance2;
	}
}
