#include "MaterialWater.hpp"

#include "MeshWater.hpp"

namespace fl
{
	MaterialWater::MaterialWater() :
		IMaterial(),
		m_colour(new Colour(MeshWater::WATER_COLOUR)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Waters/Water.vert", "Resources/Shaders/Waters/Water.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {}))
	{
	}

	MaterialWater::~MaterialWater()
	{
		delete m_colour;
	}

	void MaterialWater::Update()
	{
	}

	void MaterialWater::Load(LoadedValue *value)
	{
	}

	void MaterialWater::Write(LoadedValue *destination)
	{
	}

	void MaterialWater::PushUniforms(UniformHandler *uniformObject)
	{
		uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject->Push("diffuseColour", Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b, 1.0f)); // Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f
	}

	void MaterialWater::PushDescriptors(DescriptorsHandler *descriptorSet)
	{
	}
}
