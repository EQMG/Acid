#pragma once

#include "Colour.hpp"

#include "Files/Node.hpp"

namespace acid {
constexpr bool Colour::operator==(const Colour &rhs) const {
	return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
}

constexpr bool Colour::operator!=(const Colour &rhs) const {
	return !operator==(rhs);
}

constexpr Colour operator+(const Colour &lhs, const Colour &rhs) {
	return {lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a};
}

constexpr Colour operator-(const Colour &lhs, const Colour &rhs) {
	return {lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a};
}

constexpr Colour operator*(const Colour &lhs, const Colour &rhs) {
	return {lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a};
}

constexpr Colour operator/(const Colour &lhs, const Colour &rhs) {
	return {lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a};
}

constexpr Colour operator+(float lhs, const Colour &rhs) {
	return Colour(lhs, lhs, lhs, 0.0f) + rhs;
}

constexpr Colour operator-(float lhs, const Colour &rhs) {
	return Colour(lhs, lhs, lhs, 0.0f) - rhs;
}

constexpr Colour operator*(float lhs, const Colour &rhs) {
	return Colour(lhs, lhs, lhs) * rhs;
}

constexpr Colour operator/(float lhs, const Colour &rhs) {
	return Colour(lhs, lhs, lhs) / rhs;
}

constexpr Colour operator+(const Colour &lhs, float rhs) {
	return lhs + Colour(rhs, rhs, rhs, 0.0f);
}

constexpr Colour operator-(const Colour &lhs, float rhs) {
	return lhs - Colour(rhs, rhs, rhs, 0.0f);
}

constexpr Colour operator*(const Colour &lhs, float rhs) {
	return lhs * Colour(rhs, rhs, rhs);
}

constexpr Colour operator/(const Colour &lhs, float rhs) {
	return lhs / Colour(rhs, rhs, rhs);
}

constexpr Colour &Colour::operator+=(const Colour &rhs) {
	return *this = *this + rhs;
}

constexpr Colour &Colour::operator-=(const Colour &rhs) {
	return *this = *this - rhs;
}

constexpr Colour &Colour::operator*=(const Colour &rhs) {
	return *this = *this * rhs;
}

constexpr Colour &Colour::operator/=(const Colour &rhs) {
	return *this = *this / rhs;
}

constexpr Colour &Colour::operator+=(float rhs) {
	return *this = *this + rhs;
}

constexpr Colour &Colour::operator-=(float rhs) {
	return *this = *this - rhs;
}

constexpr Colour &Colour::operator*=(float rhs) {
	return *this = *this * rhs;
}

constexpr Colour &Colour::operator/=(float rhs) {
	return *this = *this / rhs;
}

inline const Node &operator>>(const Node &node, Colour &colour) {
	// Loads from hex if RGBA is not provided.
	if (node.GetProperties().empty()) {
		std::string hex;
		node >> hex;
		colour = hex;
	} else {
		node["r"].Get(colour.r);
		node["g"].Get(colour.g);
		node["b"].Get(colour.b);
		node["a"].Get(colour.a);
	}

	return node;
}

inline Node &operator<<(Node &node, const Colour &colour) {
	//node << colour.GetHex();
	node["r"].Set(colour.r);
	node["g"].Set(colour.g);
	node["b"].Set(colour.b);
	node["a"].Set(colour.a);
	return node;
}

inline std::ostream &operator<<(std::ostream &stream, const Colour &colour) {
	return stream << colour.r << ", " << colour.g << ", " << colour.b << ", " << colour.a;
}
}

namespace std {
template<>
struct hash<acid::Colour> {
	size_t operator()(const acid::Colour &colour) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, colour.r);
		acid::Maths::HashCombine(seed, colour.g);
		acid::Maths::HashCombine(seed, colour.b);
		acid::Maths::HashCombine(seed, colour.a);
		return seed;
	}
};
}
