#include "Matrix3.hpp"

#include "Matrix2.hpp"
#include "Matrix4.hpp"

namespace acid {
Matrix3::Matrix3(float diagonal) {
	std::memset(m_rows, 0, 3 * 3 * sizeof(float));
	m_rows[0][0] = diagonal;
	m_rows[1][1] = diagonal;
	m_rows[2][2] = diagonal;
}

Matrix3::Matrix3(const Matrix2 &source) {
	std::memset(m_rows, 0, 3 * 3 * sizeof(float));
	m_rows[2][2] = 1.0f;
	std::memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2f));
}

Matrix3::Matrix3(const Matrix4 &source) {
	std::memcpy(m_rows, source.m_rows, 3 * sizeof(Vector3f));
}

Matrix3::Matrix3(const float *source) {
	std::memcpy(m_rows, source, 3 * 3 * sizeof(float));
}

Matrix3::Matrix3(const Vector3f *source) {
	std::memcpy(m_rows, source, 3 * sizeof(Vector3f));
}

Matrix3 Matrix3::Add(const Matrix3 &other) const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[row][col] + other[row][col];
		}
	}

	return result;
}

Matrix3 Matrix3::Subtract(const Matrix3 &other) const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[row][col] - other[row][col];
		}
	}

	return result;
}

Matrix3 Matrix3::Multiply(const Matrix3 &other) const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[0][col] * other[row][0] + m_rows[1][col] * other[row][1] + m_rows[2][col] * other[row][2];
		}
	}

	return result;
}

Vector3f Matrix3::Multiply(const Vector3f &other) const {
	Vector3f result;

	for (uint32_t row = 0; row < 3; row++) {
		result[row] = m_rows[row][0] * other[0] + m_rows[row][1] * other[1] + m_rows[row][2] * other[2];
	}

	return result;
}

Matrix3 Matrix3::Divide(const Matrix3 &other) const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[0][col] / other[row][0] + m_rows[1][col] / other[row][1] + m_rows[2][col] / other[row][2];
		}
	}

	return result;
}

Vector3f Matrix3::Transform(const Vector3f &other) const {
	Vector3f result;

	for (uint32_t row = 0; row < 3; row++) {
		result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y + m_rows[2][row] * other.m_z;
	}

	return result;
}

Matrix3 Matrix3::Scale(const Vector3f &other) const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[row][col] * other[row];
		}
	}

	return result;
}

Matrix3 Matrix3::Negate() const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = -m_rows[row][col];
		}
	}

	return result;
}

Matrix3 Matrix3::Inverse() const {
	Matrix3 result;

	auto det = Determinant();

	if (det == 0.0f) {
		throw std::runtime_error("Can't invert a matrix with a determinant of zero");
	}

	for (uint32_t j = 0; j < 3; j++) {
		for (uint32_t i = 0; i < 3; i++) {
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

Matrix3 Matrix3::Transpose() const {
	Matrix3 result;

	for (uint32_t row = 0; row < 3; row++) {
		for (uint32_t col = 0; col < 3; col++) {
			result[row][col] = m_rows[col][row];
		}
	}

	return result;
}

float Matrix3::Determinant() const {
	float result = 0.0f;

	for (uint32_t i = 0; i < 3; i++) {
		// Get minor of element [0][i].
		auto minorSubmatrix = GetSubmatrix(0, i);
		auto minor = minorSubmatrix.Determinant();

		// If this is an odd-numbered row, negate the value.
		auto factor = (i % 2 == 1) ? -1.0f : 1.0f;

		result += factor * m_rows[0][i] * minor;
	}

	return result;
}

Matrix2 Matrix3::GetSubmatrix(uint32_t row, uint32_t col) const {
	Matrix2 result;
	uint32_t colCount = 0;
	uint32_t rowCount = 0;

	for (uint32_t i = 0; i < 3; i++) {
		if (i != row) {
			colCount = 0;

			for (uint32_t j = 0; j < 3; j++) {
				if (j != col) {
					result[rowCount][colCount] = m_rows[i][j];
					colCount++;
				}
			}

			rowCount++;
		}
	}

	return result;
}

const Vector3f &Matrix3::operator[](uint32_t index) const {
	assert(index < 3);
	return m_rows[index];
}

Vector3f &Matrix3::operator[](uint32_t index) {
	assert(index < 3);
	return m_rows[index];
}

bool Matrix3::operator==(const Matrix3 &other) const {
	return m_rows[0] == other[0] && m_rows[1] == other[1] && m_rows[2] == other[2];
}

bool Matrix3::operator!=(const Matrix3 &other) const {
	return !(*this == other);
}

Matrix3 Matrix3::operator-() const {
	return Negate();
}

Matrix3 operator+(const Matrix3 &left, const Matrix3 &right) {
	return left.Add(right);
}

Matrix3 operator-(const Matrix3 &left, const Matrix3 &right) {
	return left.Subtract(right);
}

Matrix3 operator*(const Matrix3 &left, const Matrix3 &right) {
	return left.Multiply(right);
}

Matrix3 operator/(const Matrix3 &left, const Matrix3 &right) {
	return left.Divide(right);
}

Matrix3 operator*(const Vector3f &left, const Matrix3 &right) {
	return right.Scale(left);
}

Matrix3 operator/(const Vector3f &left, const Matrix3 &right) {
	return right.Scale(1.0f / left);
}

Matrix3 operator*(const Matrix3 &left, const Vector3f &right) {
	return left.Scale(right);
}

Matrix3 operator/(const Matrix3 &left, const Vector3f &right) {
	return left.Scale(1.0f / right);
}

Matrix3 operator*(float left, const Matrix3 &right) {
	return right.Scale(Vector3(left));
}

Matrix3 operator/(float left, const Matrix3 &right) {
	return right.Scale(1.0f / Vector3(left));
}

Matrix3 operator*(const Matrix3 &left, float right) {
	return left.Scale(Vector3(right));
}

Matrix3 operator/(const Matrix3 &left, float right) {
	return left.Scale(1.0f / Vector3(right));
}

Matrix3 &Matrix3::operator+=(const Matrix3 &other) {
	return *this = Add(other);
}

Matrix3 &Matrix3::operator-=(const Matrix3 &other) {
	return *this = Subtract(other);
}

Matrix3 &Matrix3::operator*=(const Matrix3 &other) {
	return *this = Multiply(other);
}

Matrix3 &Matrix3::operator/=(const Matrix3 &other) {
	return *this = Divide(other);
}

Matrix3 &Matrix3::operator*=(const Vector3f &other) {
	return *this = Scale(other);
}

Matrix3 &Matrix3::operator/=(const Vector3f &other) {
	return *this = Scale(1.0f / other);
}

Matrix3 &Matrix3::operator*=(float other) {
	return *this = Scale(Vector3(other));
}

Matrix3 &Matrix3::operator/=(float other) {
	return *this = Scale(1.0f / Vector3(other));
}

const Node &operator>>(const Node &node, Matrix3 &matrix) {
	node["m0"].Get(matrix.m_rows[0]);
	node["m1"].Get(matrix.m_rows[1]);
	node["m2"].Get(matrix.m_rows[2]);
	return node;
}

Node &operator<<(Node &node, const Matrix3 &matrix) {
	node["m0"].Set(matrix.m_rows[0]);
	node["m1"].Set(matrix.m_rows[1]);
	node["m2"].Set(matrix.m_rows[2]);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Matrix3 &matrix) {
	return stream << matrix[0] << ", " << matrix[1] << ", " << matrix[2];
}
}
