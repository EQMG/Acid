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
	explicit Rotate(const Vector3f &direction = Vector3f::Zero, const int &test = 0);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

private:
	Vector3f m_direction;
	int m_test;
	Vector3f m_rotation;
};
}
