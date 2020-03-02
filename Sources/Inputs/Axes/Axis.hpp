#pragma once

#include "Helpers/Delegate.hpp"
#include "Helpers/StreamFactory.hpp"

namespace acid {
/**
 * @brief Interface for an axis based input device.
 */
class ACID_EXPORT Axis : public StreamFactory<Axis>, public virtual Observer {
public:
	class Argument {
	public:
		friend const Node &operator>>(const Node &node, Argument &argument) {
			node["name"].Get(argument.name);
			node["type"].Get(argument.type);
			node["description"].Get(argument.description);
			return node;
		}
		friend Node &operator<<(Node &node, const Argument &argument) {
			node["name"].Set(argument.name);
			node["type"].Set(argument.type);
			node["description"].Set(argument.description);
			return node;
		}
		
		std::string name;
		std::string type;
		std::string description;
	};
	using ArgumentDescription = std::vector<Argument>;

	virtual ~Axis() = default;

	/**
	 * Gets the current value along the axis. -1 is smallest input, 1 is largest input.
	 * @return The current value of the axis in the range (-1, 1).
	 */
	virtual float GetAmount() const { return 0.0f; }

	virtual ArgumentDescription GetArgumentDescription() const { return {}; }

	/**
	 * Called when the axis changes value.
	 * @return The delegate.
	 */
	Delegate<void(float)> &OnAxis() { return onAxis; }

	float GetScale() const { return scale; }
	void SetScale(float scale) { scale = scale; }

protected:
	Delegate<void(float)> onAxis;
	float scale = 1.0f;
};
}
