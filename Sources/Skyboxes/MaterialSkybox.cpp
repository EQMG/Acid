#include "MaterialSkybox.hpp"

#include "Models/VertexDefault.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
MaterialSkybox::MaterialSkybox(std::shared_ptr<ImageCube> image, const Colour & baseColour) :
	m_image(std::move(image)),
	m_baseColour(baseColour),
	m_blend(1.0f),
	m_fogLimits(-10000.0f, -10000.0f)
{
}

void MaterialSkybox::Start()
{
	m_pipelineMaterial = PipelineMaterial::Create({ 1, 0 },
		PipelineGraphicsCreate({ "Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag" }, { VertexDefault::GetVertexInput() }, {}, PipelineGraphics::Mode::Mrt,
			PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT));
}

void MaterialSkybox::Update()
{
}

void MaterialSkybox::Decode(const Metadata &metadata)
{
	metadata.GetResource("Image", m_image);
	metadata.GetChild("Base Colour", m_baseColour);
}

void MaterialSkybox::Encode(Metadata &metadata) const
{
	metadata.SetResource("Image", m_image);
	metadata.SetChild("Base Colour", m_baseColour);
}

void MaterialSkybox::PushUniforms(UniformHandler &uniformObject)
{
	uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	uniformObject.Push("baseColour", m_baseColour);
	uniformObject.Push("fogColour", m_fogColour);
	uniformObject.Push("fogLimits", GetParent()->GetLocalTransform().GetScaling().m_y * m_fogLimits);
	uniformObject.Push("blendFactor", m_blend);
}

void MaterialSkybox::PushDescriptors(DescriptorsHandler &descriptorSet)
{
	descriptorSet.Push("samplerColour", m_image);
}
}
