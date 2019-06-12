#include "Vector3.hpp"

namespace acid
{
template<> const Vector3f Vector3f::Zero{0.0f};
template<> const Vector3f Vector3f::One{1.0f};
template<> const Vector3f Vector3f::Left{-1.0f, 0.0f, 0.0f};
template<> const Vector3f Vector3f::Right{1.0f, 0.0f, 0.0f};
template<> const Vector3f Vector3f::Up{0.0f, 1.0f, 0.0f};
template<> const Vector3f Vector3f::Down{0.0f, -1.0f, 0.0f};
template<> const Vector3f Vector3f::Front{0.0f, 0.0f, 1.0f};
template<> const Vector3f Vector3f::Back{0.0f, 0.0f, -1.0f};
template<> const Vector3f Vector3f::PositiveInfinity{+std::numeric_limits<float>::infinity()};
template<> const Vector3f Vector3f::NegativeInfinity{-std::numeric_limits<float>::infinity()};

template<> const Vector3d Vector3d::Zero{0.0};
template<> const Vector3d Vector3d::One{1.0};
template<> const Vector3d Vector3d::Left{-1.0, 0.0, 0.0};
template<> const Vector3d Vector3d::Right{1.0, 0.0, 0.0};
template<> const Vector3d Vector3d::Up{0.0, 1.0, 0.0};
template<> const Vector3d Vector3d::Down{0.0, -1.0, 0.0};
template<> const Vector3d Vector3d::Front{0.0, 0.0, 1.0};
template<> const Vector3d Vector3d::Back{0.0, 0.0, -1.0};
template<> const Vector3d Vector3d::PositiveInfinity{+std::numeric_limits<double>::infinity()};
template<> const Vector3d Vector3d::NegativeInfinity{-std::numeric_limits<double>::infinity()};

template<> const Vector3i Vector3i::Zero{0};
template<> const Vector3i Vector3i::One{1};
template<> const Vector3i Vector3i::Left{-1, 0, 0};
template<> const Vector3i Vector3i::Right{1, 0, 0};
template<> const Vector3i Vector3i::Up{0, 1, 0};
template<> const Vector3i Vector3i::Down{0, -1, 0};
template<> const Vector3i Vector3i::Front{0, 0, 1};
template<> const Vector3i Vector3i::Back{0, 0, -1};
template<> const Vector3i Vector3i::PositiveInfinity{+std::numeric_limits<int32_t>::infinity()};
template<> const Vector3i Vector3i::NegativeInfinity{-std::numeric_limits<int32_t>::infinity()};

template<> const Vector3ui Vector3ui::Zero{0};
template<> const Vector3ui Vector3ui::One{1};
}
