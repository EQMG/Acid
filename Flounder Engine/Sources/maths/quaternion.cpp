#include "quaternion.h"

namespace flounder {
	quaternion::quaternion()
	{
		this->set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	quaternion::quaternion(float x, float y, float z, float w)
	{
		this->set(x, y, z, w);
	}

	quaternion::quaternion(quaternion *source)
	{
		this->set(source);
	}

	quaternion::quaternion(matrix4x4 *source)
	{
		this->set(source);
	}

	quaternion::~quaternion()
	{
	}

	quaternion *quaternion::set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return this;
	}

	quaternion *quaternion::set(quaternion *source)
	{
		this->x = source->x;
		this->y = source->y;
		this->z = source->z;
		this->w = source->w;
		return this;
	}

	quaternion *quaternion::set(vector4 *source)
	{
		float n = (float) sqrt(x * x + y * y + z * z);
		float s = (float) sin(0.5 * source->w) / n;
		this->x = source->x * s;
		this->y = source->y * s;
		this->z = source->z * s;
		this->w = (float) cos(0.5 * source->w);
		return this;
	}

	quaternion *quaternion::set(matrix4x4 *source)
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = 0.0f;

		float diagonal = source->m00 + source->m11 + source->m22;

		if (diagonal > 0.0f)
		{
			float w4 = sqrt(diagonal + 1.0f) * 2.0f;
			w = w4 / 4.0f;
			x = (source->m21 - source->m12) / w4;
			y = (source->m02 - source->m20) / w4;
			z = (source->m10 - source->m01) / w4;
		}
		else if ((source->m00 > source->m11) && (source->m00 > source->m22))
		{
			float x4 = sqrt(1.0f + source->m00 - source->m11 - source->m22) * 2.0f;
			w = (source->m21 - source->m12) / x4;
			x = x4 / 4.0f;
			y = (source->m01 + source->m10) / x4;
			z = (source->m02 + source->m20) / x4;
		}
		else if (source->m11 > source->m22)
		{
			float y4 = sqrt(1.0f + source->m11 - source->m00 - source->m22 * 2.0f);
			w = (source->m02 - source->m20) / y4;
			x = (source->m01 + source->m10) / y4;
			y = y4 / 4.0f;
			z = (source->m12 + source->m21) / y4;
		}
		else
		{
			float z4 = sqrt(1.0f + source->m22 - source->m00 - source->m11) * 2.0f;
			w = (source->m10 - source->m01) / z4;
			x = (source->m02 + source->m20) / z4;
			y = (source->m12 + source->m21) / z4;
			z = z4 / 4.0f;
		}

