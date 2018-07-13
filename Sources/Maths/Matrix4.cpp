#include "Matrix4.hpp"

#include <cassert>
#include "Maths.hpp"
#include "Vector2.hpp"

namespace fl
{
	const Matrix4 Matrix4::IDENTITY = Matrix4(new float[16]{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});
	const Matrix4 Matrix4::ZERO = Matrix4();

	Matrix4::Matrix4(const float &diagonal)
	{
		memset(m_rows, 0, 4 * sizeof(Vector4));
		m_rows[0][0] = diagonal;
		m_rows[1][1] = diagonal;
		m_rows[2][2] = diagonal;
		m_rows[3][3] = diagonal;
	}

	Matrix4::Matrix4(const Matrix4 &source)
	{
		memcpy(m_rows, source.m_rows, 4 * sizeof(Vector4));
	}

	Matrix4::Matrix4(const float *source)
	{
		memcpy(m_rows, source, 4 * 4 * sizeof(float));
	}

	Matrix4::Matrix4(const Vector4 *source)
	{
		memcpy(m_rows, source, 4 * sizeof(Vector4));
	}

	Matrix4::~Matrix4()
	{
	}

	Matrix4 Matrix4::Add(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result[0][0] = m_rows[0][0] + other[0][0];
		result[0][1] = m_rows[0][1] + other[0][1];
		result[0][2] = m_rows[0][2] + other[0][2];
		result[0][3] = m_rows[0][3] + other[0][3];
		result[1][0] = m_rows[1][0] + other[1][0];
		result[1][1] = m_rows[1][1] + other[1][1];
		result[1][2] = m_rows[1][2] + other[1][2];
		result[1][3] = m_rows[1][3] + other[1][3];
		result[2][0] = m_rows[2][0] + other[2][0];
		result[2][1] = m_rows[2][1] + other[2][1];
		result[2][2] = m_rows[2][2] + other[2][2];
		result[2][3] = m_rows[2][3] + other[2][3];
		result[3][0] = m_rows[3][0] + other[3][0];
		result[3][1] = m_rows[3][1] + other[3][1];
		result[3][2] = m_rows[3][2] + other[3][2];
		result[3][3] = m_rows[3][3] + other[3][3];
		return result;
	}

	Matrix4 Matrix4::Subtract(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result[0][0] = m_rows[0][0] - other[0][0];
		result[0][1] = m_rows[0][1] - other[0][1];
		result[0][2] = m_rows[0][2] - other[0][2];
		result[0][3] = m_rows[0][3] - other[0][3];
		result[1][0] = m_rows[1][0] - other[1][0];
		result[1][1] = m_rows[1][1] - other[1][1];
		result[1][2] = m_rows[1][2] - other[1][2];
		result[1][3] = m_rows[1][3] - other[1][3];
		result[2][0] = m_rows[2][0] - other[2][0];
		result[2][1] = m_rows[2][1] - other[2][1];
		result[2][2] = m_rows[2][2] - other[2][2];
		result[2][3] = m_rows[2][3] - other[2][3];
		result[3][0] = m_rows[3][0] - other[3][0];
		result[3][1] = m_rows[3][1] - other[3][1];
		result[3][2] = m_rows[3][2] - other[3][2];
		result[3][3] = m_rows[3][3] - other[3][3];
		return result;
	}

