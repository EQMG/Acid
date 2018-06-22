#include "Matrix4.hpp"

#include <sstream>
#include "Maths.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace fl
{
	const Matrix4 Matrix4::IDENTITY = Matrix4().SetIdentity();
	const Matrix4 Matrix4::ZERO = Matrix4().SetZero();

	Matrix4::Matrix4() :
		m_00(1.0f),
		m_01(0.0f),
		m_02(0.0f),
		m_03(0.0f),
		m_10(0.0f),
		m_11(1.0f),
		m_12(0.0f),
		m_13(0.0f),
		m_20(0.0f),
		m_21(0.0f),
		m_22(1.0f),
		m_23(0.0f),
		m_30(0.0f),
		m_31(0.0f),
		m_32(0.0f),
		m_33(1.0f)
	{
	}

	Matrix4::Matrix4(const Matrix4 &source) :
		m_00(source.m_00),
		m_01(source.m_01),
		m_02(source.m_02),
		m_03(source.m_03),
		m_10(source.m_10),
		m_11(source.m_11),
		m_12(source.m_12),
		m_13(source.m_13),
		m_20(source.m_20),
		m_21(source.m_21),
		m_22(source.m_22),
		m_23(source.m_23),
		m_30(source.m_30),
		m_31(source.m_31),
		m_32(source.m_32),
		m_33(source.m_33)
	{
	}

	Matrix4::Matrix4(const float source[16]) :
		m_00(source[0]),
		m_01(source[1]),
		m_02(source[2]),
		m_03(source[3]),
		m_10(source[4]),
		m_11(source[5]),
		m_12(source[6]),
		m_13(source[7]),
		m_20(source[8]),
		m_21(source[9]),
		m_22(source[10]),
		m_23(source[11]),
		m_30(source[12]),
		m_31(source[13]),
		m_32(source[14]),
		m_33(source[15])
	{
	}

	Matrix4::~Matrix4()
	{
	}

	Matrix4 Matrix4::Add(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result.m_00 = m_00 + other.m_00;
		result.m_01 = m_01 + other.m_01;
		result.m_02 = m_02 + other.m_02;
		result.m_03 = m_03 + other.m_03;
		result.m_10 = m_10 + other.m_10;
		result.m_11 = m_11 + other.m_11;
		result.m_12 = m_12 + other.m_12;
		result.m_13 = m_13 + other.m_13;
		result.m_20 = m_20 + other.m_20;
		result.m_21 = m_21 + other.m_21;
		result.m_22 = m_22 + other.m_22;
		result.m_23 = m_23 + other.m_23;
		result.m_30 = m_30 + other.m_30;
		result.m_31 = m_31 + other.m_31;
		result.m_32 = m_32 + other.m_32;
		result.m_33 = m_33 + other.m_33;
		return result;
	}

	Matrix4 Matrix4::Subtract(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result.m_00 = m_00 - other.m_00;
		result.m_01 = m_01 - other.m_01;
		result.m_02 = m_02 - other.m_02;
		result.m_03 = m_03 - other.m_03;
		result.m_10 = m_10 - other.m_10;
		result.m_11 = m_11 - other.m_11;
		result.m_12 = m_12 - other.m_12;
		result.m_13 = m_13 - other.m_13;
		result.m_20 = m_20 - other.m_20;
		result.m_21 = m_21 - other.m_21;
		result.m_22 = m_22 - other.m_22;
		result.m_23 = m_23 - other.m_23;
		result.m_30 = m_30 - other.m_30;
		result.m_31 = m_31 - other.m_31;
		result.m_32 = m_32 - other.m_32;
		result.m_33 = m_33 - other.m_33;
		return result;
	}

	Matrix4 Matrix4::Multiply(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result.m_00 = m_00 * other.m_00 + m_10 * other.m_01 + m_20 * other.m_02 + m_30 * other.m_03;
		result.m_01 = m_01 * other.m_00 + m_11 * other.m_01 + m_21 * other.m_02 + m_31 * other.m_03;
		result.m_02 = m_02 * other.m_00 + m_12 * other.m_01 + m_22 * other.m_02 + m_32 * other.m_03;
		result.m_03 = m_03 * other.m_00 + m_13 * other.m_01 + m_23 * other.m_02 + m_33 * other.m_03;
		result.m_10 = m_00 * other.m_10 + m_10 * other.m_11 + m_20 * other.m_12 + m_30 * other.m_13;
		result.m_11 = m_01 * other.m_10 + m_11 * other.m_11 + m_21 * other.m_12 + m_31 * other.m_13;
		result.m_12 = m_02 * other.m_10 + m_12 * other.m_11 + m_22 * other.m_12 + m_32 * other.m_13;
		result.m_13 = m_03 * other.m_10 + m_13 * other.m_11 + m_23 * other.m_12 + m_33 * other.m_13;
		result.m_20 = m_00 * other.m_20 + m_10 * other.m_21 + m_20 * other.m_22 + m_30 * other.m_23;
		result.m_21 = m_01 * other.m_20 + m_11 * other.m_21 + m_21 * other.m_22 + m_31 * other.m_23;
		result.m_22 = m_02 * other.m_20 + m_12 * other.m_21 + m_22 * other.m_22 + m_32 * other.m_23;
		result.m_23 = m_03 * other.m_20 + m_13 * other.m_21 + m_23 * other.m_22 + m_33 * other.m_23;
		result.m_30 = m_00 * other.m_30 + m_10 * other.m_31 + m_20 * other.m_32 + m_30 * other.m_33;
		result.m_31 = m_01 * other.m_30 + m_11 * other.m_31 + m_21 * other.m_32 + m_31 * other.m_33;
		result.m_32 = m_02 * other.m_30 + m_12 * other.m_31 + m_22 * other.m_32 + m_32 * other.m_33;
		result.m_33 = m_03 * other.m_30 + m_13 * other.m_31 + m_23 * other.m_32 + m_33 * other.m_33;
		return result;
	}

	Vector4 Matrix4::Multiply(const Vector4 &other) const
	{
		float x = m_00 * other.m_x + m_10 * other.m_y + m_20 * other.m_z + m_30 * other.m_w;
		float y = m_01 * other.m_x + m_11 * other.m_y + m_21 * other.m_z + m_31 * other.m_w;
		float z = m_02 * other.m_x + m_12 * other.m_y + m_22 * other.m_z + m_32 * other.m_w;
		float w = m_03 * other.m_x + m_13 * other.m_y + m_23 * other.m_z + m_33 * other.m_w;
		return Vector4(x, y, z, w);
	}

	Matrix4 Matrix4::Divide(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();
		result.m_00 = m_00 / other.m_00 + m_10 / other.m_01 + m_20 / other.m_02 + m_30 / other.m_03;
		result.m_01 = m_01 / other.m_00 + m_11 / other.m_01 + m_21 / other.m_02 + m_31 / other.m_03;
		result.m_02 = m_02 / other.m_00 + m_12 / other.m_01 + m_22 / other.m_02 + m_32 / other.m_03;
		result.m_03 = m_03 / other.m_00 + m_13 / other.m_01 + m_23 / other.m_02 + m_33 / other.m_03;
		result.m_10 = m_00 / other.m_10 + m_10 / other.m_11 + m_20 / other.m_12 + m_30 / other.m_13;
		result.m_11 = m_01 / other.m_10 + m_11 / other.m_11 + m_21 / other.m_12 + m_31 / other.m_13;
		result.m_12 = m_02 / other.m_10 + m_12 / other.m_11 + m_22 / other.m_12 + m_32 / other.m_13;
		result.m_13 = m_03 / other.m_10 + m_13 / other.m_11 + m_23 / other.m_12 + m_33 / other.m_13;
		result.m_20 = m_00 / other.m_20 + m_10 / other.m_21 + m_20 / other.m_22 + m_30 / other.m_23;
		result.m_21 = m_01 / other.m_20 + m_11 / other.m_21 + m_21 / other.m_22 + m_31 / other.m_23;
		result.m_22 = m_02 / other.m_20 + m_12 / other.m_21 + m_22 / other.m_22 + m_32 / other.m_23;
		result.m_23 = m_03 / other.m_20 + m_13 / other.m_21 + m_23 / other.m_22 + m_33 / other.m_23;
		result.m_30 = m_00 / other.m_30 + m_10 / other.m_31 + m_20 / other.m_32 + m_30 / other.m_33;
		result.m_31 = m_01 / other.m_30 + m_11 / other.m_31 + m_21 / other.m_32 + m_31 / other.m_33;
		result.m_32 = m_02 / other.m_30 + m_12 / other.m_31 + m_22 / other.m_32 + m_32 / other.m_33;
		result.m_33 = m_03 / other.m_30 + m_13 / other.m_31 + m_23 / other.m_32 + m_33 / other.m_33;
		return result;
	}

	Vector4 Matrix4::Transform(const Vector4 &other) const
	{
		float x = m_00 * other.m_x + m_10 * other.m_y + m_20 * other.m_z + m_30 * other.m_w;
		float y = m_01 * other.m_x + m_11 * other.m_y + m_21 * other.m_z + m_31 * other.m_w;
		float z = m_02 * other.m_x + m_12 * other.m_y + m_22 * other.m_z + m_32 * other.m_w;
		float w = m_03 * other.m_x + m_13 * other.m_y + m_23 * other.m_z + m_33 * other.m_w;
		return Vector4(x, y, z, w);
	}

	Matrix4 Matrix4::Translate(const Vector2 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result.m_30 += m_00 * other.m_x + m_10 * other.m_y;
		result.m_31 += m_01 * other.m_x + m_11 * other.m_y;
		result.m_32 += m_02 * other.m_x + m_12 * other.m_y;
		result.m_33 += m_03 * other.m_x + m_13 * other.m_y;
		return result;
	}

	Matrix4 Matrix4::Translate(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result.m_30 += m_00 * other.m_x + m_10 * other.m_y + m_20 * other.m_z;
		result.m_31 += m_01 * other.m_x + m_11 * other.m_y + m_21 * other.m_z;
		result.m_32 += m_02 * other.m_x + m_12 * other.m_y + m_22 * other.m_z;
		result.m_33 += m_03 * other.m_x + m_13 * other.m_y + m_23 * other.m_z;
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result.m_00 *= other.m_x;
		result.m_01 *= other.m_x;
		result.m_02 *= other.m_x;
		result.m_03 *= other.m_x;
		result.m_10 *= other.m_y;
		result.m_11 *= other.m_y;
		result.m_12 *= other.m_y;
		result.m_13 *= other.m_y;
		result.m_20 *= other.m_z;
		result.m_21 *= other.m_z;
		result.m_22 *= other.m_z;
		result.m_23 *= other.m_z;
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector4 &other) const
	{
		Matrix4 result = Matrix4(*this);
		result.m_00 *= other.m_x;
		result.m_01 *= other.m_x;
		result.m_02 *= other.m_x;
		result.m_03 *= other.m_x;
		result.m_10 *= other.m_y;
		result.m_11 *= other.m_y;
		result.m_12 *= other.m_y;
		result.m_13 *= other.m_y;
		result.m_20 *= other.m_z;
		result.m_21 *= other.m_z;
		result.m_22 *= other.m_z;
		result.m_23 *= other.m_z;
		result.m_30 *= other.m_w;
		result.m_31 *= other.m_w;
		result.m_32 *= other.m_w;
		result.m_33 *= other.m_w;
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

		result.m_00 = m_00 * f00 + m_10 * f01 + m_20 * f02;
		result.m_01 = m_01 * f00 + m_11 * f01 + m_21 * f02;
		result.m_02 = m_02 * f00 + m_12 * f01 + m_22 * f02;
		result.m_03 = m_03 * f00 + m_13 * f01 + m_23 * f02;
		result.m_10 = m_00 * f10 + m_10 * f11 + m_20 * f12;
		result.m_11 = m_01 * f10 + m_11 * f11 + m_21 * f12;
		result.m_12 = m_02 * f10 + m_12 * f11 + m_22 * f12;
		result.m_13 = m_03 * f10 + m_13 * f11 + m_23 * f12;
		result.m_20 = m_00 * f20 + m_10 * f21 + m_20 * f22;
		result.m_21 = m_01 * f20 + m_11 * f21 + m_21 * f22;
		result.m_22 = m_02 * f20 + m_12 * f21 + m_22 * f22;
		result.m_23 = m_03 * f20 + m_13 * f21 + m_23 * f22;
		return result;
	}

	Matrix4 Matrix4::Negate() const
	{
		Matrix4 result = Matrix4();
		result.m_00 = -m_00;
		result.m_01 = -m_01;
		result.m_02 = -m_02;
		result.m_03 = -m_03;
		result.m_10 = -m_10;
		result.m_11 = -m_11;
		result.m_12 = -m_12;
		result.m_13 = -m_13;
		result.m_20 = -m_20;
		result.m_21 = -m_21;
		result.m_22 = -m_22;
		result.m_23 = -m_23;
		result.m_30 = -m_30;
		result.m_31 = -m_31;
		result.m_32 = -m_32;
		result.m_33 = -m_33;
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
			float t00 = Determinant3x3(m_11, m_12, m_13, m_21, m_22, m_23, m_31, m_32, m_33);
			float t01 = -Determinant3x3(m_10, m_12, m_13, m_20, m_22, m_23, m_30, m_32, m_33);
			float t02 = Determinant3x3(m_10, m_11, m_13, m_20, m_21, m_23, m_30, m_31, m_33);
			float t03 = -Determinant3x3(m_10, m_11, m_12, m_20, m_21, m_22, m_30, m_31, m_32);

			// Second row.
			float t10 = -Determinant3x3(m_01, m_02, m_03, m_21, m_22, m_23, m_31, m_32, m_33);
			float t11 = Determinant3x3(m_00, m_02, m_03, m_20, m_22, m_23, m_30, m_32, m_33);
			float t12 = -Determinant3x3(m_00, m_01, m_03, m_20, m_21, m_23, m_30, m_31, m_33);
			float t13 = Determinant3x3(m_00, m_01, m_02, m_20, m_21, m_22, m_30, m_31, m_32);

			// Third row.
			float t20 = Determinant3x3(m_01, m_02, m_03, m_11, m_12, m_13, m_31, m_32, m_33);
			float t21 = -Determinant3x3(m_00, m_02, m_03, m_10, m_12, m_13, m_30, m_32, m_33);
			float t22 = Determinant3x3(m_00, m_01, m_03, m_10, m_11, m_13, m_30, m_31, m_33);
			float t23 = -Determinant3x3(m_00, m_01, m_02, m_10, m_11, m_12, m_30, m_31, m_32);

			// Fourth row.
			float t30 = -Determinant3x3(m_01, m_02, m_03, m_11, m_12, m_13, m_21, m_22, m_23);
			float t31 = Determinant3x3(m_00, m_02, m_03, m_10, m_12, m_13, m_20, m_22, m_23);
			float t32 = -Determinant3x3(m_00, m_01, m_03, m_10, m_11, m_13, m_20, m_21, m_23);
			float t33 = Determinant3x3(m_00, m_01, m_02, m_10, m_11, m_12, m_20, m_21, m_22);

			// Transpose and divide by the determinant.
			result.m_00 = t00 * determinantInv;
			result.m_11 = t11 * determinantInv;
			result.m_22 = t22 * determinantInv;
			result.m_33 = t33 * determinantInv;
			result.m_01 = t10 * determinantInv;
			result.m_10 = t01 * determinantInv;
			result.m_20 = t02 * determinantInv;
			result.m_02 = t20 * determinantInv;
			result.m_12 = t21 * determinantInv;
			result.m_21 = t12 * determinantInv;
			result.m_03 = t30 * determinantInv;
			result.m_30 = t03 * determinantInv;
			result.m_13 = t31 * determinantInv;
			result.m_31 = t13 * determinantInv;
			result.m_32 = t23 * determinantInv;
			result.m_23 = t32 * determinantInv;
		}

		return result;
	}

	Matrix4 Matrix4::Transpose() const
	{
		Matrix4 result = Matrix4();
		result.m_00 = m_00;
		result.m_01 = m_10;
		result.m_02 = m_20;
		result.m_03 = m_30;
		result.m_10 = m_01;
		result.m_11 = m_11;
		result.m_12 = m_21;
		result.m_13 = m_31;
		result.m_20 = m_02;
		result.m_21 = m_12;
		result.m_22 = m_22;
		result.m_23 = m_32;
		result.m_30 = m_03;
		result.m_31 = m_13;
		result.m_32 = m_23;
		result.m_33 = m_33;
		return result;
	}

	float Matrix4::Determinant() const
	{
		return (m_00 * (m_11 * m_22 * m_33 + m_12 * m_23 * m_31 + m_13 * m_21 * m_32 - m_13 * m_22 * m_31 - m_11 * m_23 * m_32 - m_12 * m_21 * m_33))
			- (m_01 * (m_10 * m_22 * m_33 + m_12 * m_23 * m_30 + m_13 * m_20 * m_32 - m_13 * m_22 * m_30 - m_10 * m_23 * m_32 - m_12 * m_20 * m_33))
			+ (m_02 * (m_10 * m_21 * m_33 + m_11 * m_23 * m_30 + m_13 * m_20 * m_31 - m_13 * m_21 * m_30 - m_10 * m_23 * m_31 - m_11 * m_20 * m_33))
			- (m_03 * (m_10 * m_21 * m_32 + m_11 * m_22 * m_30 + m_12 * m_20 * m_31 - m_12 * m_21 * m_30 - m_10 * m_22 * m_31 - m_11 * m_20 * m_32));
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

	Matrix4 Matrix4::TransformationMatrix(const Vector2 &translation, const float &scale)
	{
		return TransformationMatrix(Vector3(translation.m_x, translation.m_y, 0.0f), Vector3::ZERO, Vector3(scale, scale, scale));
	}

	Matrix4 Matrix4::TransformationMatrix(const Vector2 &translation, const Vector3 &scale)
	{
		return TransformationMatrix(Vector3(translation.m_x, translation.m_y, 0.0f), Vector3::ZERO, scale);
	}

	Matrix4 Matrix4::TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const float &scale)
	{
		return TransformationMatrix(translation, rotation, Vector3(scale, scale, scale));
	}

	Matrix4 Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar)
	{
		Matrix4 result = Matrix4();

		float yScale = 1.0f / std::tan(Maths::Radians(fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float length = zFar - zNear;

		result.m_00 = xScale;
		result.m_11 = -yScale;
		result.m_22 = -((zFar + zNear) / length);
		result.m_23 = -1.0f;
		result.m_32 = -((2.0f * zNear * zFar) / length);
		result.m_33 = 0.0f;
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

		result.m_00 = ox;
		result.m_11 = oy;
		result.m_22 = oz;
		result.m_03 = tx;
		result.m_13 = ty;
		result.m_23 = tz;
		result.m_33 = 1.0f;
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

	Matrix4 Matrix4::SetZero()
	{
		m_00 = 0.0f;
		m_01 = 0.0f;
		m_02 = 0.0f;
		m_03 = 0.0f;
		m_10 = 0.0f;
		m_11 = 0.0f;
		m_12 = 0.0f;
		m_13 = 0.0f;
		m_20 = 0.0f;
		m_21 = 0.0f;
		m_22 = 0.0f;
		m_23 = 0.0f;
		m_30 = 0.0f;
		m_31 = 0.0f;
		m_32 = 0.0f;
		m_33 = 0.0f;
		return *this;
	}

	Matrix4 Matrix4::SetIdentity()
	{
		m_00 = 1.0f;
		m_01 = 0.0f;
		m_02 = 0.0f;
		m_03 = 0.0f;
		m_10 = 0.0f;
		m_11 = 1.0f;
		m_12 = 0.0f;
		m_13 = 0.0f;
		m_20 = 0.0f;
		m_21 = 0.0f;
		m_22 = 1.0f;
		m_23 = 0.0f;
		m_30 = 0.0f;
		m_31 = 0.0f;
		m_32 = 0.0f;
		m_33 = 1.0f;
		return *this;
	}

	void Matrix4::Write(std::shared_ptr<LoadedValue> destination)
	{
		m_0->Write(destination->GetChild("m0", true));
		m_1->Write(destination->GetChild("m1", true));
		m_2->Write(destination->GetChild("m2", true));
		m_3->Write(destination->GetChild("m3", true));
	}

	Matrix4 &Matrix4::operator=(const Matrix4 &other)
	{
		m_00 = other.m_00;
		m_01 = other.m_01;
		m_02 = other.m_02;
		m_03 = other.m_03;
		m_10 = other.m_10;
		m_11 = other.m_11;
		m_12 = other.m_12;
		m_13 = other.m_13;
		m_20 = other.m_20;
		m_21 = other.m_21;
		m_22 = other.m_22;
		m_23 = other.m_23;
		m_30 = other.m_30;
		m_31 = other.m_31;
		m_32 = other.m_32;
		m_33 = other.m_33;
		return *this;
	}

	Matrix4 &Matrix4::operator=(const float array[16])
	{
		m_00 = array[0];
		m_01 = array[1];
		m_02 = array[2];
		m_03 = array[3];
		m_10 = array[4];
		m_11 = array[5];
		m_12 = array[6];
		m_13 = array[7];
		m_20 = array[8];
		m_21 = array[9];
		m_22 = array[10];
		m_23 = array[11];
		m_30 = array[12];
		m_31 = array[13];
		m_32 = array[14];
		m_33 = array[15];
		return *this;
	}

	Matrix4 &Matrix4::operator=(std::shared_ptr<LoadedValue> source)
	{
		*m_0 = source->GetChild("m0");
		*m_1 = source->GetChild("m1");
		*m_2 = source->GetChild("m2");
		*m_3 = source->GetChild("m3");
		return *this;
	}

	bool Matrix4::operator==(const Matrix4 &other) const
	{
		return m_00 == other.m_00 && m_01 == other.m_01 && m_02 == other.m_02 && m_03 == other.m_03 &&
			m_10 == other.m_10 && m_11 == other.m_11 && m_12 == other.m_12 && m_13 == other.m_13 &&
			m_20 == other.m_20 && m_21 == other.m_21 && m_22 == other.m_22 && m_23 == other.m_23 &&
			m_30 == other.m_30 && m_31 == other.m_31 && m_32 == other.m_32 && m_33 == other.m_33;
	}

	bool Matrix4::operator!=(const Matrix4 &other) const
	{
		return !(*this == other);
	}

	Matrix4 Matrix4::operator-()
	{
		return Negate();
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
		result << "Matrix4(" << m_00 << ", " << m_01 << ", " << m_02 << ", " << m_03 << ", " <<
			m_10 << ", " << m_11 << ", " << m_12 << ", " << m_13 << ", " <<
			m_20 << ", " << m_21 << ", " << m_22 << ", " << m_23 << ", " <<
			m_30 << ", " << m_31 << ", " << m_32 << ", " << m_33 << ")";
		return result.str();
	}

	float Matrix4::Determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22)
	{
		return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
	}
}
