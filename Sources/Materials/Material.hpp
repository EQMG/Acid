#pragma once

#include "Scenes/Component.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "PipelineMaterial.hpp"

namespace acid
{
/**
 * @brief Component that represents a material shader that is used to render a mesh.
 * The child of this object should initialize {@link Material#m_pipelineMaterial} in {@link Material#Start}.
 */
class ACID_EXPORT Material :
	public Component
{
public:
	virtual ~Material() = default;

	/**
	 * Used to update the main uniform handler used in a material.
	 * A material can defined it's own uniforms and push them via {@link Material#PushDescriptors}.
	 * @param uniformObject The uniform handler to update.
	 */
	virtual void PushUniforms(UniformHandler &uniformObject) = 0;

	/**
	 * Used to update a descriptor set containing descriptors used in this materials shader.
	 * @param descriptorSet The descriptor handler to update.
	 */
	virtual void PushDescriptors(DescriptorsHandler &descriptorSet) = 0;

	/**
	 * Gets the material pipeline defined in this material.
	 * @return The material pipeline.
	 */
	const std::shared_ptr<PipelineMaterial> &GetPipelineMaterial() const { return m_pipelineMaterial; }

protected:
	std::shared_ptr<PipelineMaterial> m_pipelineMaterial;
};
}
