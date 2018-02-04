#include "Matrix3.hpp"

#include <sstream>

namespace Flounder
{
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
		m_10(source[3]),
		m_11(source[4]),
		m_12(source[5]),
		m_20(source[6]),
		m_21(source[7]),
		m_22(source[8])
	{
	}

	Matrix3::~Matrix3()
	{
	}

	Matrix3 *Matrix3::Set(const Matrix3 &source)
	{
		m_00 = source.m_00;
		m_01 = source.m_01;
		m_02 = source.m_02;
		m_10 = source.m_10;
		m_11 = source.m_11;
		m_12 = source.m_12;
		m_20 = source.m_20;
		m_21 = source.m_21;
		m_22 = source.m_22;
		return this;
	}

	Matrix3 *Matrix3::Set(const float source[9])
	{
		m_00 = source[0];
		m_01 = source[1];
		m_02 = source[2];
		m_10 = source[3];
		m_11 = source[4];
		m_12 = source[5];
		m_20 = source[6];
		m_21 = source[7];
		m_22 = source[8];
		return this;
	}

	Matrix3 *Matrix3::SetIdentity(Matrix3 *source)
	{
		source->m_00 = 1.0f;
		source->m_01 = 0.0f;
		source->m_02 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 1.0f;
		source->m_12 = 0.0f;
		source->m_20 = 0.0f;
		source->m_21 = 0.0f;
		source->m_22 = 1.0f;
		return source;
	}

	float Matrix3::Determinant(const Matrix3 &source)
	{
		return source.m_00 * (source.m_11 * source.m_22 - source.m_12 * source.m_21) +
			source.m_01 * (source.m_12 * source.m_20 - source.m_10 * source.m_22) +
			source.m_02 * (source.m_10 * source.m_21 - source.m_11 * source.m_20);
	}

	Matrix3 *Matrix3::Add(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		destination->m_00 = left.m_00 + right.m_00;
		destination->m_01 = left.m_01 + right.m_01;
		destination->m_02 = left.m_02 + right.m_02;
		destination->m_10 = left.m_10 + right.m_10;
		destination->m_11 = left.m_11 + right.m_11;
		destination->m_12 = left.m_12 + right.m_12;
		destination->m_20 = left.m_20 + right.m_20;
		destination->m_21 = left.m_21 + right.m_21;
		destination->m_22 = left.m_22 + right.m_22;
		return destination;
	}

	Matrix3 *Matrix3::Subtract(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		destination->m_00 = left.m_00 - right.m_00;
		destination->m_01 = left.m_01 - right.m_01;
		destination->m_02 = left.m_02 - right.m_02;
		destination->m_10 = left.m_10 - right.m_10;
		destination->m_11 = left.m_11 - right.m_11;
		destination->m_12 = left.m_12 - right.m_12;
		destination->m_20 = left.m_20 - right.m_20;
		destination->m_21 = left.m_21 - right.m_21;
		destination->m_22 = left.m_22 - right.m_22;
		return destination;
	}

	Matrix3 *Matrix3::Multiply(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		const float m00 = left.m_00 * right.m_00 + left.m_10 * right.m_01 + left.m_20 * right.m_02;
		const float m01 = left.m_01 * right.m_00 + left.m_11 * right.m_01 + left.m_21 * right.m_02;
		const float m02 = left.m_02 * right.m_00 + left.m_12 * right.m_01 + left.m_22 * right.m_02;
		const float m10 = left.m_00 * right.m_10 + left.m_10 * right.m_11 + left.m_20 * right.m_12;
		const float m11 = left.m_01 * right.m_10 + left.m_11 * right.m_11 + left.m_21 * right.m_12;
		const float m12 = left.m_02 * right.m_10 + left.m_12 * right.m_11 + left.m_22 * right.m_12;
		const float m20 = left.m_00 * right.m_20 + left.m_10 * right.m_21 + left.m_20 * right.m_22;
		const float m21 = left.m_01 * right.m_20 + left.m_11 * right.m_21 + left.m_21 * right.m_22;
		const float m22 = left.m_02 * right.m_20 + left.m_12 * right.m_21 + left.m_22 * right.m_22;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		return destination;
	}

