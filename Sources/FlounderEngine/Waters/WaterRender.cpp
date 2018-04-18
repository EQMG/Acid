#include "WaterRender.hpp"

#include "Meshes/Mesh.hpp"
#include "MeshWater.hpp"

namespace Flounder
{
	WaterRender::WaterRender() :
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler()),
		m_colour(new Colour(MeshWater::WATER_COLOUR))
	{
	}

	WaterRender::~WaterRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
		delete m_colour;
	}

	void WaterRender::Update()
	{
		// Updates uniforms.
		m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		m_uniformObject->Push("diffuseColour", Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b, 1.0f)); // Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f
	}

	void WaterRender::Load(LoadedValue *value)
	{
	}

	void WaterRender::Write(LoadedValue *value)
	{
	}

	void WaterRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene)
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
		bool descriptorsSet = m_descriptorSet->Update(pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
