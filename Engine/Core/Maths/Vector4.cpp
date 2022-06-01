#include "Vector4.hpp"

namespace acid {
template<typename T>
const Vector4<T> Vector4<T>::Zero(0.0f);
template<typename T>
const Vector4<T> Vector4<T>::One(1.0f);
template<typename T>
const Vector4<T> Vector4<T>::Infinity(std::numeric_limits<float>::infinity());
}
