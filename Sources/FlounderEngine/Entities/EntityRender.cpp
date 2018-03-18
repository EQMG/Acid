#include "EntityRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Worlds/Worlds.hpp"
#include "../Materials/Material.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosEntities.hpp"

namespace Flounder
{
	EntityRender::EntityRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_descriptorSet(nullptr),
		m_nullTexture(Texture::Resource("Resources/Undefined.png"))
	{
	}

	EntityRender::~EntityRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void EntityRender::Update()
	{
		auto material = GetGameObject()->GetComponent<Material>();

		if (material == nullptr)
		{
			return;
		}

		// Updates uniforms.
		UbosEntities::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.samples.m_x = material->GetDiffuse()->GetTexture() != nullptr;
		uboObject.samples.m_y = material->GetSurface()->GetTexture() != nullptr;
		uboObject.samples.m_z = material->GetNormal()->GetTexture() != nullptr;
		uboObject.baseColor = *material->GetDiffuse()->GetBaseColor();
		uboObject.surface = Vector4(material->GetSurface()->GetMetallic(), material->GetSurface()->GetRoughness(),
			static_cast<float>(material->GetSurface()->GetIgnoreFog()), static_cast<float>(material->GetSurface()->GetIgnoreLighting()));
		m_uniformObject->Update(&uboObject);
	}

	void EntityRender::Load(LoadedValue *value)
	{
	}

	void EntityRender::Write(LoadedValue *value)
	{
	}

	void EntityRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();
		auto material = GetGameObject()->GetComponent<Material>();

		if (mesh == nullptr || mesh->GetModel() == nullptr || material == nullptr)
		{
			return;
		}

		//auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		/*if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(*Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
		}

		m_descriptorSet->Update({
			uniformScene,
			m_uniformObject,
			material->GetDiffuse()->GetTexture() == nullptr ? m_nullTexture : material->GetDiffuse()->GetTexture(),
			material->GetSurface()->GetTexture() == nullptr ? m_nullTexture : material->GetSurface()->GetTexture(),
			material->GetNormal()->GetTexture() == nullptr ? m_nullTexture : material->GetNormal()->GetTexture()
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
