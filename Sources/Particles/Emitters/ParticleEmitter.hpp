#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Serialized/Metadata.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
	/// <summary>
	/// A component interface that defines a particle spawn type.
	/// </summary>
	class ACID_EXPORT ParticleEmitter :
		public Component
	{
	protected:
		Transform m_localTransform;
	public:
		/// <summary>
		/// Creates a new particle emitter.
		/// </summary>
		/// <param name="localTransform"> The local transform from the parents space. </param>
		explicit ParticleEmitter(const Transform &localTransform = Transform::IDENTITY) :
			m_localTransform(localTransform)
		{
		}

		/// <summary>
		/// Creates a new particle position.
		/// </summary>
		/// <returns> The new particles position. </returns>
		virtual Vector3 GeneratePosition() const = 0;

		Transform GetLocalTransform() const { return m_localTransform; }

		void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }
	};
}
