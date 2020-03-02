#include "MaterialTerrain.hpp"

#include <Maths/Transform.hpp>

namespace test {
MaterialTerrain::MaterialTerrain(std::shared_ptr<Image2d> imageR, std::shared_ptr<Image2d> imageG) :
	imageR(std::move(imageR)),
	imageG(std::move(imageG)) {
}

void MaterialTerrain::CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) {
	pipelineMaterial = PipelineMaterial::Create({1, 0}, {{"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"}, {vertexInput}});
}

void MaterialTerrain::PushUniforms(UniformHandler &uniformObject, const Transform *transform) {
	if (transform)
		uniformObject.Push("transform", transform->GetWorldMatrix());
}

void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet) {
	descriptorSet.Push("samplerR", imageR);
	descriptorSet.Push("samplerG", imageG);
}

const Node &operator>>(const Node &node, MaterialTerrain &material) {
	node["imageR"].Get(material.imageR);
	node["imageG"].Get(material.imageG);
	return node;
}

Node &operator<<(Node &node, const MaterialTerrain &material) {
	node["imageR"].Set(material.imageR);
	node["imageG"].Set(material.imageG);
	return node;
}
}
