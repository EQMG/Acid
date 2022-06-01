#pragma once

#include <cassert>
#include <sstream>
#include <iomanip>

#include "Maths.hpp"

namespace acid {
class Node;

/**
 * @brief Holds a RGBA colour.
 */
class ACID_CORE_EXPORT Colour {
public:
	/// In order of how bits are mapped [24, 16, 8, 0xFF].
	enum class Type {
		RGBA, ARGB, RGB
	};

	Colour() = default;

	/**
	 * Constructor for Colour.
	 * @param r The new R value.
	 * @param g The new G value.
	 * @param b The new B value.
	 * @param a The new A value.
	 */
	constexpr Colour(float r, float g, float b, float a = 1.0f) :
		r(r),
		g(g),
		b(b),
		a(a) {
	}

	/**
	 * Constructor for Colour.
	 * @param i The integer value.
	 * @param type The order components of colour are packed.
	 */
	constexpr Colour(uint32_t i, Type type = Type::RGB) {
		switch (type) {
		case Type::RGBA:
			r = static_cast<float>((uint8_t)(i >> 24 & 0xFF)) / 255.0f;
			g = static_cast<float>((uint8_t)(i >> 16 & 0xFF)) / 255.0f;
			b = static_cast<float>((uint8_t)(i >> 8 & 0xFF)) / 255.0f;
			a = static_cast<float>((uint8_t)(i & 0xFF)) / 255.0f;
			break;
		case Type::ARGB:
			r = static_cast<float>((uint8_t)(i >> 16)) / 255.0f;
			g = static_cast<float>((uint8_t)(i >> 8)) / 255.0f;
			b = static_cast<float>((uint8_t)(i & 0xFF)) / 255.0f;
			a = static_cast<float>((uint8_t)(i >> 24)) / 255.0f;
			break;
		case Type::RGB:
			r = static_cast<float>((uint8_t)(i >> 16)) / 255.0f;
			g = static_cast<float>((uint8_t)(i >> 8)) / 255.0f;
			b = static_cast<float>((uint8_t)(i & 0xFF)) / 255.0f;
			a = 1.0f;
			break;
		default:
			throw std::runtime_error("Unknown Color type");
		}
	}
	
	/**
	 * Constructor for Colour.
	 * @param hex The new values from HEX.
	 * @param a The new A value.
	 */
	Colour(std::string hex, float a = 1.0f) :
		a(a) {
		if (hex[0] == '#') {
			hex.erase(0, 1);
		}

		assert(hex.size() == 6);
		auto hexValue = std::stoul(hex, nullptr, 16);

		r = static_cast<float>((hexValue >> 16) & 0xff) / 255.0f;
		g = static_cast<float>((hexValue >> 8) & 0xff) / 255.0f;
		b = static_cast<float>((hexValue >> 0) & 0xff) / 255.0f;
	}

	/**
	 * Calculates the linear interpolation between this colour and another colour.
	 * @param other The other quaternion.
	 * @param progression The progression.
	 * @return Left lerp right.
	 */
	constexpr Colour Lerp(const Colour &other, float progression) const {
		auto ta = *this * (1.0f - progression);
		auto tb = other * progression;
		return ta + tb;
	}

	/**
	 * Normalizes this colour.
	 * @return The normalized colour.
	 */
	Colour Normalize() const {
		auto l = Length();

		if (l == 0.0f)
			throw std::runtime_error("Can't normalize a zero length vector");

		return {r / l, g / l, b / l, a / l};
	}

	/**
	 * Gets the length squared of this colour.
	 * @return The length squared.
	 */
	constexpr float Length2() const {
		return r * r + g * g + b * b + a * a;
	}

	/**
	 * Gets the length of this colour.
	 * @return The length.
	 */
	float Length() const {
		return std::sqrt(Length2());
	}

	/**
	 * Gradually changes this colour to a target.
	 * @param target The target colour.
	 * @param rate The rate to go from current to the target.
	 * @return The changed colour.
	 */
	constexpr Colour SmoothDamp(const Colour &target, const Colour &rate) const {
		return Maths::SmoothDamp(*this, target, rate);
	}

	/**
	 * Gets a colour representing the unit value of this colour.
	 * @return The unit colour.
	 */
	Colour GetUnit() const {
		auto l = Length();
		return {r / l, g / l, b / l, a / l};
	}

	/**
	 * Gets a packed integer representing this colour.
	 * @param type The order components of colour are packed.
	 * @return The packed integer.
	 */
	constexpr uint32_t GetInt(Type type = Type::RGBA) const {
		switch (type) {
		case Type::RGBA:
			return (static_cast<uint8_t>(r * 255.0f) << 24) | (static_cast<uint8_t>(g * 255.0f) << 16) | (static_cast<uint8_t>(b * 255.0f) << 8) | (static_cast<uint8_t>(a * 255.0f) &
				0xFF);
		case Type::ARGB:
			return (static_cast<uint8_t>(a * 255.0f) << 24) | (static_cast<uint8_t>(r * 255.0f) << 16) | (static_cast<uint8_t>(g * 255.0f) << 8) | (static_cast<uint8_t>(b * 255.0f) &
				0xFF);
		case Type::RGB:
			return (static_cast<uint8_t>(r * 255.0f) << 16) | (static_cast<uint8_t>(g * 255.0f) << 8) | (static_cast<uint8_t>(b * 255.0f) & 0xFF);
		default:
			throw std::runtime_error("Unknown Color type");
		}
	}
	
	/**
	 * Gets the hex code from this colour.
	 * @return The hex code.
	 */
	std::string GetHex() const {
		std::stringstream stream;
		stream << "#";

		auto hexValue = ((static_cast<uint32_t>(r * 255.0f) & 0xff) << 16) +
			((static_cast<uint32_t>(g * 255.0f) & 0xff) << 8) +
			((static_cast<uint32_t>(b * 255.0f) & 0xff) << 0);
		stream << std::hex << std::setfill('0') << std::setw(6) << hexValue;

		return stream.str();
	}

	constexpr float operator[](uint32_t i) const {
		assert(i < 4 && "Colour subscript out of range");
		return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
	}
	constexpr float &operator[](uint32_t i){
		assert(i < 4 && "Colour subscript out of range");
		return i == 0 ? r : i == 1 ? g : i == 2 ? b : a;
	}

	constexpr bool operator==(const Colour &rhs) const;
	constexpr bool operator!=(const Colour &rhs) const;

	constexpr friend Colour operator+(const Colour &lhs, const Colour &rhs);
	constexpr friend Colour operator-(const Colour &lhs, const Colour &rhs);
	constexpr friend Colour operator*(const Colour &lhs, const Colour &rhs);
	constexpr friend Colour operator/(const Colour &lhs, const Colour &rhs);
	constexpr friend Colour operator+(float lhs, const Colour &rhs);
	constexpr friend Colour operator-(float lhs, const Colour &rhs);
	constexpr friend Colour operator*(float lhs, const Colour &rhs);
	constexpr friend Colour operator/(float lhs, const Colour &rhs);
	constexpr friend Colour operator+(const Colour &lhs, float rhs);
	constexpr friend Colour operator-(const Colour &lhs, float rhs);
	constexpr friend Colour operator*(const Colour &lhs, float rhs);
	constexpr friend Colour operator/(const Colour &lhs, float rhs);

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
