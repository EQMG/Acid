#include "Quaternion.hpp"

namespace acid {
const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::One(1.0f, 1.0f, 1.0f, 1.0f);
const Quaternion Quaternion::PositiveInfinity(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(),
	+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
const Quaternion Quaternion::NegativeInfinity(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

Quaternion::Quaternion(float x, float y, float z, float w) :
	m_x(x),
	m_y(y),
	m_z(z),
	m_w(w) {
}

Quaternion::Quaternion(const Vector3f &source) {
	auto sx = std::sin(source.m_x * 0.5f);
	auto cx = Maths::CosFromSin(sx, source.m_x * 0.5f);
	auto sy = std::sin(source.m_y * 0.5f);
	auto cy = Maths::CosFromSin(sy, source.m_y * 0.5f);
	auto sz = std::sin(source.m_z * 0.5f);
	auto cz = Maths::CosFromSin(sz, source.m_z * 0.5f);

	auto cycz = cy * cz;
	auto sysz = sy * sz;
	auto sycz = sy * cz;
	auto cysz = cy * sz;

	m_w = cx * cycz - sx * sysz;
	m_x = sx * cycz + cx * sysz;
	m_y = cx * sycz - sx * cysz;
	m_z = cx * cysz + sx * sycz;
}

Quaternion::Quaternion(const Matrix4 &source) {
	auto diagonal = source[0][0] + source[1][1] + source[2][2];

	if (diagonal > 0.0f) {
		auto w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
		m_w = w4 / 4.0f;
		m_x = (source[2][1] - source[1][2]) / w4;
		m_y = (source[0][2] - source[2][0]) / w4;
		m_z = (source[1][0] - source[0][1]) / w4;
	} else if ((source[0][0] > source[1][1]) && (source[0][0] > source[2][2])) {
		auto x4 = std::sqrt(1.0f + source[0][0] - source[1][1] - source[2][2]) * 2.0f;
		m_w = (source[2][1] - source[1][2]) / x4;
		m_x = x4 / 4.0f;
		m_y = (source[0][1] + source[1][0]) / x4;
		m_z = (source[0][2] + source[2][0]) / x4;
	} else if (source[1][1] > source[2][2]) {
		auto y4 = std::sqrt(1.0f + source[1][1] - source[0][0] - source[2][2]) * 2.0f;
		m_w = (source[0][2] - source[2][0]) / y4;
		m_x = (source[0][1] + source[1][0]) / y4;
		m_y = y4 / 4.0f;
		m_z = (source[1][2] + source[2][1]) / y4;
	} else {
		auto z4 = std::sqrt(1.0f + source[2][2] - source[0][0] - source[1][1]) * 2.0f;
		m_w = (source[1][0] - source[0][1]) / z4;
		m_x = (source[0][2] + source[2][0]) / z4;
		m_y = (source[1][2] + source[2][1]) / z4;
		m_z = z4 / 4.0f;
	}
}

Quaternion::Quaternion(const Vector3f &axisX, const Vector3f &axisY, const Vector3f &axisZ) :
	m_x(0.0f),
	m_y(0.0f),
	m_z(0.0f),
	m_w(1.0f) {
	Matrix4 rotation;

	rotation[0][0] = axisX.m_x;
	rotation[1][0] = axisX.m_y;
	rotation[2][0] = axisX.m_z;
	rotation[0][1] = axisY.m_x;
	rotation[1][1] = axisY.m_y;
	rotation[2][1] = axisY.m_z;
	rotation[0][2] = axisZ.m_x;
	rotation[1][2] = axisZ.m_y;
	rotation[2][2] = axisZ.m_z;
	*this = rotation;
}

Quaternion Quaternion::Add(const Quaternion &other) const {
	return {m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w};
}

Quaternion Quaternion::Subtract(const Quaternion &other) const {
	return {m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w};
}

Quaternion Quaternion::Multiply(const Quaternion &other) const {
	return {
		m_x * other.m_w + m_w * other.m_x + m_y * other.m_z - m_z * other.m_y, m_y * other.m_w + m_w * other.m_y + m_z * other.m_x - m_x * other.m_z,
		m_z * other.m_w + m_w * other.m_z + m_x * other.m_y - m_y * other.m_x, m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z
	};
}

Vector3f Quaternion::Multiply(const Vector3f &other) const {
	Vector3f q(m_x, m_y, m_z);
	auto cross1 = q.Cross(other);
	auto cross2 = q.Cross(cross1);
	return other + 2.0f * (cross1 * m_w + cross2);
}

Quaternion Quaternion::MultiplyInverse(const Quaternion &other) const {
	auto n = other.LengthSquared();
	n = n == 0.0f ? n : 1.0f / n;
	return {
		(m_x * other.m_w - m_w * other.m_x - m_y * other.m_z + m_z * other.m_y) * n, (m_y * other.m_w - m_w * other.m_y - m_z * other.m_x + m_x * other.m_z) * n,
		(m_z * other.m_w - m_w * other.m_z - m_x * other.m_y + m_y * other.m_x) * n, (m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z) * n
	};
}

float Quaternion::Dot(const Quaternion &other) const {
	return m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Quaternion Quaternion::Slerp(const Quaternion &other, float progression) const {
	auto cosom = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
	auto absCosom = std::abs(cosom);
	float scale0, scale1;

	if (1.0f - absCosom > 1E-6f) {
		auto sinSqr = 1.0f - absCosom * absCosom;
		auto sinom = 1.0f / std::sqrt(sinSqr);
		auto omega = std::atan2(sinSqr * sinom, absCosom);
		scale0 = std::sin((1.0f - progression) * omega) * sinom;
		scale1 = std::sin(progression * omega) * sinom;
	} else {
		scale0 = 1.0f - progression;
		scale1 = progression;
	}

	scale1 = cosom >= 0.0f ? scale1 : -scale1;
	Quaternion result;
	result.m_x = scale0 * m_x + scale1 * other.m_x;
	result.m_y = scale0 * m_y + scale1 * other.m_y;
	result.m_z = scale0 * m_z + scale1 * other.m_z;
	result.m_w = scale0 * m_w + scale1 * other.m_w;
	return result;
}

Quaternion Quaternion::Scale(float scalar) const {
	return {m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar};
}

Quaternion Quaternion::Normalize() const {
	auto l = Length();
	return {m_x / l, m_y / l, m_z / l, m_w / l};
}

float Quaternion::LengthSquared() const {
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

float Quaternion::Length() const {
	return std::sqrt(LengthSquared());
}

float Quaternion::MaxComponent() const {
	return std::max({m_x, m_y, m_z, m_w});
}

float Quaternion::MinComponent() const {
	return std::min({m_x, m_y, m_z, m_w});
}

Matrix4 Quaternion::ToMatrix() const {
	auto w2 = m_w * m_w;
	auto x2 = m_x * m_x;
	auto y2 = m_y * m_y;
	auto z2 = m_z * m_z;
	auto zw = m_z * m_w;
	auto xy = m_x * m_y;
	auto xz = m_x * m_z;
	auto yw = m_y * m_w;
	auto yz = m_y * m_z;
	auto xw = m_x * m_w;

	Matrix4 result;
	result[0][0] = w2 + x2 - z2 - y2;
	result[0][1] = xy + zw + zw + xy;
	result[0][2] = xz - yw + xz - yw;
	result[1][0] = -zw + xy - zw + xy;
	result[1][1] = y2 - z2 + w2 - x2;
	result[1][2] = yz + yz + xw + xw;
	result[2][0] = yw + xz + xz + yw;
	result[2][1] = yz + yz - xw - xw;
	result[2][2] = z2 - y2 - x2 + w2;
	return result;
}

Matrix4 Quaternion::ToRotationMatrix() const {
	auto xy = m_x * m_y;
	auto xz = m_x * m_z;
	auto xw = m_x * m_w;
	auto yz = m_y * m_z;
	auto yw = m_y * m_w;
	auto zw = m_z * m_w;
	auto xSquared = m_x * m_x;
	auto ySquared = m_y * m_y;
	auto zSquared = m_z * m_z;

	Matrix4 result;
	result[0][0] = 1.0f - 2.0f * (ySquared + zSquared);
	result[0][1] = 2.0f * (xy - zw);
	result[0][2] = 2.0f * (xz + yw);
	result[0][3] = 0.0f;
	result[1][0] = 2.0f * (xy + zw);
	result[1][1] = 1.0f - 2.0f * (xSquared + zSquared);
	result[1][2] = 2.0f * (yz - xw);
	result[1][3] = 0.0f;
	result[2][0] = 2.0f * (xz - yw);
	result[2][1] = 2.0f * (yz + xw);
	result[2][2] = 1.0f - 2.0f * (xSquared + ySquared);
	return result;
}

Vector3f Quaternion::ToEuler() const {
	Vector3f result;
	result.m_x = std::atan2(2.0f * (m_x * m_w - m_y * m_z), 1.0f - 2.0f * (m_x * m_x + m_y * m_y));
	result.m_y = std::asin(2.0f * (m_x * m_z + m_y * m_w));
	result.m_z = std::atan2(2.0f * (m_z * m_w - m_x * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z));
	return result;
}

float Quaternion::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	case 3:
		return m_w;
	default:
		throw std::runtime_error("Quaternion index out of bounds!");
	}
}

float &Quaternion::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	case 3:
		return m_w;
	default:
		throw std::runtime_error("Quaternion index out of bounds!");
	}
}

bool Quaternion::operator==(const Quaternion &other) const {
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
}

bool Quaternion::operator!=(const Quaternion &other) const {
	return !operator==(other);
}

Quaternion Quaternion::operator-() const {
	return {-m_x, -m_y, -m_z, -m_w};
}

Quaternion operator+(const Quaternion &left, const Quaternion &right) {
	return left.Add(right);
}

Quaternion operator-(const Quaternion &left, const Quaternion &right) {
	return left.Subtract(right);
}

Quaternion operator*(const Quaternion &left, const Quaternion &right) {
	return left.Multiply(right);
}

Vector3f operator*(const Vector3f &left, const Quaternion &right) {
	return right.Multiply(left);
}

Vector3f operator*(const Quaternion &left, const Vector3f &right) {
	return left.Multiply(right);
}

Quaternion operator*(float left, const Quaternion &right) {
	return right.Scale(left);
}

Quaternion operator*(const Quaternion &left, float right) {
	return left.Scale(right);
}

Quaternion &Quaternion::operator*=(const Quaternion &other) {
	return *this = Multiply(other);
}

Quaternion &Quaternion::operator*=(float other) {
	return *this = Scale(other);
}

const Node &operator>>(const Node &node, Quaternion &quaternion) {
	node["x"].Get(quaternion.m_x);
	node["y"].Get(quaternion.m_y);
	node["z"].Get(quaternion.m_z);
	node["w"].Get(quaternion.m_w);
	return node;
}

Node &operator<<(Node &node, const Quaternion &quaternion) {
	node["x"].Set(quaternion.m_x);
	node["y"].Set(quaternion.m_y);
	node["z"].Set(quaternion.m_z);
	node["w"].Set(quaternion.m_w);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Quaternion &quaternion) {
	return stream << quaternion.m_x << ", " << quaternion.m_y << ", " << quaternion.m_z << ", " << quaternion.m_w;
}
}
