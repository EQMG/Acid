#include "MaterialDefault.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Maths/Transform.hpp"

namespace acid {
bool MaterialDefault::registered = Register("default");

MaterialDefault::MaterialDefault(const Colour &baseDiffuse, std::shared_ptr<Image2d> imageDiffuse, float metallic, float roughness,
	std::shared_ptr<Image2d> imageMaterial, std::shared_ptr<Image2d> imageNormal, bool castsShadows, bool ignoreLighting, bool ignoreFog) :
	m_baseDiffuse(baseDiffuse),
	m_imageDiffuse(std::move(imageDiffuse)),
	m_metallic(metallic),
	m_roughness(roughness),
	m_imageMaterial(std::move(imageMaterial)),
	m_imageNormal(std::move(imageNormal)),
	m_castsShadows(castsShadows),
	m_ignoreLighting(ignoreLighting),
	m_ignoreFog(ignoreFog) {
}

void MaterialDefault::Start(const Shader::VertexInput &vertexInput) {
	m_pipelineMaterial = PipelineMaterial::Create({1, 0}, {
		{"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"},
		{vertexInput}, GetDefines(), PipelineGraphics::Mode::Mrt
	});
}

void MaterialDefault::PushUniforms(UniformHandler &uniformObject, const Transform &transform) {
	uniformObject.Push("transform", transform.GetWorldMatrix());
	uniformObject.Push("baseDiffuse", m_baseDiffuse);
	uniformObject.Push("metallic", m_metallic);
	uniformObject.Push("roughness", m_roughness);
	uniformObject.Push("ignoreFog", static_cast<float>(m_ignoreFog));
	uniformObject.Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
}

void MaterialDefault::PushDescriptors(DescriptorsHandler &descriptorSet) {
	descriptorSet.Push("samplerDiffuse", m_imageDiffuse);
	descriptorSet.Push("samplerMaterial", m_imageMaterial);
	descriptorSet.Push("samplerNormal", m_imageNormal);
}

std::vector<Shader::Define> MaterialDefault::GetDefines() const {
	return {
		{"DIFFUSE_MAPPING", String::To<int32_t>(m_imageDiffuse != nullptr)},
		{"MATERIAL_MAPPING", String::To<int32_t>(m_imageMaterial != nullptr)},
		{"NORMAL_MAPPING", String::To<int32_t>(m_imageNormal != nullptr)},
		{"ANIMATED", String::To<int32_t>(m_animated)},
		{"MAX_JOINTS", String::To(MeshAnimated::MaxJoints)},
		{"MAX_WEIGHTS", String::To(MeshAnimated::MaxWeights)}
	};
}

const Node &operator>>(const Node &node, MaterialDefault &material) {
	node["baseDiffuse"].Get(material.m_baseDiffuse);
	node["imageDiffuse"].Get(material.m_imageDiffuse);
	node["metallic"].Get(material.m_metallic);
	node["roughness"].Get(material.m_roughness);
	node["imageMaterial"].Get(material.m_imageMaterial);
	node["imageNormal"].Get(material.m_imageNormal);
	node["castsShadows"].Get(material.m_castsShadows);
	node["ignoreLighting"].Get(material.m_ignoreLighting);
	node["ignoreFog"].Get(material.m_ignoreFog);
	return node;
}

Node &operator<<(Node &node, const MaterialDefault &material) {
	node["type"].Set("material");
	node["baseDiffuse"].Set(material.m_baseDiffuse);
	node["imageDiffuse"].Set(material.m_imageDiffuse);
	node["metallic"].Set(material.m_metallic);
	node["roughness"].Set(material.m_roughness);
	node["imageMaterial"].Set(material.m_imageMaterial);
	node["imageNormal"].Set(material.m_imageNormal);
	node["castsShadows"].Set(material.m_castsShadows);
	node["ignoreLighting"].Set(material.m_ignoreLighting);
	node["ignoreFog"].Set(material.m_ignoreFog);
	return node;
}
}
