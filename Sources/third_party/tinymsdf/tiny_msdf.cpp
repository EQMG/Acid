#include "tiny_msdf.hpp"

#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <stdexcept>
#include <ft2build.h>
#include <optional>

#include FT_FREETYPE_H
#include FT_OUTLINE_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tinymsdf {
class ArithmeticException : std::runtime_error {
public:
	explicit ArithmeticException(const std::string &err) : runtime_error(err) {}
	explicit ArithmeticException(const char *err) : runtime_error(err) {}
	~ArithmeticException() override = default;
};
class FtException : std::runtime_error {
public:
	explicit FtException(const std::string &err) : runtime_error(err) {}
	explicit FtException(const char *err) : runtime_error(err) {}
	~FtException() override = default;
};
class MsdfException : std::runtime_error {
public:
	explicit MsdfException(const std::string &err) : runtime_error(err) {}
	explicit MsdfException(const char *err) : runtime_error(err) {}
	~MsdfException() override = default;
};

/// Returns the middle out of three values
template <typename T>
T Median(T a, T b, T c) {
	return std::max(std::min(a, b), std::min(std::max(a, b), c));
}
/// Returns the weighted average of a and b.
template <typename T, typename S>
T Mix(T a, T b, S weight) {
	return T((S(1) - weight) * a + weight * b);
}
/// Returns 1 for positive values, -1 for negative values, and 0 for zero.
template <typename T>
int Sign(T n) {
	return (T(0) < n) - (n < T(0));
}
/// Returns 1 for non-negative values and -1 for negative values.
template <typename T>
int NonZeroSign(T n) {
	return 2 * (n > T(0)) - 1;
}

#define F26DOT6_TO_DOUBLE(x) (1/64.0*double(x))

class Vector2 {
public:
	Vector2() = default;
	Vector2(double val) :
		x(val),
		y(val) {
	}
	Vector2(double x, double y) :
		x(x),
		y(y) {
	}
	Vector2(const FT_Vector &vector) :
		x(F26DOT6_TO_DOUBLE(vector.x)),
		y(F26DOT6_TO_DOUBLE(vector.y)) {
	}

	/// Returns the vector's length.
	double Length() const {
		return std::sqrt(x * x + y * y);
	}
	/// Returns the angle of the vector in radians (atan2).
	double Direction() const {
		return std::atan2(y, x);
	}
	/// Returns the normalized vector - one that has the same direction but unit length.
	Vector2 Normalize() const {
		double len = Length();
		if (len == 0)
			throw ArithmeticException("Vector length is zero");
		return Vector2(x / len, y / len);
	}
	/// Dot product of two vectors.
	double Dot(Vector2 right) const {
		return x * right.x + y * right.y;
	}
	/// A special version of the cross product for 2D vectors (returns scalar value).
	double Cross(Vector2 right) const {
		return x * right.y - y * right.x;
	}
	/// Returns a vector with the same length that is orthogonal to this one.
	Vector2 Orthonormal() const {
		double len = Length();
		if (len == 0)
			throw ArithmeticException("Vector length is zero");
		return Vector2(y / len, -x / len);
	}

	bool operator==(Vector2 right) const {
		return x == right.x && y == right.y;
	}
	bool operator!=(Vector2 right) const {
		return x != right.x || y != right.y;
	}
	Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	Vector2 operator+(Vector2 right) const {
		return Vector2(x + right.x, y + right.y);
	}
	Vector2 operator-(Vector2 right) const {
		return Vector2(x - right.x, y - right.y);
	}
	Vector2 operator*(Vector2 right) const {
		return Vector2(x * right.x, y * right.y);
	}
	Vector2 operator/(Vector2 right) const {
		return Vector2(x / right.x, y / right.y);
	}
	Vector2 operator*(double right) const {
		return Vector2(x * right, y * right);
	}
	Vector2 operator/(double right) const {
		return Vector2(x / right, y / right);
	}
	friend Vector2 operator*(double left, Vector2 right) {
		return Vector2(left * right.x, left * right.y);
	}
	friend Vector2 operator/(double left, Vector2 right) {
		return Vector2(left / right.x, left / right.y);
	}

	Vector2 &operator+=(Vector2 right) {
		x += right.x, y += right.y;
		return *this;
	}
	Vector2 &operator-=(Vector2 right) {
		x -= right.x, y -= right.y;
		return *this;
	}
	Vector2 &operator*=(Vector2 right) {
		x *= right.x, y *= right.y;
		return *this;
	}
	Vector2 &operator/=(Vector2 right) {
		x /= right.x, y /= right.y;
		return *this;
	}
	Vector2 &operator*=(double right) {
		x *= right, y *= right;
		return *this;
	}
	Vector2 &operator/=(double right) {
		x /= right, y /= right;
		return *this;
	}

	double x = 0.0, y = 0.0;
};

constexpr double InfinateDistance = -1e240;
class SignedDistance {
public:
	SignedDistance() = default;
	SignedDistance(double dist, double d) : distance(dist), dot(d) {}

	friend bool operator<(const SignedDistance &left, const SignedDistance &right) {
		return std::fabs(left.distance) < std::fabs(right.distance) || (std::fabs(left.distance) == std::fabs(right.distance) && left.dot < right.dot);
	}

	double distance = InfinateDistance;
	double dot = 1;
};

class Bounds {
public:
	void PointBounds(Vector2 p) {
		if (p.x < l) l = p.x;
		if (p.y < b) b = p.y;
		if (p.x > r) r = p.x;
		if (p.y > t) t = p.y;
	}

	double l, b, r, t;
};

