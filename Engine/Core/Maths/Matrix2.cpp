#include "Matrix2.hpp"

#include <cassert>
#include <cstring>

#include "Matrix3.hpp"
#include "Matrix4.hpp"

namespace acid {
Matrix2::Matrix2(float diagonal) {
	std::memset(rows, 0, 2 * 2 * sizeof(float));
	rows[0][0] = diagonal;
	rows[1][1] = diagonal;
}

Matrix2::Matrix2(const Matrix3 &source) {
	std::memcpy(rows, source.rows, 2 * sizeof(Vector2f));
}

Matrix2::Matrix2(const Matrix4 &source) {
	std::memcpy(rows, source.rows, 2 * sizeof(Vector2f));
}

Matrix2::Matrix2(const float *source) {
	std::memcpy(rows, source, 2 * 2 * sizeof(float));
}

Matrix2::Matrix2(const Vector2f *source) {
	std::memcpy(rows, source, 2 * sizeof(Vector2f));
}

Matrix2 Matrix2::Add(const Matrix2 &other) const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[row][col] + other[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Subtract(const Matrix2 &other) const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[row][col] - other[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Multiply(const Matrix2 &other) const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[0][col] * other[row][0] + rows[1][col] * other[row][1];
		}
	}

	return result;
}

Matrix2 Matrix2::Divide(const Matrix2 &other) const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[0][col] / other[row][0] + rows[1][col] / other[row][1];
		}
	}

	return result;
}

Vector2f Matrix2::Transform(const Vector2f &other) const {
	Vector2f result;

	for (uint32_t row = 0; row < 3; row++) {
		result[row] = rows[0][row] * other.x + rows[1][row] * other.y;
	}

	return result;
}

Matrix2 Matrix2::Scale(const Vector2f &other) const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[row][col] * other[row];
		}
	}

	return result;
}

Matrix2 Matrix2::Negate() const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = -rows[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Inverse() const {
	Matrix2 result;

	auto det = Determinant();

	if (det == 0.0f) {
		throw std::runtime_error("Can't invert a matrix with a determinant of zero");
	}

	for (uint32_t j = 0; j < 2; j++) {
		for (uint32_t i = 0; i < 2; i++) {
			// Get minor of element [j][i] - not [i][j], this is where the transpose happens.
			auto minor = GetSubmatrix(j, i);

			// Multiply by (−1)^{i+j}.
			auto factor = ((i + j) % 2 == 1) ? -1.0f : 1.0f;
			auto cofactor = minor * factor;

			result[i][j] = cofactor / det;
		}
	}

	return result;
}

Matrix2 Matrix2::Transpose() const {
	Matrix2 result;

	for (uint32_t row = 0; row < 2; row++) {
		for (uint32_t col = 0; col < 2; col++) {
			result[row][col] = rows[col][row];
		}
	}

	return result;
}

float Matrix2::Determinant() const {
	float result = 0.0f;

	for (uint32_t i = 0; i < 2; i++) {
		// Get minor of element [0][i].
		auto minor = GetSubmatrix(0, i);

		// If this is an odd-numbered row, negate the value.
		auto factor = (i % 2 == 1) ? -1.0f : 1.0f;

		result += factor * rows[0][i] * minor;
	}

	return result;
}

float Matrix2::GetSubmatrix(uint32_t row, uint32_t col) const {
	float result = 0.0f;
	uint32_t colCount = 0;
	uint32_t rowCount = 0;

	for (uint32_t i = 0; i < 2; i++) {
		if (i != row) {
			colCount = 0;

			for (uint32_t j = 0; j < 2; j++) {
				if (j != col) {
					result = rows[i][j];
					colCount++;
				}
			}

			rowCount++;
		}
	}

	return result;
}

const Vector2f &Matrix2::operator[](uint32_t index) const {
	assert(index < 2);
	return rows[index];
}

Vector2f &Matrix2::operator[](uint32_t index) {
	assert(index < 2);
	return rows[index];
}

bool Matrix2::operator==(const Matrix2 &other) const {
	return rows[0] == other.rows[0] && rows[1] == other.rows[1];
}

bool Matrix2::operator!=(const Matrix2 &other) const {
	return !operator==(other);
}

Matrix2 Matrix2::operator-() const {
	return Negate();
}

Matrix2 operator+(const Matrix2 &left, const Matrix2 &right) {
	return left.Add(right);
}

Matrix2 operator-(const Matrix2 &left, const Matrix2 &right) {
	return left.Subtract(right);
}

Matrix2 operator*(const Matrix2 &left, const Matrix2 &right) {
	return left.Multiply(right);
}

Matrix2 operator/(const Matrix2 &left, const Matrix2 &right) {
	return left.Divide(right);
}

Matrix2 operator*(const Vector2f &left, const Matrix2 &right) {
	return right.Scale(left);
}

Matrix2 operator/(const Vector2f &left, const Matrix2 &right) {
	return right.Scale(1.0f / left);
}

Matrix2 operator*(const Matrix2 &left, const Vector2f &right) {
	return left.Scale(right);
}

Matrix2 operator/(const Matrix2 &left, const Vector2f &right) {
	return left.Scale(1.0f / right);
}

Matrix2 operator*(float left, const Matrix2 &right) {
	return right.Scale(Vector2(left));
}

Matrix2 operator/(float left, const Matrix2 &right) {
	return right.Scale(1.0f / Vector2(left));
}

Matrix2 operator*(const Matrix2 &left, float right) {
	return left.Scale(Vector2(right));
}

Matrix2 operator/(const Matrix2 &left, float right) {
	return left.Scale(1.0f / Vector2(right));
}

Matrix2 &Matrix2::operator+=(const Matrix2 &other) {
	return *this = Add(other);
}

Matrix2 &Matrix2::operator-=(const Matrix2 &other) {
	return *this = Subtract(other);
}

Matrix2 &Matrix2::operator*=(const Matrix2 &other) {
	return *this = Multiply(other);
}

Matrix2 &Matrix2::operator/=(const Matrix2 &other) {
	return *this = Divide(other);
}

Matrix2 &Matrix2::operator*=(const Vector2f &other) {
	return *this = Scale(other);
}

Matrix2 &Matrix2::operator/=(const Vector2f &other) {
	return *this = Scale(1.0f / other);
}

Matrix2 &Matrix2::operator*=(float other) {
	return *this = Scale(Vector2(other));
}

Matrix2 &Matrix2::operator/=(float other) {
	return *this = Scale(1.0f / Vector2(other));
}

const Node &operator>>(const Node &node, Matrix2 &matrix) {
	node["m0"].Get(matrix.rows[0]);
	node["m1"].Get(matrix.rows[1]);
	return node;
}

Node &operator<<(Node &node, const Matrix2 &matrix) {
	node["m0"].Set(matrix.rows[0]);
	node["m1"].Set(matrix.rows[1]);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix) {
	return stream << matrix[0] << ", " << matrix[1];
}
}
