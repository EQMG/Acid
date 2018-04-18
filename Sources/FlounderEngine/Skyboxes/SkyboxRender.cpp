#include "SkyboxRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Worlds/Worlds.hpp"

namespace Flounder
{
	SkyboxRender::SkyboxRender(Cubemap *cubemap, const bool &enableFog) :
		Component(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler()),
		m_cubemap(cubemap),
		m_enableFog(enableFog),
		m_blend(1.0f)
	{
	}

	SkyboxRender::~SkyboxRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
		delete m_cubemap;
	}

	void SkyboxRender::Update()
	{
		if (m_enableFog)
		{
			GetGameObject()->GetTransform()->SetRotation(*Worlds::Get()->GetSkyboxRotation());
			m_blend = Worlds::Get()->GetStarIntensity();
		}

		// Updates uniforms.
		if (m_enableFog)
		{
			m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
			m_uniformObject->Push("skyColour", *Worlds::Get()->GetSkyColour());
			m_uniformObject->Push("fogColour", *Worlds::Get()->GetFog()->m_colour);
			m_uniformObject->Push("fogLimits", GetGameObject()->GetTransform()->m_scaling->m_y * Vector2(Worlds::Get()->GetFog()->m_lowerLimit,
				Worlds::Get()->GetFog()->m_upperLimit));
			m_uniformObject->Push("blendFactor", m_blend);
		}
		else
		{
			m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
			m_uniformObject->Push("skyColour", Colour::WHITE);
			m_uniformObject->Push("fogColour", Colour::WHITE);
			m_uniformObject->Push("fogLimits", Vector2(-1000000.0f, -1000000.0f));
			m_uniformObject->Push("blendFactor", 1.0f);
		}

	}

	void SkyboxRender::Load(LoadedValue *value)
	{
		TrySetCubemap(value->GetChild("Cubemap")->GetString());
	}

	void SkyboxRender::Write(LoadedValue *value)
	{
		value->GetChild("Cubemap", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
	}

	void SkyboxRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", uniformScene);
		m_descriptorSet->Push("UboObject", m_uniformObject);
		m_descriptorSet->Push("samplerCubemap", m_cubemap);
		bool descriptorsSet = m_descriptorSet->Update(pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}

	void SkyboxRender::TrySetCubemap(const std::string &filename)
	{
		if (!filename.empty())
		{
			m_cubemap = Cubemap::Resource(filename, ".png");
		}
	}
}
