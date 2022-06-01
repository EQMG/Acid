#include "Matrix4.hpp"

#include <cassert>
#include <cstring>

#include "Matrix2.hpp"
#include "Matrix3.hpp"

namespace acid {
Matrix4::Matrix4(float diagonal) {
	std::memset(rows, 0, 4 * sizeof(Vector4f));
	rows[0][0] = diagonal;
	rows[1][1] = diagonal;
	rows[2][2] = diagonal;
	rows[3][3] = diagonal;
}

Matrix4::Matrix4(const Matrix2 &source) {
	std::memset(rows, 0, 4 * sizeof(Vector4f));
	rows[2][2] = 1.0f;
	rows[3][3] = 1.0f;
	std::memcpy(rows, source.rows, 2 * sizeof(Vector2f));
}

Matrix4::Matrix4(const Matrix3 &source) {
	std::memset(rows, 0, 4 * sizeof(Vector4f));
	rows[3][3] = 1.0f;
	std::memcpy(rows, source.rows, 3 * sizeof(Vector3f));
}

Matrix4::Matrix4(const float *source) {
	std::memcpy(rows, source, 4 * 4 * sizeof(float));
}

Matrix4::Matrix4(const Vector4f *source) {
	std::memcpy(rows, source, 4 * sizeof(Vector4f));
}

Matrix4 Matrix4::Add(const Matrix4 &other) const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[row][col] + other[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Subtract(const Matrix4 &other) const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[row][col] - other[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Multiply(const Matrix4 &other) const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[0][col] * other[row][0] + rows[1][col] * other[row][1] + rows[2][col] * other[row][2] + rows[3][col] * other[row][3];
		}
	}

	return result;
}

Vector4f Matrix4::Multiply(const Vector4f &other) const {
	Vector4f result;

	for (uint32_t row = 0; row < 4; row++) {
		result[row] = rows[0][row] * other.x + rows[1][row] * other.y + rows[2][row] * other.z + rows[3][row] * other.w;
	}

	return result;
}

Matrix4 Matrix4::Divide(const Matrix4 &other) const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[0][col] / other[row][0] + rows[1][col] / other[row][1] + rows[2][col] / other[row][2] + rows[3][col] / other[row][3];
		}
	}

	return result;
}

Vector4f Matrix4::Transform(const Vector4f &other) const {
	Vector4f result;

	for (uint32_t row = 0; row < 4; row++) {
		result[row] = rows[0][row] * other.x + rows[1][row] * other.y + rows[2][row] * other.z + rows[3][row] * other.w;
	}

	return result;
}

Matrix4 Matrix4::Translate(const Vector2f &other) const {
	Matrix4 result(*this);

	for (uint32_t col = 0; col < 4; col++) {
		result[3][col] += rows[0][col] * other.x + rows[1][col] * other.y;
	}

	return result;
}

Matrix4 Matrix4::Translate(const Vector3f &other) const {
	Matrix4 result(*this);

	for (uint32_t col = 0; col < 4; col++) {
		result[3][col] += rows[0][col] * other.x + rows[1][col] * other.y + rows[2][col] * other.z;
	}

	return result;
}

Matrix4 Matrix4::Scale(const Vector3f &other) const {
	Matrix4 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[row][col] * other[row];
		}
	}

	result[3] = rows[3];
	return result;
}

Matrix4 Matrix4::Scale(const Vector4f &other) const {
	Matrix4 result(*this);

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] *= other[row];
		}
	}

	return result;
}

Matrix4 Matrix4::Rotate(float angle, const Vector3f &axis) const {
	Matrix4 result;

	auto c = std::cos(angle);
	auto s = std::sin(angle);
	auto o = 1.0f - c;
	auto xy = axis.x * axis.y;
	auto yz = axis.y * axis.z;
	auto xz = axis.x * axis.z;
	auto xs = axis.x * s;
	auto ys = axis.y * s;
	auto zs = axis.z * s;

	Matrix3 f;
	f[0][0] = axis.x * axis.x * o + c;
	f[0][1] = xy * o + zs;
	f[0][2] = xz * o - ys;
	f[1][0] = xy * o - zs;
	f[1][1] = axis.y * axis.y * o + c;
	f[1][2] = yz * o + xs;
	f[2][0] = xz * o + ys;
	f[2][1] = yz * o - xs;
	f[2][2] = axis.z * axis.z * o + c;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[0][col] * f[row][0] + rows[1][col] * f[row][1] + rows[2][col] * f[row][2];
		}
	}

	result[3] = rows[3];
	return result;
}

