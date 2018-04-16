#include "WaterRender.hpp"

#include "Meshes/Mesh.hpp"
#include "MeshWater.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	WaterRender::WaterRender() :
		m_uniformObject(new UniformBuffer(sizeof(UbosWaters::UboObject))),
		m_descriptorSet(nullptr),
		m_colour(new Colour(MeshWater::WATER_COLOUR))
	{
	}

	WaterRender::~WaterRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
		delete m_colour;
	}

	void WaterRender::Update()
	{
		// Updates uniforms.
		UbosWaters::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b, 1.0f); // Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f
		m_uniformObject->Update(&uboObject);
	}

	void WaterRender::Load(LoadedValue *value)
	{
	}

	void WaterRender::Write(LoadedValue *value)
	{
	}

	void WaterRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
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
			m_uniformObject
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
