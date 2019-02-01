#pragma once

#include "Scenes/Component.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "PipelineMaterial.hpp"

namespace acid
{
	/// <summary>
	/// A component that represents a material shader that is used to render a mesh.
	/// The child of this object should initialize <seealso cref="#m_pipelineMaterial"/> in <seealso cref="#Start()"/>.
	/// </summary>
	class ACID_EXPORT Material :
		public Component
	{
	public:
		Material() :
			m_pipelineMaterial(nullptr)
		{
		}

		virtual ~Material() = default;

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
		const std::shared_ptr<PipelineMaterial> &GetPipelineMaterial() const { return m_pipelineMaterial; }
	protected:
		std::shared_ptr<PipelineMaterial> m_pipelineMaterial;
	};
}
