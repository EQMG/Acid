#include "ShadowRender.hpp"

#include "Meshes/Mesh.hpp"
#include "RendererShadows.hpp"

namespace acid
{
	ShadowRender::ShadowRender() :
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler())
	{
	}

	void ShadowRender::Start()
	{
	}

	void ShadowRender::Update()
	{
		auto instancePos = std::vector<Vector4>(3);
		instancePos[0] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		instancePos[1] = Vector4(-4.0f, 0.0, -4.0f, 0.0f);
		instancePos[2] = Vector4(4.0f, 0.0, -4.0f, 0.0f);

		// Updates uniforms.
		m_uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
		m_uniformObject.Push("instancePos", *instancePos.data(), sizeof(Vector4) * 3);
	}

	void ShadowRender::Decode(const Metadata &metadata)
	{
	}

	void ShadowRender::Encode(Metadata &metadata) const
	{
	}

	bool ShadowRender::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer, RendererShadows::NUM_CASCADES);
		return true;
	}
}
