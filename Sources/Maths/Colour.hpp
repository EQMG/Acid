#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
/**
 * @brief Holds a RGBA colour.
 **/
class Colour
{
public:
	/**
	 * Constructor for Colour.
	 * @param r The new R value. 
	 * @param g The new G value. 
	 * @param b The new B value. 
	 * @param a The new A value. 
	 **/
	explicit Colour(const float &r = 0.0f, const float &g = 0.0f, const float &b = 0.0f, const float &a = 1.0f);

	/**
	 * Constructor for Colour.
	 * @param hex The new values from HEX. 
	 * @param a The new A value. 
	 **/
	Colour(const std::string &hex, const float &a = 1.0f);

	/**
	 * Adds this colour to another colour.
	 * @param other The other colour. 
	 * @return The resultant colour. 
	 **/
	Colour Add(const Colour &other) const;

	/**
	 * Subtracts this colour to another colour.
	 * @param other The other colour. 
	 * @return The resultant colour. 
	 **/
	Colour Subtract(const Colour &other) const;

	/**
	 * Multiplies this colour with another colour.
	 * @param other The other colour. 
	 * @return The resultant colour. 
	 **/
	Colour Multiply(const Colour &other) const;

	/**
	 * Divides this colour by another colour.
	 * @param other The other colour. 
	 * @return The resultant colour. 
	 **/
	Colour Divide(const Colour &other) const;

	/**
	 * Scales this colour by a scalar.
	 * @param scalar The scalar value. 
	 * @return The scaled colour. 
	 **/
	Colour Scale(const float &scalar) const;

	/**
	 * Calculates the linear interpolation between this colour and another colour.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	Colour Lerp(const Colour &other, const float &progression) const;

	/**
	 * Normalizes this colour.
	 * @return The normalized colour. 
	 **/
	Colour Normalize() const;

	/**
	 * Gets the length squared of this colour.
	 * @return The length squared. 
	 **/
	float LengthSquared() const;

	/**
	 * Gets the length of this colour.
	 * @return The length. 
	 **/
	float Length() const;

	/**
	 * Gradually changes this colour to a target.
	 * @param target The target colour. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed colour. 
	 **/
	Colour SmoothDamp(const Colour &target, const Colour &rate) const;

	/**
	 * Gets a colour representing the unit value of this colour.
	 * @return The unit colour. 
	 **/
	Colour GetUnit() const;

	/**
	 * Gets the hex code from this colour.
	 * @return The hex code. 
	 **/
	std::string GetHex() const;

	const float &GetR() const { return m_r; }

	void SetR(const float &r) { m_r = r; }

	const float &GetG() const { return m_g; }

	void SetG(const float &g) { m_g = g; }

	const float &GetB() const { return m_b; }

	void SetB(const float &b) { m_b = b; }

	const float &GetA() const { return m_a; }

	void SetA(const float &a) { m_a = a; }

	std::string ToString() const;

	bool operator==(const Colour &other) const;

	bool operator!=(const Colour &other) const;

	const float &operator[](const uint32_t &index) const;

	float &operator[](const uint32_t &index);

	friend Colour operator+(const Colour &left, const Colour &right);

	friend Colour operator-(const Colour &left, const Colour &right);

	friend Colour operator*(const Colour &left, const Colour &right);

	friend Colour operator/(const Colour &left, const Colour &right);

	friend Colour operator+(const float &value, const Colour &left);

	friend Colour operator-(const float &value, const Colour &left);

	friend Colour operator*(const float &value, const Colour &left);

	friend Colour operator/(const float &value, const Colour &left);

	friend Colour operator+(const Colour &left, const float &value);

	friend Colour operator-(const Colour &left, const float &value);

	friend Colour operator*(const Colour &left, const float &value);

	friend Colour operator/(const Colour &left, const float &value);

	Colour &operator+=(const Colour &other);

	Colour &operator-=(const Colour &other);

	Colour &operator*=(const Colour &other);

	Colour &operator/=(const Colour &other);

	Colour &operator+=(const float &value);

	Colour &operator-=(const float &value);

	Colour &operator*=(const float &value);

	Colour &operator/=(const float &value);

	friend const Metadata &operator>>(const Metadata &metadata, Colour &colour);

	friend Metadata &operator<<(Metadata &metadata, const Colour &colour);

	friend std::ostream &operator<<(std::ostream &stream, const Colour &colour);

	ACID_EXPORT static const Colour Clear;
	ACID_EXPORT static const Colour Black;
	ACID_EXPORT static const Colour Grey;
	ACID_EXPORT static const Colour Silver;
	ACID_EXPORT static const Colour White;
	ACID_EXPORT static const Colour Maroon;
	ACID_EXPORT static const Colour Red;
	ACID_EXPORT static const Colour Olive;
	ACID_EXPORT static const Colour Yellow;
	ACID_EXPORT static const Colour Green;
	ACID_EXPORT static const Colour Lime;
	ACID_EXPORT static const Colour Teal;
	ACID_EXPORT static const Colour Aqua;
	ACID_EXPORT static const Colour Navy;
	ACID_EXPORT static const Colour Blue;
	ACID_EXPORT static const Colour Purple;
	ACID_EXPORT static const Colour Fuchsia;

	float m_r, m_g, m_b, m_a;
};
}

namespace std
{
template<>
struct hash<acid::Colour>
{
	size_t operator()(const acid::Colour &colour) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, colour.m_r);
		acid::Maths::HashCombine(seed, colour.m_g);
		acid::Maths::HashCombine(seed, colour.m_b);
		acid::Maths::HashCombine(seed, colour.m_a);
		return seed;
	}
};
}
