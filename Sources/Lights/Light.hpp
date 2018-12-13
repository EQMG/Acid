#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Objects/IComponent.hpp"

namespace acid
{
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class ACID_EXPORT Light :
		public IComponent
	{
	private:
		Colour m_colour;
		Vector3 m_position;
		float m_radius;
		Transform m_localTransform;
		mutable Transform m_worldTransform;
	public:
		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="localTransform"> The local transform from the parents space. </param>
		explicit Light(const Colour &colour = Colour::WHITE, const float &radius = -1.0f, const Transform &localTransform = Transform::ZERO);

		/// <summary>
		/// Creates a new point light from a source object.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		Light(const Light &source);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Transform GetLocalTransform() const { return m_localTransform; }

		void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

		Transform GetWorldTransform() const;
	};
}
