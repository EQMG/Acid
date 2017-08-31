#pragma once

#include "../maths/colour.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a fog in the world.
	/// </summary>
	class fog
	{
	public:
		colour *m_colour;
		float m_density;
		float m_gradient;
		float m_lowerLimit;
		float m_upperLimit;

		/// <summary>
		/// Creates a new fog.
		/// </summary>
		/// <param name="colour"> The colour of the fog. </param>
		/// <param name="density"> How dense the fog will be. </param>
		/// <param name="gradient"> The gradient of the fog. </param>
		/// <param name="lowerLimit"> At what height will the skybox fog begin to appear. </param>
		/// <param name="upperLimit"> At what height will there be skybox no fog. </param>
		fog(colour *colour, const float &density, const float &gradient, const float &lowerLimit, const float &upperLimit);

		/// <summary>
		/// Deconstructor for fog.
		/// </summary>
		~fog();
	};
}
