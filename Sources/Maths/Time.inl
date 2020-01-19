#pragma once

#include "Time.hpp"

#include <iomanip>

#include "Files/Node.hpp"

namespace acid {
template<typename Rep, typename Period>
constexpr Time::Time(const std::chrono::duration<Rep, Period> &duration) :
	m_value(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) {
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
	return static_cast<T>(m_value.count()) / static_cast<T>(1000000);
}

template<typename T>
constexpr auto Time::AsMilliseconds() const {
	return static_cast<T>(m_value.count()) / static_cast<T>(1000);
}

template<typename T>
constexpr auto Time::AsMicroseconds() const {
	return static_cast<T>(m_value.count());
}

inline Time Time::Now() {
	//static const auto LocalEpoch = std::chrono::high_resolution_clock::now();
	//return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - LocalEpoch);

	auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
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
	return std::modf(std::chrono::duration_cast<std::chrono::duration<double, Period>>(m_value),
		std::chrono::duration_cast<std::chrono::duration<double, Period>>(other.m_value));
}*/

template<typename Rep, typename Period>
constexpr Time::operator std::chrono::duration<Rep, Period>() const {
	return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(m_value);
}

constexpr bool Time::operator==(const Time &other) const {
	return m_value == other.m_value;
}

constexpr bool Time::operator!=(const Time &other) const {
	return m_value != other.m_value;
}

constexpr bool Time::operator<(const Time &other) const {
	return m_value < other.m_value;
}

constexpr bool Time::operator<=(const Time &other) const {
	return m_value <= other.m_value;
}

constexpr bool Time::operator>(const Time &other) const {
	return m_value > other.m_value;
}

constexpr bool Time::operator>=(const Time &other) const {
	return m_value >= other.m_value;
}

constexpr Time Time::operator-() const {
	return Time(-m_value);
}

constexpr Time operator+(const Time &left, const Time &right) {
	return left.m_value + right.m_value;
}

constexpr Time operator-(const Time &left, const Time &right) {
	return left.m_value - right.m_value;
}

constexpr Time operator*(const Time &left, float right) {
	return left.m_value * right;
}

constexpr Time operator*(const Time &left, int64_t right) {
	return left.m_value * right;
}

constexpr Time operator*(float left, const Time &right) {
	return right * left;
}

constexpr Time operator*(int64_t left, const Time &right) {
	return right * left;
}

constexpr Time operator/(const Time &left, float right) {
	return left.m_value / right;
}

constexpr Time operator/(const Time &left, int64_t right) {
	return left.m_value / right;
}

constexpr double operator/(const Time &left, const Time &right) {
	return static_cast<double>(left.m_value.count()) / static_cast<double>(right.m_value.count());
}

constexpr Time &Time::operator+=(const Time &other) {
	return *this = *this + other;
}

constexpr Time &Time::operator-=(const Time &other) {
	return *this = *this - other;
}

constexpr Time &Time::operator*=(float other) {
	return *this = *this * other;
}

constexpr Time &Time::operator*=(int64_t other) {
	return *this = *this * other;
}

constexpr Time &Time::operator/=(float other) {
	return *this = *this / other;
}

constexpr Time &Time::operator/=(int64_t other) {
	return *this = *this / other;
}

inline const Node &operator>>(const Node &node, Time &time) {
	time.m_value = std::chrono::microseconds(node.Get<int64_t>());
	return node;
}

inline Node &operator<<(Node &node, const Time &time) {
	node.Set(time.m_value.count());
	return node;
}
}
