#pragma once

namespace Flounder
{
	/// <summary>
	/// Attenuation is used in calculating the range of lights.
	/// </summary>
	class attenuation
	{
	public:
		float m_constant;
		float m_linear;
		float m_exponent;

		/// <summary>
		/// Creates a attenuation object used in lights. The calculation used is as follows:<br>
		/// {@code factor = constant + (linear * cameraDistance) + (exponent * (cameraDistance * cameraDistance))}
		/// </summary>
		/// <param name="constant"> The constant attenuation value. </param>
		/// <param name="linear"> The linear attenuation value. </param>
		/// <param name="exponent"> The exponent attenuation value. </param>
		attenuation(const float &constant, const float &linear, const float &exponent);

		/// <summary>
		/// Creates a Attenuation object used in lights from a source object.
		/// </summary>
		/// <param name="source"> The source attenuation value. </param>
		attenuation(attenuation *source);

		/// <summary>
		/// Deconstructor for attenuation.
		/// </summary>
		~attenuation();

		/// <summary>
		/// Sets values in the attenuation.
		/// </summary>
		/// <param name="constant"> The new constant value. </param>
		/// <param name="linear"> The new linear value. </param>
		/// <param name="exponent"> The new exponent value. </param>
		/// <returns> This. </returns>
		attenuation set(const float &constant, const float &linear, const float &exponent);

		/// <summary>
		/// Sets values in the attenuation.
		/// </summary>
		/// <param name="source"> Creates this attenuation out of a existing one. </param>
		/// <returns> This. </returns>
		attenuation set(attenuation *source);
	};
}
