#pragma once

#include "maths.h"

namespace flounder {
	/// <summary>
	/// Holds a RGBA colour.
	/// </summary>
	class colour
	{
	public:
		float r, g, b, a;

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		colour();

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="source"> Creates this colour out of a existing one. </param>
		colour(colour *source);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		colour(const float r, const float g, const float b);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="convert"> Converts the colours from 0-255 to 0-1. </param>
		colour(const float r, const float g, const float b, const bool convert);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		colour(const float r, const float g, const float b, const float a);

		/// <summary>
		/// Constructor for Colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		/// <param name="convert"> Converts the colours from 0-255 to 0-1. </param>
		colour(const float r, const float g, const float b, const float a, const bool convert);

		/// <summary>
		/// Deconstructor for colour.
		/// </summary>
		~colour();

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="source"> The source colour. </param>
		/// <returns> This. </returns>
		colour *set(colour *source);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <returns> This. </returns>
		colour *set(const float r, const float g, const float b);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="convert"> Converts the colours from 0-255 to 0-1. </param>
		/// <returns> This. </returns>
		colour *set(const float r, const float g, const float b, const bool convert);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		/// <returns> This. </returns>
		colour *set(const float r, const float g, const float b, const float a);

		/// <summary>
		/// Sets values in the colour.
		/// </summary>
		/// <param name="r"> The new R value. </param>
		/// <param name="g"> The new G value. </param>
		/// <param name="b"> The new B value. </param>
		/// <param name="a"> The new A value. </param>
		/// <param name="convert"> Converts the colours from 0-255 to 0-1. </param>
		/// <returns> This. </returns>
		colour *set(const float r, const float g, const float b, const float a, const bool convert);

		/// <summary>
		/// Adds two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or null if a new colour is to be created.
		/// </param>
		/// <returns> The destination colour. </returns>
		static colour *add(colour *left, colour *right, colour *destination);

		/// <summary>
		/// Subtracts two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or null if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static colour *subtract(colour *left, colour *right, colour *destination);

		/// <summary>
		/// Multiplies two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or null if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static colour *multiply(colour *left, colour *right, colour *destination);

		/// <summary>
		/// Divides two colours together and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="destination"> The destination colour or null if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static colour *divide(colour *left, colour *right, colour *destination);

		/// <summary>
		/// Interpolates between two colours and places the result in the destination colour.
		/// </summary>
		/// <param name="left"> The left source colour. </param>
		/// <param name="right"> The right source colour. </param>
		/// <param name="blend"> The blend factor. </param>
		/// <param name="destination"> The destination colour or null if a new colour is to be created. </param>
		/// <returns> The destination colour. </returns>
		static colour *interpolate(colour *left, colour *right, float blend, colour *destination);

		/// <summary>
		/// Gets a colour representing the unit value of this colour.
		/// </summary>
		/// <returns> The unit colour. </returns>
		colour *getUnit();

		/// <summary>
		/// Scales the colour by a scalar.
		/// </summary>
		/// <param name="scalar"> The scaling value.
		/// </param>
		/// <returns> this. </returns>
		colour *scale(const float scalar);

		/// <returns> The length of the colour. </returns>
		float length();

		/// <returns> The length squared of the colour. </returns>
		float lengthSquared();
	};
}
