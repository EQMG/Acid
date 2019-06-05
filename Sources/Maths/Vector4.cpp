#include "Vector4.hpp"

namespace acid
{
template<> const Vector4f Vector4f::Zero = Vector4f(0.0f);
template<> const Vector4f Vector4f::One = Vector4f(1.0f);
template<> const Vector4f Vector4f::PositiveInfinity = Vector4f(+std::numeric_limits<float>::infinity());
template<> const Vector4f Vector4f::NegativeInfinity = Vector4f(-std::numeric_limits<float>::infinity());

template<> const Vector4d Vector4d::Zero = Vector4d(0.0);
template<> const Vector4d Vector4d::One = Vector4d(1.0);
template<> const Vector4d Vector4d::PositiveInfinity = Vector4d(+std::numeric_limits<double>::infinity());
template<> const Vector4d Vector4d::NegativeInfinity = Vector4d(-std::numeric_limits<double>::infinity());

template<> const Vector4i Vector4i::Zero = Vector4i(0);
template<> const Vector4i Vector4i::One = Vector4i(1);
template<> const Vector4i Vector4i::PositiveInfinity = Vector4i(+std::numeric_limits<int32_t>::infinity());
template<> const Vector4i Vector4i::NegativeInfinity = Vector4i(-std::numeric_limits<int32_t>::infinity());

template<> const Vector4ui Vector4ui::Zero = Vector4ui(0);
template<> const Vector4ui Vector4ui::One = Vector4ui(1);
}
