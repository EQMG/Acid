#include "Matrix3.hpp"

#include <cassert>
#include "Maths.hpp"

namespace fl
{
	const Matrix3 Matrix3::IDENTITY = Matrix3(new float[9]{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f});
	const Matrix3 Matrix3::ZERO = Matrix3();

	Matrix3::Matrix3(const float &diagonal)
	{
		memset(m_rows, 0, 3 * 3 * sizeof(float));
		m_rows[0][0] = diagonal;
		m_rows[1][1] = diagonal;
		m_rows[2][2] = diagonal;
	}

	Matrix3::Matrix3(const Matrix3 &source)
	{
		memcpy(m_rows, source.m_rows, 3 * 3 * sizeof(float));
	}

	Matrix3::Matrix3(const float *source)
	{
		memcpy(m_rows, source, 3 * 3 * sizeof(float));
	}

	Matrix3::Matrix3(const Vector3 *source)
	{
		memcpy(m_rows, source, 3 * sizeof(Vector3));
	}

	Matrix3::~Matrix3()
	{
	}

	Matrix3 Matrix3::Add(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result[0][0] = m_rows[0][0] + other[0][0];
		result[0][1] = m_rows[0][1] + other[0][1];
		result[0][2] = m_rows[0][2] + other[0][2];
		result[1][0] = m_rows[1][0] + other[1][0];
		result[1][1] = m_rows[1][1] + other[1][1];
		result[1][2] = m_rows[1][2] + other[1][2];
		result[2][0] = m_rows[2][0] + other[2][0];
		result[2][1] = m_rows[2][1] + other[2][1];
		result[2][2] = m_rows[2][2] + other[2][2];
		return result;
	}

	Matrix3 Matrix3::Subtract(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result[0][0] = m_rows[0][0] - other[0][0];
		result[0][1] = m_rows[0][1] - other[0][1];
		result[0][2] = m_rows[0][2] - other[0][2];
		result[1][0] = m_rows[1][0] - other[1][0];
		result[1][1] = m_rows[1][1] - other[1][1];
		result[1][2] = m_rows[1][2] - other[1][2];
		result[2][0] = m_rows[2][0] - other[2][0];
		result[2][1] = m_rows[2][1] - other[2][1];
		result[2][2] = m_rows[2][2] - other[2][2];
		return result;
	}

	Matrix3 Matrix3::Multiply(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result[0][0] = m_rows[0][0] * other[0][0] + m_rows[1][0] * other[0][1] + m_rows[2][0] * other[0][2];
		result[0][1] = m_rows[0][1] * other[0][0] + m_rows[1][1] * other[0][1] + m_rows[2][1] * other[0][2];
		result[0][2] = m_rows[0][2] * other[0][0] + m_rows[1][2] * other[0][1] + m_rows[2][2] * other[0][2];
		result[1][0] = m_rows[0][0] * other[1][0] + m_rows[1][0] * other[1][1] + m_rows[2][0] * other[1][2];
		result[1][1] = m_rows[0][1] * other[1][0] + m_rows[1][1] * other[1][1] + m_rows[2][1] * other[1][2];
		result[1][2] = m_rows[0][2] * other[1][0] + m_rows[1][2] * other[1][1] + m_rows[2][2] * other[1][2];
		result[2][0] = m_rows[0][0] * other[2][0] + m_rows[1][0] * other[2][1] + m_rows[2][0] * other[2][2];
		result[2][1] = m_rows[0][1] * other[2][0] + m_rows[1][1] * other[2][1] + m_rows[2][1] * other[2][2];
		result[2][2] = m_rows[0][2] * other[2][0] + m_rows[1][2] * other[2][1] + m_rows[2][2] * other[2][2];
		return result;
	}

	Vector3 Matrix3::Multiply(const Vector3 &other) const
	{
		Vector3 result = Vector3();

		for (int row = 0; row < 3; row++)
		{
			result[row] = m_rows[row][0] * other[0] + m_rows[row][1] * other[1] + m_rows[row][2] * other[2];
		}

		return result;
	}

	Matrix3 Matrix3::Divide(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result[0][0] = m_rows[0][0] / other[0][0] + m_rows[1][0] / other[0][1] + m_rows[2][0] / other[0][2];
		result[0][1] = m_rows[0][1] / other[0][0] + m_rows[1][1] / other[0][1] + m_rows[2][1] / other[0][2];
		result[0][2] = m_rows[0][2] / other[0][0] + m_rows[1][2] / other[0][1] + m_rows[2][2] / other[0][2];
		result[1][0] = m_rows[0][0] / other[1][0] + m_rows[1][0] / other[1][1] + m_rows[2][0] / other[1][2];
		result[1][1] = m_rows[0][1] / other[1][0] + m_rows[1][1] / other[1][1] + m_rows[2][1] / other[1][2];
		result[1][2] = m_rows[0][2] / other[1][0] + m_rows[1][2] / other[1][1] + m_rows[2][2] / other[1][2];
		result[2][0] = m_rows[0][0] / other[2][0] + m_rows[1][0] / other[2][1] + m_rows[2][0] / other[2][2];
		result[2][1] = m_rows[0][1] / other[2][0] + m_rows[1][1] / other[2][1] + m_rows[2][1] / other[2][2];
		result[2][2] = m_rows[0][2] / other[2][0] + m_rows[1][2] / other[2][1] + m_rows[2][2] / other[2][2];
		return result;
	}

