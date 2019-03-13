#include "MaterialSkybox.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
MaterialSkybox::MaterialSkybox(std::shared_ptr<Cubemap> cubemap, const Colour& skyColour) : m_cubemap(std::move(cubemap)), m_skyColour(skyColour), m_blend(1.0f), m_fogLimits(-10000.0f, -10000.0f) {}

void MaterialSkybox::Start()
{
	m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineGraphicsCreate({"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"}, {VertexModel::GetVertexInput()}, {}, PipelineGraphics::Mode::Mrt, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
																				 VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT));
}

void MaterialSkybox::Update() {}

void MaterialSkybox::Decode(const Metadata& metadata)
{
	metadata.GetResource("Cubemap", m_cubemap);
	metadata.GetChild("Sky Colour", m_skyColour);
}

void MaterialSkybox::Encode(Metadata& metadata) const
{
	metadata.SetResource("Cubemap", m_cubemap);
	metadata.SetChild("Sky Colour", m_skyColour);
}

void MaterialSkybox::PushUniforms(UniformHandler& uniformObject)
{
	uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	uniformObject.Push("skyColour", m_skyColour);
	uniformObject.Push("fogColour", m_fogColour);
	uniformObject.Push("fogLimits", GetParent()->GetLocalTransform().GetScaling().m_y * m_fogLimits);
	uniformObject.Push("blendFactor", m_blend);
}

void MaterialSkybox::PushDescriptors(DescriptorsHandler& descriptorSet)
{
	descriptorSet.Push("samplerCubemap", m_cubemap);
}
}
