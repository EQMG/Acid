#pragma once

#include "../maths/colour.h"
#include "../maths/vector3.h"

#include "attenuation.h"

namespace flounder {
	/// <summary>
	/// Represents a point light, contains a colour, position and attenuation.
	/// </summary>
	class light
	{
	public:
		colour *m_colour;
		vector3 *m_position;
		attenuation *m_attenuation;

		/// <summary>
		/// Creates a new point light with unlimited range.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="position"> The world position of the light. </param>
		light(colour *colour, vector3 *position);

		/// <summary>
		/// Creates a new point light.
		/// </summary>
		/// <param name="colour"> The colour of the light. </param>
		/// <param name="position"> The world position of the light. </param>
		/// <param name="attenuation"> How much the intensity of the light is lost over a distance. </param>
		light(colour *colour, vector3 *position, attenuation *attenuation);

		/// <summary>
		/// Deconstructor for light.
		/// </summary>
		~light();
	};
}
