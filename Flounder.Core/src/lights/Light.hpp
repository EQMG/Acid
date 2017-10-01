#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector3.hpp"

#include "Attenuation.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class Light
	{
	public:
		Colour *m_colour;
		Vector3 *m_position;
		Attenuation *m_attenuation;

		/// <summary>
		/// Creates a new infinate white point light with unlimited range.
		/// </summary>
		Light();

		/// <summary>
		/// Creates a new point light with unlimited range.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="position"> The world position of the light. </param>
		Light(const Colour &colour, const Vector3 &position);

		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="position"> The world position of the light. </param>
		/// <param name="attenuation"> How much the intensity of the light is lost over a distance. </param>
		Light(const Colour &colour, const Vector3 &position, const Attenuation &attenuation);

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
		/// <param name="position"> The world position of the light. </param>
		/// <param name="attenuation"> How much the intensity of the light is lost over a distance. </param>
		/// <returns> This. </returns>
		Light *Set(const Colour &colour, const Vector3 &position, const Attenuation &attenuation);
		
		/// <summary>
		/// Sets values in the light.
		/// </summary>
		/// <param name="source"> The source light object. </param>
		/// <returns> This. </returns>
		Light *Set(const Light &source);
	};
}
