#pragma once

#include <cassert>
#include <cstdint>

#include "Export.hpp"

namespace acid {
template<typename T, std::size_t N>
class VectorBase {
protected:
	constexpr VectorBase() = default;
	template<typename ...Args, typename = std::enable_if_t<sizeof...(Args) == N>>
	constexpr VectorBase(Args... args) : data{args...} {}
	template<typename T2>
	constexpr VectorBase(const VectorBase<T2, N> &v) { std::copy(std::begin(v.data), std::end(v.data), std::begin(data)); }

public:
	template<typename T2>
	constexpr VectorBase &operator=(const VectorBase<T2, N> &v) {
		std::copy(std::begin(v.data), std::end(v.data), std::begin(data));
		return *this;
	}

	constexpr const T &operator[](std::size_t i) const { return data[i]; }
	constexpr T &operator[](std::size_t i) { return data[i]; }

	T data[N]{};
};

template<typename T>
class VectorBase<T, 1> {
protected:
	constexpr VectorBase() = default;
	template<typename T1>
	constexpr VectorBase(T x) : x(x) {}
	template<typename T2>
	constexpr VectorBase(const VectorBase<T2, 1> &v) : VectorBase(v.x) {}

public:
	template<typename T2>
	constexpr VectorBase &operator=(const VectorBase<T2, 1> &v) {
		x = v.x;
		return *this;
	}

	constexpr const T &operator[](std::size_t i) const {
		assert(i < 1 && "Vector subscript out of range");
		return x;
	}
	constexpr T &operator[](std::size_t i) {
		assert(i < 1 && "Vector subscript out of range");
		return x;
	}
	
	T x{};
};

template<typename T>
class VectorBase<T, 2> {
protected:
	constexpr VectorBase() = default;
	template<typename T1, typename T2>
	constexpr VectorBase(T1 x, T2 y) : x(x), y(y) {}
	template<typename T1>
	constexpr VectorBase(const VectorBase<T1, 2> &v) : VectorBase(v.x, v.y) {}
	
public:
	template<typename T1>
	constexpr VectorBase &operator=(const VectorBase<T1, 2> &v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	constexpr const T &operator[](std::size_t i) const {
		assert(i < 2 && "Vector subscript out of range");
		return i == 0 ? x : y;
	}
	constexpr T &operator[](std::size_t i) {
		assert(i < 2 && "Vector subscript out of range");
		return i == 0 ? x : y;
	}
	
	T x{}, y{};
};

template<typename T>
class VectorBase<T, 3> {
protected:
	constexpr VectorBase() = default;
	template<typename T1, typename T2, typename T3>
	constexpr VectorBase(T1 x, T2 y, T3 z) : x(x), y(y), z(z) {}
	template<typename T2>
	constexpr VectorBase(const VectorBase<T2, 3> &v) : VectorBase(v.x, v.y, v.z) {}

public:
	template<typename T2>
	constexpr VectorBase &operator=(const VectorBase<T2, 3> & v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	constexpr const T &operator[](std::size_t i) const {
		assert(i < 3 && "Vector subscript out of range");
		return i == 0 ? x : i == 1 ? y : z;
	}
	constexpr T &operator[](std::size_t i) {
		assert(i < 3 && "Vector subscript out of range");
		return i == 0 ? x : i == 1 ? y : z;
	}

	T x{}, y{}, z{};
};

template<typename T>
class VectorBase<T, 4> {
protected:
	constexpr VectorBase() = default;
	template<typename T1, typename T2, typename T3, typename T4>
	constexpr VectorBase(T1 x, T2 y, T3 z, T4 w) : x(x), y(y), z(z), w(w) {}
	template<typename T2>
	constexpr VectorBase(const VectorBase<T2, 4> &v) : VectorBase(v.x, v.y, v.z, v.w) {}

public:
	template<typename T2>
	constexpr VectorBase &operator=(const VectorBase<T2, 4> &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	constexpr const T &operator[](std::size_t i) const {
		assert(i < 4 && "Vector subscript out of range");
		return i == 0 ? x : i == 1 ? y : i == 2 ? z : w;
	}
	constexpr T &operator[](std::size_t i) {
		assert(i < 4 && "Vector subscript out of range");
		return i == 0 ? x : i == 1 ? y : i == 2 ? z : w;
	}

	T x{}, y{}, z{}, w{};
};

template<typename T, std::size_t N>
class Vector : public VectorBase<T, N> {
public:
	constexpr Vector() = default;
	template<typename ...Args, typename = std::enable_if_t<sizeof...(Args) == N>>
	constexpr Vector(Args... args) : VectorBase(args...) {}
	constexpr explicit Vector(T s) : VectorBase(/*(static_cast<void>(N), s)...*/) {
		for (std::size_t i = 0; i < N; i++)
			at(i) = s;
	}
	//template<typename T2, std::size_t N2, typename... Args, typename = std::enable_if_t<N2 < N && sizeof...(Args) == (N - N2)>>
	//constexpr explicit Vector(const Vector<T2, N2> &v, Args... args) : VectorBase(v, args...) {}
	//template<typename T2, typename T3, typename = std::enable_if_t<N % 2 == 0>>
	//constexpr Vector(const Vector<T2, N / 2> &v1, const Vector<T3, N / 2> &v2) : VectorBase(v1, v2) {}
	template<typename T2>
	constexpr Vector(const Vector<T2, N> &v) : VectorBase(v) {}

