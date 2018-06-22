#include "MaterialSkybox.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	MaterialSkybox::MaterialSkybox(std::shared_ptr<Cubemap> cubemap, const Colour &skyColour) :
		IMaterial(),
		m_cubemap(cubemap),
		m_skyColour(skyColour),
		m_blend(1.0f),
		m_fogColour(Colour()),
		m_fogLimits(Vector2(-1000000.0f, -1000000.0f)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Skyboxes/Skybox.vert", "Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_FRONT), {}))
	{
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Update()
	{
		GetGameObject()->GetTransform().SetPosition(Scenes::Get()->GetCamera()->GetPosition());
	}

	void MaterialSkybox::Load(std::shared_ptr<LoadedValue> value)
	{
		TrySetCubemap(value->GetChild("Cubemap Texture")->GetString(), value->GetChild("Cubemap Extension")->GetString());
		m_skyColour = value->GetChild("Sky Colour")->GetString();
	}

	void MaterialSkybox::Write(std::shared_ptr<LoadedValue> destination)
	{
		destination->GetChild("Cubemap Texture", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		destination->GetChild("Cubemap Extension", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetExtension());
		destination->GetChild("Sky Colour", true)->SetString(m_skyColour.GetHex());
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