/// Edge color specifies which color channels an edge belongs to.
enum EdgeColor {
	BLACK = 0,
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	MAGENTA = 5,
	CYAN = 6,
	WHITE = 7
};
enum EdgeType {
	LINEAR = 0,
	QUADRATIC = 1,
	CUBIC = 2
};

constexpr double TOO_LARGE_RATIO = 1e12;
// Parameters for iterative search of closest point on a cubic Bezier curve. Increase for higher precision.
constexpr int MSDFGEN_CUBIC_SEARCH_STARTS = 4;
constexpr int MSDFGEN_CUBIC_SEARCH_STEPS = 4;

int SolveQuadratic(double x[2], double a, double b, double c) {
	// a = 0 -> linear equation
	if (a == 0 || std::fabs(b) + std::fabs(c) > TOO_LARGE_RATIO * std::fabs(a)) {
		// a, b = 0 -> no solution
		if (b == 0 || std::fabs(c) > TOO_LARGE_RATIO * std::fabs(b)) {
			if (c == 0)
				return -1; // 0 = 0
			return 0;
		}
		x[0] = -c / b;
		return 1;
	}
	double dscr = b * b - 4 * a * c;
	if (dscr > 0) {
		dscr = std::sqrt(dscr);
		x[0] = (-b + dscr) / (2 * a);
		x[1] = (-b - dscr) / (2 * a);
		return 2;
	} else if (dscr == 0) {
		x[0] = -b / (2 * a);
		return 1;
	} else
		return 0;
}

int SolveCubicNormed(double x[3], double a, double b, double c) {
	double a2 = a * a;
	double q = (a2 - 3 * b) / 9;
	double r = (a * (2 * a2 - 9 * b) + 27 * c) / 54;
	double r2 = r * r;
	double q3 = q * q * q;
	double A, B;
	if (r2 < q3) {
		double t = r / std::sqrt(q3);
		if (t < -1) t = -1;
		if (t > 1) t = 1;
		t = std::acos(t);
		a /= 3; q = -2 * std::sqrt(q);
		x[0] = q * std::cos(t / 3) - a;
		x[1] = q * std::cos((t + 2 * M_PI) / 3) - a;
		x[2] = q * std::cos((t - 2 * M_PI) / 3) - a;
		return 3;
	} else {
		A = -std::pow(std::fabs(r) + std::sqrt(r2 - q3), 1 / 3.0);
		if (r < 0) A = -A;
		B = A == 0 ? 0 : q / A;
		a /= 3;
		x[0] = (A + B) - a;
		x[1] = -0.5 * (A + B) - a;
		x[2] = 0.5 * std::sqrt(3.0) * (A - B);
		if (std::fabs(x[2]) < 1e-14)
			return 2;
		return 1;
	}
}

int SolveCubic(double x[3], double a, double b, double c, double d) {
	if (a != 0) {
		double bn = b / a, cn = c / a, dn = d / a;
		// Check that a isn't "almost zero"
		if (std::fabs(bn) < TOO_LARGE_RATIO && std::fabs(cn) < TOO_LARGE_RATIO && std::fabs(dn) < TOO_LARGE_RATIO)
			return SolveCubicNormed(x, bn, cn, dn);
	}
	return SolveQuadratic(x, b, c, d);
}

/// An abstract edge segment.
class EdgeSegment {
public:
	EdgeSegment() = default;
	/// A line segment.
	EdgeSegment(Vector2 p0, Vector2 p1, EdgeColor color = WHITE) :
		color(color), type(LINEAR) {
		p[0] = p0;
		p[1] = p1;
	}
	/// A quadratic Bezier curve.
	EdgeSegment(Vector2 p0, Vector2 p1, Vector2 p2, EdgeColor color = WHITE) :
		color(color), type(QUADRATIC) {
		if (p1 == p0 || p1 == p2)
			p1 = 0.5 * (p0 + p2);
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
	}
	/// A cubic Bezier curve.
	EdgeSegment(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, EdgeColor color = WHITE) :
		color(color), type(CUBIC) {
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		p[3] = p3;
	}

