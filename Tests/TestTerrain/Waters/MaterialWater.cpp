#include "MaterialWater.hpp"

#include "MeshWater.hpp"

namespace test
{
	MaterialWater::MaterialWater() :
		IMaterial(),
		m_colour(Colour(MeshWater::WATER_COLOUR)),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Waters/Water.vert", "Resources/Shaders/Waters/Water.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	MaterialWater::~MaterialWater()
	{
	}

	void MaterialWater::Update()
	{
		m_colour.m_a = 1.0f; // Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f
	}

	void MaterialWater::Load(LoadedValue *value)
	{
	}

	void MaterialWater::Write(LoadedValue *destination)
	{
	}

	void MaterialWater::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject.Push("diffuseColour", m_colour);
	}

	void MaterialWater::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
