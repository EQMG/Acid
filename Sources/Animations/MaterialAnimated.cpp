#include "MaterialAnimated.hpp"

#include "MeshAnimated.hpp"

namespace fl
{
	MaterialAnimated::MaterialAnimated() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Animated/Animated.vert", "Resources/Shaders/Animated/Animated.frag"},
			VertexAnimated::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {{"MAX_JOINTS", "50"}, {"MAX_WEIGHTS", "3"}}))
	{
	}

	MaterialAnimated::~MaterialAnimated()
	{
	}

	void MaterialAnimated::Update()
	{
	}

	void MaterialAnimated::Load(LoadedValue *value)
	{
	}

	void MaterialAnimated::Write(LoadedValue *destination)
	{
	}

	void MaterialAnimated::PushUniforms(UniformHandler *uniformObject)
	{
		auto meshAnimated = GetGameObject()->GetComponent<MeshAnimated>();

		uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject->Push("jointTransforms", meshAnimated->GetJointTransforms().data());
	}

	void MaterialAnimated::PushDescriptors(DescriptorsHandler *descriptorSet)
	{
	}
}
