#include "EntityRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
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
		m_nullTexture(Texture::Resource(""))
	{
	}

	EntityRender::~EntityRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void EntityRender::Update()
	{
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
			material->GetTextureDiffuse() == nullptr ? m_nullTexture : material->GetTextureDiffuse(),
			material->GetTextureMaterial() == nullptr ? m_nullTexture : material->GetTextureMaterial(),
			material->GetTextureNormal() == nullptr ? m_nullTexture : material->GetTextureNormal()
		});

		// Updates uniforms.
		UbosEntities::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.samples.m_x = material->GetTextureDiffuse() != nullptr;
		uboObject.samples.m_y = material->GetTextureMaterial() != nullptr;
		uboObject.samples.m_z = material->GetTextureNormal() != nullptr;
		uboObject.baseColor = *material->GetBaseColor();
		uboObject.surface = Vector4(material->GetMetallic(), material->GetRoughness(),
			static_cast<float>(material->GetIgnoreFog()), static_cast<float>(material->GetIgnoreLighting()));
		m_uniformObject->Update(&uboObject);

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
