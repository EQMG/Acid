#pragma once

#include <Maths/Vector2.hpp>

namespace acid
{
	enum class QuadraticSolution
	{
		None, All, Touch, One, Two
	};

	struct Rect
	{
		float min_x;
		float min_y;
		float max_x;
		float max_y;
	};

	bool BboxBezier2Intersect(const Rect *bbox, const Vector2 bezier[3]);

	float LineSignedDistance(Vector2 a, Vector2 b, Vector2 p);

	float LineCalculateT(Vector2 a, Vector2 b, Vector2 p);

	void Bezier2Point(Vector2 &r, const Vector2 bezier[3], float t);

	void Bezier2SplitLr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], float t);

	void Bezier2Split_5P(Vector2 ret[5], const Vector2 bezier[3], float t);

	void Bezier2Split_3P(Vector2 ret[3], const Vector2 bezier[3], float t);

	void Bezier2Derivative(const Vector2 bezier[3], Vector2 derivative[2]);

	void Bezier2Bbox(const Vector2 bezier[3], Rect *bbox);

	void Bezier2AlignToSelf(Vector2 r[3], const Vector2 bezier[3]);

	void Bezier2AlignToLine(Vector2 r[3], const Vector2 bezier[3], Vector2 line0, Vector2 line1);

	bool Bezier2LineIsIntersecting(const Vector2 bezier[3], Vector2 line0, Vector2 line1);
}
