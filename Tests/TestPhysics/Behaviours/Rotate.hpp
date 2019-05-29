#pragma once

#include <Scenes/Component.hpp>
#include <Maths/Vector3.hpp>

using namespace acid;

namespace test
{
class Rotate :
	public Component
{
public:
	explicit Rotate(const Vector3f &direction = Vector3f(), const int &test = 0);

	void Start() override;

	void Update() override;

	friend const Metadata &operator>>(const Metadata &metadata, Rotate &rotate);

	friend Metadata &operator<<(Metadata &metadata, const Rotate &rotate);

private:
	Vector3f m_direction;
	int m_test;
	Vector3f m_rotation;
};
}