	Vector3 Matrix3::Transform(const Vector3 &other) const
	{
		float x = m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y + m_rows[2][0] * other.m_z;
		float y = m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y + m_rows[2][1] * other.m_z;
		float z = m_rows[0][2] * other.m_x + m_rows[1][2] * other.m_y + m_rows[2][2] * other.m_z;
		return Vector3(x, y, z);
	}

	Matrix3 Matrix3::Scale(const Vector3 &other) const
	{
		Matrix3 result = Matrix3(*this);
		result[0][0] = m_rows[0][0] * other.m_x;
		result[0][1] = m_rows[0][1] * other.m_x;
		result[0][2] = m_rows[0][2] * other.m_x;
		result[1][0] = m_rows[1][0] * other.m_y;
		result[1][1] = m_rows[1][1] * other.m_y;
		result[1][2] = m_rows[1][2] * other.m_y;
		result[2][0] = m_rows[2][0] * other.m_z;
		result[2][1] = m_rows[2][1] * other.m_z;
		result[2][2] = m_rows[2][2] * other.m_z;
		return result;
	}

	Matrix3 Matrix3::Negate() const
	{
		Matrix3 result = Matrix3();
		result[0][0] = -m_rows[0][0];
		result[0][1] = -m_rows[0][1];
		result[0][2] = -m_rows[0][2];
		result[1][0] = -m_rows[1][0];
		result[1][1] = -m_rows[1][1];
		result[1][2] = -m_rows[1][2];
		result[2][0] = -m_rows[2][0];
		result[2][1] = -m_rows[2][1];
		result[2][2] = -m_rows[2][2];
		return result;
	}

	Matrix3 Matrix3::Invert() const
	{
		Matrix3 result = Matrix3();
		const float d = Determinant();

		if (d != 0.0f)
		{
			float determinantInv = 1.0f / d;

			// Get the conjugate matrix.
			const float t00 = m_rows[1][1] * m_rows[2][2] - m_rows[1][2] * m_rows[2][1];
			const float t01 = -m_rows[1][0] * m_rows[2][2] + m_rows[1][2] * m_rows[2][0];
			const float t02 = m_rows[1][0] * m_rows[2][1] - m_rows[1][1] * m_rows[2][0];
			const float t10 = -m_rows[0][1] * m_rows[2][2] + m_rows[0][2] * m_rows[2][1];
			const float t11 = m_rows[0][0] * m_rows[2][2] - m_rows[0][2] * m_rows[2][0];
			const float t12 = -m_rows[0][0] * m_rows[2][1] + m_rows[0][1] * m_rows[2][0];
			const float t20 = m_rows[0][1] * m_rows[1][2] - m_rows[0][2] * m_rows[1][1];
			const float t21 = -m_rows[0][0] * m_rows[1][2] + m_rows[0][2] * m_rows[1][0];
			const float t22 = m_rows[0][0] * m_rows[1][1] - m_rows[0][1] * m_rows[1][0];

			// Transpose and divide by the determinant.
			result[0][0] = t00 * determinantInv;
			result[1][1] = t11 * determinantInv;
			result[2][2] = t22 * determinantInv;
			result[0][1] = t10 * determinantInv;
			result[1][0] = t01 * determinantInv;
			result[2][0] = t02 * determinantInv;
			result[0][2] = t20 * determinantInv;
			result[1][2] = t21 * determinantInv;
			result[2][1] = t12 * determinantInv;
		}

		return result;
	}

	Matrix3 Matrix3::Transpose() const
	{
		Matrix3 result = Matrix3();
		result[0][0] = m_rows[0][0];
		result[0][1] = m_rows[1][0];
		result[0][2] = m_rows[2][0];
		result[1][0] = m_rows[0][1];
		result[1][1] = m_rows[1][1];
		result[1][2] = m_rows[2][1];
		result[2][0] = m_rows[0][2];
		result[2][1] = m_rows[1][2];
		result[2][2] = m_rows[2][2];
		return result;
	}

