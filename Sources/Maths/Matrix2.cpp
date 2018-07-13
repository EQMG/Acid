#include "Matrix2.hpp"

#include <cassert>
#include "Maths.hpp"

namespace fl
{
	const Matrix2 Matrix2::IDENTITY = Matrix2(new float[4]{1.0f, 0.0f, 0.0f, 1.0f});
	const Matrix2 Matrix2::ZERO = Matrix2();

	Matrix2::Matrix2(const float &diagonal)
	{
		memset(m_rows, 0, 2 * 2 * sizeof(float));
		m_rows[0][0] = diagonal;
		m_rows[1][1] = diagonal;
	}

	Matrix2::Matrix2(const Matrix2 &source)
	{
		memcpy(m_rows, source.m_rows, 2 * 2 * sizeof(float));
	}

	Matrix2::Matrix2(const float *source)
	{
		memcpy(m_rows, source, 2 * 2 * sizeof(float));
	}

	Matrix2::Matrix2(const Vector2 *source)
	{
		memcpy(m_rows, source, 2 * sizeof(Vector2));
	}

	Matrix2::~Matrix2()
	{
	}

	Matrix2 Matrix2::Add(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = m_rows[0][0] + other.m_rows[0][0];
		result.m_rows[0][1] = m_rows[0][1] + other.m_rows[0][1];
		result.m_rows[1][0] = m_rows[1][0] + other.m_rows[1][0];
		result.m_rows[1][1] = m_rows[1][1] + other.m_rows[1][1];
		return result;
	}

	Matrix2 Matrix2::Subtract(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = m_rows[0][0] - other.m_rows[0][0];
		result.m_rows[0][1] = m_rows[0][1] - other.m_rows[0][1];
		result.m_rows[1][0] = m_rows[1][0] - other.m_rows[1][0];
		result.m_rows[1][1] = m_rows[1][1] - other.m_rows[1][1];
		return result;
	}

	Matrix2 Matrix2::Multiply(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = m_rows[0][0] * other.m_rows[0][0] + m_rows[1][0] * other.m_rows[0][1];
		result.m_rows[0][1] = m_rows[0][1] * other.m_rows[0][0] + m_rows[1][1] * other.m_rows[0][1];
		result.m_rows[1][0] = m_rows[0][0] * other.m_rows[1][0] + m_rows[1][0] * other.m_rows[1][1];
		result.m_rows[1][1] = m_rows[0][1] * other.m_rows[1][0] + m_rows[1][1] * other.m_rows[1][1];
		return result;
	}

	Matrix2 Matrix2::Divide(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = m_rows[0][0] / other.m_rows[0][0] + m_rows[1][0] / other.m_rows[0][1];
		result.m_rows[0][1] = m_rows[0][1] / other.m_rows[0][0] + m_rows[1][1] / other.m_rows[0][1];
		result.m_rows[1][0] = m_rows[0][0] / other.m_rows[1][0] + m_rows[1][0] / other.m_rows[1][1];
		result.m_rows[1][1] = m_rows[0][1] / other.m_rows[1][0] + m_rows[1][1] / other.m_rows[1][1];
		return result;
	}

	Vector2 Matrix2::Transform(const Vector2 &other) const
	{
		float x = m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y;
		float y = m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y;
		return Vector2(x, y);
	}

	Matrix2 Matrix2::Scale(const Vector2 &other) const
	{
		Matrix2 result = Matrix2(*this);
		result.m_rows[0][0] = m_rows[0][0] * other.m_x;
		result.m_rows[0][1] = m_rows[0][1] * other.m_x;
		result.m_rows[1][0] = m_rows[1][0] * other.m_y;
		result.m_rows[1][1] = m_rows[1][1] * other.m_y;
		return result;
	}

