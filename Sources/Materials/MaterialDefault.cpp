#include "MaterialDefault.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Meshes/Mesh.hpp"
#include "Models/VertexDefault.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
MaterialDefault::MaterialDefault(const Colour &baseDiffuse, std::shared_ptr<Image2d> imageDiffuse, const float &metallic, const float &roughness,
	std::shared_ptr<Image2d> imageMaterial, std::shared_ptr<Image2d> imageNormal, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
	m_animated(false),
	m_baseDiffuse(baseDiffuse),
	m_imageDiffuse(std::move(imageDiffuse)),
	m_metallic(metallic),
	m_roughness(roughness),
	m_imageMaterial(std::move(imageMaterial)),
	m_imageNormal(std::move(imageNormal)),
	m_castsShadows(castsShadows),
	m_ignoreLighting(ignoreLighting),
	m_ignoreFog(ignoreFog)
{
}

void MaterialDefault::Start()
{
	auto mesh = GetParent()->GetComponent<Mesh>(true);

	if (mesh == nullptr)
	{
		Log::Error("Cannot have a material attached to a object without a mesh!\n");
		return;
	}

	m_animated = dynamic_cast<MeshAnimated *>(mesh) != nullptr;
	m_pipelineMaterial = PipelineMaterial::Create({ 1, 0 },
		PipelineGraphicsCreate({ "Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag" }, { mesh->GetVertexInput() }, GetDefines(), PipelineGraphics::Mode::Mrt));
}

void MaterialDefault::Update()
{
}

void MaterialDefault::PushUniforms(UniformHandler &uniformObject)
{
	if (m_animated)
	{
		auto meshAnimated = GetParent()->GetComponent<MeshAnimated>();
		auto joints = meshAnimated->GetJointTransforms(); // TODO: Move into storage buffer and update every frame.
		uniformObject.Push("jointTransforms", *joints.data(), sizeof(Matrix4) * joints.size());
	}

	uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	uniformObject.Push("baseDiffuse", m_baseDiffuse);
	uniformObject.Push("metallic", m_metallic);
	uniformObject.Push("roughness", m_roughness);
	uniformObject.Push("ignoreFog", static_cast<float>(m_ignoreFog));
	uniformObject.Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
}

void MaterialDefault::PushDescriptors(DescriptorsHandler &descriptorSet)
{
	descriptorSet.Push("samplerDiffuse", m_imageDiffuse);
	descriptorSet.Push("samplerMaterial", m_imageMaterial);
	descriptorSet.Push("samplerNormal", m_imageNormal);
}

std::vector<Shader::Define> MaterialDefault::GetDefines() const
{
	std::vector<Shader::Define> defines;
	defines.emplace_back("DIFFUSE_MAPPING", String::To<int32_t>(m_imageDiffuse != nullptr));
	defines.emplace_back("MATERIAL_MAPPING", String::To<int32_t>(m_imageMaterial != nullptr));
	defines.emplace_back("NORMAL_MAPPING", String::To<int32_t>(m_imageNormal != nullptr));
	defines.emplace_back("ANIMATED", String::To<int32_t>(m_animated));
	defines.emplace_back("MAX_JOINTS", String::To(MeshAnimated::MaxJoints));
	defines.emplace_back("MAX_WEIGHTS", String::To(MeshAnimated::MaxWeights));
	return defines;
}

const Metadata& operator>>(const Metadata& metadata, MaterialDefault& material)
{
	metadata.GetChild("Base Diffuse", material.m_baseDiffuse);
	metadata.GetResource("Image Diffuse", material.m_imageDiffuse);

	metadata.GetChild("Metallic", material.m_metallic);
	metadata.GetChild("Roughness", material.m_roughness);
	metadata.GetResource("Image Material", material.m_imageMaterial);
	metadata.GetResource("Image Normal", material.m_imageNormal);

	metadata.GetChild("Casts Shadows", material.m_castsShadows);
	metadata.GetChild("Ignore Lighting", material.m_ignoreLighting);
	metadata.GetChild("Ignore Fog", material.m_ignoreFog);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const MaterialDefault& material)
{
	metadata.SetChild("Base Diffuse", material.m_baseDiffuse);
	metadata.SetResource("Image Diffuse", material.m_imageDiffuse);

	metadata.SetChild("Metallic", material.m_metallic);
	metadata.SetChild("Roughness", material.m_roughness);
	metadata.SetResource("Image Material", material.m_imageMaterial);
	metadata.SetResource("Image Normal", material.m_imageNormal);

	metadata.SetChild("Casts Shadows", material.m_castsShadows);
	metadata.SetChild("Ignore Lighting", material.m_ignoreLighting);
	metadata.SetChild("Ignore Fog", material.m_ignoreFog);
	return metadata;
}
}
