#include "SkyboxMaterial.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
SkyboxMaterial::SkyboxMaterial(std::shared_ptr<ImageCube> image, const Colour &baseColour) :
	image(std::move(image)),
	baseColour(baseColour),
	blend(1.0f),
	fogLimits(-10000.0f) {
}

void SkyboxMaterial::CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) {
	pipelineMaterial = MaterialPipeline::Create({1, 0}, {
		{"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"},
		{vertexInput}, {}, PipelineGraphics::Mode::MRT, PipelineGraphics::Depth::None,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT
	});
}

void SkyboxMaterial::PushUniforms(UniformHandler &uniformObject, const Transform *transform) {
	if (transform) {
		uniformObject.Push("transform", transform->GetWorldMatrix());
		uniformObject.Push("fogLimits", transform->GetScale().y * fogLimits);
	}
	
	uniformObject.Push("baseColour", baseColour);
	uniformObject.Push("fogColour", fogColour);
	uniformObject.Push("blendFactor", blend);
}

void SkyboxMaterial::PushDescriptors(DescriptorsHandler &descriptorSet) {
	descriptorSet.Push("samplerColour", image);
}

const Node &operator>>(const Node &node, SkyboxMaterial &material) {
	node["image"].Get(material.image);
	node["baseColour"].Get(material.baseColour);
	return node;
}

Node &operator<<(Node &node, const SkyboxMaterial &material) {
	node["image"].Set(material.image);
	node["baseColour"].Set(material.baseColour);
	return node;
}
}
