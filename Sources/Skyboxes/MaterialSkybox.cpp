#include "MaterialSkybox.hpp"

#include "Models/VertexModel.hpp"
#include "Renderer/Pipelines/Compute.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	MaterialSkybox::MaterialSkybox(const std::shared_ptr<Cubemap> &cubemap, const Colour &skyColour) :
		IMaterial(),
		m_cubemap(cubemap),
		m_skyColour(skyColour),
		m_blend(1.0f),
		m_fogColour(Colour()),
		m_fogLimits(Vector2(-1000000.0f, -1000000.0f)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {})))
	{
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Start()
	{
	}

	void MaterialSkybox::Update()
	{
	}

	void MaterialSkybox::Load(LoadedValue &value)
	{
		TrySetCubemap(value.FindChild("Cubemap Texture")->GetString(), value.FindChild("Cubemap Extension")->GetString());
		m_skyColour = value.FindChild("Sky Colour")->GetString();
	}

	void MaterialSkybox::Write(LoadedValue &destination)
	{
		destination.FindChild("Cubemap Texture", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		destination.FindChild("Cubemap Extension", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetExtension());
		destination.FindChild("Sky Colour", true)->SetString(m_skyColour.GetHex());
	}

	void MaterialSkybox::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		uniformObject.Push("skyColour", m_skyColour);
		uniformObject.Push("fogColour", m_fogColour);
		uniformObject.Push("fogLimits", GetGameObject()->GetTransform().GetScaling().m_y * m_fogLimits);
		uniformObject.Push("blendFactor", m_blend);
	}

	void MaterialSkybox::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerCubemap", m_cubemap);
	}
}
