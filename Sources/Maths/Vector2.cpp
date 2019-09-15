#include "Vector2.hpp"

namespace acid {
template<>
const Vector2f Vector2f::Zero(0.0f);
template<>
const Vector2f Vector2f::One(1.0f);
template<>
const Vector2f Vector2f::Left(-1.0f, 0.0f);
template<>
const Vector2f Vector2f::Right(1.0f, 0.0f);
template<>
const Vector2f Vector2f::Up(0.0f, 1.0f);
template<>
const Vector2f Vector2f::Down(0.0f, -1.0f);
template<>
const Vector2f Vector2f::PositiveInfinity(+std::numeric_limits<float>::infinity());
template<>
const Vector2f Vector2f::NegativeInfinity(-std::numeric_limits<float>::infinity());

template<>
const Vector2d Vector2d::Zero(0.0);
template<>
const Vector2d Vector2d::One(1.0);
template<>
const Vector2d Vector2d::Left(-1.0, 0.0);
template<>
const Vector2d Vector2d::Right(1.0, 0.0);
template<>
const Vector2d Vector2d::Up(0.0, 1.0);
template<>
const Vector2d Vector2d::Down(0.0, -1.0);
template<>
const Vector2d Vector2d::PositiveInfinity(+std::numeric_limits<double>::infinity());
template<>
const Vector2d Vector2d::NegativeInfinity(-std::numeric_limits<double>::infinity());

template<>
const Vector2i Vector2i::Zero(0);
template<>
const Vector2i Vector2i::One(1);
template<>
const Vector2i Vector2i::Left(-1, 0);
template<>
const Vector2i Vector2i::Right(1, 0);
template<>
const Vector2i Vector2i::Up(0, 1);
template<>
const Vector2i Vector2i::Down(0, -1);
template<>
const Vector2i Vector2i::PositiveInfinity(+std::numeric_limits<int32_t>::infinity());
template<>
const Vector2i Vector2i::NegativeInfinity(-std::numeric_limits<int32_t>::infinity());

template<>
const Vector2ui Vector2ui::Zero(0);
template<>
const Vector2ui Vector2ui::One(1);
}
