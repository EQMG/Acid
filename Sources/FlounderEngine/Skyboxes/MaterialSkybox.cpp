#include "MaterialSkybox.hpp"

#include "Worlds/Worlds.hpp"

namespace Flounder
{
	MaterialSkybox::MaterialSkybox(Cubemap *cubemap, const bool &enableFog) :
		IMaterial(),
		m_cubemap(cubemap),
		m_enableFog(enableFog),
		m_blend(1.0f),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Skyboxes/Skybox.vert", "Resources/Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT), GetDefines()))
	{
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Update()
	{
		if (m_enableFog)
		{
			GetGameObject()->GetTransform()->SetRotation(*Worlds::Get()->GetSkyboxRotation());
			m_blend = Worlds::Get()->GetStarIntensity();
		}
	}

	void MaterialSkybox::Load(LoadedValue *value)
	{
		TrySetCubemap(value->GetChild("Cubemap Texture")->GetString());
		m_enableFog = (bool) value->GetChild("Enable Fog")->Get<int>();
	}

	void MaterialSkybox::Write(LoadedValue *destination)
	{
		destination->GetChild("Diffuse Texture", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
		destination->GetChild("Enable Fog", true)->Set((int) m_enableFog);
	}

	void MaterialSkybox::PushUniforms(UniformHandler *uniformObject)
	{
		// Updates uniforms.
		if (m_enableFog)
		{
			uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
			uniformObject->Push("skyColour", *Worlds::Get()->GetSkyColour());
			uniformObject->Push("fogColour", *Worlds::Get()->GetFog()->m_colour);
			uniformObject->Push("fogLimits", GetGameObject()->GetTransform()->m_scaling->m_y * Vector2(Worlds::Get()->GetFog()->m_lowerLimit,
				Worlds::Get()->GetFog()->m_upperLimit));
			uniformObject->Push("blendFactor", m_blend);
		}
		else
		{
			uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
			uniformObject->Push("skyColour", Colour::WHITE);
			uniformObject->Push("fogColour", Colour::WHITE);
			uniformObject->Push("fogLimits", Vector2(-1000000.0f, -1000000.0f));
			uniformObject->Push("blendFactor", 1.0f);
		}
	}

	void MaterialSkybox::PushDescriptors(DescriptorsHandler *descriptorSet)
	{
		descriptorSet->Push("samplerCubemap", m_cubemap);
	}

	std::vector<Define> MaterialSkybox::GetDefines()
	{
		std::vector<Define> result = {};

		return result;
	}
}
