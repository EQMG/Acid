#include "quaternion.h"

namespace flounder
{
	quaternion::quaternion()
	{
		set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	quaternion::quaternion(const float &x, const float &y, const float &z, const float &w)
	{
		set(x, y, z, w);
	}

	quaternion::quaternion(const quaternion &source)
	{
		set(source);
	}

	quaternion::quaternion(const matrix4x4 &source)
	{
		set(source);
	}

	quaternion::~quaternion()
	{
	}

	quaternion *quaternion::set(const float &x, const float &y, const float &z, const float &w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
		return this;
	}

	quaternion *quaternion::set(const quaternion &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = source.m_w;
		return this;
	}

	quaternion *quaternion::set(const vector4 &source)
	{
		float n = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		float s = sin(0.5f * source.m_w) / n;
		m_x = source.m_x * s;
		m_y = source.m_y * s;
		m_z = source.m_z * s;
		m_w = cos(0.5f * source.m_w);
		return this;
	}

	quaternion *quaternion::set(const matrix4x4 &source)
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
		m_w = 0.0f;

		float diagonal = source.m_00 + source.m_11 + source.m_22;

		if (diagonal > 0.0f)
		{
			float w4 = sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (source.m_21 - source.m_12) / w4;
			m_y = (source.m_02 - source.m_20) / w4;
			m_z = (source.m_10 - source.m_01) / w4;
		}
		else if ((source.m_00 > source.m_11) && (source.m_00 > source.m_22))
		{
			float x4 = sqrt(1.0f + source.m_00 - source.m_11 - source.m_22) * 2.0f;
			m_w = (source.m_21 - source.m_12) / x4;
			m_x = x4 / 4.0f;
			m_y = (source.m_01 + source.m_10) / x4;
			m_z = (source.m_02 + source.m_20) / x4;
		}
		else if (source.m_11 > source.m_22)
		{
			float y4 = sqrt(1.0f + source.m_11 - source.m_00 - source.m_22 * 2.0f);
			m_w = (source.m_02 - source.m_20) / y4;
			m_x = (source.m_01 + source.m_10) / y4;
			m_y = y4 / 4.0f;
			m_z = (source.m_12 + source.m_21) / y4;
		}
		else
		{
			float z4 = sqrt(1.0f + source.m_22 - source.m_00 - source.m_11) * 2.0f;
			m_w = (source.m_10 - source.m_01) / z4;
			m_x = (source.m_02 + source.m_20) / z4;
			m_y = (source.m_12 + source.m_21) / z4;
			m_z = z4 / 4.0f;
		}

		return this;
	}

	quaternion *quaternion::set(const matrix3x3 &source)
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
		m_w = 0.0f;

		float tr = source.m_00 + source.m_11 + source.m_22;

		if (tr >= 0.0)
		{
			float s = sqrt(tr + 1.0f);
			m_w = s * 0.5f;
			s = 0.5f / s;
			m_x = (source.m_21 - source.m_12) * s;
			m_y = (source.m_02 - source.m_20) * s;
			m_z = (source.m_10 - source.m_01) * s;
		}
		else
		{
			float max = __max(__max(source.m_00, source.m_11), source.m_22);

			if (max == source.m_00)
			{
				float s = sqrt(source.m_00 - (source.m_11 + source.m_22) + 1.0f);
				m_x = s * 0.5f;
				s = 0.5f / s;
				m_y = (source.m_01 + source.m_10) * s;
				m_z = (source.m_20 + source.m_02) * s;
				m_w = (source.m_21 - source.m_12) * s;
			}
			else if (max == source.m_11)
			{
				float s = sqrt(source.m_11 - (source.m_22 + source.m_00) + 1.0f);
				m_y = s * 0.5f;
				s = 0.5f / s;
				m_z = (source.m_12 + source.m_21) * s;
				m_x = (source.m_01 + source.m_10) * s;
				m_w = (source.m_02 - source.m_20) * s;
			}
			else
			{
				float s = sqrt(source.m_22 - (source.m_00 + source.m_11) + 1.0f);
				m_z = s * 0.5f;
				s = 0.5f / s;
				m_x = (source.m_20 + source.m_02) * s;
				m_y = (source.m_12 + source.m_21) * s;
				m_w = (source.m_10 - source.m_01) * s;
			}
		}

