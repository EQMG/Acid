#include "MeshRender.hpp"

namespace fl
{
	MeshRender::MeshRender() :
		IComponent(),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler())
	{
	}

	MeshRender::~MeshRender()
	{
	}

	void MeshRender::Start()
	{
	}

	void MeshRender::Update()
	{
		auto material = GetGameObject()->GetComponent<IMaterial>(false);

		if (material == nullptr)
		{
			return;
		}

		// Updates uniforms.
		material->PushUniforms(m_uniformObject);
	}

	void MeshRender::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene)
	{
		// Checks if the mesh is in view.
		/*auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

		// Gets required components.
		auto material = GetGameObject()->GetComponent<IMaterial>(false);
		auto mesh = GetGameObject()->GetComponent<Mesh>(false);

		if (material == nullptr || mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Binds the material pipeline.
		material->GetMaterial()->GetPipeline().BindPipeline(commandBuffer);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		material->PushDescriptors(m_descriptorSet);
		bool updateSuccess = m_descriptorSet.Update(material->GetMaterial()->GetPipeline());

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}

	void MeshRender::Load(LoadedValue *value)
	{
	}

	void MeshRender::Write(LoadedValue *destination)
	{
	}
}