	float Matrix3::Determinant() const
	{
		return m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[1][2] * m_rows[2][1]) +
			m_rows[0][1] * (m_rows[1][2] * m_rows[2][0] - m_rows[1][0] * m_rows[2][2]) +
			m_rows[0][2] * (m_rows[1][0] * m_rows[2][1] - m_rows[1][1] * m_rows[2][0]);
	}

	void Matrix3::Write(LoadedValue *destination)
	{
		m_rows[0].Write(destination->GetChild("m0", true));
		m_rows[1].Write(destination->GetChild("m1", true));
		m_rows[2].Write(destination->GetChild("m2", true));
	}

	Matrix3 &Matrix3::operator=(const Matrix3 &other)
	{
		m_rows[0][0] = other[0][0];
		m_rows[0][1] = other[0][1];
		m_rows[0][2] = other[0][2];
		m_rows[1][0] = other[1][0];
		m_rows[1][1] = other[1][1];
		m_rows[1][2] = other[1][2];
		m_rows[2][0] = other[2][0];
		m_rows[2][1] = other[2][1];
		m_rows[2][2] = other[2][2];
		return *this;
	}

	Matrix3 &Matrix3::operator=(const float *array)
	{
		m_rows[0][0] = array[0];
		m_rows[0][1] = array[1];
		m_rows[0][2] = array[2];
		m_rows[1][0] = array[3];
		m_rows[1][1] = array[4];
		m_rows[1][2] = array[5];
		m_rows[2][0] = array[6];
		m_rows[2][1] = array[7];
		m_rows[2][2] = array[8];
		return *this;
	}

	Matrix3 &Matrix3::operator=(LoadedValue *source)
	{
		m_rows[0] = source->GetChild("m0");
		m_rows[1] = source->GetChild("m1");
		m_rows[2] = source->GetChild("m2");
		return *this;
	}

	bool Matrix3::operator==(const Matrix3 &other) const
	{
		return m_rows[0] == other[0] && m_rows[1] == other[1] && m_rows[2] == other[2];
	}

	bool Matrix3::operator!=(const Matrix3 &other) const
	{
		return !(*this == other);
	}

	Matrix3 Matrix3::operator-() const
	{
		return Negate();
	}

	const Vector3 &Matrix3::operator[](const uint32_t &index) const
	{
		assert(index < 3);
		return m_rows[index];
	}

	Vector3 &Matrix3::operator[](const uint32_t &index)
	{
		assert(index < 3);
		return m_rows[index];
	}

	Matrix3 operator+(Matrix3 left, const Matrix3 &right)
	{
		return left.Add(right);
	}

	Matrix3 operator-(Matrix3 left, const Matrix3 &right)
	{
		return left.Subtract(right);
	}

	Matrix3 operator*(Matrix3 left, const Matrix3 &right)
	{
		return left.Multiply(right);
	}

	Matrix3 operator/(Matrix3 left, const Matrix3 &right)
	{
		return left.Divide(right);
	}

	Matrix3 operator*(Matrix3 left, Vector3 right)
	{
		return left.Scale(right);
	}

	Matrix3 operator/(Matrix3 left, Vector3 right)
	{
		return left.Scale(1.0f / right);
	}

	Matrix3 operator*(Matrix3 left, float right)
	{
		return left.Scale(Vector3(right, right, right));
	}

	Matrix3 operator/(Matrix3 left, float right)
	{
		return left.Scale(1.0f / Vector3(right, right, right));
	}

	Matrix3 operator*(float left, Matrix3 right)
	{
		return right.Scale(Vector3(left, left, left));
	}

	Matrix3 operator/(float left, Matrix3 right)
	{
		return right.Scale(1.0f / Vector3(left, left, left));
	}

	Vector3 operator*(Vector3 left, Matrix3 right)
	{
		return right.Multiply(left);
	}

	Matrix3 &Matrix3::operator+=(const Matrix3 &other)
	{
		return *this = Add(other);
	}

	Matrix3 &Matrix3::operator-=(const Matrix3 &other)
	{
		return *this = Subtract(other);
	}

	Matrix3 &Matrix3::operator*=(const Matrix3 &other)
	{
		return *this = Multiply(other);
	}

	Matrix3 &Matrix3::operator/=(const Matrix3 &other)
	{
		return *this = Divide(other);
	}

	std::ostream &operator<<(std::ostream &stream, const Matrix3 &matrix)
	{
		stream << matrix.ToString();
		return stream;
	}

	std::string Matrix3::ToString() const
	{
		std::stringstream result;
		result << "Matrix3(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " << m_rows[0][2] << ", " <<
			m_rows[1][0] << ", " << m_rows[1][1] << ", " << m_rows[1][2] << ", " <<
			m_rows[2][0] << ", " << m_rows[2][1] << ", " << m_rows[2][2] << ")";
		return result.str();
	}
}
