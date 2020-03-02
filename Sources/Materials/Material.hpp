#pragma once

#include "Helpers/StreamFactory.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Maths/Transform.hpp"
#include "PipelineMaterial.hpp"

namespace acid {
/**
 * @brief Component that represents a material shader that is used to render a model.
 * The implementation of this object must initialize {@link Material#pipelineMaterial} in {@link Material#CreatePipeline()}.
 */
class ACID_EXPORT Material : public StreamFactory<Material> {
public:
	virtual ~Material() = default;

	// TODO: Remove method
	virtual void CreatePipeline(const Shader::VertexInput &vertexInput, bool animated) = 0;

	/**
	 * Used to update the main uniform handler used in a material.
	 * A material can defined it's own uniforms and push them via {@link Material#PushDescriptors()}.
	 * @param uniformObject The uniform handler to update.
	 */
	virtual void PushUniforms(UniformHandler &uniformObject, const Transform *) = 0;

	/**
	 * Used to update a descriptor set containing descriptors used in this materials shader.
	 * @param descriptorSet The descriptor handler to update.
	 */
	virtual void PushDescriptors(DescriptorsHandler &descriptorSet) = 0;

	/**
	 * Gets the material pipeline defined in this material.
	 * @return The material pipeline.
	 */
	const std::shared_ptr<PipelineMaterial> &GetPipelineMaterial() const { return pipelineMaterial; }

protected:
	std::shared_ptr<PipelineMaterial> pipelineMaterial;
};
}
