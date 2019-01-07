#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Serialized/Metadata.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
	/// <summary>
	/// A component interface that defines a emitter volume.
	/// </summary>
	class ACID_EXPORT Emitter :
		public Component
	{
	protected:
		Transform m_localTransform;
	public:
		/// <summary>
		/// Creates a new emitter.
		/// </summary>
		/// <param name="localTransform"> The local transform from the parents space. </param>
		explicit Emitter(const Transform &localTransform = Transform::IDENTITY) :
			m_localTransform(localTransform)
		{
		}

		/// <summary>
		/// Creates a new objects position.
		/// </summary>
		/// <returns> The new objects position. </returns>
		virtual Vector3 GeneratePosition() const = 0;

		const Transform &GetLocalTransform() const { return m_localTransform; }

		void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }
	};
}
