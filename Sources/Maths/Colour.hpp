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
		RGBA, ARGB, RGB
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
	constexpr Colour(float r, float g, float b, float a = 1.0f);

	/**
	 * Constructor for Colour.
	 * @param i The integer value.
	 * @param type The order components of colour are packed.
	 */
	constexpr Colour(uint32_t i, Type type = Type::RGB);
	
	/**
	 * Constructor for Colour.
	 * @param hex The new values from HEX.
	 * @param a The new A value.
	 */
	Colour(std::string hex, float a = 1.0f);

	/**
	 * Calculates the linear interpolation between this colour and another colour.
	 * @param other The other quaternion.
	 * @param progression The progression.
	 * @return Left lerp right.
	 */
	constexpr Colour Lerp(const Colour &other, float progression) const;

	/**
	 * Normalizes this colour.
	 * @return The normalized colour.
	 */
	Colour Normalize() const;

	/**
	 * Gets the length squared of this colour.
	 * @return The length squared.
	 */
	constexpr float LengthSquared() const;

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
	constexpr Colour SmoothDamp(const Colour &target, const Colour &rate) const;

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
	constexpr uint32_t GetInt(Type type = Type::RGBA) const;
	
	/**
	 * Gets the hex code from this colour.
	 * @return The hex code.
	 */
	std::string GetHex() const;

	constexpr float operator[](uint32_t index) const;
	constexpr float &operator[](uint32_t index);

	constexpr bool operator==(const Colour &rhs) const;
	constexpr bool operator!=(const Colour &rhs) const;

	friend constexpr Colour operator+(const Colour &lhs, const Colour &rhs);
	friend constexpr Colour operator-(const Colour &lhs, const Colour &rhs);
	friend constexpr Colour operator*(const Colour &lhs, const Colour &rhs);
	friend constexpr Colour operator/(const Colour &lhs, const Colour &rhs);
	friend constexpr Colour operator+(float lhs, const Colour &rhs);
	friend constexpr Colour operator-(float lhs, const Colour &rhs);
	friend constexpr Colour operator*(float lhs, const Colour &rhs);
	friend constexpr Colour operator/(float lhs, const Colour &rhs);
	friend constexpr Colour operator+(const Colour &lhs, float rhs);
	friend constexpr Colour operator-(const Colour &lhs, float rhs);
	friend constexpr Colour operator*(const Colour &lhs, float rhs);
	friend constexpr Colour operator/(const Colour &lhs, float rhs);

	constexpr Colour &operator+=(const Colour &rhs);
	constexpr Colour &operator-=(const Colour &rhs);
	constexpr Colour &operator*=(const Colour &rhs);
	constexpr Colour &operator/=(const Colour &rhs);
	constexpr Colour &operator+=(float rhs);
	constexpr Colour &operator-=(float rhs);
	constexpr Colour &operator*=(float rhs);
	constexpr Colour &operator/=(float rhs);

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

	float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
};
}

#include "Colour.inl"
