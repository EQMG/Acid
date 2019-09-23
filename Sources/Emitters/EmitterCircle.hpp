#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterCircle : public Component::Registrar<EmitterCircle>, public Emitter {
public:
	explicit EmitterCircle(float radius = 1.0f, const Vector3f &heading = Vector3f::Up);

	void Start() override;
	void Update() override;
	Vector3f GeneratePosition() const override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius) { m_radius = radius; }

	const Vector3f &GetHeading() const { return m_heading; }
	void SetHeading(const Vector3f &heading) { m_heading = heading; }

	friend const Node &operator>>(const Node &node, EmitterCircle &emitter);
	friend Node &operator<<(Node &node, const EmitterCircle &emitter);

private:
	static bool registered;

	float m_radius;
	Vector3f m_heading;
};
}