	/// Returns the point on the edge specified by the parameter (between 0 and 1).
	Vector2 Point(double param) const {
		switch (type) {
		case LINEAR: {
			return Mix(p[0], p[1], param);
		}
		case QUADRATIC: {
			return Mix(Mix(p[0], p[1], param), Mix(p[1], p[2], param), param);
		}
		case CUBIC: {
			Vector2 p12 = Mix(p[1], p[2], param);
			return Mix(Mix(Mix(p[0], p[1], param), p12, param), Mix(p12, Mix(p[2], p[3], param), param), param);
		}
		}
	}
	/// Returns the direction the edge has at the point specified by the parameter.
	Vector2 Direction(double param) const {
		switch (type) {
		case LINEAR: {
			return p[1] - p[0];
		}
		case QUADRATIC: {
			Vector2 tangent = Mix(p[1] - p[0], p[2] - p[1], param);
			if (!tangent.x && !tangent.y)
				return p[2] - p[0];
			return tangent;
		}
		case CUBIC: {
			Vector2 tangent = Mix(Mix(p[1] - p[0], p[2] - p[1], param), Mix(p[2] - p[1], p[3] - p[2], param), param);
			if (!tangent.x && !tangent.y) {
				if (param == 0) return p[2] - p[0];
				if (param == 1) return p[3] - p[1];
			}
			return tangent;
		}
		}
	}
	/// Returns the minimum signed distance between origin and the edge.
	SignedDistance MinSignedDistance(Vector2 origin, double &param) const {
		switch (type) {
		case LINEAR: {
			Vector2 aq = origin - p[0];
			Vector2 ab = p[1] - p[0];
			param = aq.Dot(ab) / ab.Dot(ab);
			Vector2 eq = p[param > 0.5] - origin;
			double endpointDistance = eq.Length();
			if (param > 0 && param < 1) {
				double orthoDistance = ab.Orthonormal().Dot(aq);
				if (std::fabs(orthoDistance) < endpointDistance)
					return SignedDistance(orthoDistance, 0);
			}
			return SignedDistance(NonZeroSign(aq.Cross(ab)) * endpointDistance, std::fabs(ab.Normalize().Dot(eq.Normalize())));
		}
		case QUADRATIC: {
			Vector2 qa = p[0] - origin;
			Vector2 ab = p[1] - p[0];
			Vector2 br = p[2] - p[1] - ab;
			double a = br.Dot(br);
			double b = 3 * ab.Dot(br);
			double c = 2 * ab.Dot(ab) + qa.Dot(br);
			double d = qa.Dot(ab);
			double t[3];
			int solutions = SolveCubic(t, a, b, c, d);

			Vector2 epDir = Direction(0);
			double minDistance = NonZeroSign(epDir.Cross(qa)) * qa.Length(); // distance from A
			param = -qa.Dot(epDir) / epDir.Dot(epDir);
			epDir = Direction(1);
			double distance = NonZeroSign(epDir.Cross(p[2] - origin)) * (p[2] - origin).Length(); // distance from B
			if (std::fabs(distance) < std::fabs(minDistance)) {
				minDistance = distance;
				param = (origin - p[1]).Dot(epDir) / epDir.Dot(epDir);
			}
			for (int i = 0; i < solutions; ++i) {
				if (t[i] > 0 && t[i] < 1) {
					Vector2 qe = p[0] + 2 * t[i] * ab + t[i] * t[i] * br - origin;
					double distance = NonZeroSign((p[2] - p[0]).Cross(qe)) * qe.Length();
					if (std::fabs(distance) <= std::fabs(minDistance)) {
						minDistance = distance;
						param = t[i];
					}
				}
			}

			if (param >= 0 && param <= 1)
				return SignedDistance(minDistance, 0);
			if (param < 0.5)
				return SignedDistance(minDistance, std::fabs(Direction(0).Normalize().Dot(qa.Normalize())));
			else
				return SignedDistance(minDistance, std::fabs(Direction(1).Normalize().Dot((p[2] - origin).Normalize())));
		}
		case CUBIC: {
			Vector2 qa = p[0] - origin;
			Vector2 ab = p[1] - p[0];
			Vector2 br = p[2] - p[1] - ab;
			Vector2 as = (p[3] - p[2]) - (p[2] - p[1]) - br;

			Vector2 epDir = Direction(0);
			double minDistance = NonZeroSign(epDir.Cross(qa)) * qa.Length(); // distance from A
			param = -qa.Dot(epDir) / epDir.Dot(epDir);
			{
				epDir = Direction(1);
				double distance = NonZeroSign(epDir.Cross(p[3] - origin)) * (p[3] - origin).Length(); // distance from B
				if (fabs(distance) < fabs(minDistance)) {
					minDistance = distance;
					param = (epDir - (p[3] - origin)).Dot(epDir) / epDir.Dot(epDir);
				}
			}
			// Iterative minimum distance search
			for (int i = 0; i <= MSDFGEN_CUBIC_SEARCH_STARTS; ++i) {
				double t = (double)i / MSDFGEN_CUBIC_SEARCH_STARTS;
				for (int step = 0;; ++step) {
					Vector2 qe = p[0] + 3 * t * ab + 3 * t * t * br + t * t * t * as - origin; // do not simplify with qa !!!
					double distance = NonZeroSign(Direction(t).Cross(qe)) * qe.Length();
					if (fabs(distance) < fabs(minDistance)) {
						minDistance = distance;
						param = t;
					}
					if (step == MSDFGEN_CUBIC_SEARCH_STEPS)
						break;
					// Improve t
					Vector2 d1 = 3 * as * t * t + 6 * br * t + 3 * ab;
					Vector2 d2 = 6 * as * t + 6 * br;
					t -= qe.Dot(d1) / (d1.Dot(d1) + qe.Dot(d2));
					if (t < 0 || t > 1)
						break;
				}
			}

			if (param >= 0 && param <= 1)
				return SignedDistance(minDistance, 0);
			if (param < 0.5)
				return SignedDistance(minDistance, std::fabs(Direction(0).Normalize().Dot(qa.Normalize())));
			else
				return SignedDistance(minDistance, std::fabs(Direction(1).Normalize().Dot((p[3] - origin).Normalize())));
		}
		}
	}
	/// Converts a previously retrieved signed distance from origin to pseudo-distance.
	void DistanceToPseudoDistance(SignedDistance &distance, Vector2 origin, double param) const {
		if (param < 0) {
			Vector2 dir = Direction(0).Normalize();
			Vector2 aq = origin - Point(0);
			double ts = aq.Dot(dir);
			if (ts < 0) {
				double pseudoDistance = aq.Cross(dir);
				if (std::fabs(pseudoDistance) <= std::fabs(distance.distance)) {
					distance.distance = pseudoDistance;
					distance.dot = 0;
				}
			}
		} else if (param > 1) {
			Vector2 dir = Direction(1).Normalize();
			Vector2 bq = origin - Point(1);
			double ts = bq.Dot(dir);
			if (ts > 0) {
				double pseudoDistance = bq.Cross(dir);
				if (std::fabs(pseudoDistance) <= std::fabs(distance.distance)) {
					distance.distance = pseudoDistance;
					distance.dot = 0;
				}
			}
		}
	}
	/// Adjusts the bounding box to fit the edge segment.
	void Bound(Bounds &bounds) const {
		switch (type) {
		case LINEAR: {
			bounds.PointBounds(p[0]);
			bounds.PointBounds(p[1]);
		}
		case QUADRATIC: {
			bounds.PointBounds(p[0]);
			bounds.PointBounds(p[2]);
			Vector2 bot = (p[1] - p[0]) - (p[2] - p[1]);
			if (bot.x) {
				double param = (p[1].x - p[0].x) / bot.x;
				if (param > 0 && param < 1)
					bounds.PointBounds(Point(param));
			}
			if (bot.y) {
				double param = (p[1].y - p[0].y) / bot.y;
				if (param > 0 && param < 1)
					bounds.PointBounds(Point(param));
			}
		}
		case CUBIC: {
			bounds.PointBounds(p[0]);
			bounds.PointBounds(p[3]);
			Vector2 a0 = p[1] - p[0];
			Vector2 a1 = 2 * (p[2] - p[1] - a0);
			Vector2 a2 = p[3] - 3 * p[2] + 3 * p[1] - p[0];
			double params[2];
			int solutions;
			solutions = SolveQuadratic(params, a2.x, a1.x, a0.x);
			for (int i = 0; i < solutions; ++i)
				if (params[i] > 0 && params[i] < 1)
					bounds.PointBounds(Point(params[i]));
			solutions = SolveQuadratic(params, a2.y, a1.y, a0.y);
			for (int i = 0; i < solutions; ++i)
				if (params[i] > 0 && params[i] < 1)
					bounds.PointBounds(Point(params[i]));
		}
		}
	}
	/// Splits the edge segments into thirds which together represent the original edge.
	void SplitInThirds(EdgeSegment &part1, EdgeSegment &part2, EdgeSegment &part3) const {
		switch (type) {
		case LINEAR: {
			part1 = EdgeSegment(p[0], Point(1 / 3.0), color);
			part2 = EdgeSegment(Point(1 / 3.0), Point(2 / 3.0), color);
			part3 = EdgeSegment(Point(2 / 3.0), p[1], color);
		}
		case QUADRATIC: {
			part1 = EdgeSegment(p[0], Mix(p[0], p[1], 1 / 3.0), Point(1 / 3.0), color);
			part2 = EdgeSegment(Point(1 / 3.0), Mix(Mix(p[0], p[1], 5 / 9.0), Mix(p[1], p[2], 4 / 9.0), 0.5), Point(2 / 3.0), color);
			part3 = EdgeSegment(Point(2 / 3.0), Mix(p[1], p[2], 2 / 3.0), p[2], color);
		}
		case CUBIC: {
			part1 = EdgeSegment(p[0], p[0] == p[1] ? p[0] : Mix(p[0], p[1], 1 / 3.0), Mix(Mix(p[0], p[1], 1 / 3.0), Mix(p[1], p[2], 1 / 3.0), 1 / 3.0), Point(1 / 3.0), color);
			part2 = EdgeSegment(Point(1 / 3.0),
				Mix(Mix(Mix(p[0], p[1], 1 / 3.0), Mix(p[1], p[2], 1 / 3.0), 1 / 3.0), Mix(Mix(p[1], p[2], 1 / 3.0), Mix(p[2], p[3], 1 / 3.0), 1 / 3.0), 2 / 3.0),
				Mix(Mix(Mix(p[0], p[1], 2 / 3.0), Mix(p[1], p[2], 2 / 3.0), 2 / 3.0), Mix(Mix(p[1], p[2], 2 / 3.0), Mix(p[2], p[3], 2 / 3.0), 2 / 3.0), 1 / 3.0),
				Point(2 / 3.0), color);
			part3 = EdgeSegment(Point(2 / 3.0), Mix(Mix(p[1], p[2], 2 / 3.0), Mix(p[2], p[3], 2 / 3.0), 2 / 3.0), p[2] == p[3] ? p[3] : Mix(p[2], p[3], 2 / 3.0), p[3], color);
		}
		}
	}

