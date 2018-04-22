#include "Matrix3.hpp"

#include <sstream>
#include "Maths.hpp"

namespace Flounder
{
	const Matrix3 Matrix3::IDENTITY = Matrix3().SetIdentity();
	const Matrix3 Matrix3::ZERO = Matrix3().SetZero();

	Matrix3::Matrix3() :
		m_00(1.0f),
		m_01(0.0f),
		m_02(0.0f),
		m_10(0.0f),
		m_11(1.0f),
		m_12(0.0f),
		m_20(0.0f),
		m_21(0.0f),
		m_22(1.0f)
	{
	}

	Matrix3::Matrix3(const Matrix3 &source) :
		m_00(source.m_00),
		m_01(source.m_01),
		m_02(source.m_02),
		m_10(source.m_10),
		m_11(source.m_11),
		m_12(source.m_12),
		m_20(source.m_20),
		m_21(source.m_21),
		m_22(source.m_22)
	{
	}

	Matrix3::Matrix3(const float source[9]) :
		m_00(source[0]),
		m_01(source[1]),
		m_02(source[2]),
		m_10(source[4]),
		m_11(source[5]),
		m_12(source[6]),
		m_20(source[8]),
		m_21(source[9]),
		m_22(source[10])
	{
	}

	Matrix3::~Matrix3()
	{
	}


	Matrix3 Matrix3::Add(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result.m_00 = m_00 + other.m_00;
		result.m_01 = m_01 + other.m_01;
		result.m_02 = m_02 + other.m_02;
		result.m_10 = m_10 + other.m_10;
		result.m_11 = m_11 + other.m_11;
		result.m_12 = m_12 + other.m_12;
		result.m_20 = m_20 + other.m_20;
		result.m_21 = m_21 + other.m_21;
		result.m_22 = m_22 + other.m_22;
		return result;
	}

	Matrix3 Matrix3::Subtract(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result.m_00 = m_00 - other.m_00;
		result.m_01 = m_01 - other.m_01;
		result.m_02 = m_02 - other.m_02;
		result.m_10 = m_10 - other.m_10;
		result.m_11 = m_11 - other.m_11;
		result.m_12 = m_12 - other.m_12;
		result.m_20 = m_20 - other.m_20;
		result.m_21 = m_21 - other.m_21;
		result.m_22 = m_22 - other.m_22;
		return result;
	}

	Matrix3 Matrix3::Multiply(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result.m_00 = m_00 * other.m_00 + m_10 * other.m_01 + m_20 * other.m_02;
		result.m_01 = m_01 * other.m_00 + m_11 * other.m_01 + m_21 * other.m_02;
		result.m_02 = m_02 * other.m_00 + m_12 * other.m_01 + m_22 * other.m_02;
		result.m_10 = m_00 * other.m_10 + m_10 * other.m_11 + m_20 * other.m_12;
		result.m_11 = m_01 * other.m_10 + m_11 * other.m_11 + m_21 * other.m_12;
		result.m_12 = m_02 * other.m_10 + m_12 * other.m_11 + m_22 * other.m_12;
		result.m_20 = m_00 * other.m_20 + m_10 * other.m_21 + m_20 * other.m_22;
		result.m_21 = m_01 * other.m_20 + m_11 * other.m_21 + m_21 * other.m_22;
		result.m_22 = m_02 * other.m_20 + m_12 * other.m_21 + m_22 * other.m_22;
		return result;
	}

