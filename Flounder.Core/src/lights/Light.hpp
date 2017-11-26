#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class Light
	{
	public:
		Colour *m_colour;
		float m_radius;
		Vector3 *m_position;

		/// <summary>
		/// Creates a new infinate white point light with unlimited range.
		/// </summary>
		Light();

		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="position"> The world position of the light. </param>
		Light(const Colour &colour, const float &radius, const Vector3 &position = Vector3());

		/// <summary>
		/// Creates a new point light from a source object.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		Light(const Light &source);

		/// <summary>
		/// Deconstructor for light.
		/// </summary>
		~Light();

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="position"> The world position of the light. </param>
		/// <returns> This. </returns>
		Light *Set(const Colour &colour, const Vector3 &position);

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="radius"> How far the light will have influence (-1 sets this to a directional light). </param>
		/// <param name="position"> The world position of the light. </param>
		/// <returns> This. </returns>
		Light *Set(const Colour &colour, const float &radius, const Vector3 &position);

		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		/// <returns> This. </returns>
		Light *Set(const Light &source);
	};
}
