#include "WaterRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Worlds/Worlds.hpp"
#include "Waters.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const float WaterRender::SIDE_LENGTH = 1800.0f;
	const float WaterRender::SQUARE_SIZE = 200.0f;
	const int WaterRender::VERTEX_COUNT = static_cast<int>((2.0f * SIDE_LENGTH) / SQUARE_SIZE) + 1;
	const float WaterRender::TEXTURE_SCALE = 1.0f;

	const Colour WaterRender::WATER_COLOUR = Colour("#0077be");

	WaterRender::WaterRender() :
		m_uniformObject(new UniformBuffer(sizeof(UbosWaters::UboObject))),
		m_descriptorSet(nullptr),
		m_colour(new Colour(WATER_COLOUR))
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
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b, Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f);
		uboObject.worldCurvature = Worlds::WORLD_CURVATURE;
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