		return this;
	}

	quaternion *quaternion::set(matrix3x3 *source)
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = 0.0f;

		float tr = source->m00 + source->m11 + source->m22;

		if (tr >= 0.0)
		{
			float s = (float) sqrt(tr + 1.0);
			w = s * 0.5f;
			s = 0.5f / s;
			x = (source->m21 - source->m12) * s;
			y = (source->m02 - source->m20) * s;
			z = (source->m10 - source->m01) * s;
		}
		else
		{
			float max = __max(__max(source->m00, source->m11), source->m22);

			if (max == source->m00)
			{
				float s = sqrt(source->m00 - (source->m11 + source->m22) + 1.0f);
				x = s * 0.5f;
				s = 0.5f / s;
				y = (source->m01 + source->m10) * s;
				z = (source->m20 + source->m02) * s;
				w = (source->m21 - source->m12) * s;
			}
			else if (max == source->m11)
			{
				float s = sqrt(source->m11 - (source->m22 + source->m00) + 1.0f);
				y = s * 0.5f;
				s = 0.5f / s;
				z = (source->m12 + source->m21) * s;
				x = (source->m01 + source->m10) * s;
				w = (source->m02 - source->m20) * s;
			}
			else
			{
				float s = sqrt(source->m22 - (source->m00 + source->m11) + 1.0f);
				z = s * 0.5f;
				s = 0.5f / s;
				x = (source->m20 + source->m02) * s;
				y = (source->m12 + source->m21) * s;
				w = (source->m10 - source->m01) * s;
			}
		}

		return this;
	}

	quaternion * quaternion::setIdentity(quaternion * source)
	{
		source->x = 0.0f;
		source->y = 0.0f;
		source->z = 0.0f;
		source->w = 1.0f;
		return source;
	}

	quaternion *quaternion::multiply(quaternion *left, quaternion *right, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(left->x * right->w + left->w * right->x + left->y * right->z - left->z * right->y, left->y * right->w + left->w * right->y + left->z * right->x - left->x * right->z, left->z * right->w + left->w * right->z + left->x * right->y - left->y * right->x, left->w * right->w - left->x * right->x - left->y * right->y - left->z * right->z);
	}

	quaternion *quaternion::multiplyInverse(quaternion *left, quaternion *right, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		float n = right->lengthSquared();
		n = (n == 0.0f ? n : 1.0f / n);
		return destination->set((left->x * right->w - left->w * right->x - left->y * right->z + left->z * right->y) * n, (left->y * right->w - left->w * right->y - left->z * right->x + left->x * right->z) * n, (left->z * right->w - left->w * right->z - left->x * right->y + left->y * right->x) * n, (left->w * right->w + left->x * right->x + left->y * right->y + left->z * right->z) * n);
	}

	float quaternion::dot(quaternion *left, quaternion *right)
	{
		return left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
	}

	quaternion *quaternion::slerp(quaternion *left, quaternion *right, float progression)
	{
		left->normalize();
		right->normalize();
		const float d = left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
		float absDot = d < 0.0f ? -d : d;
		float scale0 = 1.0f - progression;
		float scale1 = progression;

		if ((1.0f - absDot) > 0.1f)
		{
			const float angle = (float) acos(absDot);
			const float invSinTheta = 1.0f / (float) sin(angle);
			scale0 = (float) sin((1.0f - progression) * angle) * invSinTheta;
			scale1 = (float) sin(progression * angle) * invSinTheta;
		}

		if (d < 0.0f)
		{
			scale1 = -scale1;
		}

		float newX = (scale0 * left->x) + (scale1 * right->x);
		float newY = (scale0 * left->y) + (scale1 * right->y);
		float newZ = (scale0 * left->z) + (scale1 * right->z);
		float newW = (scale0 * left->w) + (scale1 * right->w);
		return new quaternion(newX, newY, newZ, newW);
	}

	quaternion * quaternion::scale(quaternion * source, float scalar, quaternion * destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(source->x * scalar, source->y * scalar, source->z * scalar, source->w * scalar);
	}

	quaternion *quaternion::negate(quaternion *source, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		return destination->set(-source->x, -source->y, -source->z, -source->w);
	}

	quaternion *quaternion::normalize(quaternion *source, quaternion *destination)
	{
		if (destination == NULL)
		{
			destination = new quaternion();
		}

		float length = source->length();
		
		if (length != 0.0f)
		{
			return destination->set(source->x / length, source->y / length, source->z / length, source->w / length);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	matrix4x4 * quaternion::toMatrix(quaternion *source, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float xSquared = source->x * source->x;
		float twoXY = 2.0f * source->x * source->y;
		float twoXZ = 2.0f * source->x * source->z;
		float twoXW = 2.0f * source->x * source->w;
		float ySquared = source->y *source->y;
		float twoYZ = 2.0f * source->y * source->z;
		float twoYW = 2.0f * source->y * source->w;
		float twoZW = 2.0f * source->z * source->w;
		float zSquared = source->z * source->z;
		float wSquared = source->w * source->w;

		destination->m00 = wSquared + xSquared - ySquared - zSquared;
		destination->m01 = twoXY - twoZW;
		destination->m02 = twoXZ + twoYW;
		destination->m03 = 0.0f;
		destination->m10 = twoXY + twoZW;
		destination->m11 = wSquared - xSquared + ySquared - zSquared;
		destination->m12 = twoYZ - twoXW;
		destination->m13 = 0.0f;
		destination->m20 = twoXZ - twoYW;
		destination->m21 = twoYZ + twoXW;
		destination->m22 = wSquared - xSquared - ySquared + zSquared;
		destination->m23 = 0.0f;
		destination->m30 = 0.0f;
		destination->m31 = 0.0f;
		destination->m32 = 0.0f;
		destination->m33 = 1.0f;
		return destination;
	}

	matrix4x4 * quaternion::toRotationMatrix(quaternion * source, matrix4x4 * destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float xy = source->x * source->y;
		float xz = source->x * source->z;
		float xw = source->x * source->w;
		float yz = source->y * source->z;
		float yw = source->y * source->w;
		float zw = source->z * source->w;
		float xSquared = source->x * source->x;
		float ySquared = source->y * source->y;
		float zSquared = source->z * source->z;

		destination->m00 = 1.0f - 2.0f * (ySquared + zSquared);
		destination->m01 = 2.0f * (xy - zw);
		destination->m02 = 2.0f * (xz + yw);
		destination->m03 = 0.0f;
		destination->m10 = 2.0f * (xy + zw);
		destination->m11 = 1.0f - 2.0f * (xSquared + zSquared);
		destination->m12 = 2.0f * (yz - xw);
		destination->m13 = 0.0f;
		destination->m20 = 2.0f * (xz - yw);
		destination->m21 = 2.0f * (yz + xw);
		destination->m22 = 1.0f - 2.0f * (xSquared + ySquared);
		destination->m23 = 0.0f;
		destination->m30 = 0.0f;
		destination->m31 = 0.0f;
		destination->m32 = 0.0f;
		destination->m33 = 1.0f;
		return destination;
	}

	quaternion * quaternion::setIdentity()
	{
		return setIdentity(this);
	}

	quaternion * quaternion::negate()
	{
		return negate(this, this);
	}

	quaternion * quaternion::normalize()
	{
		return normalize(this, this);
	}

	quaternion * quaternion::scale(float scalar)
	{
		return scale(this, scalar, this);
	}

	bool quaternion::isZero()
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}

	float quaternion::length()
	{
		return sqrt(lengthSquared());
	}

	float quaternion::lengthSquared()
	{
		return x * x + y * y + z * z + w * w;
	}
}
