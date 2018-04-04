#include "EntityRender.hpp"

#include "../Meshes/Mesh.hpp"
#include "../Meshes/Animations/MeshAnimated.hpp"
#include "../Worlds/Worlds.hpp"
#include "Materials/MaterialDefault.hpp"
#include "UbosEntities.hpp"

namespace Flounder
{
	EntityRender::EntityRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_descriptorSet(nullptr)
	{
	}

	EntityRender::~EntityRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void EntityRender::Update()
	{
		auto material = GetGameObject()->GetComponent<MaterialDefault>();

		if (material == nullptr)
		{
			return;
		}

		/*std::vector<Matrix4> jointTransforms = {};
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
		}*/

		// Updates uniforms.
		UbosEntities::UboObject uboObject = {};

		//for (unsigned int i = 0; i < jointTransforms.size(); i++)
		//{
		//	uboObject.jointTransforms[i] = jointTransforms.at(i);
		//}

		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.baseColor = *material->GetBaseColor();
		uboObject.metallic = material->GetMetallic();
		uboObject.roughness = material->GetRoughness();
		uboObject.ignoreFog = static_cast<float>(material->IsIgnoringFog());
		uboObject.ignoreLighting = static_cast<float>(material->IsIgnoringLighting());
		m_uniformObject->Update(&uboObject);

		//m_uniformObject->UpdateMap("UniformObject", pipeline.GetShaderProgram(), {
		//	{"jointTransforms", jointTransforms.data()},
		//	{"transform", GetGameObject()->GetTransform()->GetWorldMatrix()},
		// 	{"baseColor", *material->GetDiffuse()->GetBaseColor()},
		//	{"metallic", material->GetSurface()->GetMetallic()},
		//	{"roughness", material->GetSurface()->GetRoughness()},
		//	{"ignoreFog", static_cast<float>(material->GetSurface()->GetIgnoreFog())},
		//	{"ignoreLighting", static_cast<float>(material->GetSurface()->GetIgnoreLighting())}
		//});
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
		auto material = GetGameObject()->GetComponent<MaterialDefault>();

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
		/*if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
		}

		m_descriptorSet->UpdateMap({
			{"UboScene", uniformScene},
			{"UboObject", m_uniformObject},
			{"samplerDiffuse", material->GetDiffuseTexture()},
			{"samplerMaterial", material->GetMaterialTexture()},
			{"samplerNormal", material->GetNormalTexture()}
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);*/
		material->CmdRender(commandBuffer, mesh->GetModel(), uniformScene, m_uniformObject);
	}
}
