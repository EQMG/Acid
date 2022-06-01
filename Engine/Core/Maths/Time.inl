#pragma once

#include "Time.hpp"

#include "Files/Node.hpp"

namespace acid {
constexpr bool Time::operator==(const Time &rhs) const {
	return value == rhs.value;
}

constexpr bool Time::operator!=(const Time &rhs) const {
	return value != rhs.value;
}

constexpr bool Time::operator<(const Time &rhs) const {
	return value < rhs.value;
}

constexpr bool Time::operator<=(const Time &rhs) const {
	return value <= rhs.value;
}

constexpr bool Time::operator>(const Time &rhs) const {
	return value > rhs.value;
}

constexpr bool Time::operator>=(const Time &rhs) const {
	return value >= rhs.value;
}

constexpr Time Time::operator-() const {
	return Time(-value);
}

constexpr Time operator+(const Time &lhs, const Time &rhs) {
	return lhs.value + rhs.value;
}

constexpr Time operator-(const Time &lhs, const Time &rhs) {
	return lhs.value - rhs.value;
}

constexpr Time operator*(const Time &lhs, float rhs) {
	return lhs.value * rhs;
}

constexpr Time operator*(const Time &lhs, int64_t rhs) {
	return lhs.value * rhs;
}

constexpr Time operator*(float lhs, const Time &rhs) {
	return rhs * lhs;
}

constexpr Time operator*(int64_t lhs, const Time &rhs) {
	return rhs * lhs;
}

constexpr Time operator/(const Time &lhs, float rhs) {
	return lhs.value / rhs;
}

constexpr Time operator/(const Time &lhs, int64_t rhs) {
	return lhs.value / rhs;
}

constexpr double operator/(const Time &lhs, const Time &rhs) {
	return static_cast<double>(lhs.value.count()) / static_cast<double>(rhs.value.count());
}

constexpr Time &Time::operator+=(const Time &rhs) {
	return *this = *this + rhs;
}

constexpr Time &Time::operator-=(const Time &rhs) {
	return *this = *this - rhs;
}

constexpr Time &Time::operator*=(float rhs) {
	return *this = *this * rhs;
}

constexpr Time &Time::operator*=(int64_t rhs) {
	return *this = *this * rhs;
}

constexpr Time &Time::operator/=(float rhs) {
	return *this = *this / rhs;
}

constexpr Time &Time::operator/=(int64_t rhs) {
	return *this = *this / rhs;
}

inline const Node &operator>>(const Node &node, Time &time) {
	time.value = std::chrono::microseconds(node.Get<int64_t>());
	return node;
}

inline Node &operator<<(Node &node, const Time &time) {
	node.Set(time.value.count());
	return node;
}
}
