#include "MaterialDefault.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Models/VertexModel.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
	MaterialDefault::MaterialDefault(const Colour &baseDiffuse, const std::shared_ptr<Texture> &diffuseTexture, 
		const float &metallic, const float &roughness, const std::shared_ptr<Texture> &materialTexture, const std::shared_ptr<Texture> &normalTexture, 
		const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		m_animated(false),
		m_baseDiffuse(baseDiffuse),
		m_diffuseTexture(diffuseTexture),
		m_metallic(metallic),
		m_roughness(roughness),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture),
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
		m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineGraphicsCreate({"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"}, {mesh->GetVertexInput()},
			PipelineGraphics::Mode::Mrt, PipelineGraphics::Depth::ReadWrite, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false, GetDefines()));
	}

	void MaterialDefault::Update()
	{
	}

	void MaterialDefault::Decode(const Metadata &metadata)
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

	void MaterialDefault::Encode(Metadata &metadata) const
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
		descriptorSet.Push("samplerDiffuse", m_diffuseTexture);
		descriptorSet.Push("samplerMaterial", m_materialTexture);
		descriptorSet.Push("samplerNormal", m_normalTexture);
	}

	std::vector<Shader::Define> MaterialDefault::GetDefines()
	{
		std::vector<Shader::Define> result = {};
		result.emplace_back("DIFFUSE_MAPPING", String::To<int32_t>(m_diffuseTexture != nullptr));
		result.emplace_back("MATERIAL_MAPPING", String::To<int32_t>(m_materialTexture != nullptr));
		result.emplace_back("NORMAL_MAPPING", String::To<int32_t>(m_normalTexture != nullptr));
		result.emplace_back("ANIMATED", String::To<int32_t>(m_animated));
		result.emplace_back("MAX_JOINTS", String::To(MeshAnimated::MaxJoints));
		result.emplace_back("MAX_WEIGHTS", String::To(MeshAnimated::MaxWeights));
		return result;
	}
}
