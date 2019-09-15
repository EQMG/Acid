#include "Vector4.hpp"

namespace acid
{
template<> const Vector4f Vector4f::Zero(0.0f);
template<> const Vector4f Vector4f::One(1.0f);
template<> const Vector4f Vector4f::PositiveInfinity(+std::numeric_limits<float>::infinity());
template<> const Vector4f Vector4f::NegativeInfinity(-std::numeric_limits<float>::infinity());

template<> const Vector4d Vector4d::Zero(0.0);
template<> const Vector4d Vector4d::One(1.0);
template<> const Vector4d Vector4d::PositiveInfinity(+std::numeric_limits<double>::infinity());
template<> const Vector4d Vector4d::NegativeInfinity(-std::numeric_limits<double>::infinity());

template<> const Vector4i Vector4i::Zero(0);
template<> const Vector4i Vector4i::One(1);
template<> const Vector4i Vector4i::PositiveInfinity(+std::numeric_limits<int32_t>::infinity());
template<> const Vector4i Vector4i::NegativeInfinity(-std::numeric_limits<int32_t>::infinity());

template<> const Vector4ui Vector4ui::Zero(0);
template<> const Vector4ui Vector4ui::One(1);
}