	Matrix3 Matrix3::Divide(const Matrix3 &other) const
	{
		Matrix3 result = Matrix3();
		result.m_00 = m_00 / other.m_00 + m_10 / other.m_01 + m_20 / other.m_02;
		result.m_01 = m_01 / other.m_00 + m_11 / other.m_01 + m_21 / other.m_02;
		result.m_02 = m_02 / other.m_00 + m_12 / other.m_01 + m_22 / other.m_02;
		result.m_10 = m_00 / other.m_10 + m_10 / other.m_11 + m_20 / other.m_12;
		result.m_11 = m_01 / other.m_10 + m_11 / other.m_11 + m_21 / other.m_12;
		result.m_12 = m_02 / other.m_10 + m_12 / other.m_11 + m_22 / other.m_12;
		result.m_20 = m_00 / other.m_20 + m_10 / other.m_21 + m_20 / other.m_22;
		result.m_21 = m_01 / other.m_20 + m_11 / other.m_21 + m_21 / other.m_22;
		result.m_22 = m_02 / other.m_20 + m_12 / other.m_21 + m_22 / other.m_22;
		return result;
	}

	Vector3 Matrix3::Transform(const Vector3 &other) const
	{
		float x = m_00 * other.m_x + m_10 * other.m_y + m_20 * other.m_z;
		float y = m_01 * other.m_x + m_11 * other.m_y + m_21 * other.m_z;
		float z = m_02 * other.m_x + m_12 * other.m_y + m_22 * other.m_z;
		return Vector3(x, y, z);
	}

	Matrix3 Matrix3::Scale(const Vector3 &other) const
	{
		Matrix3 result = Matrix3(*this);
		result.m_00 = m_00 * other.m_x;
		result.m_01 = m_01 * other.m_x;
		result.m_02 = m_02 * other.m_x;
		result.m_10 = m_10 * other.m_y;
		result.m_11 = m_11 * other.m_y;
		result.m_12 = m_12 * other.m_y;
		result.m_20 = m_20 * other.m_z;
		result.m_21 = m_21 * other.m_z;
		result.m_22 = m_22 * other.m_z;
		return result;
	}

	Matrix3 Matrix3::Negate() const
	{
		Matrix3 result = Matrix3();
		result.m_00 = -m_00;
		result.m_01 = -m_01;
		result.m_02 = -m_02;
		result.m_10 = -m_10;
		result.m_11 = -m_11;
		result.m_12 = -m_12;
		result.m_20 = -m_20;
		result.m_21 = -m_21;
		result.m_22 = -m_22;
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
			const float t00 = m_11 * m_22 - m_12 * m_21;
			const float t01 = -m_10 * m_22 + m_12 * m_20;
			const float t02 = m_10 * m_21 - m_11 * m_20;
			const float t10 = -m_01 * m_22 + m_02 * m_21;
			const float t11 = m_00 * m_22 - m_02 * m_20;
			const float t12 = -m_00 * m_21 + m_01 * m_20;
			const float t20 = m_01 * m_12 - m_02 * m_11;
			const float t21 = -m_00 * m_12 + m_02 * m_10;
			const float t22 = m_00 * m_11 - m_01 * m_10;

			// Transpose and divide by the determinant.
			result.m_00 = t00 * determinantInv;
			result.m_11 = t11 * determinantInv;
			result.m_22 = t22 * determinantInv;
			result.m_01 = t10 * determinantInv;
			result.m_10 = t01 * determinantInv;
			result.m_20 = t02 * determinantInv;
			result.m_02 = t20 * determinantInv;
			result.m_12 = t21 * determinantInv;
			result.m_21 = t12 * determinantInv;
		}

