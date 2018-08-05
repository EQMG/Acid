#include "MaterialDefault.hpp"

#include "Objects/GameObject.hpp"
#include "Animations/MeshAnimated.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	MaterialDefault::MaterialDefault(const Colour &baseColor, std::shared_ptr<Texture> diffuseTexture,
									 const float &metallic, const float &roughness, std::shared_ptr<Texture> materialTexture, std::shared_ptr<Texture> normalTexture,
									 const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		IMaterial(),
		m_animated(false),
		m_baseColor(baseColor),
		m_diffuseTexture(diffuseTexture),
		m_metallic(metallic),
		m_roughness(roughness),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_material(nullptr)
	{
	}

	MaterialDefault::~MaterialDefault()
	{
	}

	void MaterialDefault::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh == nullptr)
		{
			fprintf(stderr, "Cannot have a material attached to a object without a mesh!\n");
			return;
		}

		m_animated = dynamic_cast<MeshAnimated *>(mesh) != nullptr;
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"},
			mesh->GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK, GetDefines()));
	}

	void MaterialDefault::Update()
	{
	}

	void MaterialDefault::Load(LoadedValue *value)
	{
		m_baseColor = value->GetChild("Base Colour")->GetString();
		TrySetDiffuseTexture(value->GetChild("Diffuse Texture")->GetString());

		m_metallic = value->GetChild("Metallic")->Get<float>();
		m_roughness = value->GetChild("Roughness")->Get<float>();
		TrySetMaterialTexture(value->GetChild("Material Texture")->GetString());
		TrySetNormalTexture(value->GetChild("Normal Texture")->GetString());

		m_castsShadows = value->GetChild("Casts Shadows")->Get<bool>();
		m_ignoreLighting = value->GetChild("Ignore Lighting")->Get<bool>();
		m_ignoreFog = value->GetChild("Ignore Fog")->Get<bool>();
	}

	void MaterialDefault::Write(LoadedValue *destination)
	{
		destination->GetChild("Base Colour", true)->SetString(m_baseColor.GetHex());
		destination->GetChild("Diffuse Texture", true)->SetString(m_diffuseTexture == nullptr ? "" : m_diffuseTexture->GetName());

		destination->GetChild("Metallic", true)->Set(m_metallic);
		destination->GetChild("Roughness", true)->Set(m_roughness);
		destination->GetChild("Material Texture", true)->SetString(m_materialTexture == nullptr ? "" : m_materialTexture->GetName());
		destination->GetChild("Normal Texture", true)->SetString(m_normalTexture == nullptr ? "" : m_normalTexture->GetName());

		destination->GetChild("Casts Shadows", true)->Set(m_castsShadows);
		destination->GetChild("Ignore Lighting", true)->Set(m_ignoreLighting);
		destination->GetChild("Ignore Fog", true)->Set(m_ignoreFog);
	}

	void MaterialDefault::PushUniforms(UniformHandler &uniformObject)
	{
		if (m_animated)
		{
			auto meshAnimated = GetGameObject()->GetComponent<MeshAnimated>();
			auto joints = meshAnimated->GetJointTransforms();
			uniformObject.Push("jointTransforms", *joints.data(), sizeof(Matrix4) * joints.size());
		}

		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("baseColor", m_baseColor);
		uniformObject.Push("metallic", m_metallic);
		uniformObject.Push("roughness", m_roughness);
		uniformObject.Push("ignoreFog", static_cast<float>(m_ignoreFog));
		uniformObject.Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
	}

	void MaterialDefault::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerDiffuse", m_diffuseTexture);
		descriptorSet.Push("samplerMaterial", m_materialTexture);
		descriptorSet.Push("samplerNormal", m_normalTexture);
	}

	std::vector<PipelineDefine> MaterialDefault::GetDefines()
	{
		std::vector<PipelineDefine> result = {};

		if (m_diffuseTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("COLOUR_MAPPING", "TRUE"));
		}

		if (m_materialTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("MATERIAL_MAPPING", "TRUE"));
		}

		if (m_animated)
		{
			result.emplace_back(PipelineDefine("ANIMATED", "TRUE"));
			result.emplace_back(PipelineDefine("MAX_JOINTS", std::to_string(MeshAnimated::MAX_JOINTS)));
			result.emplace_back(PipelineDefine("MAX_WEIGHTS", std::to_string(MeshAnimated::MAX_WEIGHTS)));
		}

		/*if (m_normalTexture != nullptr)
		{
			result.emplace_back(PipelineDefine("NORMAL_MAPPING", "TRUE"));
		}*/

		return result;
	}
}
