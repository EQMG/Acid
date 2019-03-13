#include "MaterialDefault.hpp"

#include "Meshes/Mesh.hpp"
#include "Models/VertexModel.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
MaterialDefault::MaterialDefault(const Colour& baseDiffuse, std::shared_ptr<Texture> diffuseTexture, const float& metallic, const float& roughness, std::shared_ptr<Texture> materialTexture, std::shared_ptr<Texture> normalTexture, const bool& castsShadows,
								 const bool& ignoreLighting, const bool& ignoreFog)
	: m_baseDiffuse(baseDiffuse), m_diffuseTexture(std::move(diffuseTexture)), m_metallic(metallic), m_roughness(roughness), m_materialTexture(std::move(materialTexture)), m_normalTexture(std::move(normalTexture)), m_castsShadows(castsShadows), m_ignoreLighting(ignoreLighting),
	  m_ignoreFog(ignoreFog)
{
}

void MaterialDefault::Start()
{
	auto mesh = GetParent()->GetComponent<Mesh>(true);

	if(mesh == nullptr)
		{
			Log::Error("Cannot have a material attached to a object without a mesh!\n");
			return;
		}

	m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineGraphicsCreate({"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"}, {mesh->GetVertexInput()}, GetDefines(), PipelineGraphics::Mode::Mrt));
}

void MaterialDefault::Update() {}

void MaterialDefault::Decode(const Metadata& metadata)
{
	metadata.GetChild("Base Diffuse", m_baseDiffuse);
	metadata.GetResource("Diffuse Texture", m_diffuseTexture);

	metadata.GetChild("Metallic", m_metallic);
	metadata.GetChild("Roughness", m_roughness);
	metadata.GetResource("Material Texture", m_materialTexture);
	metadata.GetResource("Normal Texture", m_normalTexture);

	metadata.GetChild("Casts Shadows", m_castsShadows);
	metadata.GetChild("Ignore Lighting", m_ignoreLighting);
	metadata.GetChild("Ignore Fog", m_ignoreFog);
}

void MaterialDefault::Encode(Metadata& metadata) const
{
	metadata.SetChild("Base Diffuse", m_baseDiffuse);
	metadata.SetResource<Texture>("Diffuse Texture", m_diffuseTexture);

	metadata.SetChild("Metallic", m_metallic);
	metadata.SetChild("Roughness", m_roughness);
	metadata.SetResource<Texture>("Material Texture", m_materialTexture);
	metadata.SetResource<Texture>("Normal Texture", m_normalTexture);

	metadata.SetChild("Casts Shadows", m_castsShadows);
	metadata.SetChild("Ignore Lighting", m_ignoreLighting);
	metadata.SetChild("Ignore Fog", m_ignoreFog);
}

void MaterialDefault::PushUniforms(UniformHandler& uniformObject)
{
	uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	uniformObject.Push("baseDiffuse", m_baseDiffuse);
	uniformObject.Push("metallic", m_metallic);
	uniformObject.Push("roughness", m_roughness);
	uniformObject.Push("ignoreFog", static_cast<float>(m_ignoreFog));
	uniformObject.Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
}

void MaterialDefault::PushDescriptors(DescriptorsHandler& descriptorSet)
{
	descriptorSet.Push("samplerDiffuse", m_diffuseTexture);
	descriptorSet.Push("samplerMaterial", m_materialTexture);
	descriptorSet.Push("samplerNormal", m_normalTexture);
}

std::vector<Shader::Define> MaterialDefault::GetDefines() const
{
	std::vector<Shader::Define> result = {};
	result.emplace_back("DIFFUSE_MAPPING", String::To<int32_t>(m_diffuseTexture != nullptr));
	result.emplace_back("MATERIAL_MAPPING", String::To<int32_t>(m_materialTexture != nullptr));
	result.emplace_back("NORMAL_MAPPING", String::To<int32_t>(m_normalTexture != nullptr));
	return result;
}
}