Matrix4 Matrix4::Negate() const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = -rows[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Inverse() const {
	Matrix4 result;

	auto det = Determinant();

	if (det == 0.0f) {
		throw std::runtime_error("Can't invert a matrix with a determinant of zero");
	}

	for (uint32_t j = 0; j < 4; j++) {
		for (uint32_t i = 0; i < 4; i++) {
			// Get minor of element [j][i] - not [i][j], this is where the transpose happens.
			auto minorSubmatrix = GetSubmatrix(j, i);
			auto minor = minorSubmatrix.Determinant();

			// Multiply by (−1)^{i+j}.
			auto factor = ((i + j) % 2 == 1) ? -1.0f : 1.0f;
			auto cofactor = minor * factor;

			result[i][j] = cofactor / det;
		}
	}

	return result;
}

Matrix4 Matrix4::Transpose() const {
	Matrix4 result;

	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t col = 0; col < 4; col++) {
			result[row][col] = rows[col][row];
		}
	}

	return result;
}

float Matrix4::Determinant() const {
	float result = 0.0f;

	for (uint32_t i = 0; i < 4; i++) {
		// Get minor of element [0][i].
		auto minorSubmatrix = GetSubmatrix(0, i);
		auto minor = minorSubmatrix.Determinant();

		// If this is an odd-numbered row, negate the value.
		auto factor = (i % 2 == 1) ? -1.0f : 1.0f;

		result += factor * rows[0][i] * minor;
	}

	return result;
}

Matrix3 Matrix4::GetSubmatrix(uint32_t row, uint32_t col) const {
	Matrix3 result;
	uint32_t colCount = 0;
	uint32_t rowCount = 0;

	for (uint32_t i = 0; i < 4; i++) {
		if (i != row) {
			colCount = 0;

			for (uint32_t j = 0; j < 4; j++) {
				if (j != col) {
					result[rowCount][colCount] = rows[i][j];
					colCount++;
				}
			}

			rowCount++;
		}
	}

	return result;
}

Matrix4 Matrix4::TransformationMatrix(const Vector3f &translation, const Vector3f &rotation, const Vector3f &scale) {
	Matrix4 result;
	result = result.Translate(translation);
	result = result.Rotate(rotation.x, Vector3f::Right);
	result = result.Rotate(rotation.y, Vector3f::Up);
	result = result.Rotate(rotation.z, Vector3f::Front);
	result = result.Scale(scale);
	return result;
}

