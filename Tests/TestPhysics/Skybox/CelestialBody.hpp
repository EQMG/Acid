#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>
#include <Serialized/Node.hpp>

using namespace acid;

namespace test {
class CelestialBody :
	public Component {
public:
	enum class Type {
		Sun = 0,
		Moon = 1
	};

	explicit CelestialBody(const Type &type = Type::Sun);

	void Start() override;

	void Update() override;

	Type GetType() const { return m_type; }

	void SetType(const Type &type) { m_type = type; }

	friend const Node &operator>>(const Node &node, CelestialBody &celestialBody);

	friend Node &operator<<(Node &node, const CelestialBody &celestialBody);

private:
	Type m_type;
};
}
