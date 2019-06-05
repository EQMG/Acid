#include "Vector2.hpp"

namespace acid
{
template<> const Vector2f Vector2f::Zero = Vector2f(0.0f);
template<> const Vector2f Vector2f::One = Vector2f(1.0f);
template<> const Vector2f Vector2f::Left = Vector2f(-1.0f, 0.0f);
template<> const Vector2f Vector2f::Right = Vector2f(1.0f, 0.0f);
template<> const Vector2f Vector2f::Up = Vector2f(0.0f, 1.0f);
template<> const Vector2f Vector2f::Down = Vector2f(0.0f, -1.0f);
template<> const Vector2f Vector2f::PositiveInfinity = Vector2f(+std::numeric_limits<float>::infinity());
template<> const Vector2f Vector2f::NegativeInfinity = Vector2f(-std::numeric_limits<float>::infinity());

template<> const Vector2d Vector2d::Zero = Vector2d(0.0);
template<> const Vector2d Vector2d::One = Vector2d(1.0);
template<> const Vector2d Vector2d::Left = Vector2d(-1.0, 0.0);
template<> const Vector2d Vector2d::Right = Vector2d(1.0, 0.0);
template<> const Vector2d Vector2d::Up = Vector2d(0.0, 1.0);
template<> const Vector2d Vector2d::Down = Vector2d(0.0, -1.0);
template<> const Vector2d Vector2d::PositiveInfinity = Vector2d(+std::numeric_limits<double>::infinity());
template<> const Vector2d Vector2d::NegativeInfinity = Vector2d(-std::numeric_limits<double>::infinity());

template<> const Vector2i Vector2i::Zero = Vector2i(0);
template<> const Vector2i Vector2i::One = Vector2i(1);
template<> const Vector2i Vector2i::Left = Vector2i(-1, 0);
template<> const Vector2i Vector2i::Right = Vector2i(1, 0);
template<> const Vector2i Vector2i::Up = Vector2i(0, 1);
template<> const Vector2i Vector2i::Down = Vector2i(0, -1);
template<> const Vector2i Vector2i::PositiveInfinity = Vector2i(+std::numeric_limits<int32_t>::infinity());
template<> const Vector2i Vector2i::NegativeInfinity = Vector2i(-std::numeric_limits<int32_t>::infinity());

template<> const Vector2ui Vector2ui::Zero = Vector2ui(0);
template<> const Vector2ui Vector2ui::One = Vector2ui(1);
}
