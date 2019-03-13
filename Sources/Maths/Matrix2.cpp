#include "Matrix2.hpp"

#include "Matrix3.hpp"
#include "Matrix4.hpp"

namespace acid
{
const Matrix2 Matrix2::Identity = Matrix2(1.0f);
const Matrix2 Matrix2::Zero = Matrix2(0.0f);

Matrix2::Matrix2(const float& diagonal)
{
	memset(m_rows, 0, 2 * 2 * sizeof(float));
	m_rows[0][0] = diagonal;
	m_rows[1][1] = diagonal;
}

Matrix2::Matrix2(const Matrix3& source)
{
	memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2));
}

Matrix2::Matrix2(const Matrix4& source)
{
	memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2));
}

Matrix2::Matrix2(const float* source)
{
	memcpy(m_rows, source, 2 * 2 * sizeof(float));
}

Matrix2::Matrix2(const Vector2* source)
{
	memcpy(m_rows, source, 2 * sizeof(Vector2));
}

Matrix2 Matrix2::Add(const Matrix2& other) const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = m_rows[row][col] + other[row][col];
				}
		}

	return result;
}

Matrix2 Matrix2::Subtract(const Matrix2& other) const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = m_rows[row][col] - other[row][col];
				}
		}

	return result;
}

Matrix2 Matrix2::Multiply(const Matrix2& other) const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = m_rows[0][col] * other[row][0] + m_rows[1][col] * other[row][1];
				}
		}

	return result;
}

Matrix2 Matrix2::Divide(const Matrix2& other) const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = m_rows[0][col] / other[row][0] + m_rows[1][col] / other[row][1];
				}
		}

	return result;
}

Vector2 Matrix2::Transform(const Vector2& other) const
{
	Vector2 result = Vector2();

	for(int32_t row = 0; row < 3; row++)
		{
			result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y;
		}

	return result;
}

Matrix2 Matrix2::Scale(const Vector2& other) const
{
	Matrix2 result = Matrix2(*this);

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] *= other[row];
				}
		}

	return result;
}

Matrix2 Matrix2::Negate() const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = -m_rows[row][col];
				}
		}

	return result;
}

Matrix2 Matrix2::Inverse() const
{
	Matrix2 result = Matrix2();

	float det = Determinant();

	if(det == 0.0f)
		{
			throw std::runtime_error("Can't invert a matrix with a determinant of zero");
		}

	for(int32_t j = 0; j < 2; j++)
		{
			for(int32_t i = 0; i < 2; i++)
				{
					// Get minor of element [j][i] - not [i][j], this is where the transpose happens.
					float minor = GetSubmatrix(j, i);

					// Multiply by (−1)^{i+j}.
					float factor = ((i + j) % 2 == 1) ? -1.0f : 1.0f;
					float cofactor = minor * factor;

					result[i][j] = cofactor / det;
				}
		}

	return result;
}

Matrix2 Matrix2::Transpose() const
{
	Matrix2 result = Matrix2();

	for(int32_t row = 0; row < 2; row++)
		{
			for(int32_t col = 0; col < 2; col++)
				{
					result[row][col] = m_rows[col][row];
				}
		}

	return result;
}

float Matrix2::Determinant() const
{
	float result = 0.0f;

	for(int32_t i = 0; i < 2; i++)
		{
			// Get minor of element [0][i].
			float minor = GetSubmatrix(0, i);

			// If this is an odd-numbered row, negate the value.
			float factor = (i % 2 == 1) ? -1.0f : 1.0f;

			result += factor * m_rows[0][i] * minor;
		}

	return result;
}

float Matrix2::GetSubmatrix(const int32_t& row, const int32_t& col) const
{
	float result = 0.0f;
	int32_t colCount = 0;
	int32_t rowCount = 0;

	for(int32_t i = 0; i < 2; i++)
		{
			if(i != row)
				{
					colCount = 0;

					for(int32_t j = 0; j < 2; j++)
						{
							if(j != col)
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

void Matrix2::Decode(const Metadata& metadata)
{
	metadata.GetChild("m0", m_rows[0]);
	metadata.GetChild("m1", m_rows[1]);
}

void Matrix2::Encode(Metadata& metadata) const
{
	metadata.SetChild("m0", m_rows[0]);
	metadata.SetChild("m1", m_rows[1]);
}

bool Matrix2::operator==(const Matrix2& other) const
{
	return m_rows[0] == other.m_rows[0] && m_rows[1] == other.m_rows[1];
}

bool Matrix2::operator!=(const Matrix2& other) const
{
	return !(*this == other);
}

Matrix2 Matrix2::operator-() const
{
	return Negate();
}

const Vector2& Matrix2::operator[](const uint32_t& index) const
{
	assert(index < 2);
	return m_rows[index];
}

Vector2& Matrix2::operator[](const uint32_t& index)
{
	assert(index < 2);
	return m_rows[index];
}

Matrix2 operator+(const Matrix2& left, const Matrix2& right)
{
	return left.Add(right);
}

Matrix2 operator-(const Matrix2& left, const Matrix2& right)
{
	return left.Subtract(right);
}

Matrix2 operator*(const Matrix2& left, const Matrix2& right)
{
	return left.Multiply(right);
}

Matrix2 operator/(const Matrix2& left, const Matrix2& right)
{
	return left.Divide(right);
}

Matrix2 operator*(const Vector2& left, const Matrix2& right)
{
	return right.Scale(left);
}

Matrix2 operator/(const Vector2& left, const Matrix2& right)
{
	return right.Scale(1.0f / left);
}

Matrix2 operator*(const Matrix2& left, const Vector2& right)
{
	return left.Scale(right);
}

Matrix2 operator/(const Matrix2& left, const Vector2& right)
{
	return left.Scale(1.0f / right);
}

Matrix2 operator*(const float& left, const Matrix2& right)
{
	return right.Scale(Vector2(left, left));
}

Matrix2 operator/(const float& left, const Matrix2& right)
{
	return right.Scale(1.0f / Vector2(left, left));
}

Matrix2 operator*(const Matrix2& left, const float& right)
{
	return left.Scale(Vector2(right, right));
}

Matrix2 operator/(const Matrix2& left, const float& right)
{
	return left.Scale(1.0f / Vector2(right, right));
}

Matrix2& Matrix2::operator+=(const Matrix2& other)
{
	return *this = Add(other);
}

Matrix2& Matrix2::operator-=(const Matrix2& other)
{
	return *this = Subtract(other);
}

Matrix2& Matrix2::operator*=(const Matrix2& other)
{
	return *this = Multiply(other);
}

Matrix2& Matrix2::operator/=(const Matrix2& other)
{
	return *this = Divide(other);
}

Matrix2& Matrix2::operator*=(const Vector2& other)
{
	return *this = Scale(other);
}

Matrix2& Matrix2::operator/=(const Vector2& other)
{
	return *this = Scale(1.0f / other);
}

Matrix2& Matrix2::operator*=(const float& other)
{
	return *this = Scale(Vector2(other, other));
}

Matrix2& Matrix2::operator/=(const float& other)
{
	return *this = Scale(1.0f / Vector2(other, other));
}

std::ostream& operator<<(std::ostream& stream, const Matrix2& matrix)
{
	stream << matrix.ToString();
	return stream;
}

std::string Matrix2::ToString() const
{
	std::stringstream result;
	result.precision(10);
	result << "Matrix2(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " << m_rows[1][0] << ", " << m_rows[1][1] << ")";
	return result.str();
}
}
