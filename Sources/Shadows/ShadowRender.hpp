#pragma once

#include "Scenes/Component.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
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

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene);

private:
	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;
};
}
