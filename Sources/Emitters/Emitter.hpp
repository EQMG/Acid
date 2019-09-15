#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"

namespace acid {
/**
 * @brief Component interface that defines a emitter volume.
 */
class ACID_EXPORT Emitter : public Component {
public:
	virtual ~Emitter() = default;

	/**
	 * Creates a new objects position.
	 * @return The new objects position.
	 */
	virtual Vector3f GeneratePosition() const = 0;

	static Vector3f RandomUnitVector() {
		auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi<float>;
		auto z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
		auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		auto x = rootOneMinusZSquared * std::cos(theta);
		auto y = rootOneMinusZSquared * std::sin(theta);
		return {x, y, z};
	}
};
}
