#pragma once

#include "Scenes/Component.hpp"
#include "Maths/Matrix4.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Buffers/PushHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
/**
 * @brief Component that is used to render a entity as a shadow.
 */
class ACID_EXPORT ShadowRender :
	public Component
{
public:
	ShadowRender();

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

private:
	DescriptorsHandler m_descriptorSet;
	PushHandler m_pushObject;
};
}