		return result;
	}

	Matrix3 Matrix3::Transpose() const
	{
		Matrix3 result = Matrix3();
		result.m_00 = m_00;
		result.m_01 = m_10;
		result.m_02 = m_20;
		result.m_10 = m_01;
		result.m_11 = m_11;
		result.m_12 = m_21;
		result.m_20 = m_02;
		result.m_21 = m_12;
		result.m_22 = m_22;
		return result;
	}

	float Matrix3::Determinant() const
	{
		return m_00 * (m_11 * m_22 - m_12 * m_21) +
			m_01 * (m_12 * m_20 - m_10 * m_22) +
			m_02 * (m_10 * m_21 - m_11 * m_20);
	}

	Matrix3 Matrix3::SetZero()
	{
		m_00 = 0.0f;
		m_01 = 0.0f;
		m_02 = 0.0f;
		m_10 = 0.0f;
		m_11 = 0.0f;
		m_12 = 0.0f;
		m_20 = 0.0f;
		m_21 = 0.0f;
		m_22 = 0.0f;
		return *this;
	}

	Matrix3 Matrix3::SetIdentity()
	{
		m_00 = 1.0f;
		m_01 = 0.0f;
		m_02 = 0.0f;
		m_10 = 0.0f;
		m_11 = 1.0f;
		m_12 = 0.0f;
		m_20 = 0.0f;
		m_21 = 0.0f;
		m_22 = 1.0f;
		return *this;
	}

	float *Matrix3::ToArray() const
	{
		float *result = new float[9];
		result[0] = m_00;
		result[1] = m_01;
		result[2] = m_02;
		result[3] = m_10;
		result[4] = m_11;
		result[5] = m_12;
		result[6] = m_20;
		result[7] = m_21;
		result[8] = m_22;
		return result;
	}

	void Matrix3::Write(LoadedValue *destination)
	{
		m_0->Write(destination->GetChild("m0", true));
		m_1->Write(destination->GetChild("m1", true));
		m_2->Write(destination->GetChild("m2", true));
	}

	Matrix3 &Matrix3::operator=(const Matrix3 &other)
	{
		m_00 = other.m_00;
		m_01 = other.m_01;
		m_02 = other.m_02;
		m_10 = other.m_10;
		m_11 = other.m_11;
		m_12 = other.m_12;
		m_20 = other.m_20;
		m_21 = other.m_21;
		m_22 = other.m_22;
		return *this;
	}

	Matrix3 &Matrix3::operator=(const float array[9])
	{
		m_00 = array[0];
		m_01 = array[1];
		m_02 = array[2];
		m_10 = array[3];
		m_11 = array[4];
		m_12 = array[5];
		m_20 = array[6];
		m_21 = array[7];
		m_22 = array[8];
		return *this;
	}

	Matrix3 &Matrix3::operator=(LoadedValue *source)
	{
		*m_0 = source->GetChild("m0");
		*m_1 = source->GetChild("m1");
		*m_2 = source->GetChild("m2");
		return *this;
	}

	bool Matrix3::operator==(const Matrix3 &other) const
	{
		return m_00 == other.m_00 && m_01 == other.m_01 && m_02 == other.m_02 &&
			m_10 == other.m_10 && m_11 == other.m_11 && m_12 == other.m_12 &&
			m_20 == other.m_20 && m_21 == other.m_21 && m_22 == other.m_22;
	}

	bool Matrix3::operator!=(const Matrix3 &other) const
	{
		return !(*this == other);
	}

	Matrix3 Matrix3::operator-()
	{
		return Negate();
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

	Matrix3 operator*(Matrix3 left, Vector3 value)
	{
		return left.Scale(value);
	}

	Matrix3 operator/(Matrix3 left, Vector3 value)
	{
		return left.Scale(1.0f / value);
	}

	Matrix3 operator*(Matrix3 left, float value)
	{
		return left.Scale(Vector3(value, value, value));
	}

	Matrix3 operator/(Matrix3 left, float value)
	{
		return left.Scale(1.0f / Vector3(value, value, value));
	}

	Matrix3 operator*(float value, Matrix3 left)
	{
		return left.Scale(Vector3(value, value, value));
	}

	Matrix3 operator/(float value, Matrix3 left)
	{
		return left.Scale(1.0f / Vector3(value, value, value));
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
		result << "Matrix3(" << m_00 << ", " << m_01 << ", " << m_02 << ", " <<
			m_10 << ", " << m_11 << ", " << m_12 << ", " <<
			m_20 << ", " << m_21 << ", " << m_22 << ")";
		return result.str();
	}
}
