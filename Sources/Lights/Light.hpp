#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"
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
		Vector3 m_offset;
		float m_radius;

		Vector3 m_position;
	public:
		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="offset"> The parent offset of the light. </param>
		Light(const Colour &colour = Colour::WHITE, const float &radius = -1.0f, const Vector3 &offset = Vector3::ZERO);

		/// <summary>
		/// Creates a new point light from a source object.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		Light(const Light &source);

		~Light();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 GetOffset() const { return m_offset; }

		void SetOffset(const Vector3 &offset) { m_offset = offset; }

		Vector3 GetPosition() const { return m_position; }
	};
}