	Matrix3 *Matrix3::Divide(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		const float m00 = left.m_00 / right.m_00 + left.m_10 / right.m_01 + left.m_20 / right.m_02;
		const float m01 = left.m_01 / right.m_00 + left.m_11 / right.m_01 + left.m_21 / right.m_02;
		const float m02 = left.m_02 / right.m_00 + left.m_12 / right.m_01 + left.m_22 / right.m_02;
		const float m10 = left.m_00 / right.m_10 + left.m_10 / right.m_11 + left.m_20 / right.m_12;
		const float m11 = left.m_01 / right.m_10 + left.m_11 / right.m_11 + left.m_21 / right.m_12;
		const float m12 = left.m_02 / right.m_10 + left.m_12 / right.m_11 + left.m_22 / right.m_12;
		const float m20 = left.m_00 / right.m_20 + left.m_10 / right.m_21 + left.m_20 / right.m_22;
		const float m21 = left.m_01 / right.m_20 + left.m_11 / right.m_21 + left.m_21 / right.m_22;
		const float m22 = left.m_02 / right.m_20 + left.m_12 / right.m_21 + left.m_22 / right.m_22;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		return destination;
	}

	Vector3 *Matrix3::Transform(const Matrix3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		const float x = left.m_00 * right.m_x + left.m_10 * right.m_y + left.m_20 * right.m_z;
		const float y = left.m_01 * right.m_x + left.m_11 * right.m_y + left.m_21 * right.m_z;
		const float z = left.m_02 * right.m_x + left.m_12 * right.m_y + left.m_22 * right.m_z;

		return destination->Set(x, y, z);
	}

	Matrix3 *Matrix3::Scale(const Matrix3 &left, const Vector3 &right, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		destination->m_00 = left.m_00 * right.m_x;
		destination->m_01 = left.m_01 * right.m_x;
		destination->m_02 = left.m_02 * right.m_x;
		destination->m_10 = left.m_10 * right.m_y;
		destination->m_11 = left.m_11 * right.m_y;
		destination->m_12 = left.m_12 * right.m_y;
		destination->m_20 = left.m_20 * right.m_z;
		destination->m_21 = left.m_21 * right.m_z;
		destination->m_22 = left.m_22 * right.m_z;
		return destination;
	}

	Matrix3 *Matrix3::Invert(const Matrix3 &source, Matrix3 *destination)
	{
		const float d = Determinant(source);

		if (d != 0.0f)
		{
			if (destination == nullptr)
			{
				destination = new Matrix3();
			}

			/*
			* Does it the ordinary way. inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix) m_00 m_01 m_02 m_10 m_11 m_12 m_20 m_21 m_22
			*/
			const float determinantInv = 1.0f / d;

			// Get the conjugate matrix.
			const float t00 = source.m_11 * source.m_22 - source.m_12 * source.m_21;
			const float t01 = -source.m_10 * source.m_22 + source.m_12 * source.m_20;
			const float t02 = source.m_10 * source.m_21 - source.m_11 * source.m_20;
			const float t10 = -source.m_01 * source.m_22 + source.m_02 * source.m_21;
			const float t11 = source.m_00 * source.m_22 - source.m_02 * source.m_20;
			const float t12 = -source.m_00 * source.m_21 + source.m_01 * source.m_20;
			const float t20 = source.m_01 * source.m_12 - source.m_02 * source.m_11;
			const float t21 = -source.m_00 * source.m_12 + source.m_02 * source.m_10;
			const float t22 = source.m_00 * source.m_11 - source.m_01 * source.m_10;

			destination->m_00 = t00 * determinantInv;
			destination->m_11 = t11 * determinantInv;
			destination->m_22 = t22 * determinantInv;
			destination->m_01 = t10 * determinantInv;
			destination->m_10 = t01 * determinantInv;
			destination->m_20 = t02 * determinantInv;
			destination->m_02 = t20 * determinantInv;
			destination->m_12 = t21 * determinantInv;
			destination->m_21 = t12 * determinantInv;
			return destination;
		}

		return nullptr;
	}