	Matrix2 Matrix2::Negate() const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = -m_rows[0][0];
		result.m_rows[0][1] = -m_rows[0][1];
		result.m_rows[1][0] = -m_rows[1][0];
		result.m_rows[1][1] = -m_rows[1][1];
		return result;
	}

	Matrix2 Matrix2::Invert() const
	{
		Matrix2 result = Matrix2();
		const float d = Determinant();

		if (d != 0.0f)
		{
			float determinantInv = 1.0f / d;

			// Get the conjugate matrix.
			float t00 = m_rows[1][1];
			float t01 = -m_rows[0][1];
			float t11 = m_rows[0][0];
			float t10 = -m_rows[1][0];

			// Transpose and divide by the determinant.
			result.m_rows[0][0] = t00 * determinantInv;
			result.m_rows[0][1] = t01 * determinantInv;
			result.m_rows[1][0] = t10 * determinantInv;
			result.m_rows[1][1] = t11 * determinantInv;
		}

		return result;
	}

	Matrix2 Matrix2::Transpose() const
	{
		Matrix2 result = Matrix2();
		result.m_rows[0][0] = m_rows[0][0];
		result.m_rows[0][1] = m_rows[1][0];
		result.m_rows[1][0] = m_rows[0][1];
		result.m_rows[1][1] = m_rows[1][1];
		return result;
	}

	float Matrix2::Determinant() const
	{
		return m_rows[0][0] * m_rows[1][1] - m_rows[0][1] * m_rows[1][0];
	}

	void Matrix2::Write(LoadedValue *destination)
	{
		m_rows[0].Write(destination->GetChild("m0", true));
		m_rows[1].Write(destination->GetChild("m1", true));
	}

	Matrix2 &Matrix2::operator=(const Matrix2 &other)
	{
		m_rows[0][0] = other.m_rows[0][0];
		m_rows[0][1] = other.m_rows[0][1];
		m_rows[1][0] = other.m_rows[1][0];
		m_rows[1][1] = other.m_rows[1][1];
		return *this;
	}

	Matrix2 &Matrix2::operator=(const float *array)
	{
		m_rows[0][0] = array[0];
		m_rows[0][1] = array[1];
		m_rows[1][0] = array[2];
		m_rows[1][1] = array[3];
		return *this;
	}

	Matrix2 &Matrix2::operator=(LoadedValue *source)
	{
		m_rows[0] = source->GetChild("m0");
		m_rows[1] = source->GetChild("m1");
		return *this;
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

	const Vector2 &Matrix2::operator[](uint32_t index) const
	{
		assert(index < 2);
		return m_rows[index];
	}

	Vector2 &Matrix2::operator[](uint32_t index)
	{
		assert(index < 2);
		return m_rows[index];
	}

	Matrix2 operator+(Matrix2 left, const Matrix2 &right)
	{
		return left.Add(right);
	}

	Matrix2 operator-(Matrix2 left, const Matrix2 &right)
	{
		return left.Subtract(right);
	}

	Matrix2 operator*(Matrix2 left, const Matrix2 &right)
	{
		return left.Multiply(right);
	}

	Matrix2 operator/(Matrix2 left, const Matrix2 &right)
	{
		return left.Divide(right);
	}

	Matrix2 operator*(Matrix2 left, Vector2 value)
	{
		return left.Scale(value);
	}

	Matrix2 operator/(Matrix2 left, Vector2 value)
	{
		return left.Scale(1.0f / value);
	}

	Matrix2 operator*(Matrix2 left, float value)
	{
		return left.Scale(Vector2(value, value));
	}

	Matrix2 operator/(Matrix2 left, float value)
	{
		return left.Scale(1.0f / Vector2(value, value));
	}

	Matrix2 operator*(float value, Matrix2 left)
	{
		return left.Scale(Vector2(value, value));
	}

	Matrix2 operator/(float value, Matrix2 left)
	{
		return left.Scale(1.0f / Vector2(value, value));
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

	std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix)
	{
		stream << matrix.ToString();
		return stream;
	}

	std::string Matrix2::ToString() const
	{
		std::stringstream result;
		result << "Matrix2(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " <<
			m_rows[1][0] << ", " << m_rows[1][1] << ")";
		return result.str();
	}
}
