#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class ACID_EXPORT Light :
		public Component
	{
	public:
		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="localTransform"> The local transform from the parents space. </param>
		explicit Light(const Colour &colour = Colour::White, const float &radius = -1.0f, const Transform &localTransform = Transform::Identity);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		const Colour &GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		const float &GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		const Transform &GetLocalTransform() const { return m_localTransform; }

		void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

		Transform GetWorldTransform() const;

	private:
		Colour m_colour;
		Vector3 m_position;
		float m_radius;
		Transform m_localTransform;
		mutable Transform m_worldTransform;
	};
}