	Matrix4 Matrix4::Multiply(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result[0][0] = m_rows[0][0] * other[0][0] + m_rows[1][0] * other[0][1] + m_rows[2][0] * other[0][2] + m_rows[3][0] * other[0][3];
		result[0][1] = m_rows[0][1] * other[0][0] + m_rows[1][1] * other[0][1] + m_rows[2][1] * other[0][2] + m_rows[3][1] * other[0][3];
		result[0][2] = m_rows[0][2] * other[0][0] + m_rows[1][2] * other[0][1] + m_rows[2][2] * other[0][2] + m_rows[3][2] * other[0][3];
		result[0][3] = m_rows[0][3] * other[0][0] + m_rows[1][3] * other[0][1] + m_rows[2][3] * other[0][2] + m_rows[3][3] * other[0][3];
		result[1][0] = m_rows[0][0] * other[1][0] + m_rows[1][0] * other[1][1] + m_rows[2][0] * other[1][2] + m_rows[3][0] * other[1][3];
		result[1][1] = m_rows[0][1] * other[1][0] + m_rows[1][1] * other[1][1] + m_rows[2][1] * other[1][2] + m_rows[3][1] * other[1][3];
		result[1][2] = m_rows[0][2] * other[1][0] + m_rows[1][2] * other[1][1] + m_rows[2][2] * other[1][2] + m_rows[3][2] * other[1][3];
		result[1][3] = m_rows[0][3] * other[1][0] + m_rows[1][3] * other[1][1] + m_rows[2][3] * other[1][2] + m_rows[3][3] * other[1][3];
		result[2][0] = m_rows[0][0] * other[2][0] + m_rows[1][0] * other[2][1] + m_rows[2][0] * other[2][2] + m_rows[3][0] * other[2][3];
		result[2][1] = m_rows[0][1] * other[2][0] + m_rows[1][1] * other[2][1] + m_rows[2][1] * other[2][2] + m_rows[3][1] * other[2][3];
		result[2][2] = m_rows[0][2] * other[2][0] + m_rows[1][2] * other[2][1] + m_rows[2][2] * other[2][2] + m_rows[3][2] * other[2][3];
		result[2][3] = m_rows[0][3] * other[2][0] + m_rows[1][3] * other[2][1] + m_rows[2][3] * other[2][2] + m_rows[3][3] * other[2][3];
		result[3][0] = m_rows[0][0] * other[3][0] + m_rows[1][0] * other[3][1] + m_rows[2][0] * other[3][2] + m_rows[3][0] * other[3][3];
		result[3][1] = m_rows[0][1] * other[3][0] + m_rows[1][1] * other[3][1] + m_rows[2][1] * other[3][2] + m_rows[3][1] * other[3][3];
		result[3][2] = m_rows[0][2] * other[3][0] + m_rows[1][2] * other[3][1] + m_rows[2][2] * other[3][2] + m_rows[3][2] * other[3][3];
		result[3][3] = m_rows[0][3] * other[3][0] + m_rows[1][3] * other[3][1] + m_rows[2][3] * other[3][2] + m_rows[3][3] * other[3][3];
		return result;
	}

	Vector4 Matrix4::Multiply(const Vector4 &other) const
	{
		float x = m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y + m_rows[2][0] * other.m_z + m_rows[3][0] * other.m_w;
		float y = m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y + m_rows[2][1] * other.m_z + m_rows[3][1] * other.m_w;
		float z = m_rows[0][2] * other.m_x + m_rows[1][2] * other.m_y + m_rows[2][2] * other.m_z + m_rows[3][2] * other.m_w;
		float w = m_rows[0][3] * other.m_x + m_rows[1][3] * other.m_y + m_rows[2][3] * other.m_z + m_rows[3][3] * other.m_w;
		return Vector4(x, y, z, w);
	}

