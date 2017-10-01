#include "Matrix2.hpp"

#include "Maths.hpp"

namespace Flounder
{
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
		m_10(source[2]),
		m_11(source[3])
	{
	}

	Matrix2::~Matrix2()
	{
	}

	Matrix2 *Matrix2::Set(const Matrix2 &source)
	{
		m_00 = source.m_00;
		m_01 = source.m_01;
		m_10 = source.m_10;
		m_11 = source.m_11;
		return this;
	}

	Matrix2 *Matrix2::Set(const float source[4])
	{
		m_00 = source[0];
		m_01 = source[1];
		m_10 = source[2];
		m_11 = source[3];
		return this;
	}

	Matrix2 *Matrix2::SetIdentity(Matrix2 *source)
	{
		source->m_00 = 1.0f;
		source->m_01 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 1.0f;
		return source;
	}

	float Matrix2::Determinant(const Matrix2 &source)
	{
		return source.m_00 * source.m_11 - source.m_01 * source.m_10;
	}

	Matrix2 *Matrix2::Add(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		destination->m_00 = left.m_00 + right.m_00;
		destination->m_01 = left.m_01 + right.m_01;
		destination->m_10 = left.m_10 + right.m_10;
		destination->m_11 = left.m_11 + right.m_11;
		return destination;
	}

	Matrix2 *Matrix2::Subtract(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		destination->m_00 = left.m_00 - right.m_00;
		destination->m_01 = left.m_01 - right.m_01;
		destination->m_10 = left.m_10 - right.m_10;
		destination->m_11 = left.m_11 - right.m_11;
		return destination;
	}

	Matrix2 *Matrix2::Multiply(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		float m00 = left.m_00 * right.m_00 + left.m_10 * right.m_01;
		float m01 = left.m_01 * right.m_00 + left.m_11 * right.m_01;
		float m10 = left.m_00 * right.m_10 + left.m_10 * right.m_11;
		float m11 = left.m_01 * right.m_10 + left.m_11 * right.m_11;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_10 = m10;
		destination->m_11 = m11;
		return destination;
	}

	Vector2 *Matrix2::Transform(const Matrix2 &left, const Vector2 &right, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		float x = left.m_00 * right.m_x + left.m_10 * right.m_y;
		float y = left.m_01 * right.m_x + left.m_11 * right.m_y;

		return destination->Set(x, y);
	}

	Matrix2 *Matrix2::Scale(const Matrix2 &left, const Vector2 &right, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		destination->m_00 = left.m_00 * right.m_x;
		destination->m_01 = left.m_01 * right.m_x;
		destination->m_10 = left.m_10 * right.m_y;
		destination->m_11 = left.m_11 * right.m_y;
		return destination;
	}

	Matrix2 *Matrix2::Invert(const Matrix2 &source, Matrix2 *destination)
	{
		float d = Determinant(source);

		if (d != 0.0f)
		{
			if (destination == nullptr)
			{
				destination = new Matrix2();
			}

			float determinant_inv = 1.0f / d;
			float t00 = source.m_11 * determinant_inv;
			float t01 = -source.m_01 * determinant_inv;
			float t11 = source.m_00 * determinant_inv;
			float t10 = -source.m_10 * determinant_inv;

			destination->m_00 = t00;
			destination->m_01 = t01;
			destination->m_10 = t10;
			destination->m_11 = t11;
			return destination;
		}
		return nullptr;
	}

	Matrix2 *Matrix2::Negate(const Matrix2 &source, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		destination->m_00 = -source.m_00;
		destination->m_01 = -source.m_01;
		destination->m_10 = -source.m_10;
		destination->m_11 = -source.m_11;
		return destination;
	}

	Matrix2 *Matrix2::Transpose(const Matrix2 &source, Matrix2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix2();
		}

		float m01 = source.m_10;
		float m10 = source.m_01;

		destination->m_01 = m01;
		destination->m_10 = m10;
		return destination;
	}

	float *Matrix2::ToArray(const Matrix2 &matrix)
	{
		float *result = new float[4];
		result[0] = matrix.m_00;
		result[1] = matrix.m_01;
		result[2] = matrix.m_10;
		result[3] = matrix.m_11;
		return result;
	}

	Matrix2 *Matrix2::SetZero(Matrix2 *source)
	{
		source->m_00 = 0.0f;
		source->m_01 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 0.0f;
		return source;
	}

	Matrix2 *Matrix2::SetIdentity()
	{
		return SetIdentity(this);
	}

	float Matrix2::Determinant()
	{
		return Determinant(*this);
	}

	Matrix2 *Matrix2::Invert()
	{
		return Invert(*this, this);
	}

	Matrix2 *Matrix2::Negate()
	{
		return Negate(*this, this);
	}

	Matrix2 *Matrix2::Transpose()
	{
		return Transpose(*this, this);
	}

	Matrix2 *Matrix2::SetZero()
	{
		return SetZero(this);
	}
}
