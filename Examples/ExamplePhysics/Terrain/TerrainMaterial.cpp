#include "TerrainMaterial.hpp"

#include <Maths/Transform.hpp>

namespace test {
TerrainMaterial::TerrainMaterial(std::shared_ptr<Image2d> imageR, std::shared_ptr<Image2d> imageG) :
	imageR(std::move(imageR)),
	imageG(std::move(imageG)) {
}

void TerrainMaterial::CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) {
	pipelineMaterial = MaterialPipeline::Create({1, 0}, {
		{"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"},
		{vertexInput}, {}, PipelineGraphics::Mode::MRT
	});
}

void TerrainMaterial::PushUniforms(UniformHandler &uniformObject, const Transform *transform) {
	if (transform)
		uniformObject.Push("transform", transform->GetWorldMatrix());
}

void TerrainMaterial::PushDescriptors(DescriptorsHandler &descriptorSet) {
	descriptorSet.Push("samplerR", imageR);
	descriptorSet.Push("samplerG", imageG);
}

const Node &operator>>(const Node &node, TerrainMaterial &material) {
	node["imageR"].Get(material.imageR);
	node["imageG"].Get(material.imageG);
	return node;
}

Node &operator<<(Node &node, const TerrainMaterial &material) {
	node["imageR"].Set(material.imageR);
	node["imageG"].Set(material.imageG);
	return node;
}
}