	EdgeColor color = WHITE;
	EdgeType type;
	Vector2 p[4];
};

double Shoelace(Vector2 a, Vector2 b) {
	return (b.x - a.x) * (a.y + b.y);
}

/// A single closed contour of a shape.
class Contour {
public:
	/// Adds an edge to the contour.
	EdgeSegment &AddEdge(EdgeSegment &&edge) {
		return edges.emplace_back(std::move(edge));
	}
	/// Adjusts the bounding box to fit the contour.
	void Bound(Bounds &bounds) const {
		for (const auto &edge : edges)
			edge.Bound(bounds);
	}
	/// Adjusts the bounding box to fit the contour border's mitered corners.
	void BoundMiters(Bounds &bounds, double border, double miterLimit, int polarity) const {
		if (edges.empty())
			return;
		Vector2 prevDir = edges.back().Direction(1).Normalize();
		for (const auto &edge : edges) {
			Vector2 dir = -edge.Direction(0).Normalize();
			if (polarity * prevDir.Cross(dir) >= 0) {
				double miterLength = miterLimit;
				double q = 0.5 * (1.0 - prevDir.Dot(dir));
				if (q > 0)
					miterLength = std::min(1 / sqrt(q), miterLimit);
				auto miter = edge.Point(0) + border * miterLength * (prevDir + dir).Normalize();
				bounds.PointBounds(miter);
			}
			prevDir = edge.Direction(1).Normalize();
		}
	}
	/// Computes the winding of the contour. Returns 1 if positive, -1 if negative.
	int Winding() const {
		if (edges.empty())
			return 0;
		double total = 0;
		if (edges.size() == 1) {
			Vector2 a = edges[0].Point(0), b = edges[0].Point(1 / 3.0), c = edges[0].Point(2 / 3.0);
			total += Shoelace(a, b);
			total += Shoelace(b, c);
			total += Shoelace(c, a);
		} else if (edges.size() == 2) {
			Vector2 a = edges[0].Point(0), b = edges[0].Point(0.5), c = edges[1].Point(0), d = edges[1].Point(0.5);
			total += Shoelace(a, b);
			total += Shoelace(b, c);
			total += Shoelace(c, d);
			total += Shoelace(d, a);
		} else {
			Vector2 prev = edges.back().Point(0);
			for (const auto &edge : edges) {
				Vector2 cur = edge.Point(0);
				total += Shoelace(prev, cur);
				prev = cur;
			}
		}
		return Sign(total);
	}

