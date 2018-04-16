#include "SkyboxRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Worlds/Worlds.hpp"
#include "UbosSkyboxes.hpp"

namespace Flounder
{
	SkyboxRender::SkyboxRender(Cubemap *cubemap, const bool &enableFog) :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosSkyboxes::UboObject))),
		m_descriptorSet(nullptr),
		m_cubemap(cubemap),
		m_enableFog(enableFog),
		m_blend(1.0f)
	{
	}

	SkyboxRender::~SkyboxRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
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
		UbosSkyboxes::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);

		if (m_enableFog)
		{
			uboObject.skyColour = Colour(*Worlds::Get()->GetSkyColour());
			uboObject.fogColour = Colour(*Worlds::Get()->GetFog()->m_colour);
			uboObject.fogLimits = GetGameObject()->GetTransform()->m_scaling->m_y * Vector2(Worlds::Get()->GetFog()->m_lowerLimit,
				Worlds::Get()->GetFog()->m_upperLimit);
			uboObject.blendFactor = m_blend;
		}
		else
		{
			uboObject.fogLimits = Vector2(-1000000.0f, -1000000.0f);
			uboObject.blendFactor = 1.0f;
		}

		m_uniformObject->Update(&uboObject);
	}

	void SkyboxRender::Load(LoadedValue *value)
	{
		TrySetCubemap(value->GetChild("Cubemap")->GetString());
	}

	void SkyboxRender::Write(LoadedValue *value)
	{
		value->GetChild("Cubemap", true)->SetString(m_cubemap == nullptr ? "" : m_cubemap->GetFilename());
	}

	void SkyboxRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
		}

		m_descriptorSet->Update({
			uniformScene,
			m_uniformObject,
			m_cubemap
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
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