Matrix4 Matrix4::PerspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar) {
	Matrix4 result(0.0f);

	auto f = std::tan(0.5f * fov);

	result[0][0] = 1.0f / (aspectRatio * f);
	result[1][1] = 1.0f / f;
	result[2][2] = zFar / (zNear - zFar);
	result[2][3] = -1.0f;
	result[3][2] = -(zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::PerspectiveMatrix(float fov, float aspectRatio, float zNear) {
	Matrix4 result(0.0f);

	auto range = std::tan(0.5f * fov) * zNear;
	auto left = -range * aspectRatio;
	auto right = range * aspectRatio;
	auto bottom = -range;
	auto top = range;

	result[0][0] = (2.0f * zNear) / (right - left);
	result[1][1] = (2.0f * zNear) / (top - bottom);
	result[2][2] = -1.0f;
	result[2][3] = -1.0f;
	result[3][2] = -2.0f * zNear;
	return result;
}

Matrix4 Matrix4::OrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar) {
	Matrix4 result;

	result[0][0] = 2.0f / (right - left);
	result[1][1] = 2.0f / (top - bottom);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[2][2] = -1.0f / (zFar - zNear);
	result[2][3] = zNear / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::FrustumMatrix(float left, float right, float bottom, float top, float zNear, float zFar) {
	Matrix4 result(0.0f);

	result[0][0] = (2.0f * zNear) / (right - left);
	result[1][1] = (2.0f * zNear) / (top - bottom);
	result[2][0] = (right + left) / (right - left);
	result[2][1] = (top + bottom) / (top - bottom);
	result[2][3] = -1.0f;
	result[2][2] = zFar / (zNear - zFar);
	result[3][2] = -(zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::ViewMatrix(const Vector3f &position, const Vector3f &rotation) {
	Matrix4 result;

	result = result.Rotate(rotation.x, Vector3f::Right);
	result = result.Rotate(rotation.y, Vector3f::Up);
	result = result.Rotate(rotation.z, Vector3f::Front);
	result = result.Translate(-position);
	return result;
}

Vector3f Matrix4::Project(const Vector3f &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) {
	Vector4f point4(worldSpace, 1.0f);
	point4 = viewMatrix.Transform(point4);
	point4 = projectionMatrix.Transform(point4);

	Vector3f result(point4);
	result.x /= result.z;
	result.y /= result.z;
	return result;
}

Vector3f Matrix4::Unproject(const Vector3f &screenSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) {
	Vector3f result;
	// TODO: Write unproject method
	return result;
}

Matrix4 Matrix4::LookAt(const Vector3f &eye, const Vector3f &centre, const Vector3f &up) {
	Matrix4 result;

	auto f = (centre - eye).Normalize();
	auto s = f.Cross(up).Normalize();
	auto u = s.Cross(f);

	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;
	result[3][0] = -s.Dot(eye);
	result[3][1] = -u.Dot(eye);
	result[3][2] = f.Dot(eye);
	return result;
}

const Vector4f &Matrix4::operator[](uint32_t index) const {
	assert(index < 4);
	return rows[index];
}

Vector4f &Matrix4::operator[](uint32_t index) {
	assert(index < 4);
	return rows[index];
}

bool Matrix4::operator==(const Matrix4 &other) const {
	return rows[0] == other[0] && rows[1] == other[1] && rows[2] == other[2] && rows[3] == other[3];
}

bool Matrix4::operator!=(const Matrix4 &other) const {
	return !operator==(other);
}

Matrix4 Matrix4::operator-() const {
	return Negate();
}

Matrix4 operator+(const Matrix4 &left, const Matrix4 &right) {
	return left.Add(right);
}

Matrix4 operator-(const Matrix4 &left, const Matrix4 &right) {
	return left.Subtract(right);
}

Matrix4 operator*(const Matrix4 &left, const Matrix4 &right) {
	return left.Multiply(right);
}

Matrix4 operator/(const Matrix4 &left, const Matrix4 &right) {
	return left.Divide(right);
}

Matrix4 operator*(const Vector4f &left, const Matrix4 &right) {
	return right.Scale(left);
}

Matrix4 operator/(const Vector4f &left, const Matrix4 &right) {
	return right.Scale(1.0f / left);
}

Matrix4 operator*(const Matrix4 &left, const Vector4f &right) {
	return left.Scale(right);
}

Matrix4 operator/(const Matrix4 &left, const Vector4f &right) {
	return left.Scale(1.0f / right);
}

Matrix4 operator*(float left, const Matrix4 &right) {
	return right.Scale(Vector4f(left));
}

Matrix4 operator/(float left, const Matrix4 &right) {
	return right.Scale(1.0f / Vector4f(left));
}

Matrix4 operator*(const Matrix4 &left, float right) {
	return left.Scale(Vector4f(right));
}

Matrix4 operator/(const Matrix4 &left, float right) {
	return left.Scale(1.0f / Vector4f(right));
}

Matrix4 &Matrix4::operator+=(const Matrix4 &other) {
	return *this = Add(other);
}

Matrix4 &Matrix4::operator-=(const Matrix4 &other) {
	return *this = Subtract(other);
}

Matrix4 &Matrix4::operator*=(const Matrix4 &other) {
	return *this = Multiply(other);
}

Matrix4 &Matrix4::operator/=(const Matrix4 &other) {
	return *this = Divide(other);
}

Matrix4 &Matrix4::operator*=(const Vector4f &other) {
	return *this = Scale(other);
}

Matrix4 &Matrix4::operator/=(const Vector4f &other) {
	return *this = Scale(1.0f / other);
}

Matrix4 &Matrix4::operator*=(float other) {
	return *this = Scale(Vector4f(other));
}

Matrix4 &Matrix4::operator/=(float other) {
	return *this = Scale(1.0f / Vector4f(other));
}

const Node &operator>>(const Node &node, Matrix4 &matrix) {
	node["m0"].Get(matrix.rows[0]);
	node["m1"].Get(matrix.rows[1]);
	node["m2"].Get(matrix.rows[2]);
	node["m3"].Get(matrix.rows[3]);
	return node;
}

Node &operator<<(Node &node, const Matrix4 &matrix) {
	node["m0"].Set(matrix.rows[0]);
	node["m1"].Set(matrix.rows[1]);
	node["m2"].Set(matrix.rows[2]);
	node["m3"].Set(matrix.rows[3]);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix) {
	return stream << matrix[0] << ", " << matrix[1] << ", " << matrix[2] << ", " << matrix[3];
}
}