	Matrix4 Matrix4::Divide(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result[0][0] = m_rows[0][0] / other[0][0] + m_rows[1][0] / other[0][1] + m_rows[2][0] / other[0][2] + m_rows[3][0] / other[0][3];
		result[0][1] = m_rows[0][1] / other[0][0] + m_rows[1][1] / other[0][1] + m_rows[2][1] / other[0][2] + m_rows[3][1] / other[0][3];
		result[0][2] = m_rows[0][2] / other[0][0] + m_rows[1][2] / other[0][1] + m_rows[2][2] / other[0][2] + m_rows[3][2] / other[0][3];
		result[0][3] = m_rows[0][3] / other[0][0] + m_rows[1][3] / other[0][1] + m_rows[2][3] / other[0][2] + m_rows[3][3] / other[0][3];
		result[1][0] = m_rows[0][0] / other[1][0] + m_rows[1][0] / other[1][1] + m_rows[2][0] / other[1][2] + m_rows[3][0] / other[1][3];
		result[1][1] = m_rows[0][1] / other[1][0] + m_rows[1][1] / other[1][1] + m_rows[2][1] / other[1][2] + m_rows[3][1] / other[1][3];
		result[1][2] = m_rows[0][2] / other[1][0] + m_rows[1][2] / other[1][1] + m_rows[2][2] / other[1][2] + m_rows[3][2] / other[1][3];
		result[1][3] = m_rows[0][3] / other[1][0] + m_rows[1][3] / other[1][1] + m_rows[2][3] / other[1][2] + m_rows[3][3] / other[1][3];
		result[2][0] = m_rows[0][0] / other[2][0] + m_rows[1][0] / other[2][1] + m_rows[2][0] / other[2][2] + m_rows[3][0] / other[2][3];
		result[2][1] = m_rows[0][1] / other[2][0] + m_rows[1][1] / other[2][1] + m_rows[2][1] / other[2][2] + m_rows[3][1] / other[2][3];
		result[2][2] = m_rows[0][2] / other[2][0] + m_rows[1][2] / other[2][1] + m_rows[2][2] / other[2][2] + m_rows[3][2] / other[2][3];
		result[2][3] = m_rows[0][3] / other[2][0] + m_rows[1][3] / other[2][1] + m_rows[2][3] / other[2][2] + m_rows[3][3] / other[2][3];
		result[3][0] = m_rows[0][0] / other[3][0] + m_rows[1][0] / other[3][1] + m_rows[2][0] / other[3][2] + m_rows[3][0] / other[3][3];
		result[3][1] = m_rows[0][1] / other[3][0] + m_rows[1][1] / other[3][1] + m_rows[2][1] / other[3][2] + m_rows[3][1] / other[3][3];
		result[3][2] = m_rows[0][2] / other[3][0] + m_rows[1][2] / other[3][1] + m_rows[2][2] / other[3][2] + m_rows[3][2] / other[3][3];
		result[3][3] = m_rows[0][3] / other[3][0] + m_rows[1][3] / other[3][1] + m_rows[2][3] / other[3][2] + m_rows[3][3] / other[3][3];
		return result;
	}

	Vector4 Matrix4::Transform(const Vector4 &other) const
	{
		float x = m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y + m_rows[2][0] * other.m_z + m_rows[3][0] * other.m_w;
		float y = m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y + m_rows[2][1] * other.m_z + m_rows[3][1] * other.m_w;
		float z = m_rows[0][2] * other.m_x + m_rows[1][2] * other.m_y + m_rows[2][2] * other.m_z + m_rows[3][2] * other.m_w;
		float w = m_rows[0][3] * other.m_x + m_rows[1][3] * other.m_y + m_rows[2][3] * other.m_z + m_rows[3][3] * other.m_w;
		return Vector4(x, y, z, w);
	}

	Matrix4 Matrix4::Translate(const Vector2 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result[3][0] += m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y;
		result[3][1] += m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y;
		result[3][2] += m_rows[0][2] * other.m_x + m_rows[1][2] * other.m_y;
		result[3][3] += m_rows[0][3] * other.m_x + m_rows[1][3] * other.m_y;
		return result;
	}

