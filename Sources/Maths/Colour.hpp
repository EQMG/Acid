#pragma once

#include "Files/Node.hpp"
#include "Maths.hpp"

namespace acid {
/**
 * @brief Holds a RGBA colour.
 */
class ACID_EXPORT Colour {
public:
	// In order of how bits are mapped [24, 16, 8, 0xFF].
	enum class Type {
		RGBA,
		ARGB,
		RGB
	};

	/**
	 * Constructor for Colour.
	 */
	Colour() = default;

	/**
	 * Constructor for Colour.
	 * @param r The new R value.
	 * @param g The new G value.
	 * @param b The new B value.
	 * @param a The new A value.
	 */
	Colour(float r, float g, float b, float a = 1.0f);

	/**
	 * Constructor for Colour.
	 * @param i The integer value.
	 * @param type The order components of colour are packed.
	 */
	Colour(uint32_t i, Type type = Type::RGB);
	
	/**
	 * Constructor for Colour.
	 * @param hex The new values from HEX.
	 * @param a The new A value.
	 */
	Colour(std::string hex, float a = 1.0f);

	/**
	 * Adds this colour to another colour.
	 * @param other The other colour.
	 * @return The resultant colour.
	 */
	Colour Add(const Colour &other) const;

	/**
	 * Subtracts this colour to another colour.
	 * @param other The other colour.
	 * @return The resultant colour.
	 */
	Colour Subtract(const Colour &other) const;

	/**
	 * Multiplies this colour with another colour.
	 * @param other The other colour.
	 * @return The resultant colour.
	 */
	Colour Multiply(const Colour &other) const;

	/**
	 * Divides this colour by another colour.
	 * @param other The other colour.
	 * @return The resultant colour.
	 */
	Colour Divide(const Colour &other) const;

	/**
	 * Scales this colour by a scalar.
	 * @param scalar The scalar value.
	 * @return The scaled colour.
	 */
	Colour Scale(float scalar) const;

	/**
	 * Calculates the linear interpolation between this colour and another colour.
	 * @param other The other quaternion.
	 * @param progression The progression.
	 * @return Left lerp right.
	 */
	Colour Lerp(const Colour &other, float progression) const;

	/**
	 * Normalizes this colour.
	 * @return The normalized colour.
	 */
	Colour Normalize() const;

	/**
	 * Gets the length squared of this colour.
	 * @return The length squared.
	 */
	float LengthSquared() const;

	/**
	 * Gets the length of this colour.
	 * @return The length.
	 */
	float Length() const;

	/**
	 * Gradually changes this colour to a target.
	 * @param target The target colour.
	 * @param rate The rate to go from current to the target.
	 * @return The changed colour.
	 */
	Colour SmoothDamp(const Colour &target, const Colour &rate) const;

	/**
	 * Gets a colour representing the unit value of this colour.
	 * @return The unit colour.
	 */
	Colour GetUnit() const;

	/**
	 * Gets a packed integer representing this colour.
	 * @param type The order components of colour are packed.
	 * @return The packed integer.
	 */
	uint32_t GetInt(Type type = Type::RGBA) const;
	
	/**
	 * Gets the hex code from this colour.
	 * @return The hex code.
	 */
	std::string GetHex() const;

	float operator[](uint32_t index) const;
	float &operator[](uint32_t index);

	bool operator==(const Colour &other) const;
	bool operator!=(const Colour &other) const;

	friend Colour operator+(const Colour &left, const Colour &right);
	friend Colour operator-(const Colour &left, const Colour &right);
	friend Colour operator*(const Colour &left, const Colour &right);
	friend Colour operator/(const Colour &left, const Colour &right);
	friend Colour operator+(float value, const Colour &left);
	friend Colour operator-(float value, const Colour &left);
	friend Colour operator*(float value, const Colour &left);
	friend Colour operator/(float value, const Colour &left);
	friend Colour operator+(const Colour &left, float value);
	friend Colour operator-(const Colour &left, float value);
	friend Colour operator*(const Colour &left, float value);
	friend Colour operator/(const Colour &left, float value);

	Colour &operator+=(const Colour &other);
	Colour &operator-=(const Colour &other);
	Colour &operator*=(const Colour &other);
	Colour &operator/=(const Colour &other);
	Colour &operator+=(float value);
	Colour &operator-=(float value);
	Colour &operator*=(float value);
	Colour &operator/=(float value);

	friend const Node &operator>>(const Node &node, Colour &colour);
	friend Node &operator<<(Node &node, const Colour &colour);
	friend std::ostream &operator<<(std::ostream &stream, const Colour &colour);

	static const Colour Clear;
	static const Colour Black;
	static const Colour Grey;
	static const Colour Silver;
	static const Colour White;
	static const Colour Maroon;
	static const Colour Red;
	static const Colour Olive;
	static const Colour Yellow;
	static const Colour Green;
	static const Colour Lime;
	static const Colour Teal;
	static const Colour Aqua;
	static const Colour Navy;
	static const Colour Blue;
	static const Colour Purple;
	static const Colour Fuchsia;

	float m_r = 0.0f, m_g = 0.0f, m_b = 0.0f, m_a = 1.0f;
};
}

namespace std {
template<>
struct hash<acid::Colour> {
	size_t operator()(const acid::Colour &colour) const {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, colour.m_r);
		acid::Maths::HashCombine(seed, colour.m_g);
		acid::Maths::HashCombine(seed, colour.m_b);
		acid::Maths::HashCombine(seed, colour.m_a);
		return seed;
	}
};
}