	/// The sequence of edges that make up the contour.
	std::vector<EdgeSegment> edges;
};

/// Vector shape representation.
class Shape {
public:
	/// Adds a contour.
	Contour &AddContour() {
		return contours.emplace_back();
	}
	/// Performs basic checks to determine if the object represents a valid shape.
	bool Validate() const {
		for (const auto &contour : contours) {
			if (!contour.edges.empty()) {
				auto corner = contour.edges.back().Point(1);
				for (const auto &edge : contour.edges) {
					//if (!edge)
					//	return false;
					if (edge.Point(0) != corner)
						return false;
					corner = edge.Point(1);
				}
			}
		}
		return true;
	}
	/// Normalizes the shape geometry for distance field generation.
	void Normalize() {
		for (auto &contour : contours) {
			if (contour.edges.size() == 1) {
				EdgeSegment parts[3];
				contour.edges[0].SplitInThirds(parts[0], parts[1], parts[2]);
				contour.edges.clear();
				contour.edges.emplace_back(std::move(parts[0]));
				contour.edges.emplace_back(std::move(parts[1]));
				contour.edges.emplace_back(std::move(parts[2]));
			}
		}
	}
	/// Adjusts the bounding box to fit the shape.
	void Bound(Bounds &bounds) const {
		for (auto &contour : contours)
			contour.Bound(bounds);
	}
	/// Adjusts the bounding box to fit the shape border's mitered corners.
	void BoundMiters(Bounds &bounds, double border, double miterLimit, int polarity) const {
		for (auto &contour : contours)
			contour.BoundMiters(bounds, border, miterLimit, polarity);
	}
	/// Computes the minimum bounding box that fits the shape, optionally with a (mitered) border.
	Bounds GetBounds(double border = 0, double miterLimit = 0, int polarity = 0) const {
		static constexpr double LARGE_VALUE = 1e240;
		Bounds bounds = {+LARGE_VALUE, +LARGE_VALUE, -LARGE_VALUE, -LARGE_VALUE};
		Bound(bounds);
		if (border > 0) {
			bounds.l -= border, bounds.b -= border;
			bounds.r += border, bounds.t += border;
			if (miterLimit > 0)
				BoundMiters(bounds, border, miterLimit, polarity);
		}
		return bounds;
	}

	/// The list of contours the shape consists of.
	std::vector<Contour> contours;
	/// Specifies whether the shape uses bottom-to-top (false) or top-to-bottom (true) Y coordinates.
	bool inverseYAxis = false;
};

/// Loads the geometry of a glyph from a font file.
Shape LoadGlyph(FT_Face face, unicode_t unicode) {
	FT_Error error = FT_Load_Char(face, unicode, FT_LOAD_NO_SCALE);
	if (error)
		throw FtException("Unable to load character");

	Shape shape;

	struct FtContext {
		Shape *shape;
		Contour *contour = nullptr;
		Vector2 position;
	} context;
	context.shape = &shape;

	FT_Outline_Funcs ftFunctions = {};
	ftFunctions.move_to = [](const FT_Vector *to, void *user) {
		auto context = reinterpret_cast<FtContext *>(user);
		if (!(context->contour && context->contour->edges.empty()))
			context->contour = &context->shape->AddContour();
		context->position = *to;
		return 0;
	};
	ftFunctions.line_to = [](const FT_Vector *to, void *user) {
		auto context = reinterpret_cast<FtContext *>(user);
		Vector2 endpoint = *to;
		if (endpoint != context->position) {
			context->contour->AddEdge(EdgeSegment(context->position, endpoint));
			context->position = endpoint;
		}
		return 0;
	};
	ftFunctions.conic_to = [](const FT_Vector *control, const FT_Vector *to, void *user) {
		auto context = reinterpret_cast<FtContext *>(user);
		context->contour->AddEdge(EdgeSegment(context->position, *control, *to));
		context->position = *to;
		return 0;
	};
	ftFunctions.cubic_to = [](const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
		auto context = reinterpret_cast<FtContext *>(user);
		context->contour->AddEdge(EdgeSegment(context->position, *control1, *control2, *to));
		context->position = *to;
		return 0;
	};

	error = FT_Outline_Decompose(&face->glyph->outline, &ftFunctions, &context);
	if (error)
		throw FtException("Unable to decompose glyph");

	if (!shape.contours.empty() && shape.contours.back().edges.empty())
		shape.contours.pop_back();
	return shape;
}

