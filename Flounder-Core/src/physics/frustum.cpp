#include "frustum.hpp"

namespace flounder
{
	const int frustum::RIGHT = 0;
	const int frustum::LEFT = 1;
	const int frustum::BOTTOM = 2;
	const int frustum::TOP = 3;
	const int frustum::BACK = 4;
	const int frustum::FRONT = 5;

	const int frustum::A = 0;
	const int frustum::B = 1;
	const int frustum::C = 2;
	const int frustum::D = 3;

	frustum::frustum() :
		m_frustum(helperarray::recFloatArray(6, 4))
	{
	}

	frustum::~frustum()
	{
		delete m_frustum;
	}

	void frustum::update(const matrix4x4 &viewMatrix, const matrix4x4 &projectionMatrix)
	{
		float *view = matrix4x4::toArray(viewMatrix);
		float *proj = matrix4x4::toArray(projectionMatrix);
		float *clip = new float[16];

		clip[0] = view[0] * proj[0] + view[1] * proj[4] + view[2] * proj[8] + view[3] * proj[12];
		clip[1] = view[0] * proj[1] + view[1] * proj[5] + view[2] * proj[9] + view[3] * proj[13];
		clip[2] = view[0] * proj[2] + view[1] * proj[6] + view[2] * proj[10] + view[3] * proj[14];
		clip[3] = view[0] * proj[3] + view[1] * proj[7] + view[2] * proj[11] + view[3] * proj[15];

		clip[4] = view[4] * proj[0] + view[5] * proj[4] + view[6] * proj[8] + view[7] * proj[12];
		clip[5] = view[4] * proj[1] + view[5] * proj[5] + view[6] * proj[9] + view[7] * proj[13];
		clip[6] = view[4] * proj[2] + view[5] * proj[6] + view[6] * proj[10] + view[7] * proj[14];
		clip[7] = view[4] * proj[3] + view[5] * proj[7] + view[6] * proj[11] + view[7] * proj[15];

		clip[8] = view[8] * proj[0] + view[9] * proj[4] + view[10] * proj[8] + view[11] * proj[12];
		clip[9] = view[8] * proj[1] + view[9] * proj[5] + view[10] * proj[9] + view[11] * proj[13];
		clip[10] = view[8] * proj[2] + view[9] * proj[6] + view[10] * proj[10] + view[11] * proj[14];
		clip[11] = view[8] * proj[3] + view[9] * proj[7] + view[10] * proj[11] + view[11] * proj[15];

		clip[12] = view[12] * proj[0] + view[13] * proj[4] + view[14] * proj[8] + view[15] * proj[12];
		clip[13] = view[12] * proj[1] + view[13] * proj[5] + view[14] * proj[9] + view[15] * proj[13];
		clip[14] = view[12] * proj[2] + view[13] * proj[6] + view[14] * proj[10] + view[15] * proj[14];
		clip[15] = view[12] * proj[3] + view[13] * proj[7] + view[14] * proj[11] + view[15] * proj[15];

		// This will extract the LEFT side of the frustum.
		m_frustum[LEFT][A] = clip[3] - clip[0];
		m_frustum[LEFT][B] = clip[7] - clip[4];
		m_frustum[LEFT][C] = clip[11] - clip[8];
		m_frustum[LEFT][D] = clip[15] - clip[12];

		normalizePlane(m_frustum, LEFT);

		// This will extract the RIGHT side of the frustum.
		m_frustum[RIGHT][A] = clip[3] + clip[0];
		m_frustum[RIGHT][B] = clip[7] + clip[4];
		m_frustum[RIGHT][C] = clip[11] + clip[8];
		m_frustum[RIGHT][D] = clip[15] + clip[12];

		normalizePlane(m_frustum, RIGHT);

		// This will extract the BOTTOM side of the frustum.
		m_frustum[BOTTOM][A] = clip[3] + clip[1];
		m_frustum[BOTTOM][B] = clip[7] + clip[5];
		m_frustum[BOTTOM][C] = clip[11] + clip[9];
		m_frustum[BOTTOM][D] = clip[15] + clip[13];

		normalizePlane(m_frustum, BOTTOM);

		// This will extract the TOP side of the frustum.
		m_frustum[TOP][A] = clip[3] - clip[1];
		m_frustum[TOP][B] = clip[7] - clip[5];
		m_frustum[TOP][C] = clip[11] - clip[9];
		m_frustum[TOP][D] = clip[15] - clip[13];

		normalizePlane(m_frustum, TOP);

		// This will extract the FRONT side of the frustum.
		m_frustum[FRONT][A] = clip[3] - clip[2];
		m_frustum[FRONT][B] = clip[7] - clip[6];
		m_frustum[FRONT][C] = clip[11] - clip[10];
		m_frustum[FRONT][D] = clip[15] - clip[14];

		normalizePlane(m_frustum, FRONT);

		// This will extract the BACK side of the frustum.
		m_frustum[BACK][A] = clip[3] + clip[2];
		m_frustum[BACK][B] = clip[7] + clip[6];
		m_frustum[BACK][C] = clip[11] + clip[10];
		m_frustum[BACK][D] = clip[15] + clip[14];

		normalizePlane(m_frustum, BACK);

		// Deletes the arrays used to update the frustum.
		delete[] view;
		delete[] proj;
		delete[] clip;
	}

	bool frustum::pointInFrustum(const float &x, const float &y, const float &z) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustum[i][0] * x + m_frustum[i][1] * y + m_frustum[i][2] * z + m_frustum[i][3] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool frustum::sphereInFrustum(const float &x, const float &y, const float &z, const float &radius) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustum[i][0] * x + m_frustum[i][1] * y + m_frustum[i][2] * z + m_frustum[i][3] <= -radius)
			{
				return false;
			}
		}

		return true;
	}

	bool frustum::cubeInFrustum(const float &x1, const float &y1, const float &z1, const float &x2, const float &y2, const float &z2) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustum[i][0] * x1 + m_frustum[i][1] * y1 + m_frustum[i][2] * z1 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x2 + m_frustum[i][1] * y1 + m_frustum[i][2] * z1 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x1 + m_frustum[i][1] * y2 + m_frustum[i][2] * z1 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x2 + m_frustum[i][1] * y2 + m_frustum[i][2] * z1 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x1 + m_frustum[i][1] * y1 + m_frustum[i][2] * z2 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x2 + m_frustum[i][1] * y1 + m_frustum[i][2] * z2 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x1 + m_frustum[i][1] * y2 + m_frustum[i][2] * z2 + m_frustum[i][3] <= 0.0f && m_frustum[i][0] * x2 + m_frustum[i][1] * y2 + m_frustum[i][2] * z2 + m_frustum[i][3] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	void frustum::normalizePlane(float **frustum, const int &side)
	{
		float magnitude = sqrt(m_frustum[side][A] * m_frustum[side][A] + m_frustum[side][B] * m_frustum[side][B] + m_frustum[side][C] * m_frustum[side][C]);
		m_frustum[side][A] /= magnitude;
		m_frustum[side][B] /= magnitude;
		m_frustum[side][C] /= magnitude;
		m_frustum[side][D] /= magnitude;
	}
}
