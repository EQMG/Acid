#pragma once

namespace Flounder
{
	/// <summary>
	/// Attenuation is used in calculating the range of lights.
	/// </summary>
	class Attenuation
	{
	public:
		union
		{
			struct
			{
				float m_constant, m_linear, m_exponent;
			};

			struct
			{
				float m_x, m_y, m_z;
			};

			struct
			{
				float m_elements[3];
			};
		};

		/// <summary>
		/// Creates a attenuation object used in lights, the default constructor creates a non diminishing attenuation.
		/// </summary>
		Attenuation();

		/// <summary>
		/// Creates a attenuation object used in lights. The calculation used is as follows:<br>
		/// {@code factor = constant + (linear * cameraDistance) + (exponent * (cameraDistance * cameraDistance))}
		/// </summary>
		/// <param name="constant"> The constant attenuation value. </param>
		/// <param name="linear"> The linear attenuation value. </param>
		/// <param name="exponent"> The exponent attenuation value. </param>
		Attenuation(const float &constant, const float &linear, const float &exponent);

		/// <summary>
		/// Creates a attenuation object used in lights from a source object.
		/// </summary>
		/// <param name="source"> The source attenuation object. </param>
		Attenuation(const Attenuation &source);

		/// <summary>
		/// Deconstructor for attenuation.
		/// </summary>
		~Attenuation();

		/// <summary>
		/// Sets values in the attenuation.
		/// </summary>
		/// <param name="constant"> The new constant value. </param>
		/// <param name="linear"> The new linear value. </param>
		/// <param name="exponent"> The new exponent value. </param>
		/// <returns> This. </returns>
		Attenuation *Set(const float &constant, const float &linear, const float &exponent);

		/// <summary>
		/// Sets values in the attenuation.
		/// </summary>
		/// <param name="source"> The source attenuation object. </param>
		/// <returns> This. </returns>
		Attenuation *Set(const Attenuation &source);
	};
}
