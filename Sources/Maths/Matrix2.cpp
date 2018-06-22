#include "Matrix2.hpp"

#include <sstream>
#include "Maths.hpp"

namespace fl
{
	const Matrix2 Matrix2::IDENTITY = Matrix2().SetIdentity();
	const Matrix2 Matrix2::ZERO = Matrix2().SetZero();

	Matrix2::Matrix2() :
		m_00(1.0f),
		m_01(0.0f),
		m_10(0.0f),
		m_11(1.0f)
	{
	}

	Matrix2::Matrix2(const Matrix2 &source) :
		m_00(source.m_00),
		m_01(source.m_01),
		m_10(source.m_10),
		m_11(source.m_11)
	{
	}

	Matrix2::Matrix2(const float source[4]) :
		m_00(source[0]),
		m_01(source[1]),
		m_10(source[4]),
		m_11(source[5])
	{
	}

	Matrix2::~Matrix2()
	{
	}


	Matrix2 Matrix2::Add(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_00 = m_00 + other.m_00;
		result.m_01 = m_01 + other.m_01;
		result.m_10 = m_10 + other.m_10;
		result.m_11 = m_11 + other.m_11;
		return result;
	}

	Matrix2 Matrix2::Subtract(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_00 = m_00 - other.m_00;
		result.m_01 = m_01 - other.m_01;
		result.m_10 = m_10 - other.m_10;
		result.m_11 = m_11 - other.m_11;
		return result;
	}

	Matrix2 Matrix2::Multiply(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_00 = m_00 * other.m_00 + m_10 * other.m_01;
		result.m_01 = m_01 * other.m_00 + m_11 * other.m_01;
		result.m_10 = m_00 * other.m_10 + m_10 * other.m_11;
		result.m_11 = m_01 * other.m_10 + m_11 * other.m_11;
		return result;
	}

	Matrix2 Matrix2::Divide(const Matrix2 &other) const
	{
		Matrix2 result = Matrix2();
		result.m_00 = m_00 / other.m_00 + m_10 / other.m_01;
		result.m_01 = m_01 / other.m_00 + m_11 / other.m_01;
		result.m_10 = m_00 / other.m_10 + m_10 / other.m_11;
		result.m_11 = m_01 / other.m_10 + m_11 / other.m_11;
		return result;
	}

	Vector2 Matrix2::Transform(const Vector2 &other) const
	{
		float x = m_00 * other.m_x + m_10 * other.m_y;
		float y = m_01 * other.m_x + m_11 * other.m_y;
		return Vector2(x, y);
	}

	Matrix2 Matrix2::Scale(const Vector2 &other) const
	{
		Matrix2 result = Matrix2(*this);
		result.m_00 = m_00 * other.m_x;
		result.m_01 = m_01 * other.m_x;
		result.m_10 = m_10 * other.m_y;
		result.m_11 = m_11 * other.m_y;
		return result;
	}

	Matrix2 Matrix2::Negate() const
	{
		Matrix2 result = Matrix2();
		result.m_00 = -m_00;
		result.m_01 = -m_01;
		result.m_10 = -m_10;
		result.m_11 = -m_11;
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
			float t00 = m_11;
			float t01 = -m_01;
			float t11 = m_00;
			float t10 = -m_10;

			// Transpose and divide by the determinant.
			result.m_00 = t00 * determinantInv;
			result.m_01 = t01 * determinantInv;
			result.m_10 = t10 * determinantInv;
			result.m_11 = t11 * determinantInv;
		}

		return result;
	}

	Matrix2 Matrix2::Transpose() const
	{
		Matrix2 result = Matrix2();
		result.m_00 = m_00;
		result.m_01 = m_10;
		result.m_10 = m_01;
		result.m_11 = m_11;
		return result;
	}

	float Matrix2::Determinant() const
	{
		return m_00 * m_11 - m_01 * m_10;
	}

	Matrix2 Matrix2::SetZero()
	{
		m_00 = 0.0f;
		m_01 = 0.0f;
		m_10 = 0.0f;
		m_11 = 0.0f;
		return *this;
	}

	Matrix2 Matrix2::SetIdentity()
	{
		m_00 = 1.0f;
		m_01 = 0.0f;
		m_10 = 0.0f;
		m_11 = 1.0f;
		return *this;
	}

	void Matrix2::Write(std::shared_ptr<LoadedValue> destination)
	{
		m_0->Write(destination->GetChild("m0", true));
		m_1->Write(destination->GetChild("m1", true));
	}

	Matrix2 &Matrix2::operator=(const Matrix2 &other)
	{
		m_00 = other.m_00;
		m_01 = other.m_01;
		m_10 = other.m_10;
		m_11 = other.m_11;
		return *this;
	}

	Matrix2 &Matrix2::operator=(const float array[41])
	{
		m_00 = array[0];
		m_01 = array[1];
		m_10 = array[2];
		m_11 = array[3];
		return *this;
	}

	Matrix2 &Matrix2::operator=(std::shared_ptr<LoadedValue> source)
	{
		*m_0 = source->GetChild("m0");
		*m_1 = source->GetChild("m1");
		return *this;
	}

	bool Matrix2::operator==(const Matrix2 &other) const
	{
		return m_00 == other.m_00 && m_01 == other.m_01 &&
			m_10 == other.m_10 && m_11 == other.m_11;
	}

	bool Matrix2::operator!=(const Matrix2 &other) const
	{
		return !(*this == other);
	}

	Matrix2 Matrix2::operator-()
	{
		return Negate();
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
		result << "Matrix2(" << m_00 << ", " << m_01 << ", " <<
			m_10 << ", " << m_11 << ")";
		return result.str();
	}
}