	Matrix4 Matrix4::Translate(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result[3][0] += m_rows[0][0] * other.m_x + m_rows[1][0] * other.m_y + m_rows[2][0] * other.m_z;
		result[3][1] += m_rows[0][1] * other.m_x + m_rows[1][1] * other.m_y + m_rows[2][1] * other.m_z;
		result[3][2] += m_rows[0][2] * other.m_x + m_rows[1][2] * other.m_y + m_rows[2][2] * other.m_z;
		result[3][3] += m_rows[0][3] * other.m_x + m_rows[1][3] * other.m_y + m_rows[2][3] * other.m_z;
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result[0][0] *= other.m_x;
		result[0][1] *= other.m_x;
		result[0][2] *= other.m_x;
		result[0][3] *= other.m_x;
		result[1][0] *= other.m_y;
		result[1][1] *= other.m_y;
		result[1][2] *= other.m_y;
		result[1][3] *= other.m_y;
		result[2][0] *= other.m_z;
		result[2][1] *= other.m_z;
		result[2][2] *= other.m_z;
		result[2][3] *= other.m_z;
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector4 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result[0][0] *= other.m_x;
		result[0][1] *= other.m_x;
		result[0][2] *= other.m_x;
		result[0][3] *= other.m_x;
		result[1][0] *= other.m_y;
		result[1][1] *= other.m_y;
		result[1][2] *= other.m_y;
		result[1][3] *= other.m_y;
		result[2][0] *= other.m_z;
		result[2][1] *= other.m_z;
		result[2][2] *= other.m_z;
		result[2][3] *= other.m_z;
		result[3][0] *= other.m_w;
		result[3][1] *= other.m_w;
		result[3][2] *= other.m_w;
		result[3][3] *= other.m_w;
		return result;
	}

	Matrix4 Matrix4::Rotate(const float &angle, const Vector3 &axis) const
	{
		Matrix4 result = Matrix4(*this);

		float c = std::cos(angle);
		float s = std::sin(angle);
		float o = 1.0f - c;
		float xy = axis.m_x * axis.m_y;
		float yz = axis.m_y * axis.m_z;
		float xz = axis.m_x * axis.m_z;
		float xs = axis.m_x * s;
		float ys = axis.m_y * s;
		float zs = axis.m_z * s;

		float f00 = axis.m_x * axis.m_x * o + c;
		float f01 = xy * o + zs;
		float f02 = xz * o - ys;
		float f10 = xy * o - zs;
		float f11 = axis.m_y * axis.m_y * o + c;
		float f12 = yz * o + xs;
		float f20 = xz * o + ys;
		float f21 = yz * o - xs;
		float f22 = axis.m_z * axis.m_z * o + c;

		result[0][0] = m_rows[0][0] * f00 + m_rows[1][0] * f01 + m_rows[2][0] * f02;
		result[0][1] = m_rows[0][1] * f00 + m_rows[1][1] * f01 + m_rows[2][1] * f02;
		result[0][2] = m_rows[0][2] * f00 + m_rows[1][2] * f01 + m_rows[2][2] * f02;
		result[0][3] = m_rows[0][3] * f00 + m_rows[1][3] * f01 + m_rows[2][3] * f02;
		result[1][0] = m_rows[0][0] * f10 + m_rows[1][0] * f11 + m_rows[2][0] * f12;
		result[1][1] = m_rows[0][1] * f10 + m_rows[1][1] * f11 + m_rows[2][1] * f12;
		result[1][2] = m_rows[0][2] * f10 + m_rows[1][2] * f11 + m_rows[2][2] * f12;
		result[1][3] = m_rows[0][3] * f10 + m_rows[1][3] * f11 + m_rows[2][3] * f12;
		result[2][0] = m_rows[0][0] * f20 + m_rows[1][0] * f21 + m_rows[2][0] * f22;
		result[2][1] = m_rows[0][1] * f20 + m_rows[1][1] * f21 + m_rows[2][1] * f22;
		result[2][2] = m_rows[0][2] * f20 + m_rows[1][2] * f21 + m_rows[2][2] * f22;
		result[2][3] = m_rows[0][3] * f20 + m_rows[1][3] * f21 + m_rows[2][3] * f22;
		return result;
	}