	template<typename T2>
	constexpr Vector &operator=(const Vector<T2, N> &v) {
		VectorBase<T, N>::operator=(v);
		return *this;
	}

	constexpr auto size() const { return N; }

	auto begin() { return &at(0); }
	auto begin() const { return &at(0); }

	auto end() { return &at(0) + N; }
	auto end() const { return &at(0) + N; }

	constexpr const auto &at(std::size_t i) const { return (*this)[i]; }
	constexpr auto &at(std::size_t i) { return (*this)[i]; }

	constexpr T Dot(const Vector &other) const {
		T result = 0;
		for (std::size_t i = 0; i < N; i++)
			result += at(i) * other[i];
		return result;
	}

	constexpr T Length2() const {
		return Dot(*this);
	}
	auto Length() const {
		return std::sqrt(Length2());
	}

	auto Normalize() const {
		return *this / Length();
	}

	template<typename = std::enable_if_t<N == 2 || N == 3>>
	constexpr auto Cross(const Vector &other) const {
		if constexpr (N == 2) {
			return x * other.y - y * other.x;
		} else {
			return Vector(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}
	}

	constexpr T Distance2(const Vector &other) const {
		return (other - *this).Length2();
	}
	auto Distance(const Vector &other) const {
		return (other - *this).Length();
	}

	constexpr auto DistanceVector(const Vector &other) const {
		return (*this - other) * (*this - other);
	}
	
	T Uangle(const Vector &other) const {
		const T d = Dot(other);
		return d > 1 ? 0 : std::acos(d < -1 ? -1 : d);
	}
	T Angle(const Vector &other) const {
		return Normalize().Uangle(other.Normalize());
	}

	constexpr auto Lerp(const Vector &other, T c) const {
		return *this * (1 - c) + other * c;
	}
	constexpr T Nlerp(const Vector &other, T t) const {
		return Lerp(other, t).Normalize();
	}
	T Slerp(const Vector &other, T t) const {
		T th = Uangle(other);
		return th == 0 ? *this : *this * (std::sin(th * (1 - t)) / std::sin(th)) + other * (std::sin(th * t) / std::sin(th));
	}

	Vector Abs() const {
		Vector result;
		for (std::size_t i = 0; i < N; i++)
			result[i] = std::abs(at(i));
		return result;
	}
	constexpr auto Min() const {
		return std::min(std::initializer_list(begin(), end()));
	}
	constexpr auto Max() const {
		return std::max(std::initializer_list(begin(), end()));
	}
	constexpr auto MinMax() const {
		return std::minmax(std::initializer_list(begin(), end()));
	}

	template<typename T2>
	constexpr auto Min(const Vector<T2, N> &other) {
		using THighestP = decltype(at(0) + other[0]);
		Vector<THighestP, N> result;
		for (std::size_t i = 0; i < N; i++)
			result[i] = std::min<THighestP>(at(i), other[1]);
		return result;
	}
	template<typename T2>
	constexpr auto Max(const Vector<T2, N> &other) {
		using THighestP = decltype(at(0) + other[0]);
		Vector<THighestP, N> result;
		for (std::size_t i = 0; i < N; i++)
			result[i] = std::max<THighestP>(at(i), other[1]);
		return result;
	}

	template<typename = std::enable_if_t<N == 2>>
	Vector Rotate(T a) const {
		const T s = std::sin(a);
		const T c = std::cos(a);
		return {x * c - y * s, x * s + y * c};
	}

	template<typename = std::enable_if_t<N == 2>>
	constexpr bool InTriangle(const Vector &v1, const Vector &v2, const Vector &v3) const {
		auto b1 = ((x - v2.x) * (v1.y - v2.y) - (v1.x - v2.y) * (y - v2.y)) < 0;
		auto b2 = ((x - v3.x) * (v2.y - v3.y) - (v2.x - v3.y) * (y - v3.y)) < 0;
		auto b3 = ((x - v1.x) * (v3.y - v1.y) - (v3.x - v1.y) * (y - v1.y)) < 0;
		return ((b1 == b2) & (b2 == b3));
	}

	template<typename = std::enable_if_t<N == 2 || N == 3>>
	auto CartesianToPolar() const {
		if constexpr (N == 2) {
			auto radius = std::sqrt(x * x + y * y);
			auto theta = std::atan2(y, x);
			return Vector<decltype(radius), N>(radius, theta);
		} else {
			auto radius = std::sqrt(x * x + y * y + z * z);
			auto theta = std::atan2(y, x);
			auto phi = std::atan2(std::sqrt(x * x + y * y), z);
			return Vector<decltype(radius), N>(radius, theta, phi);
		}
	}
	template<typename = std::enable_if_t<N == 2 || N == 3>>
	auto PolarToCartesian() const {
		if constexpr (N == 2) {
			auto x1 = x * std::cos(y);
			auto y1 = x * std::sin(x);
			return Vector<decltype(x1), N>(x1, y1);
		} else {
			auto x1 = x * std::sin(z) * std::cos(y);
			auto y1 = x * std::sin(z) * std::sin(y);
			auto z1 = x * std::cos(z);
			return Vector<decltype(x1), N>(x1, y1, z1);
		}
	}

	template<typename = std::enable_if_t<N >= 2>>
	constexpr const Vector<T, 2> &xy() const { return *reinterpret_cast<const Vector<T, 2> *>(this); }
	template<typename = std::enable_if_t<N >= 2>>
	constexpr Vector<T, 2> &xy() { return *reinterpret_cast<Vector<T, 2> *>(this); }
	template<typename = std::enable_if_t<N >= 3>>
	constexpr const Vector<T, 3> &xyz() const { return *reinterpret_cast<const Vector<T, 3> *>(this); }
	template<typename = std::enable_if_t<N >= 3>>
	constexpr Vector<T, 3> &xyz() { return *reinterpret_cast<Vector<T, 3> *>(this); }

	ACID_EXPORT static const Vector Zero;
	ACID_EXPORT static const Vector One;
	ACID_EXPORT static const Vector Infinity;
	ACID_EXPORT static const Vector Right;
	ACID_EXPORT static const Vector Left;
	ACID_EXPORT static const Vector Up;
	ACID_EXPORT static const Vector Down;
	ACID_EXPORT static const Vector Front;
	ACID_EXPORT static const Vector Back;
};

using Vector1f = Vector<float, 1>;
using Vector1d = Vector<double, 1>;
using Vector1i = Vector<int32_t, 1>;
using Vector1ui = Vector<uint32_t, 1>;

using Vector2f = Vector<float, 2>;
using Vector2d = Vector<double, 2>;
using Vector2i = Vector<int32_t, 2>;
using Vector2ui = Vector<uint32_t, 2>;

using Vector3f = Vector<float, 3>;
using Vector3d = Vector<double, 3>;
using Vector3i = Vector<int32_t, 3>;
using Vector3ui = Vector<uint32_t, 3>;

using Vector4f = Vector<float, 4>;
using Vector4d = Vector<double, 4>;
using Vector4i = Vector<int32_t, 4>;
using Vector4ui = Vector<uint32_t, 4>;
}

#include "Vector.inl"
