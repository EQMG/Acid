#include "MeshRender.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	MeshRender::MeshRender() :
		Component(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler())
	{
	}

	MeshRender::~MeshRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
	}

	void MeshRender::Update()
	{
		auto material = GetGameObject()->GetComponent<IMaterial>();

		if (material == nullptr)
		{
			return;
		}

		auto uniformScene = material->GetMaterial()->GetUniformScene();
		uniformScene->Push("projection", *Scenes::Get()->GetCamera()->GetProjectionMatrix());
		uniformScene->Push("view", *Scenes::Get()->GetCamera()->GetViewMatrix());

		// Updates uniforms.
		material->PushUniforms(m_uniformObject);
	}

	void MeshRender::CmdRender(const CommandBuffer &commandBuffer)
	{
		// Checks if the mesh is in view.
		/*auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(*Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

		// Gets required components.
		auto material = GetGameObject()->GetComponent<IMaterial>();
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (material == nullptr || mesh == nullptr)
		{
			return;
		}

		// Binds the material pipeline.
		material->GetMaterial()->GetPipeline()->BindPipeline(commandBuffer);

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", material->GetMaterial()->GetUniformScene());
		m_descriptorSet->Push("UboObject", m_uniformObject);
		material->PushDescriptors(m_descriptorSet);
		bool descriptorsSet = m_descriptorSet->Update(*material->GetMaterial()->GetPipeline());

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}

	void MeshRender::Load(LoadedValue *value)
	{
	}

	void MeshRender::Write(LoadedValue *value)
	{
	}
}
