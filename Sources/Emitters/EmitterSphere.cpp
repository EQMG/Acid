#include "EmitterSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
EmitterSphere::EmitterSphere(const float &radius) :
	m_radius{radius}
{
}

void EmitterSphere::Start()
{
}

void EmitterSphere::Update()
{
}

Vector3f EmitterSphere::GeneratePosition() const
{
	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b)
	{
		std::swap(a, b);
	}

	auto randX = b * std::cos(2.0f * Maths::Pi<float> * (a / b));
	auto randY = b * std::sin(2.0f * Maths::Pi<float> * (a / b));
	auto distance = Vector2f{randX, randY}.Length();
	return m_radius * distance * RandomUnitVector();
}

const Metadata &operator>>(const Metadata &metadata, EmitterSphere &emitter)
{
	metadata.GetChild("Radius", emitter.m_radius);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EmitterSphere &emitter)
{
	metadata.SetChild("Radius", emitter.m_radius);
	return metadata;
}
}
