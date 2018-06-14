#include "MaterialAnimated.hpp"

#include "MeshAnimated.hpp"

namespace fl
{
	MaterialAnimated::MaterialAnimated() :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Animated/Animated.vert", "Shaders/Animated/Animated.frag"},
			VertexAnimated::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK),
			{{"MAX_JOINTS", std::to_string(MeshAnimated::MAX_JOINTS)}, {"MAX_WEIGHTS", std::to_string(MeshAnimated::MAX_WEIGHTS)}}))
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

	void MaterialAnimated::PushUniforms(UniformHandler &uniformObject)
	{
		auto meshAnimated = GetGameObject()->GetComponent<MeshAnimated>();
		auto joints = meshAnimated->GetJointTransforms();

		uniformObject.Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject.Push("jointTransforms", *joints.data(), sizeof(Matrix4) * joints.size());
	}

	void MaterialAnimated::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
	}
}
