#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class Vector3;

	class Vector4;

	class Metadata;

	/// <summary>
	/// Holds a RGBA colour.
	/// </summary>
	class ACID_EXPORT Colour
	{
	public:
		union
		{
			struct
			{
				float m_elements[4];
			};

			struct
			{
				float m_r, m_g, m_b, m_a;
			};
		};

		static const Colour CLEAR;
		static const Colour BLACK;
		static const Colour GREY;
		static const Colour SILVER;
		static const Colour WHITE;
		static const Colour MAROON;
		static const Colour RED;
		static const Colour OLIVE;
		static const Colour YELLOW;
		static const Colour GREEN;
		static const Colour LIME;
		static const Colour TEAL;
		static const Colour AQUA;
		static const Colour NAVY;
		static const Colour BLUE;
		static const Colour PURPLE;
		static const Colour FUCHSIA;

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
		/// <param name="a"> The new A value. </param>
		Colour(const Vector3 &source, const float &a = 1.0f);

		/// <summary>
		/// Constructor for colour.
		/// </summary>
		/// <param name="source"> Creates this colour out of a existing vector. </param>
		Colour(const Vector4 &source);

		/// <summary>
		/// Adds this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Colour Add(const Colour &other) const;

		/// <summary>
		/// Subtracts this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Colour Subtract(const Colour &other) const;

		/// <summary>
		/// Multiplies this vector with another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Colour Multiply(const Colour &other) const;

		/// <summary>
		/// Divides this vector by another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Colour Divide(const Colour &other) const;

		/// <summary>
		/// Scales this vector by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled vector. </returns>
		Colour Scale(const float &scalar) const;

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> The normalized vector. </returns>
		Colour Normalize() const;

		/// <summary>
		/// Gets the length squared of this vector.
		/// </summary>
		/// <returns> The length squared. </returns>
		float LengthSquared() const;

		/// <summary>
		/// Gets the length of this vector.
		/// </summary>
		/// <returns> The length. </returns>
		float Length() const;

		/// <summary>
		/// Interpolates between this and another colour.
		/// </summary>
		/// <param name="other"> The other colour. </param>
		/// <param name="blend"> The blend factor. </param>
		/// <returns> The interpolated colour. </returns>
		Colour Interpolate(const Colour &other, float blend) const;

		/// <summary>
		/// Gradually changes this vector to a target.
		/// </summary>
		/// <param name="target"> The target vector. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed vector. </returns>
		Colour SmoothDamp(const Colour &target, const Colour &rate) const;

		/// <summary>
		/// Gets a colour representing the unit value of this colour.
		/// </summary>
		/// <returns> The unit colour. </returns>
		Colour GetUnit() const;

		/// <summary>
		/// Gets the hex code from this colour.
		/// </summary>
		/// <returns> The hex code. </returns>
		std::string GetHex() const;

		float GetR() const { return m_r; }

		void SetR(const float &r) { m_r = r; }

		float GetG() const { return m_g; }

		void SetG(const float &g) { m_g = g; }

		float GetB() const { return m_b; }

		void SetB(const float &b) { m_b = b; }

		float GetA() const { return m_a; }

		void SetA(const float &a) { m_a = a; }

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		bool operator==(const Colour &other) const;

		bool operator!=(const Colour &other) const;

		bool operator<(const Colour &other) const;

		bool operator<=(const Colour &other) const;

		bool operator>(const Colour &other) const;

		bool operator>=(const Colour &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

		const float &operator[](const uint32_t &index) const;

		float &operator[](const uint32_t &index);

		ACID_EXPORT friend Colour operator+(const Colour &left, const Colour &right);

		ACID_EXPORT friend Colour operator-(const Colour &left, const Colour &right);

		ACID_EXPORT friend Colour operator*(const Colour &left, const Colour &right);

		ACID_EXPORT friend Colour operator/(const Colour &left, const Colour &right);

		ACID_EXPORT friend Colour operator+(const float &value, const Colour &left);

		ACID_EXPORT friend Colour operator-(const float &value, const Colour &left);

		ACID_EXPORT friend Colour operator*(const float &value, const Colour &left);

		ACID_EXPORT friend Colour operator/(const float &value, const Colour &left);

		ACID_EXPORT friend Colour operator+(const Colour &left, const float &value);

		ACID_EXPORT friend Colour operator-(const Colour &left, const float &value);

		ACID_EXPORT friend Colour operator*(const Colour &left, const float &value);

		ACID_EXPORT friend Colour operator/(const Colour &left, const float &value);

		Colour &operator+=(const Colour &other);

		Colour &operator-=(const Colour &other);

		Colour &operator*=(const Colour &other);

		Colour &operator/=(const Colour &other);

		Colour &operator+=(const float &value);

		Colour &operator-=(const float &value);

		Colour &operator*=(const float &value);

		Colour &operator/=(const float &value);

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Colour &colour);

		std::string ToString() const;
	};
}
