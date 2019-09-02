#include "Matrix2.hpp"

#include "Matrix3.hpp"
#include "Matrix4.hpp"

namespace acid
{
Matrix2::Matrix2(const float &diagonal)
{
	std::memset(m_rows, 0, 2 * 2 * sizeof(float));
	m_rows[0][0] = diagonal;
	m_rows[1][1] = diagonal;
}

Matrix2::Matrix2(const Matrix3 &source)
{
	std::memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2f));
}

Matrix2::Matrix2(const Matrix4 &source)
{
	std::memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2f));
}

Matrix2::Matrix2(const float *source)
{
	std::memcpy(m_rows, source, 2 * 2 * sizeof(float));
}

Matrix2::Matrix2(const Vector2f *source)
{
	std::memcpy(m_rows, source, 2 * sizeof(Vector2f));
}

Matrix2 Matrix2::Add(const Matrix2 &other) const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[row][col] + other[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Subtract(const Matrix2 &other) const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[row][col] - other[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Multiply(const Matrix2 &other) const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[0][col] * other[row][0] + m_rows[1][col] * other[row][1];
		}
	}

	return result;
}

Matrix2 Matrix2::Divide(const Matrix2 &other) const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[0][col] / other[row][0] + m_rows[1][col] / other[row][1];
		}
	}

	return result;
}

Vector2f Matrix2::Transform(const Vector2f &other) const
{
	Vector2f result;

	for (uint32_t row{}; row < 3; row++)
	{
		result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y;
	}

	return result;
}

Matrix2 Matrix2::Scale(const Vector2f &other) const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[row][col] * other[row];
		}
	}

	return result;
}

Matrix2 Matrix2::Negate() const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = -m_rows[row][col];
		}
	}

	return result;
}

Matrix2 Matrix2::Inverse() const
{
	Matrix2 result;

	auto det = Determinant();

	if (det == 0.0f)
	{
		throw std::runtime_error{"Can't invert a matrix with a determinant of zero"};
	}
	
	for (uint32_t j{}; j < 2; j++)
	{
		for (uint32_t i{}; i < 2; i++)
		{
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

Matrix2 Matrix2::Transpose() const
{
	Matrix2 result;

	for (uint32_t row{}; row < 2; row++)
	{
		for (uint32_t col{}; col < 2; col++)
		{
			result[row][col] = m_rows[col][row];
		}
	}

	return result;
}

float Matrix2::Determinant() const
{
	float result{};

	for (uint32_t i{}; i < 2; i++)
	{
		// Get minor of element [0][i].
		auto minor = GetSubmatrix(0, i);

		// If this is an odd-numbered row, negate the value.
		auto factor = (i % 2 == 1) ? -1.0f : 1.0f;

		result += factor * m_rows[0][i] * minor;
	}

	return result;
}

float Matrix2::GetSubmatrix(const uint32_t &row, const uint32_t &col) const
{
	float result{};
	uint32_t colCount{};
	uint32_t rowCount{};

	for (uint32_t i{}; i < 2; i++)
	{
		if (i != row)
		{
			colCount = 0;

			for (uint32_t j{}; j < 2; j++)
			{
				if (j != col)
				{
					result = m_rows[i][j];
					colCount++;
				}
			}

			rowCount++;
		}
	}

	return result;
}

bool Matrix2::operator==(const Matrix2 &other) const
{
	return m_rows[0] == other.m_rows[0] && m_rows[1] == other.m_rows[1];
}

bool Matrix2::operator!=(const Matrix2 &other) const
{
	return !(*this == other);
}

Matrix2 Matrix2::operator-() const
{
	return Negate();
}

const Vector2f &Matrix2::operator[](const uint32_t &index) const
{
	assert(index < 2);
	return m_rows[index];
}

Vector2f &Matrix2::operator[](const uint32_t &index)
{
	assert(index < 2);
	return m_rows[index];
}

Matrix2 operator+(const Matrix2 &left, const Matrix2 &right)
{
	return left.Add(right);
}

Matrix2 operator-(const Matrix2 &left, const Matrix2 &right)
{
	return left.Subtract(right);
}

Matrix2 operator*(const Matrix2 &left, const Matrix2 &right)
{
	return left.Multiply(right);
}

Matrix2 operator/(const Matrix2 &left, const Matrix2 &right)
{
	return left.Divide(right);
}

Matrix2 operator*(const Vector2f &left, const Matrix2 &right)
{
	return right.Scale(left);
}

Matrix2 operator/(const Vector2f &left, const Matrix2 &right)
{
	return right.Scale(1.0f / left);
}

Matrix2 operator*(const Matrix2 &left, const Vector2f &right)
{
	return left.Scale(right);
}

Matrix2 operator/(const Matrix2 &left, const Vector2f &right)
{
	return left.Scale(1.0f / right);
}

Matrix2 operator*(const float &left, const Matrix2 &right)
{
	return right.Scale({left, left});
}

Matrix2 operator/(const float &left, const Matrix2 &right)
{
	return right.Scale(1.0f / Vector2f{left, left});
}

Matrix2 operator*(const Matrix2 &left, const float &right)
{
	return left.Scale({right, right});
}

Matrix2 operator/(const Matrix2 &left, const float &right)
{
	return left.Scale(1.0f / Vector2f{right, right});
}

Matrix2 &Matrix2::operator+=(const Matrix2 &other)
{
	return *this = Add(other);
}

Matrix2 &Matrix2::operator-=(const Matrix2 &other)
{
	return *this = Subtract(other);
}

Matrix2 &Matrix2::operator*=(const Matrix2 &other)
{
	return *this = Multiply(other);
}

Matrix2 &Matrix2::operator/=(const Matrix2 &other)
{
	return *this = Divide(other);
}

Matrix2 &Matrix2::operator*=(const Vector2f &other)
{
	return *this = Scale(other);
}

Matrix2 &Matrix2::operator/=(const Vector2f &other)
{
	return *this = Scale(1.0f / other);
}

Matrix2 &Matrix2::operator*=(const float &other)
{
	return *this = Scale({other, other});
}

Matrix2 &Matrix2::operator/=(const float &other)
{
	return *this = Scale(1.0f / Vector2f{other, other});
}

const Node &operator>>(const Node &node, Matrix2 &matrix)
{
	node["m0"].Get(matrix.m_rows[0]);
	node["m1"].Get(matrix.m_rows[1]);
	return node;
}

Node &operator<<(Node &node, const Matrix2 &matrix)
{
	node["m0"].Set(matrix.m_rows[0]);
	node["m1"].Set(matrix.m_rows[1]);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix)
{
	return stream << matrix[0] << ", " << matrix[1];
}
}
