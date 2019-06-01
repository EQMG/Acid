#pragma once

#include "Scenes/Component.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/PushHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

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

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	friend const Metadata &operator>>(const Metadata &metadata, ShadowRender &shadowRender);

	friend Metadata &operator<<(Metadata &metadata, const ShadowRender &shadowRender);

private:
	DescriptorsHandler m_descriptorSet;
	PushHandler m_pushObject;
};
}
