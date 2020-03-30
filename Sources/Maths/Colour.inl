#pragma once

#include "Colour.hpp"

#include <cassert>
#include <iomanip>

namespace acid {
constexpr Colour::Colour(float r, float g, float b, float a) :
	r(r),
	g(g),
	b(b),
	a(a) {
}

constexpr Colour::Colour(uint32_t i, Type type) {
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

inline Colour::Colour(std::string hex, float a) :
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

constexpr Colour Colour::Lerp(const Colour &other, float progression) const {
	auto ta = *this * (1.0f - progression);
	auto tb = other * progression;
	return ta + tb;
}

inline Colour Colour::Normalize() const {
	auto l = Length();

	if (l == 0.0f)
		throw std::runtime_error("Can't normalize a zero length vector");

	return {r / l, g / l, b / l, a / l};
}

constexpr float Colour::LengthSquared() const {
	return r * r + g * g + b * b + a * a;
}

inline float Colour::Length() const {
	return std::sqrt(LengthSquared());
}

constexpr Colour Colour::SmoothDamp(const Colour &target, const Colour &rate) const {
	return Maths::SmoothDamp(*this, target, rate);
}

inline Colour Colour::GetUnit() const {
	auto l = Length();
	return {r / l, g / l, b / l, a / l};
}

constexpr uint32_t Colour::GetInt(Type type) const {
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

inline std::string Colour::GetHex() const {
	std::stringstream stream;
	stream << "#";

	auto hexValue = ((static_cast<uint32_t>(r * 255.0f) & 0xff) << 16) +
		((static_cast<uint32_t>(g * 255.0f) & 0xff) << 8) +
		((static_cast<uint32_t>(b * 255.0f) & 0xff) << 0);
	stream << std::hex << std::setfill('0') << std::setw(6) << hexValue;

	return stream.str();
}

constexpr float Colour::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	case 3:
		return a;
	default:
		throw std::runtime_error("Colour index out of bounds!");
	}
}

constexpr float &Colour::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	case 3:
		return a;
	default:
		throw std::runtime_error("Colour index out of bounds!");
	}
}

constexpr bool Colour::operator==(const Colour &other) const {
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

constexpr bool Colour::operator!=(const Colour &other) const {
	return !operator==(other);
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

constexpr Colour &Colour::operator+=(const Colour &other) {
	return *this = *this + other;
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
