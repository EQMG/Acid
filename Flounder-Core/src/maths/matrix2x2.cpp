#include "matrix2x2.hpp"

namespace Flounder
{
	matrix2x2::matrix2x2() :
		m_00(1.0f),
		m_01(0.0f),
		m_10(0.0f),
		m_11(1.0f)
	{
	}

	matrix2x2::matrix2x2(const matrix2x2 &source) :
		m_00(source.m_00),
		m_01(source.m_01),
		m_10(source.m_10),
		m_11(source.m_11)
	{
	}

	matrix2x2::matrix2x2(const float source[4]) :
		m_00(source[0]),
		m_01(source[1]),
		m_10(source[2]),
		m_11(source[3])
	{
	}

	matrix2x2::~matrix2x2()
	{
	}

	matrix2x2 *matrix2x2::set(const matrix2x2 &source)
	{
		m_00 = source.m_00;
		m_01 = source.m_01;
		m_10 = source.m_10;
		m_11 = source.m_11;
		return this;
	}

	matrix2x2 *matrix2x2::set(const float source[4])
	{
		m_00 = source[0];
		m_01 = source[1];
		m_10 = source[2];
		m_11 = source[3];
		return this;
	}

	matrix2x2 *matrix2x2::setIdentity(matrix2x2 *source)
	{
		source->m_00 = 1.0f;
		source->m_01 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 1.0f;
		return source;
	}

	float matrix2x2::determinant(const matrix2x2 &source)
	{
		return source.m_00 * source.m_11 - source.m_01 * source.m_10;
	}

	matrix2x2 *matrix2x2::add(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
		}

		destination->m_00 = left.m_00 + right.m_00;
		destination->m_01 = left.m_01 + right.m_01;
		destination->m_10 = left.m_10 + right.m_10;
		destination->m_11 = left.m_11 + right.m_11;
		return destination;
	}

	matrix2x2 *matrix2x2::subtract(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
		}

		destination->m_00 = left.m_00 - right.m_00;
		destination->m_01 = left.m_01 - right.m_01;
		destination->m_10 = left.m_10 - right.m_10;
		destination->m_11 = left.m_11 - right.m_11;
		return destination;
	}

	matrix2x2 *matrix2x2::multiply(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
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

	vector2 *matrix2x2::transform(const matrix2x2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		float x = left.m_00 * right.m_x + left.m_10 * right.m_y;
		float y = left.m_01 * right.m_x + left.m_11 * right.m_y;

		return destination->set(x, y);
	}

	matrix2x2 *matrix2x2::scale(const matrix2x2 &left, const vector2 &right, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
		}

		destination->m_00 = left.m_00 * right.m_x;
		destination->m_01 = left.m_01 * right.m_x;
		destination->m_10 = left.m_10 * right.m_y;
		destination->m_11 = left.m_11 * right.m_y;
		return destination;
	}

	matrix2x2 *matrix2x2::invert(const matrix2x2 &source, matrix2x2 *destination)
	{
		float d = determinant(source);

		if (d != 0.0f)
		{
			if (destination == nullptr)
			{
				destination = new matrix2x2();
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

	matrix2x2 *matrix2x2::negate(const matrix2x2 &source, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
		}

		destination->m_00 = -source.m_00;
		destination->m_01 = -source.m_01;
		destination->m_10 = -source.m_10;
		destination->m_11 = -source.m_11;
		return destination;
	}

	matrix2x2 *matrix2x2::transpose(const matrix2x2 &source, matrix2x2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix2x2();
		}

		float m01 = source.m_10;
		float m10 = source.m_01;

		destination->m_01 = m01;
		destination->m_10 = m10;
		return destination;
	}

	float *matrix2x2::toArray(const matrix2x2 &matrix)
	{
		float *result = new float[4];
		result[0] = matrix.m_00;
		result[1] = matrix.m_01;
		result[2] = matrix.m_10;
		result[3] = matrix.m_11;
		return result;
	}

	matrix2x2 *matrix2x2::setZero(matrix2x2 *source)
	{
		source->m_00 = 0.0f;
		source->m_01 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 0.0f;
		return source;
	}

	matrix2x2 *matrix2x2::setIdentity()
	{
		return setIdentity(this);
	}

	float matrix2x2::determinant()
	{
		return determinant(*this);
	}

	matrix2x2 *matrix2x2::invert()
	{
		return invert(*this, this);
	}

	matrix2x2 *matrix2x2::negate()
	{
		return negate(*this, this);
	}

	matrix2x2 *matrix2x2::transpose()
	{
		return transpose(*this, this);
	}

	matrix2x2 *matrix2x2::setZero()
	{
		return setZero(this);
	}
}