	Matrix3 *Matrix3::Negate(const Matrix3 &source, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		destination->m_00 = -source.m_00;
		destination->m_01 = -source.m_02;
		destination->m_02 = -source.m_01;
		destination->m_10 = -source.m_10;
		destination->m_11 = -source.m_12;
		destination->m_12 = -source.m_11;
		destination->m_20 = -source.m_20;
		destination->m_21 = -source.m_22;
		destination->m_22 = -source.m_21;
		return destination;
	}

	Matrix3 *Matrix3::Transpose(const Matrix3 &source, Matrix3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix3();
		}

		const float m00 = source.m_00;
		const float m01 = source.m_10;
		const float m02 = source.m_20;
		const float m10 = source.m_01;
		const float m11 = source.m_11;
		const float m12 = source.m_21;
		const float m20 = source.m_02;
		const float m21 = source.m_12;
		const float m22 = source.m_22;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		return destination;
	}

	float *Matrix3::ToArray(const Matrix3 &matrix)
	{
		float *result = new float[9];
		result[0] = matrix.m_00;
		result[1] = matrix.m_01;
		result[2] = matrix.m_02;
		result[3] = matrix.m_10;
		result[4] = matrix.m_11;
		result[5] = matrix.m_12;
		result[6] = matrix.m_20;
		result[7] = matrix.m_21;
		result[8] = matrix.m_22;
		return result;
	}

	Matrix3 *Matrix3::SetZero(Matrix3 *source)
	{
		source->m_00 = 0.0f;
		source->m_01 = 0.0f;
		source->m_02 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 0.0f;
		source->m_12 = 0.0f;
		source->m_20 = 0.0f;
		source->m_21 = 0.0f;
		source->m_22 = 0.0f;
		return source;
	}

	Matrix3 *Matrix3::SetIdentity()
	{
		return SetIdentity(this);
	}

	float Matrix3::Determinant()
	{
		return Determinant(*this);
	}

	Matrix3 *Matrix3::Invert()
	{
		return Invert(*this, this);
	}

	Matrix3 *Matrix3::Negate()
	{
		return Negate(*this, this);
	}

	Matrix3 *Matrix3::Transpose()
	{
		return Transpose(*this, this);
	}

	Matrix3 *Matrix3::SetZero()
	{
		return SetZero(this);
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

	Matrix3 &Matrix3::operator-()
	{
		return *Matrix3(*this).Negate();
	}

	Matrix3 operator+(Matrix3 left, const Matrix3 &right)
	{
		return *Matrix3::Add(left, right, &left);
	}

	Matrix3 operator-(Matrix3 left, const Matrix3 &right)
	{
		return *Matrix3::Subtract(left, right, &left);
	}

	Matrix3 operator*(Matrix3 left, const Matrix3 &right)
	{
		return *Matrix3::Multiply(left, right, &left);
	}

	Matrix3 operator/(Matrix3 left, const Matrix3 &right)
	{
		return *Matrix3::Divide(left, right, &left);
	}

	Matrix3 operator*(Matrix3 left, Vector3 value)
	{
		return *Matrix3::Scale(left, value, &left);
	}

	Matrix3 operator/(Matrix3 left, Vector3 value)
	{
		return *Matrix3::Scale(left, 1.0f / value, &left);
	}

	Matrix3 &Matrix3::operator+=(const Matrix3 &other)
	{
		Matrix3 result = Matrix3();
		return *Matrix3::Add(*this, other, &result);
	}

	Matrix3 &Matrix3::operator-=(const Matrix3 &other)
	{
		Matrix3 result = Matrix3();
		return *Matrix3::Subtract(*this, other, &result);
	}

	Matrix3 &Matrix3::operator*=(const Matrix3 &other)
	{
		Matrix3 result = Matrix3();
		return *Matrix3::Multiply(*this, other, &result);
	}

	Matrix3 &Matrix3::operator/=(const Matrix3 &other)
	{
		Matrix3 result = Matrix3();
		return *Matrix3::Divide(*this, other, &result);
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
