#include "matrix3x3.hpp"

namespace flounder
{
	matrix3x3::matrix3x3()
	{
		setIdentity();
	}

	matrix3x3::matrix3x3(const matrix3x3 &source)
	{
		set(source);
	}

	matrix3x3::matrix3x3(const float source[9])
	{
		set(source);
	}

	matrix3x3::~matrix3x3()
	{
	}

	matrix3x3 *matrix3x3::set(const matrix3x3 &source)
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

	matrix3x3 *matrix3x3::set(const float source[9])
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

	matrix3x3 *matrix3x3::setIdentity(matrix3x3 *source)
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

	float matrix3x3::determinant(const matrix3x3 &source)
	{
		return source.m_00 * (source.m_11 * source.m_22 - source.m_12 * source.m_21) + source.m_01 * (source.m_12 * source.m_20 - source.m_10 * source.m_22) + source.m_02 * (source.m_10 * source.m_21 - source.m_11 * source.m_20);
	}

	matrix3x3 *matrix3x3::add(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
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

	matrix3x3 *matrix3x3::subtract(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
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

	matrix3x3 *matrix3x3::multiply(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
		}

		float m00 = left.m_00 * right.m_00 + left.m_10 * right.m_01 + left.m_20 * right.m_02;
		float m01 = left.m_01 * right.m_00 + left.m_11 * right.m_01 + left.m_21 * right.m_02;
		float m02 = left.m_02 * right.m_00 + left.m_12 * right.m_01 + left.m_22 * right.m_02;
		float m10 = left.m_00 * right.m_10 + left.m_10 * right.m_11 + left.m_20 * right.m_12;
		float m11 = left.m_01 * right.m_10 + left.m_11 * right.m_11 + left.m_21 * right.m_12;
		float m12 = left.m_02 * right.m_10 + left.m_12 * right.m_11 + left.m_22 * right.m_12;
		float m20 = left.m_00 * right.m_20 + left.m_10 * right.m_21 + left.m_20 * right.m_22;
		float m21 = left.m_01 * right.m_20 + left.m_11 * right.m_21 + left.m_21 * right.m_22;
		float m22 = left.m_02 * right.m_20 + left.m_12 * right.m_21 + left.m_22 * right.m_22;

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

	vector3 *matrix3x3::transform(const matrix3x3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector3();
		}

		float x = left.m_00 * right.m_x + left.m_10 * right.m_y + left.m_20 * right.m_z;
		float y = left.m_01 * right.m_x + left.m_11 * right.m_y + left.m_21 * right.m_z;
		float z = left.m_02 * right.m_x + left.m_12 * right.m_y + left.m_22 * right.m_z;

		return destination->set(x, y, z);
	}

	matrix3x3 *matrix3x3::scale(const matrix3x3 &left, const vector3 &right, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
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

	matrix3x3 *matrix3x3::invert(const matrix3x3 &source, matrix3x3 *destination)
	{
		float d = determinant(source);

		if (d != 0.0f)
		{
			if (destination == nullptr)
			{
				destination = new matrix3x3();
			}

			/*
			* Does it the ordinary way. inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix) m_00 m_01 m_02 m_10 m_11 m_12 m_20 m_21 m_22
			*/
			float determinant_inv = 1.0f / d;

			// Get the conjugate matrix.
			float t00 = source.m_11 * source.m_22 - source.m_12 * source.m_21;
			float t01 = -source.m_10 * source.m_22 + source.m_12 * source.m_20;
			float t02 = source.m_10 * source.m_21 - source.m_11 * source.m_20;
			float t10 = -source.m_01 * source.m_22 + source.m_02 * source.m_21;
			float t11 = source.m_00 * source.m_22 - source.m_02 * source.m_20;
			float t12 = -source.m_00 * source.m_21 + source.m_01 * source.m_20;
			float t20 = source.m_01 * source.m_12 - source.m_02 * source.m_11;
			float t21 = -source.m_00 * source.m_12 + source.m_02 * source.m_10;
			float t22 = source.m_00 * source.m_11 - source.m_01 * source.m_10;

			destination->m_00 = t00 * determinant_inv;
			destination->m_11 = t11 * determinant_inv;
			destination->m_22 = t22 * determinant_inv;
			destination->m_01 = t10 * determinant_inv;
			destination->m_10 = t01 * determinant_inv;
			destination->m_20 = t02 * determinant_inv;
			destination->m_02 = t20 * determinant_inv;
			destination->m_12 = t21 * determinant_inv;
			destination->m_21 = t12 * determinant_inv;
			return destination;
		}
		else
		{
			return nullptr;
		}
	}

	matrix3x3 *matrix3x3::negate(const matrix3x3 &source, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
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

	matrix3x3 *matrix3x3::transpose(const matrix3x3 &source, matrix3x3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new matrix3x3();
		}

		float m00 = source.m_00;
		float m01 = source.m_10;
		float m02 = source.m_20;
		float m10 = source.m_01;
		float m11 = source.m_11;
		float m12 = source.m_21;
		float m20 = source.m_02;
		float m21 = source.m_12;
		float m22 = source.m_22;

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

	float *matrix3x3::toArray(const matrix3x3 &matrix)
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

	matrix3x3 *matrix3x3::setZero(matrix3x3 *source)
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

	matrix3x3 *matrix3x3::setIdentity()
	{
		return setIdentity(this);
	}

	float matrix3x3::determinant()
	{
		return determinant(*this);
	}

	matrix3x3 *matrix3x3::invert()
	{
		return invert(*this, this);
	}

	matrix3x3 *matrix3x3::negate()
	{
		return negate(*this, this);
	}

	matrix3x3 *matrix3x3::transpose()
	{
		return transpose(*this, this);
	}

	matrix3x3 *matrix3x3::setZero()
	{
		return setZero(this);
	}
}
