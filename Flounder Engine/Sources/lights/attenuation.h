#pragma once

namespace flounder {
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
		attenuation(const float constant, const float linear, const float exponent);

		/// <summary>
		/// Creates a Attenuation object used in lights from a source object.
		/// </summary>
		/// <param name="source"> The source attenuation value. </param>
		attenuation(attenuation *source);

		/// <summary>
		/// Deconstructor for attenuation.
		/// </summary>
		~attenuation();

		void set(const float constant, const float linear, const float exponent);

		void set(attenuation *source);
	};
}
