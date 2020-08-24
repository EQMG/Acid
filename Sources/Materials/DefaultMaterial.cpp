#include "DefaultMaterial.hpp"

#include "Animations/AnimatedMesh.hpp"
#include "Maths/Transform.hpp"

namespace acid {
const bool DefaultMaterial::Registered = Register("default");

DefaultMaterial::DefaultMaterial(const Colour &baseDiffuse, std::shared_ptr<Image2d> imageDiffuse, float metallic, float roughness,
	std::shared_ptr<Image2d> imageMaterial, std::shared_ptr<Image2d> imageNormal, bool castsShadows, bool ignoreLighting, bool ignoreFog) :
	baseDiffuse(baseDiffuse),
	imageDiffuse(std::move(imageDiffuse)),
	metallic(metallic),
	roughness(roughness),
	imageMaterial(std::move(imageMaterial)),
	imageNormal(std::move(imageNormal)),
	castsShadows(castsShadows),
	ignoreLighting(ignoreLighting),
	ignoreFog(ignoreFog) {
}

void DefaultMaterial::CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) {
	this->animated = animated; // TODO: Remove
	pipelineMaterial = MaterialPipeline::Create({1, 0}, {
		{"Shaders/Defaults/Default.vert", "Shaders/Defaults/Default.frag"},
		{vertexInput}, GetDefines(), PipelineGraphics::Mode::MRT
	});
}

void DefaultMaterial::PushUniforms(UniformHandler &uniformObject, const Transform *transform) {
	if (transform)
		uniformObject.Push("transform", transform->GetWorldMatrix());
	
	uniformObject.Push("baseDiffuse", baseDiffuse);
	uniformObject.Push("metallic", metallic);
	uniformObject.Push("roughness", roughness);
	uniformObject.Push("ignoreFog", static_cast<float>(ignoreFog));
	uniformObject.Push("ignoreLighting", static_cast<float>(ignoreLighting));
}

void DefaultMaterial::PushDescriptors(DescriptorsHandler &descriptorSet) {
	descriptorSet.Push("samplerDiffuse", imageDiffuse);
	descriptorSet.Push("samplerMaterial", imageMaterial);
	descriptorSet.Push("samplerNormal", imageNormal);
}

std::vector<Shader::Define> DefaultMaterial::GetDefines() const {
	return {
		{"DIFFUSE_MAPPING", String::To<int32_t>(imageDiffuse != nullptr)},
		{"MATERIAL_MAPPING", String::To<int32_t>(imageMaterial != nullptr)},
		{"NORMAL_MAPPING", String::To<int32_t>(imageNormal != nullptr)},
		{"ANIMATED", String::To<int32_t>(animated)},
		{"MAX_JOINTS", String::To(AnimatedMesh::MaxJoints)},
		{"MAX_WEIGHTS", String::To(AnimatedMesh::MaxWeights)}
	};
}

const Node &operator>>(const Node &node, DefaultMaterial &material) {
	node["baseDiffuse"].Get(material.baseDiffuse);
	node["imageDiffuse"].Get(material.imageDiffuse);
	node["metallic"].Get(material.metallic);
	node["roughness"].Get(material.roughness);
	node["imageMaterial"].Get(material.imageMaterial);
	node["imageNormal"].Get(material.imageNormal);
	node["castsShadows"].Get(material.castsShadows);
	node["ignoreLighting"].Get(material.ignoreLighting);
	node["ignoreFog"].Get(material.ignoreFog);
	return node;
}

Node &operator<<(Node &node, const DefaultMaterial &material) {
	node["baseDiffuse"].Set(material.baseDiffuse);
	node["imageDiffuse"].Set(material.imageDiffuse);
	node["metallic"].Set(material.metallic);
	node["roughness"].Set(material.roughness);
	node["imageMaterial"].Set(material.imageMaterial);
	node["imageNormal"].Set(material.imageNormal);
	node["castsShadows"].Set(material.castsShadows);
	node["ignoreLighting"].Set(material.ignoreLighting);
	node["ignoreFog"].Set(material.ignoreFog);
	return node;
}
}
