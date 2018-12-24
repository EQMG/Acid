#include "MaterialSkybox.hpp"

#include "Models/VertexModel.hpp"
#include "Renderer/Pipelines/Compute.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	MaterialSkybox::MaterialSkybox(const std::shared_ptr<Cubemap> &cubemap, const Colour &skyColour) :
		m_cubemap(cubemap),
		m_skyColour(skyColour),
		m_blend(1.0f),
		m_fogColour(Colour()),
		m_fogLimits(Vector2(-1000000.0f, -1000000.0f)),
		m_pipelineMaterial(nullptr)
	{
	}

	void MaterialSkybox::Start()
	{
		m_pipelineMaterial = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_MRT, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {}));
	}

	void MaterialSkybox::Update()
	{
	}

	void MaterialSkybox::Decode(const Metadata &metadata)
	{
		m_cubemap = Cubemap::Resource(metadata.GetChild<std::string>("Cubemap Texture"), metadata.GetChild<std::string>("Cubemap Suffix"));
		m_skyColour = metadata.GetChild<Colour>("Sky Colour");
	}

	void MaterialSkybox::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Cubemap Texture", m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		metadata.SetChild<std::string>("Cubemap Suffix", m_cubemap == nullptr ? "" : m_cubemap->GetFileSuffix());
		metadata.SetChild<Colour>("Sky Colour", m_skyColour.GetHex());
	}

	void MaterialSkybox::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetParent()->GetWorldMatrix());
		uniformObject.Push("skyColour", m_skyColour);
		uniformObject.Push("fogColour", m_fogColour);
		uniformObject.Push("fogLimits", GetParent()->GetLocalTransform().GetScaling().m_y * m_fogLimits);
		uniformObject.Push("blendFactor", m_blend);
	}

	void MaterialSkybox::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerCubemap", m_cubemap);
	}
}