/// Automatically scales and translates the shape to fit.
void AutoFrame(const Shape &shape, int width, int height, double pxRange, Vector2 &translate, Vector2 &scale) {
	auto bounds = shape.GetBounds();

	double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
	Vector2 frame(width - pxRange, height - pxRange);
	if (l >= r || b >= t)
		l = 0, b = 0, r = 1, t = 1;
	if (frame.x <= 0 || frame.y <= 0)
		throw MsdfException("Cannot fit the specified pixel range");
	Vector2 dims(r - l, t - b);
	if (dims.x *frame.y < dims.y *frame.x) {
		translate = Vector2(0.5 * (frame.x / frame.y * dims.y - dims.x) - l, -b);
		scale = frame.y / dims.y;
	} else {
		translate = Vector2(-l, 0.5 * (frame.y / frame.x * dims.x - dims.y) - b);
		scale = frame.x / dims.x;
	}
	translate += 0.5 * pxRange / scale;
}

bool IsCorner(Vector2 aDir, Vector2 bDir, double crossThreshold) {
	return aDir.Dot(bDir) <= 0 || std::fabs(aDir.Cross(bDir)) > crossThreshold;
}
void SwitchColor(EdgeColor &color, uint64_t &seed, EdgeColor banned = BLACK) {
	EdgeColor combined = EdgeColor(color & banned);
	if (combined == RED || combined == GREEN || combined == BLUE) {
		color = EdgeColor(combined ^ WHITE);
		return;
	}
	if (color == BLACK || color == WHITE) {
		static const EdgeColor start[3] = {CYAN, MAGENTA, YELLOW};
		color = start[seed % 3];
		seed /= 3;
		return;
	}
	int shifted = color << (1 + (seed & 1));
	color = EdgeColor((shifted | shifted >> 3) & WHITE);
	seed >>= 1;
}
void EdgeColoringSimple(Shape &shape, double angleThreshold, uint64_t seed) {
	double crossThreshold = std::sin(angleThreshold);
	std::vector<int> corners;
	for (auto &contour : shape.contours) {
		// Identify corners
		corners.clear();
		if (!contour.edges.empty()) {
			Vector2 prevDirection = contour.edges.back().Direction(1);
			int index = 0;
			for (const auto &edge : contour.edges) {
				if (IsCorner(prevDirection.Normalize(), edge.Direction(0).Normalize(), crossThreshold))
					corners.emplace_back(index);
				prevDirection = edge.Direction(1);
				++index;
			}
		}

		// Smooth contour
		if (corners.empty()) {
			for (auto &edge : contour.edges)
				edge.color = WHITE;
		} else if (corners.size() == 1) { // "Teardrop" case
			EdgeColor colors[3] = {WHITE, WHITE};
			SwitchColor(colors[0], seed);
			SwitchColor(colors[2] = colors[0], seed);
			int corner = corners[0];
			if (contour.edges.size() >= 3) {
				int m = (int)contour.edges.size();
				for (int i = 0; i < m; ++i)
					contour.edges[(corner + i) % m].color = (colors + 1)[int(3 + 2.875 * i / (m - 1) - 1.4375 + 0.5) - 3];
			} else if (contour.edges.size() >= 1) {
				// Less than three edge segments for three colors => edges must be split
				std::optional<EdgeSegment> parts[7];
				contour.edges[0].SplitInThirds(*parts[0 + 3 * corner], *parts[1 + 3 * corner], *parts[2 + 3 * corner]);
				if (contour.edges.size() >= 2) {
					contour.edges[1].SplitInThirds(*parts[3 - 3 * corner], *parts[4 - 3 * corner], *parts[5 - 3 * corner]);
					parts[0]->color = parts[1]->color = colors[0];
					parts[2]->color = parts[3]->color = colors[1];
					parts[4]->color = parts[5]->color = colors[2];
				} else {
					parts[0]->color = colors[0];
					parts[1]->color = colors[1];
					parts[2]->color = colors[2];
				}
				contour.edges.clear();
				for (int i = 0; parts[i]; ++i)
					contour.edges.emplace_back(std::move(*parts[i]));
			}
		} else { // Multiple corners
			int cornerCount = (int)corners.size();
			int spline = 0;
			int start = corners[0];
			int m = (int)contour.edges.size();
			EdgeColor color = WHITE;
			SwitchColor(color, seed);
			EdgeColor initialColor = color;
			for (int i = 0; i < m; ++i) {
				int index = (start + i) % m;
				if (spline + 1 < cornerCount && corners[spline + 1] == index) {
					++spline;
					SwitchColor(color, seed, EdgeColor((spline == cornerCount - 1) * (int)initialColor));
				}
				contour.edges[index].color = color;
			}
		}
	}
}

