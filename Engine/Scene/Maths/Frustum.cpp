#include "Frustum.hpp"

namespace acid {
void Frustum::Update(const Matrix4 &view, const Matrix4 &projection) {
	std::array<float, 16> clip;

	clip[0] = view[0][0] * projection[0][0] + view[0][1] * projection[1][0] + view[0][2] * projection[2][0] + view[0][3] * projection[3][0];
	clip[1] = view[0][0] * projection[0][1] + view[0][1] * projection[1][1] + view[0][2] * projection[2][1] + view[0][3] * projection[3][1];
	clip[2] = view[0][0] * projection[0][2] + view[0][1] * projection[1][2] + view[0][2] * projection[2][2] + view[0][3] * projection[3][2];
	clip[3] = view[0][0] * projection[0][3] + view[0][1] * projection[1][3] + view[0][2] * projection[2][3] + view[0][3] * projection[3][3];

	clip[4] = view[1][0] * projection[0][0] + view[1][1] * projection[1][0] + view[1][2] * projection[2][0] + view[1][3] * projection[3][0];
	clip[5] = view[1][0] * projection[0][1] + view[1][1] * projection[1][1] + view[1][2] * projection[2][1] + view[1][3] * projection[3][1];
	clip[6] = view[1][0] * projection[0][2] + view[1][1] * projection[1][2] + view[1][2] * projection[2][2] + view[1][3] * projection[3][2];
	clip[7] = view[1][0] * projection[0][3] + view[1][1] * projection[1][3] + view[1][2] * projection[2][3] + view[1][3] * projection[3][3];

	clip[8] = view[2][0] * projection[0][0] + view[2][1] * projection[1][0] + view[2][2] * projection[2][0] + view[2][3] * projection[3][0];
	clip[9] = view[2][0] * projection[0][1] + view[2][1] * projection[1][1] + view[2][2] * projection[2][1] + view[2][3] * projection[3][1];
	clip[10] = view[2][0] * projection[0][2] + view[2][1] * projection[1][2] + view[2][2] * projection[2][2] + view[2][3] * projection[3][2];
	clip[11] = view[2][0] * projection[0][3] + view[2][1] * projection[1][3] + view[2][2] * projection[2][3] + view[2][3] * projection[3][3];

	clip[12] = view[3][0] * projection[0][0] + view[3][1] * projection[1][0] + view[3][2] * projection[2][0] + view[3][3] * projection[3][0];
	clip[13] = view[3][0] * projection[0][1] + view[3][1] * projection[1][1] + view[3][2] * projection[2][1] + view[3][3] * projection[3][1];
	clip[14] = view[3][0] * projection[0][2] + view[3][1] * projection[1][2] + view[3][2] * projection[2][2] + view[3][3] * projection[3][2];
	clip[15] = view[3][0] * projection[0][3] + view[3][1] * projection[1][3] + view[3][2] * projection[2][3] + view[3][3] * projection[3][3];

	// This will extract the LEFT side of the frustum.
	frustum[1][0] = clip[3] - clip[0];
	frustum[1][1] = clip[7] - clip[4];
	frustum[1][2] = clip[11] - clip[8];
	frustum[1][3] = clip[15] - clip[12];

	NormalizePlane(1);

	// This will extract the RIGHT side of the frustum.
	frustum[0][0] = clip[3] + clip[0];
	frustum[0][1] = clip[7] + clip[4];
	frustum[0][2] = clip[11] + clip[8];
	frustum[0][3] = clip[15] + clip[12];

	NormalizePlane(0);

	// This will extract the BOTTOM side of the frustum.
	frustum[2][0] = clip[3] + clip[1];
	frustum[2][1] = clip[7] + clip[5];
	frustum[2][2] = clip[11] + clip[9];
	frustum[2][3] = clip[15] + clip[13];

	NormalizePlane(2);

	// This will extract the TOP side of the frustum.
	frustum[3][0] = clip[3] - clip[1];
	frustum[3][1] = clip[7] - clip[5];
	frustum[3][2] = clip[11] - clip[9];
	frustum[3][3] = clip[15] - clip[13];

	NormalizePlane(3);

	// This will extract the BACK side of the frustum.
	frustum[4][0] = clip[3] + clip[2];
	frustum[4][1] = clip[7] + clip[6];
	frustum[4][2] = clip[11] + clip[10];
	frustum[4][3] = clip[15] + clip[14];

	NormalizePlane(4);

	// This will extract the FRONT side of the frustum.
	frustum[5][0] = clip[3] - clip[2];
	frustum[5][1] = clip[7] - clip[6];
	frustum[5][2] = clip[11] - clip[10];
	frustum[5][3] = clip[15] - clip[14];

	NormalizePlane(5);
}

bool Frustum::PointInFrustum(const Vector3f &position) const {
	for (uint32_t i = 0; i < 6; i++) {
		if (frustum[i][0] * position.x + frustum[i][1] * position.y + frustum[i][2] * position.z + frustum[i][3] <= 0.0f) {
			return false;
		}
	}

	return true;
}

bool Frustum::SphereInFrustum(const Vector3f &position, float radius) const {
	for (uint32_t i = 0; i < 6; i++) {
		if (frustum[i][0] * position.x + frustum[i][1] * position.y + frustum[i][2] * position.z + frustum[i][3] <= -radius) {
			return false;
		}
	}

	return true;
}

bool Frustum::CubeInFrustum(const Vector3f &min, const Vector3f &max) const {
	for (uint32_t i = 0; i < 6; i++) {
		if (frustum[i][0] * min.x + frustum[i][1] * min.y + frustum[i][2] * min.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * max.x + frustum[i][1] * min.y + frustum[i][2] * min.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * min.x + frustum[i][1] * max.y + frustum[i][2] * min.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * max.x + frustum[i][1] * max.y + frustum[i][2] * min.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * min.x + frustum[i][1] * min.y + frustum[i][2] * max.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * max.x + frustum[i][1] * min.y + frustum[i][2] * max.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * min.x + frustum[i][1] * max.y + frustum[i][2] * max.z + frustum[i][3] <= 0.0f
			&& frustum[i][0] * max.x + frustum[i][1] * max.y + frustum[i][2] * max.z + frustum[i][3] <= 0.0f) {
			return false;
		}
	}

	return true;
}

void Frustum::NormalizePlane(int32_t side) {
	auto magnitude = std::sqrt(frustum[side][0] * frustum[side][0] + frustum[side][1] * frustum[side][1] + frustum[side][2] * frustum[side][2]);
	frustum[side][0] /= magnitude;
	frustum[side][1] /= magnitude;
	frustum[side][2] /= magnitude;
	frustum[side][3] /= magnitude;
}
}
