#pragma once

#include "Objects/Component.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "PipelineMaterial.hpp"

namespace acid
{
	/// <summary>
	/// A component that represents a material shader that is used to render a mesh.
	/// </summary>
	class ACID_EXPORT Material :
		public Component
	{
	public:
		/// <summary>
		/// Used to update the main uniform handler used in a material.
		/// A material can defined it's own uniforms and push them via <seealso cref="#PushDescriptors()"/>.
		/// </summary>
		/// <param name="uniformObject"> The uniform handler to update. </param>
		virtual void PushUniforms(UniformHandler &uniformObject) = 0;

		/// <summary>
		/// Used to update a descriptor set containing descriptors used in this materials shader.
		/// </summary>
		/// <param name="descriptorSet"> The descriptor handler to update. </param>
		virtual void PushDescriptors(DescriptorsHandler &descriptorSet) = 0;

		/// <summary>
		/// Gets the material pipeline defined in this material.
		/// </summary>
		/// <returns> The material pipeline. </returns>
		virtual std::shared_ptr<PipelineMaterial> GetMaterialPipeline() const = 0;
	};
}