void GenerateDistanceFieldSDF(Bitmap<float, 1> &output, const Shape &shape, double range, Vector2 scale, Vector2 translate) {
	for (int y = 0; y < output.height; ++y) {
		int row = shape.inverseYAxis ? output.height - y - 1 : y;
		for (int x = 0; x < output.width; ++x) {
			double dummy;
			Vector2 p = Vector2(x + 0.5, y + 0.5) / scale - translate;
			SignedDistance minDistance;
			for (const auto &contour : shape.contours) {
				for (const auto &edge : contour.edges) {
					SignedDistance distance = edge.MinSignedDistance(p, dummy);
					if (distance < minDistance)
						minDistance = distance;
				}
			}
			*output(x, row) = float(minDistance.distance / range + 0.5);
		}
	}
}
void GenerateDistanceFieldPseudoSDF(Bitmap<float, 1> &output, const Shape &shape, double range, Vector2 scale, Vector2 translate) {
	for (int y = 0; y < output.height; ++y) {
		int row = shape.inverseYAxis ? output.height - y - 1 : y;
		for (int x = 0; x < output.width; ++x) {
			Vector2 p = Vector2(x + 0.5, y + 0.5) / scale - translate;
			SignedDistance minDistance;
			const EdgeSegment *nearEdge = nullptr;
			double nearParam = 0;
			for (const auto &contour : shape.contours) {
				for (const auto &edge : contour.edges) {
					double param;
					SignedDistance distance = edge.MinSignedDistance(p, param);
					if (distance < minDistance) {
						minDistance = distance;
						nearEdge = &edge;
						nearParam = param;
					}
				}
			}
			if (nearEdge)
				nearEdge->DistanceToPseudoDistance(minDistance, p, nearParam);
			*output(x, row) = float(minDistance.distance / range + 0.5);
		}
	}
}
void GenerateDistanceFieldMSDF(Bitmap<float, 3> &output, const Shape &shape, double range, Vector2 scale, Vector2 translate) {
	struct EdgePoint {
		SignedDistance minDistance;
		const EdgeSegment *nearEdge = nullptr;
		double nearParam = 0.0;
	};
	
	for (int y = 0; y < output.height; ++y) {
		int row = shape.inverseYAxis ? output.height - y - 1 : y;
		for (int x = 0; x < output.width; ++x) {
			Vector2 p = Vector2(x + 0.5, y + 0.5) / scale - translate;
			EdgePoint r, g, b;

			for (const auto &contour : shape.contours) {
				for (const auto &edge : contour.edges) {
					double param;
					SignedDistance distance = edge.MinSignedDistance(p, param);
					if (edge.color & RED && distance < r.minDistance) {
						r.minDistance = distance;
						r.nearEdge = &edge;
						r.nearParam = param;
					}
					if (edge.color & GREEN && distance < g.minDistance) {
						g.minDistance = distance;
						g.nearEdge = &edge;
						g.nearParam = param;
					}
					if (edge.color & BLUE && distance < b.minDistance) {
						b.minDistance = distance;
						b.nearEdge = &edge;
						b.nearParam = param;
					}
				}
			}

			if (r.nearEdge)
				r.nearEdge->DistanceToPseudoDistance(r.minDistance, p, r.nearParam);
			if (g.nearEdge)
				g.nearEdge->DistanceToPseudoDistance(g.minDistance, p, g.nearParam);
			if (b.nearEdge)
				b.nearEdge->DistanceToPseudoDistance(b.minDistance, p, b.nearParam);
			output(x, row)[0] = float(r.minDistance.distance / range + 0.5);
			output(x, row)[1] = float(g.minDistance.distance / range + 0.5);
			output(x, row)[2] = float(b.minDistance.distance / range + 0.5);
		}
	}
}
void GenerateDistanceFieldMTSDF(Bitmap<float, 4> &output, const Shape &shape, double range, Vector2 scale, Vector2 translate) {
	//std::vector<int> windings;
	//windings.reserve(shape.contours.size());
	//for (const auto &contour : shape.contours)
	//	windings.emplace_back(contour.Winding());
	
	struct EdgePoint {
		SignedDistance minDistance;
		const EdgeSegment *nearEdge = nullptr;
		double nearParam = 0.0;
	};
	
	for (int y = 0; y < output.height; ++y) {
		int row = shape.inverseYAxis ? output.height - y - 1 : y;
		for (int x = 0; x < output.width; ++x) {
			Vector2 p = Vector2(x + 0.5, y + 0.5) / scale - translate;
			SignedDistance minDistance;
			EdgePoint r, g, b;

			for (const auto &contour : shape.contours) {
				for (const auto &edge : contour.edges) {
					double param;
					SignedDistance distance = edge.MinSignedDistance(p, param);
					if (distance < minDistance)
						minDistance = distance;
					if (edge.color & RED && distance < r.minDistance) {
						r.minDistance = distance;
						r.nearEdge = &edge;
						r.nearParam = param;
					}
					if (edge.color & GREEN && distance < g.minDistance) {
						g.minDistance = distance;
						g.nearEdge = &edge;
						g.nearParam = param;
					}
					if (edge.color & BLUE && distance < b.minDistance) {
						b.minDistance = distance;
						b.nearEdge = &edge;
						b.nearParam = param;
					}
				}
			}

			if (r.nearEdge)
				r.nearEdge->DistanceToPseudoDistance(r.minDistance, p, r.nearParam);
			if (g.nearEdge)
				g.nearEdge->DistanceToPseudoDistance(g.minDistance, p, g.nearParam);
			if (b.nearEdge)
				b.nearEdge->DistanceToPseudoDistance(b.minDistance, p, b.nearParam);
			output(x, row)[0] = float(r.minDistance.distance / range + 0.5);
			output(x, row)[1] = float(g.minDistance.distance / range + 0.5);
			output(x, row)[2] = float(b.minDistance.distance / range + 0.5);
			output(x, row)[3] = float(minDistance.distance / range + 0.5);
		}
	}
}

