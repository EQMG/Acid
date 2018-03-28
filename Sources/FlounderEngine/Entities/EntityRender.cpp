#include "EntityRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Meshes/Animations/MeshAnimated.hpp"
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

		std::vector<Matrix4> jointTransforms = {};
		auto meshAnimated = GetGameObject()->GetComponent<MeshAnimated>();

		if (meshAnimated != nullptr)
		{
			auto jointMatrices = meshAnimated->GetJointTransforms();

			for (unsigned int i = 0; i < jointMatrices.size(); i++)
			{
				jointTransforms.push_back(*jointMatrices.at(i));

				if (jointTransforms.size() >= MAX_JOINTS)
				{
					break;
				}
			}
		}

		// Updates uniforms.
		UbosEntities::UboObject uboObject = {};

		// for (unsigned int i = 0; i < jointTransforms.size(); i++)
		// {
		// 	uboObject.jointTransforms[i] = jointTransforms.at(i);
		// }

		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.baseColor = *material->GetDiffuse()->GetBaseColor();
		uboObject.metallic = material->GetSurface()->GetMetallic();
		uboObject.roughness = material->GetSurface()->GetRoughness();
		uboObject.ignoreFog = static_cast<float>(material->GetSurface()->GetIgnoreFog());
		uboObject.ignoreLighting = static_cast<float>(material->GetSurface()->GetIgnoreLighting());
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
