#include "Quaternion.hpp"

namespace acid {
const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::One(1.0f, 1.0f, 1.0f, 1.0f);
const Quaternion Quaternion::PositiveInfinity(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(),
	+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
const Quaternion Quaternion::NegativeInfinity(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

Quaternion::Quaternion(float x, float y, float z, float w) :
	x(x),
	y(y),
	z(z),
	w(w) {
}

Quaternion::Quaternion(const Vector3f &source) {
	auto sx = std::sin(source.x * 0.5f);
	auto cx = Maths::CosFromSin(sx, source.x * 0.5f);
	auto sy = std::sin(source.y * 0.5f);
	auto cy = Maths::CosFromSin(sy, source.y * 0.5f);
	auto sz = std::sin(source.z * 0.5f);
	auto cz = Maths::CosFromSin(sz, source.z * 0.5f);

	auto cycz = cy * cz;
	auto sysz = sy * sz;
	auto sycz = sy * cz;
	auto cysz = cy * sz;

	w = cx * cycz - sx * sysz;
	x = sx * cycz + cx * sysz;
	y = cx * sycz - sx * cysz;
	z = cx * cysz + sx * sycz;
}

Quaternion::Quaternion(const Matrix4 &source) {
	auto diagonal = source[0][0] + source[1][1] + source[2][2];

	if (diagonal > 0.0f) {
		auto w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
		w = w4 / 4.0f;
		x = (source[2][1] - source[1][2]) / w4;
		y = (source[0][2] - source[2][0]) / w4;
		z = (source[1][0] - source[0][1]) / w4;
	} else if ((source[0][0] > source[1][1]) && (source[0][0] > source[2][2])) {
		auto x4 = std::sqrt(1.0f + source[0][0] - source[1][1] - source[2][2]) * 2.0f;
		w = (source[2][1] - source[1][2]) / x4;
		x = x4 / 4.0f;
		y = (source[0][1] + source[1][0]) / x4;
		z = (source[0][2] + source[2][0]) / x4;
	} else if (source[1][1] > source[2][2]) {
		auto y4 = std::sqrt(1.0f + source[1][1] - source[0][0] - source[2][2]) * 2.0f;
		w = (source[0][2] - source[2][0]) / y4;
		x = (source[0][1] + source[1][0]) / y4;
		y = y4 / 4.0f;
		z = (source[1][2] + source[2][1]) / y4;
	} else {
		auto z4 = std::sqrt(1.0f + source[2][2] - source[0][0] - source[1][1]) * 2.0f;
		w = (source[1][0] - source[0][1]) / z4;
		x = (source[0][2] + source[2][0]) / z4;
		y = (source[1][2] + source[2][1]) / z4;
		z = z4 / 4.0f;
	}
}

Quaternion::Quaternion(const Vector3f &axisX, const Vector3f &axisY, const Vector3f &axisZ) {
	Matrix4 rotation;

	rotation[0][0] = axisX.x;
	rotation[1][0] = axisX.y;
	rotation[2][0] = axisX.z;
	rotation[0][1] = axisY.x;
	rotation[1][1] = axisY.y;
	rotation[2][1] = axisY.z;
	rotation[0][2] = axisZ.x;
	rotation[1][2] = axisZ.y;
	rotation[2][2] = axisZ.z;
	*this = rotation;
}

Quaternion Quaternion::MultiplyInverse(const Quaternion &other) const {
	auto n = other.LengthSquared();
	n = n == 0.0f ? n : 1.0f / n;
	return {
		(x * other.w - w * other.x - y * other.z + z * other.y) * n, (y * other.w - w * other.y - z * other.x + x * other.z) * n,
		(z * other.w - w * other.z - x * other.y + y * other.x) * n, (w * other.w + x * other.x + y * other.y + z * other.z) * n
	};
}

float Quaternion::Dot(const Quaternion &other) const {
	return w * other.w + x * other.x + y * other.y + z * other.z;
}

Quaternion Quaternion::Slerp(const Quaternion &other, float progression) const {
	auto cosom = x * other.x + y * other.y + z * other.z + w * other.w;
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
	result.x = scale0 * x + scale1 * other.x;
	result.y = scale0 * y + scale1 * other.y;
	result.z = scale0 * z + scale1 * other.z;
	result.w = scale0 * w + scale1 * other.w;
	return result;
}

Quaternion Quaternion::Normalize() const {
	auto l = Length();
	return {x / l, y / l, z / l, w / l};
}

float Quaternion::LengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Length() const {
	return std::sqrt(LengthSquared());
}

float Quaternion::MaxComponent() const {
	return std::max({x, y, z, w});
}

float Quaternion::MinComponent() const {
	return std::min({x, y, z, w});
}

Matrix4 Quaternion::ToMatrix() const {
	auto w2 = w * w;
	auto x2 = x * x;
	auto y2 = y * y;
	auto z2 = z * z;
	auto zw = z * w;
	auto xy = x * y;
	auto xz = x * z;
	auto yw = y * w;
	auto yz = y * z;
	auto xw = x * w;

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
	auto xy = x * y;
	auto xz = x * z;
	auto xw = x * w;
	auto yz = y * z;
	auto yw = y * w;
	auto zw = z * w;
	auto xSquared = x * x;
	auto ySquared = y * y;
	auto zSquared = z * z;

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
	result.x = std::atan2(2.0f * (x * w - y * z), 1.0f - 2.0f * (x * x + y * y));
	result.y = std::asin(2.0f * (x * z + y * w));
	result.z = std::atan2(2.0f * (z * w - x * y), 1.0f - 2.0f * (y * y + z * z));
	return result;
}

float Quaternion::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::runtime_error("Quaternion index out of bounds!");
	}
}

float &Quaternion::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::runtime_error("Quaternion index out of bounds!");
	}
}

bool Quaternion::operator==(const Quaternion &rhs) const {
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Quaternion::operator!=(const Quaternion &rhs) const {
	return !operator==(rhs);
}

Quaternion Quaternion::operator-() const {
	return {-x, -y, -z, -w};
}

Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs) {
	return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs) {
	return {
		lhs.x * rhs.w + lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.y * rhs.w + lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.z * rhs.w + lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

Vector3f operator*(const Vector3f &lhs, const Quaternion &rhs) {
	Vector3f q(rhs.x, rhs.y, rhs.z);
	auto cross1 = q.Cross(lhs);
	auto cross2 = q.Cross(cross1);
	return lhs + 2.0f * (cross1 * rhs.w + cross2);
}

Vector3f operator*(const Quaternion &lhs, const Vector3f &rhs) {
	return rhs * lhs;
}

Quaternion operator*(float lhs, const Quaternion &rhs) {
	return {rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs};
}

Quaternion operator*(const Quaternion &lhs, float rhs) {
	return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
}

Quaternion &Quaternion::operator*=(const Quaternion &rhs) {
	return *this = *this * rhs;
}

Quaternion &Quaternion::operator*=(float rhs) {
	return *this = *this * rhs;
}

const Node &operator>>(const Node &node, Quaternion &quaternion) {
	node["x"].Get(quaternion.x);
	node["y"].Get(quaternion.y);
	node["z"].Get(quaternion.z);
	node["w"].Get(quaternion.w);
	return node;
}

Node &operator<<(Node &node, const Quaternion &quaternion) {
	node["x"].Set(quaternion.x);
	node["y"].Set(quaternion.y);
	node["z"].Set(quaternion.z);
	node["w"].Set(quaternion.w);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Quaternion &quaternion) {
	return stream << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w;
}
}
