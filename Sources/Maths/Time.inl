#pragma once

#include "Time.hpp"

#include <iomanip>

#include "Files/Node.hpp"

namespace acid {
template<typename Rep, typename Period>
constexpr Time::Time(const std::chrono::duration<Rep, Period> &duration) :
	value(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) {
}

template<typename Rep>
constexpr Time Time::Seconds(const Rep &seconds) {
	return Time(std::chrono::duration<Rep>(seconds));
}

template<typename Rep>
constexpr Time Time::Milliseconds(const Rep &milliseconds) {
	return Time(std::chrono::duration<Rep, std::micro>(milliseconds));
}

template<typename Rep>
constexpr Time Time::Microseconds(const Rep &microseconds) {
	return Time(std::chrono::duration<Rep, std::micro>(microseconds));
}

template<typename T>
constexpr auto Time::AsSeconds() const {
	return static_cast<T>(value.count()) / static_cast<T>(1000000);
}

template<typename T>
constexpr auto Time::AsMilliseconds() const {
	return static_cast<T>(value.count()) / static_cast<T>(1000);
}

template<typename T>
constexpr auto Time::AsMicroseconds() const {
	return static_cast<T>(value.count());
}

inline Time Time::Now() {
	static const auto LocalEpoch = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - LocalEpoch);

//	auto now = std::chrono::system_clock::now();
//	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
}

inline std::string Time::GetDateTime(const std::string &format) {
	auto now = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&timeT), format.c_str());
	return ss.str();
}

/*template<typename Period>
float Time::Mod(const Time &other) {
	return std::modf(std::chrono::duration_cast<std::chrono::duration<double, Period>>(value),
		std::chrono::duration_cast<std::chrono::duration<double, Period>>(other.value));
}*/

template<typename Rep, typename Period>
constexpr Time::operator std::chrono::duration<Rep, Period>() const {
	return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(value);
}

constexpr bool Time::operator==(const Time &other) const {
	return value == other.value;
}

constexpr bool Time::operator!=(const Time &other) const {
	return value != other.value;
}

constexpr bool Time::operator<(const Time &other) const {
	return value < other.value;
}

constexpr bool Time::operator<=(const Time &other) const {
	return value <= other.value;
}

constexpr bool Time::operator>(const Time &other) const {
	return value > other.value;
}

constexpr bool Time::operator>=(const Time &other) const {
	return value >= other.value;
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

constexpr double operator/(const Time &lhs, const Time &right) {
	return static_cast<double>(lhs.value.count()) / static_cast<double>(right.value.count());
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
