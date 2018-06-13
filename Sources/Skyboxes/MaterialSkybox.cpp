#include "MaterialSkybox.hpp"

#include "Scenes/Scenes.hpp"
#include "Worlds/Worlds.hpp"

namespace fl
{
	MaterialSkybox::MaterialSkybox(std::shared_ptr<Cubemap> cubemap, const bool &enableFog) :
		IMaterial(),
		m_cubemap(cubemap),
		m_enableFog(enableFog),
		m_blend(1.0f),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Skyboxes/Skybox.vert", "Resources/Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_FRONT), {}))
	{
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Update()
	{
		GetGameObject()->GetTransform()->SetPosition(Scenes::Get()->GetCamera()->GetPosition());

		if (m_enableFog)
		{
			GetGameObject()->GetTransform()->SetRotation(Worlds::Get()->GetWorld()->GetSkyboxRotation());
			m_blend = Worlds::Get()->GetWorld()->GetStarIntensity();
		}
	}

	void MaterialSkybox::Load(LoadedValue *value)
	{
		TrySetCubemap(value->GetChild("Cubemap Texture")->GetString());
		m_enableFog = value->GetChild("Enable Fog")->Get<bool>();
	}

	void MaterialSkybox::Write(LoadedValue *destination)
	{
		destination->GetChild("Diffuse Texture", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		destination->GetChild("Enable Fog", true)->Set((int) m_enableFog);
	}

	void MaterialSkybox::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject.Push("skyColour", Worlds::Get()->GetWorld()->GetSkyColour());
		uniformObject.Push("fogColour", Worlds::Get()->GetWorld()->GetFog().GetColour());

		if (m_enableFog)
		{
			uniformObject.Push("fogLimits", GetGameObject()->GetTransform()->GetScaling().m_y * Vector2(Worlds::Get()->GetWorld()->GetFog().GetLowerLimit(), Worlds::Get()->GetWorld()->GetFog().GetUpperLimit()));
			uniformObject.Push("blendFactor", m_blend);
		}
		else
		{
			uniformObject.Push("fogLimits", Vector2(-1000000.0f, -1000000.0f));
			uniformObject.Push("blendFactor", 1.0f);
		}
	}

	void MaterialSkybox::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerCubemap", m_cubemap);
	}
}