	Matrix4 Matrix4::Negate() const
	{
		Matrix4 result = Matrix4();
		result[0][0] = -m_rows[0][0];
		result[0][1] = -m_rows[0][1];
		result[0][2] = -m_rows[0][2];
		result[0][3] = -m_rows[0][3];
		result[1][0] = -m_rows[1][0];
		result[1][1] = -m_rows[1][1];
		result[1][2] = -m_rows[1][2];
		result[1][3] = -m_rows[1][3];
		result[2][0] = -m_rows[2][0];
		result[2][1] = -m_rows[2][1];
		result[2][2] = -m_rows[2][2];
		result[2][3] = -m_rows[2][3];
		result[3][0] = -m_rows[3][0];
		result[3][1] = -m_rows[3][1];
		result[3][2] = -m_rows[3][2];
		result[3][3] = -m_rows[3][3];
		return result;
	}

	Matrix4 Matrix4::Invert() const
	{
		Matrix4 result = Matrix4();
		const float d = Determinant();

		if (d != 0.0f)
		{
			float determinantInv = 1.0f / d;

			// First row.
			float t00 = Determinant3x3(m_rows[1][1], m_rows[1][2], m_rows[1][3], m_rows[2][1], m_rows[2][2], m_rows[2][3], m_rows[3][1], m_rows[3][2], m_rows[3][3]);
			float t01 = -Determinant3x3(m_rows[1][0], m_rows[1][2], m_rows[1][3], m_rows[2][0], m_rows[2][2], m_rows[2][3], m_rows[3][0], m_rows[3][2], m_rows[3][3]);
			float t02 = Determinant3x3(m_rows[1][0], m_rows[1][1], m_rows[1][3], m_rows[2][0], m_rows[2][1], m_rows[2][3], m_rows[3][0], m_rows[3][1], m_rows[3][3]);
			float t03 = -Determinant3x3(m_rows[1][0], m_rows[1][1], m_rows[1][2], m_rows[2][0], m_rows[2][1], m_rows[2][2], m_rows[3][0], m_rows[3][1], m_rows[3][2]);

			// Second row.
			float t10 = -Determinant3x3(m_rows[0][1], m_rows[0][2], m_rows[0][3], m_rows[2][1], m_rows[2][2], m_rows[2][3], m_rows[3][1], m_rows[3][2], m_rows[3][3]);
			float t11 = Determinant3x3(m_rows[0][0], m_rows[0][2], m_rows[0][3], m_rows[2][0], m_rows[2][2], m_rows[2][3], m_rows[3][0], m_rows[3][2], m_rows[3][3]);
			float t12 = -Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][3], m_rows[2][0], m_rows[2][1], m_rows[2][3], m_rows[3][0], m_rows[3][1], m_rows[3][3]);
			float t13 = Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][2], m_rows[2][0], m_rows[2][1], m_rows[2][2], m_rows[3][0], m_rows[3][1], m_rows[3][2]);

			// Third row.
			float t20 = Determinant3x3(m_rows[0][1], m_rows[0][2], m_rows[0][3], m_rows[1][1], m_rows[1][2], m_rows[1][3], m_rows[3][1], m_rows[3][2], m_rows[3][3]);
			float t21 = -Determinant3x3(m_rows[0][0], m_rows[0][2], m_rows[0][3], m_rows[1][0], m_rows[1][2], m_rows[1][3], m_rows[3][0], m_rows[3][2], m_rows[3][3]);
			float t22 = Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][3], m_rows[1][0], m_rows[1][1], m_rows[1][3], m_rows[3][0], m_rows[3][1], m_rows[3][3]);
			float t23 = -Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][2], m_rows[1][0], m_rows[1][1], m_rows[1][2], m_rows[3][0], m_rows[3][1], m_rows[3][2]);

			// Fourth row.
			float t30 = -Determinant3x3(m_rows[0][1], m_rows[0][2], m_rows[0][3], m_rows[1][1], m_rows[1][2], m_rows[1][3], m_rows[2][1], m_rows[2][2], m_rows[2][3]);
			float t31 = Determinant3x3(m_rows[0][0], m_rows[0][2], m_rows[0][3], m_rows[1][0], m_rows[1][2], m_rows[1][3], m_rows[2][0], m_rows[2][2], m_rows[2][3]);
			float t32 = -Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][3], m_rows[1][0], m_rows[1][1], m_rows[1][3], m_rows[2][0], m_rows[2][1], m_rows[2][3]);
			float t33 = Determinant3x3(m_rows[0][0], m_rows[0][1], m_rows[0][2], m_rows[1][0], m_rows[1][1], m_rows[1][2], m_rows[2][0], m_rows[2][1], m_rows[2][2]);

			// Transpose and divide by the determinant.
			result[0][0] = t00 * determinantInv;
			result[1][1] = t11 * determinantInv;
			result[2][2] = t22 * determinantInv;
			result[3][3] = t33 * determinantInv;
			result[0][1] = t10 * determinantInv;
			result[1][0] = t01 * determinantInv;
			result[2][0] = t02 * determinantInv;
			result[0][2] = t20 * determinantInv;
			result[1][2] = t21 * determinantInv;
			result[2][1] = t12 * determinantInv;
			result[0][3] = t30 * determinantInv;
			result[3][0] = t03 * determinantInv;
			result[1][3] = t31 * determinantInv;
			result[3][1] = t13 * determinantInv;
			result[3][2] = t23 * determinantInv;
			result[2][3] = t32 * determinantInv;
		}

		return result;
	}

	Matrix4 Matrix4::Transpose() const
	{
		Matrix4 result = Matrix4();
		result[0][0] = m_rows[0][0];
		result[0][1] = m_rows[1][0];
		result[0][2] = m_rows[2][0];
		result[0][3] = m_rows[3][0];
		result[1][0] = m_rows[0][1];
		result[1][1] = m_rows[1][1];
		result[1][2] = m_rows[2][1];
		result[1][3] = m_rows[3][1];
		result[2][0] = m_rows[0][2];
		result[2][1] = m_rows[1][2];
		result[2][2] = m_rows[2][2];
		result[2][3] = m_rows[3][2];
		result[3][0] = m_rows[0][3];
		result[3][1] = m_rows[1][3];
		result[3][2] = m_rows[2][3];
		result[3][3] = m_rows[3][3];
		return result;
	}

	float Matrix4::Determinant() const
	{
		return (m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] * m_rows[3][3] + m_rows[1][2] * m_rows[2][3] * m_rows[3][1] + m_rows[1][3] * m_rows[2][1] * m_rows[3][2] - m_rows[1][3] * m_rows[2][2] * m_rows[3][1] - m_rows[1][1] * m_rows[2][3] * m_rows[3][2] - m_rows[1][2] * m_rows[2][1] * m_rows[3][3]))
			- (m_rows[0][1] * (m_rows[1][0] * m_rows[2][2] * m_rows[3][3] + m_rows[1][2] * m_rows[2][3] * m_rows[3][0] + m_rows[1][3] * m_rows[2][0] * m_rows[3][2] - m_rows[1][3] * m_rows[2][2] * m_rows[3][0] - m_rows[1][0] * m_rows[2][3] * m_rows[3][2] - m_rows[1][2] * m_rows[2][0] * m_rows[3][3]))
			+ (m_rows[0][2] * (m_rows[1][0] * m_rows[2][1] * m_rows[3][3] + m_rows[1][1] * m_rows[2][3] * m_rows[3][0] + m_rows[1][3] * m_rows[2][0] * m_rows[3][1] - m_rows[1][3] * m_rows[2][1] * m_rows[3][0] - m_rows[1][0] * m_rows[2][3] * m_rows[3][1] - m_rows[1][1] * m_rows[2][0] * m_rows[3][3]))
			- (m_rows[0][3] * (m_rows[1][0] * m_rows[2][1] * m_rows[3][2] + m_rows[1][1] * m_rows[2][2] * m_rows[3][0] + m_rows[1][2] * m_rows[2][0] * m_rows[3][1] - m_rows[1][2] * m_rows[2][1] * m_rows[3][0] - m_rows[1][0] * m_rows[2][2] * m_rows[3][1] - m_rows[1][1] * m_rows[2][0] * m_rows[3][2]));
	}

	Matrix4 Matrix4::TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale)
	{
		Matrix4 result = Matrix4();

		if (translation.LengthSquared() != 0.0f)
		{
			result = result.Translate(translation);
		}

		if (rotation.LengthSquared() != 0.0f)
		{
			result = result.Rotate(Maths::Radians(rotation.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(rotation.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(rotation.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (scale.m_x != 1.0f || scale.m_y != 1.0f || scale.m_z != 1.0f)
		{
			result = result.Scale(scale);
		}

		return result;
	}

	Matrix4 Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar)
	{
		Matrix4 result = Matrix4();

		float yScale = 1.0f / std::tan(Maths::Radians(fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float length = zFar - zNear;

		result[0][0] = xScale;
		result[1][1] = -yScale;
		result[2][2] = -((zFar + zNear) / length);
		result[2][3] = -1.0f;
		result[3][2] = -((2.0f * zNear * zFar) / length);
		result[3][3] = 0.0f;
		return result;
	}

	Matrix4 Matrix4::OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far)
	{
		Matrix4 result = Matrix4();

		float ox = 2.0f / (right - left);
		float oy = 2.0f / (top - bottom);
		float oz = -2.0f / (far - near);

		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(far + near) / (far - near);

		result[0][0] = ox;
		result[1][1] = oy;
		result[2][2] = oz;
		result[0][3] = tx;
		result[1][3] = ty;
		result[2][3] = tz;
		result[3][3] = 1.0f;
		return result;
	}

	Matrix4 Matrix4::ViewMatrix(const Vector3 &position, const Vector3 &rotation)
	{
		Matrix4 result = Matrix4();

		if (rotation != 0.0f)
		{
			result = result.Rotate(Maths::Radians(rotation.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(-rotation.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(rotation.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (position != 0.0f)
		{
			result = result.Translate(position.Negate());
		}

		return result;
	}

	Vector3 Matrix4::WorldToScreenSpace(const Vector3 &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
	{
		Vector4 point4 = Vector4(worldSpace.m_x, worldSpace.m_y, worldSpace.m_z, 1.0f);
		point4 = viewMatrix.Transform(point4);
		point4 = projectionMatrix.Transform(point4);

		Vector3 result = Vector3(point4);
		result.m_x /= result.m_z;
		result.m_y /= result.m_z;
		return result;
	}

	Matrix4 Matrix4::LookAt(const Vector3 &camera, const Vector3 &object, const Vector3 &up)
	{
		Vector3 f = (object - camera).Normalize();
		Vector3 s = f.Cross(up.Normalize());
		Vector3 u = s.Cross(f);

		Matrix4 result = Matrix4();
		result[0][0] = s.m_x;
		result[0][1] = s.m_y;
		result[0][2] = s.m_z;
		result[1][0] = u.m_x;
		result[1][1] = u.m_y;
		result[1][2] = u.m_z;
		result[2][0] = -f.m_x;
		result[2][1] = -f.m_y;
		result[2][2] = -f.m_z;

		result *= IDENTITY.Translate(Vector3(-camera.m_x, -camera.m_y, -camera.m_z));

		return result;
	}

	void Matrix4::Write(LoadedValue *destination)
	{
		m_rows[0].Write(destination->GetChild("m0", true));
		m_rows[1].Write(destination->GetChild("m1", true));
		m_rows[2].Write(destination->GetChild("m2", true));
		m_rows[3].Write(destination->GetChild("m3", true));
	}

	Matrix4 &Matrix4::operator=(const Matrix4 &other)
	{
		for (int i = 0; i < 4; i++)
		{
			m_rows[i] = other[i];
		}

		return *this;
	}

	Matrix4 &Matrix4::operator=(const float *array)
	{
		memcpy(m_rows, array, 4 * 4 * sizeof(float));
		return *this;
	}

	Matrix4 &Matrix4::operator=(LoadedValue *source)
	{
		m_rows[0] = source->GetChild("m0");
		m_rows[1] = source->GetChild("m1");
		m_rows[2] = source->GetChild("m2");
		m_rows[3] = source->GetChild("m3");
		return *this;
	}

	bool Matrix4::operator==(const Matrix4 &other) const
	{
		return m_rows[0] == other[0] && m_rows[1] == other[1] && m_rows[2] == other[2] && m_rows[3] == other[3];
	}

	bool Matrix4::operator!=(const Matrix4 &other) const
	{
		return !(*this == other);
	}

	Matrix4 Matrix4::operator-() const
	{
		return Negate();
	}

	const Vector4 &Matrix4::operator[](uint32_t index) const
	{
		assert(index < 4);
		return m_rows[index];
	}

	Vector4 &Matrix4::operator[](uint32_t index)
	{
		assert(index < 4);
		return m_rows[index];
	}

	Matrix4 operator+(Matrix4 left, const Matrix4 &right)
	{
		return left.Add(right);
	}

	Matrix4 operator-(Matrix4 left, const Matrix4 &right)
	{
		return left.Subtract(right);
	}

	Matrix4 operator*(Matrix4 left, const Matrix4 &right)
	{
		return left.Multiply(right);
	}

	Matrix4 operator/(Matrix4 left, const Matrix4 &right)
	{
		return left.Divide(right);
	}

	Matrix4 operator*(Matrix4 left, Vector4 value)
	{
		return left.Scale(value);
	}

	Matrix4 operator/(Matrix4 left, Vector4 value)
	{
		return left.Scale(1.0f / value);
	}

	Matrix4 operator*(Matrix4 left, float value)
	{
		return left.Scale(Vector4(value, value, value, value));
	}

	Matrix4 operator/(Matrix4 left, float value)
	{
		return left.Scale(1.0f / Vector4(value, value, value, value));
	}

	Matrix4 operator*(float value, Matrix4 left)
	{
		return left.Scale(Vector4(value, value, value, value));
	}

	Matrix4 operator/(float value, Matrix4 left)
	{
		return left.Scale(1.0f / Vector4(value, value, value, value));
	}

	Matrix4 &Matrix4::operator+=(const Matrix4 &other)
	{
		return *this = Add(other);
	}

	Matrix4 &Matrix4::operator-=(const Matrix4 &other)
	{
		return *this = Subtract(other);
	}

	Matrix4 &Matrix4::operator*=(const Matrix4 &other)
	{
		return *this = Multiply(other);
	}

	Matrix4 &Matrix4::operator/=(const Matrix4 &other)
	{
		return *this = Divide(other);
	}

	std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix)
	{
		stream << matrix.ToString();
		return stream;
	}

	std::string Matrix4::ToString() const
	{
		std::stringstream result;
		result << "Matrix4(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " << m_rows[0][2] << ", " << m_rows[0][3] << ", " <<
			m_rows[1][0] << ", " << m_rows[1][1] << ", " << m_rows[1][2] << ", " << m_rows[1][3] << ", " <<
			m_rows[2][0] << ", " << m_rows[2][1] << ", " << m_rows[2][2] << ", " << m_rows[2][3] << ", " <<
			m_rows[3][0] << ", " << m_rows[3][1] << ", " << m_rows[3][2] << ", " << m_rows[3][3] << ")";
		return result.str();
	}

	float Matrix4::Determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22)
	{
		return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
	}
}
