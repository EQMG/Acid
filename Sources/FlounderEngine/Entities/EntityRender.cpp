#include "EntityRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Animations/MeshAnimated.hpp"
#include "Worlds/Worlds.hpp"
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
		auto materialDefault = GetGameObject()->GetComponent<MaterialDefault>();

		if (materialDefault == nullptr)
		{
			return;
		}

		/*auto material = materialDefault->GetMaterial();

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
		}*/

		// Updates uniforms.
		UbosEntities::UboObject uboObject = {};

		//for (unsigned int i = 0; i < jointTransforms.size(); i++)
		//{
		//	uboObject.jointTransforms[i] = jointTransforms.at(i);
		//}

		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.baseColor = *materialDefault->GetBaseColor();
		uboObject.metallic = materialDefault->GetMetallic();
		uboObject.roughness = materialDefault->GetRoughness();
		uboObject.ignoreFog = static_cast<float>(materialDefault->IsIgnoringFog());
		uboObject.ignoreLighting = static_cast<float>(materialDefault->IsIgnoringLighting());
		m_uniformObject->Update(&uboObject);

		//m_uniformObject->UpdateMap("UniformObject", material->GetPipeline()->GetShaderProgram(), {
		//	{"jointTransforms", jointTransforms.data()},
		//	{"transform", GetGameObject()->GetTransform()->GetWorldMatrix()},
		// 	{"baseColor", *materialDefault->GetBaseColor()},
		//	{"metallic", materialDefault->GetMetallic()},
		//	{"roughness", materialDefault->GetRoughness()},
		//	{"ignoreFog", static_cast<float>(materialDefault->IsIgnoringFog())},
		//	{"ignoreLighting", static_cast<float>(materialDefault->IsIgnoringLighting())}
		//});
	}

	void EntityRender::Load(LoadedValue *value)
	{
	}

	void EntityRender::Write(LoadedValue *value)
	{
	}

	void EntityRender::CmdRender(const VkCommandBuffer &commandBuffer, UniformBuffer *uniformScene)
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
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*material->GetPipeline());
		}

		m_descriptorSet->UpdateMap({
			{"UboScene",        uniformScene},
			{"UboObject",       m_uniformObject},
			{"samplerDiffuse",  materialDefault->GetDiffuseTexture()},
			{"samplerMaterial", materialDefault->GetMaterialTexture()},
			{"samplerNormal",   materialDefault->GetNormalTexture()}
		});

		// Draws the object.
		material->CmdRender(commandBuffer, mesh->GetModel(), m_descriptorSet);
	}
}