bool DetectClash(const float *a, const float *b, double threshold) {
	// Sort channels so that pairs (a0, b0), (a1, b1), (a2, b2) go from biggest to smallest absolute difference
	float a0 = a[0], a1 = a[1], a2 = a[2];
	float b0 = b[0], b1 = b[1], b2 = b[2];
	float tmp;
	if (std::fabs(b0 - a0) < std::fabs(b1 - a1)) {
		tmp = a0, a0 = a1, a1 = tmp;
		tmp = b0, b0 = b1, b1 = tmp;
	}
	if (std::fabs(b1 - a1) < std::fabs(b2 - a2)) {
		tmp = a1, a1 = a2, a2 = tmp;
		tmp = b1, b1 = b2, b2 = tmp;
		if (std::fabs(b0 - a0) < std::fabs(b1 - a1)) {
			tmp = a0, a0 = a1, a1 = tmp;
			tmp = b0, b0 = b1, b1 = tmp;
		}
	}
	return (std::fabs(b1 - a1) >= threshold) &&
		!(b0 == b1 && b0 == b2) && // Ignore if other pixel has been equalized
		std::fabs(a2 - 0.5f) >= std::fabs(b2 - 0.5f); // Out of the pair, only flag the pixel farther from a shape edge
}
/// Resolves multi-channel signed distance field values that may cause interpolation artifacts.
template<int N>
void MsdfErrorCorrection(Bitmap<float, N> &output, Vector2 threshold) {
	std::vector<std::pair<int, int> > clashes;
	int w = output.width, h = output.height;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			if ((x > 0 && DetectClash(output(x, y), output(x - 1, y), threshold.x)) ||
				(x < w - 1 && DetectClash(output(x, y), output(x + 1, y), threshold.x)) ||
				(y > 0 && DetectClash(output(x, y), output(x, y - 1), threshold.y)) ||
				(y < h - 1 && DetectClash(output(x, y), output(x, y + 1), threshold.y))) {
				clashes.emplace_back(x, y);
			}
		}
	}
	for (const auto &clash : clashes) {
		float *pixel = output(clash.first, clash.second);
		float med = Median(pixel[0], pixel[1], pixel[2]);
		pixel[0] = med, pixel[1] = med, pixel[2] = med;
	}
	clashes.clear();
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			if ((x > 0 && y > 0 && DetectClash(output(x, y), output(x - 1, y - 1), threshold.x + threshold.y)) ||
				(x < w - 1 && y > 0 && DetectClash(output(x, y), output(x + 1, y - 1), threshold.x + threshold.y)) ||
				(x > 0 && y < h - 1 && DetectClash(output(x, y), output(x - 1, y + 1), threshold.x + threshold.y)) ||
				(x < w - 1 && y < h - 1 && DetectClash(output(x, y), output(x + 1, y + 1), threshold.x + threshold.y))) {
				clashes.emplace_back(x, y);
			}
		}
	}
	for (const auto &clash : clashes) {
		float *pixel = output(clash.first, clash.second);
		float med = Median(pixel[0], pixel[1], pixel[2]);
		pixel[0] = med, pixel[1] = med, pixel[2] = med;
	}
}

bool GenerateSDF(Bitmap<float, 1> &output, FT_Face face, unicode_t unicode) {
	auto shape = LoadGlyph(face, unicode);

	// Validate and normalize shape
	if (!shape.Validate())
		throw MsdfException("The geometry of the loaded shape is invalid");
	shape.Normalize();

	double pxRange = 2.0;
	Vector2 translate, scale;
	AutoFrame(shape, output.width, output.height, pxRange, translate, scale);
	double range = pxRange / std::min(scale.x, scale.y);

	double angleThreshold = 3;
	uint64_t coloringSeed = 0;
	EdgeColoringSimple(shape, angleThreshold, coloringSeed);

	GenerateDistanceFieldSDF(output, shape, range, scale, translate);

	return false;
}

bool GeneratePseudoSDF(Bitmap<float, 1> &output, FT_Face face, unicode_t unicode) {
	auto shape = LoadGlyph(face, unicode);

	// Validate and normalize shape
	if (!shape.Validate())
		throw MsdfException("The geometry of the loaded shape is invalid");
	shape.Normalize();

	double pxRange = 2.0;
	Vector2 translate, scale;
	AutoFrame(shape, output.width, output.height, pxRange, translate, scale);
	double range = pxRange / std::min(scale.x, scale.y);

	double angleThreshold = 3;
	uint64_t coloringSeed = 0;
	EdgeColoringSimple(shape, angleThreshold, coloringSeed);

	GenerateDistanceFieldPseudoSDF(output, shape, range, scale, translate);

	return false;
}

bool GenerateMSDF(Bitmap<float, 3> &output, FT_Face face, unicode_t unicode, double edgeThreshold) {
	auto shape = LoadGlyph(face, unicode);

	// Validate and normalize shape
	if (!shape.Validate())
		throw MsdfException("The geometry of the loaded shape is invalid");
	shape.Normalize();

	double pxRange = 2.0;
	Vector2 translate, scale;
	AutoFrame(shape, output.width, output.height, pxRange, translate, scale);
	double range = pxRange / std::min(scale.x, scale.y);

	double angleThreshold = 3;
	uint64_t coloringSeed = 0;
	EdgeColoringSimple(shape, angleThreshold, coloringSeed);

	GenerateDistanceFieldMSDF(output, shape, range, scale, translate);
	if (edgeThreshold > 0)
		MsdfErrorCorrection(output, edgeThreshold / (scale * range));

	return false;
}

bool GenerateMTSDF(Bitmap<float, 4> &output, FT_Face face, unicode_t unicode, double edgeThreshold) {
	auto shape = LoadGlyph(face, unicode);

	// Validate and normalize shape
	if (!shape.Validate())
		throw MsdfException("The geometry of the loaded shape is invalid");
	shape.Normalize();

	double pxRange = 2.0;
	Vector2 translate, scale;
	AutoFrame(shape, output.width, output.height, pxRange, translate, scale);
	double range = pxRange / std::min(scale.x, scale.y);

	double angleThreshold = 3;
	uint64_t coloringSeed = 0;
	EdgeColoringSimple(shape, angleThreshold, coloringSeed);

	GenerateDistanceFieldMTSDF(output, shape, range, scale, translate);
	if (edgeThreshold > 0)
		MsdfErrorCorrection(output, edgeThreshold / (scale * range));

	return false;
}
}