		return this;
	}

	quaternion *quaternion::setIdentity(quaternion *source)
	{
		source->m_x = 0.0f;
		source->m_y = 0.0f;
		source->m_z = 0.0f;
		source->m_w = 1.0f;
		return source;
	}

	quaternion *quaternion::multiply(const quaternion &left, const quaternion &right, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(left.m_x * right.m_w + left.m_w * right.m_x + left.m_y * right.m_z - left.m_z * right.m_y, left.m_y * right.m_w + left.m_w * right.m_y + left.m_z * right.m_x - left.m_x * right.m_z, left.m_z * right.m_w + left.m_w * right.m_z + left.m_x * right.m_y - left.m_y * right.m_x, left.m_w * right.m_w - left.m_x * right.m_x - left.m_y * right.m_y - left.m_z * right.m_z);
	}

	quaternion *quaternion::multiplyInverse(const quaternion &left, const quaternion &right, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		float n = lengthSquared(right);
		n = (n == 0.0f ? n : 1.0f / n);
		return destination->set((left.m_x * right.m_w - left.m_w * right.m_x - left.m_y * right.m_z + left.m_z * right.m_y) * n, (left.m_y * right.m_w - left.m_w * right.m_y - left.m_z * right.m_x + left.m_x * right.m_z) * n, (left.m_z * right.m_w - left.m_w * right.m_z - left.m_x * right.m_y + left.m_y * right.m_x) * n, (left.m_w * right.m_w + left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z) * n);
	}

	float quaternion::dot(const quaternion &left, const quaternion &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
	}

	quaternion *quaternion::slerp(const quaternion &left, const quaternion &right, const float &progression, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		const float &d = left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
		float absDot = d < 0.0f ? -d : d;
		float scale0 = 1.0f - progression;
		float scale1 = progression;

		if ((1.0f - absDot) > 0.1f)
		{
			const float &angle = acos(absDot);
			const float &invSinTheta = 1.0f / sin(angle);
			scale0 = sin((1.0f - progression) * angle) * invSinTheta;
			scale1 = sin(progression * angle) * invSinTheta;
		}

		if (d < 0.0f)
		{
			scale1 = -scale1;
		}

		float newX = (scale0 * left.m_x) + (scale1 * right.m_x);
		float newY = (scale0 * left.m_y) + (scale1 * right.m_y);
		float newZ = (scale0 * left.m_z) + (scale1 * right.m_z);
		float newW = (scale0 * left.m_w) + (scale1 * right.m_w);
		return destination->set(newX, newY, newZ, newW);
	}

	quaternion *quaternion::scale(const quaternion &source, const float &scalar, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar, source.m_w * scalar);
	}

	quaternion *quaternion::negate(const quaternion &source, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(-source.m_x, -source.m_y, -source.m_z, -source.m_w);
	}

	quaternion *quaternion::normalize(const quaternion &source, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l, source.m_z / l, source.m_w / l);
		}
		else
		{
			throw std::invalid_argument("Zero length quaternion");
		}
	}

	float quaternion::length(const quaternion &source)
	{
		return sqrt(lengthSquared(source));
	}

	float quaternion::lengthSquared(const quaternion &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z + source.m_w * source.m_w;
	}

	matrix4x4 *quaternion::toMatrix(const quaternion &source, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float xSquared = source.m_x * source.m_x;
		float twoXY = 2.0f * source.m_x * source.m_y;
		float twoXZ = 2.0f * source.m_x * source.m_z;
		float twoXW = 2.0f * source.m_x * source.m_w;
		float ySquared = source.m_y * source.m_y;
		float twoYZ = 2.0f * source.m_y * source.m_z;
		float twoYW = 2.0f * source.m_y * source.m_w;
		float twoZW = 2.0f * source.m_z * source.m_w;
		float zSquared = source.m_z * source.m_z;
		float wSquared = source.m_w * source.m_w;

		destination->m_00 = wSquared + xSquared - ySquared - zSquared;
		destination->m_01 = twoXY - twoZW;
		destination->m_02 = twoXZ + twoYW;
		destination->m_03 = 0.0f;
		destination->m_10 = twoXY + twoZW;
		destination->m_11 = wSquared - xSquared + ySquared - zSquared;
		destination->m_12 = twoYZ - twoXW;
		destination->m_13 = 0.0f;
		destination->m_20 = twoXZ - twoYW;
		destination->m_21 = twoYZ + twoXW;
		destination->m_22 = wSquared - xSquared - ySquared + zSquared;
		destination->m_23 = 0.0f;
		destination->m_30 = 0.0f;
		destination->m_31 = 0.0f;
		destination->m_32 = 0.0f;
		destination->m_33 = 1.0f;
		return destination;
	}

	matrix4x4 *quaternion::toRotationMatrix(const quaternion &source, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float xy = source.m_x * source.m_y;
		float xz = source.m_x * source.m_z;
		float xw = source.m_x * source.m_w;
		float yz = source.m_y * source.m_z;
		float yw = source.m_y * source.m_w;
		float zw = source.m_z * source.m_w;
		float xSquared = source.m_x * source.m_x;
		float ySquared = source.m_y * source.m_y;
		float zSquared = source.m_z * source.m_z;

		destination->m_00 = 1.0f - 2.0f * (ySquared + zSquared);
		destination->m_01 = 2.0f * (xy - zw);
		destination->m_02 = 2.0f * (xz + yw);
		destination->m_03 = 0.0f;
		destination->m_10 = 2.0f * (xy + zw);
		destination->m_11 = 1.0f - 2.0f * (xSquared + zSquared);
		destination->m_12 = 2.0f * (yz - xw);
		destination->m_13 = 0.0f;
		destination->m_20 = 2.0f * (xz - yw);
		destination->m_21 = 2.0f * (yz + xw);
		destination->m_22 = 1.0f - 2.0f * (xSquared + ySquared);
		destination->m_23 = 0.0f;
		destination->m_30 = 0.0f;
		destination->m_31 = 0.0f;
		destination->m_32 = 0.0f;
		destination->m_33 = 1.0f;
		return destination;
	}

	quaternion *quaternion::setIdentity()
	{
		return setIdentity(this);
	}

	quaternion *quaternion::negate()
	{
		return negate(*this, this);
	}

	quaternion *quaternion::normalize()
	{
		return normalize(*this, this);
	}

	quaternion *quaternion::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool quaternion::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f && m_w == 0.0f;
	}

	float quaternion::length() const
	{
		return length(*this);
	}

	float quaternion::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
