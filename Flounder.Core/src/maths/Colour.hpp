#pragma once

#include "Maths.hpp"

namespace Flounder
{
	class Vector4;
	class Vector3;

	/// <summary>
	/// Holds a RGBA colour.
	/// </summary>
	class Colour
	{
	public:
		float m_r, m_g, m_b, m_a;

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
		Colour(const float &r, const float &g, const float &b, const float &a);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="hex"> The new values from HEX. </param>
		/// <param name="a"> The new A value. </param>
		Colour(const std::string &hex, const float &a);

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
		/// Sets values in the colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		/// <returns> This. </returns>
		Colour *Set(const float &r, const float &g, const float &b, const float &a);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="hex"> The new values from HEX. </param>
		/// <param name="a"> The new A value. </param>
		/// <returns> This. </returns>
		Colour *Set(const std::string &hex, const float &a);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <returns> This. </returns>
		Colour *Set(const Colour &source);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Colour *Set(const Vector3 &source);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Colour *Set(const Vector4 &source);

		/// <summary>
		/// Adds two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *Add(const Colour &left, const Colour &right, Colour *destination);

		/// <summary>
		/// Subtracts two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *Subtract(const Colour &left, const Colour &right, Colour *destination);

		/// <summary>
		/// Multiplies two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *Multiply(const Colour &left, const Colour &right, Colour *destination);

		/// <summary>
		/// Divides two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *Divide(const Colour &left, const Colour &right, Colour *destination);

		/// <summary>
		/// Interpolates between two colours and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="blend"> The blend factor. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *Interpolate(const Colour &left, const Colour &right, float blend, Colour *destination);

		/// <summary>
		/// Gets a colour representing the unit value of this colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <param name="destination"> The destination colour or nullptr if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static Colour *GetUnit(const Colour &source, Colour *destination);

		/// <summary>
		/// Gets the hex code from the colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <returns> The hex code from the colour. </returns>
		static std::string GetHex(const Colour &source);

		/// <summary>
		/// Gets the length of the colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <returns> The length of the colour. </returns>
		static float Length(const Colour &source);

		/// <summary>
		/// Gets the length squared of the colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <returns> The length squared of the colour. </returns>
		static float LengthSquared(const Colour &source);

		/// <summary>
		/// Scales the colour by a scalar.
		/// </summary>
		/// <param name="scalar"> The scaling value. </param>
		/// <returns> this. </returns>
		Colour *Scale(const float &scalar);

		/// <summary>
		/// Gets the length of the colour.
		/// </summary>
		/// <returns> The length of the colour. </returns>
		float Length();

		/// <summary>
		/// Gets the length squared of the colour.
		/// </summary>
		/// <returns> The length squared of the colour. </returns>
		float LengthSquared();
	};
}
