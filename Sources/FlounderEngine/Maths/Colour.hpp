#pragma once

#include <string>
#include "Vector4.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a RGBA colour.
	/// </summary>
	class F_EXPORT Colour :
		public Vector4
	{
	public:
		static const Colour CLEAR;
		static const Colour WHITE;
		static const Colour BLACK;

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		Colour();

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		Colour(const float &r, const float &g, const float &b, const float &a = 1.0f);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="hex"> The new values from HEX. </param>
		/// <param name="a"> The new A value. </param>
		Colour(const std::string &hex, const float &a = 1.0f);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="source"> Creates this colour out of a existing one. </param>
		Colour(const Colour &source);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="source"> Creates this colour out of a existing vector. </param>
		Colour(const Vector3 &source);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="source"> Creates this colour out of a existing vector. </param>
		Colour(const Vector4 &source);

		/// <summary>
		/// Deconstructor for colour.
		/// </summary>
		~Colour();

		/// <summary>
		/// Interpolates between this and another colour.
		/// </summary>
		/// <param name="other"> The other colour. </param>
		/// <param name="blend"> The blend factor. </param>
		/// <returns> The interpolated colour. </returns>
		Colour Interpolate(const Colour &other, float blend) const;

		/// <summary>
		/// Gets a colour representing the unit value of this colour.
		/// </summary>
		/// <returns> The unit colour. </returns>
		Colour GetUnit();

		/// <summary>
		/// Gets the hex code from this colour.
		/// </summary>
		/// <returns> The hex code. </returns>
		std::string GetHex();

		/// <summary>
		/// Saves this constraint into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *source);

		Colour &operator=(const std::string &hex);

		Colour &operator=(LoadedValue *value);

		F_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Colour &vector);

		std::string ToString() const;
	};
}
