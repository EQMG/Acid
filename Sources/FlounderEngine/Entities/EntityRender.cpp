#include "EntityRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Animations/MeshAnimated.hpp"
#include "Materials/MaterialDefault.hpp"
#include "Worlds/Worlds.hpp"

namespace Flounder
{
	EntityRender::EntityRender() :
		Component(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler())
	{
	}

	EntityRender::~EntityRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
	}

	void EntityRender::Update()
	{
		auto materialDefault = GetGameObject()->GetComponent<MaterialDefault>();

		if (materialDefault == nullptr)
		{
			return;
		}

		auto material = materialDefault->GetMaterial();

		// Updates uniforms.
		m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		m_uniformObject->Push("baseColor", *materialDefault->GetBaseColor());
		m_uniformObject->Push("metallic", materialDefault->GetMetallic());
		m_uniformObject->Push("roughness", materialDefault->GetRoughness());
		m_uniformObject->Push("ignoreFog", static_cast<float>(materialDefault->IsIgnoringFog()));
		m_uniformObject->Push("ignoreLighting", static_cast<float>(materialDefault->IsIgnoringLighting()));
	}

	void EntityRender::Load(LoadedValue *value)
	{
	}

	void EntityRender::Write(LoadedValue *value)
	{
	}

	void EntityRender::CmdRender(const VkCommandBuffer &commandBuffer, UniformHandler *uniformScene)
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
		auto mesh = GetGameObject()->GetComponent<Mesh>();
		auto materialDefault = GetGameObject()->GetComponent<MaterialDefault>();

		if (mesh == nullptr || mesh->GetModel() == nullptr || materialDefault == nullptr || materialDefault->GetMaterial() == nullptr)
		{
			return;
		}

		auto material = materialDefault->GetMaterial();

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", uniformScene);
		m_descriptorSet->Push("UboObject", m_uniformObject);
		m_descriptorSet->Push("samplerDiffuse", materialDefault->GetDiffuseTexture());
		m_descriptorSet->Push("samplerMaterial", materialDefault->GetMaterialTexture());
		m_descriptorSet->Push("samplerNormal", materialDefault->GetNormalTexture());
		bool descriptorsSet = m_descriptorSet->Update(*material->GetPipeline());

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		material->CmdRender(commandBuffer, mesh->GetModel(), m_descriptorSet->GetDescriptorSet());
	}
}
