#include "EmitterCircle.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterCircle::EmitterCircle(const float &radius, const Vector3f &heading, const Transform &localTransform) :
	Emitter(localTransform),
	m_radius(radius),
	m_heading(heading.Normalize())
{
}

void EmitterCircle::Start()
{
}

void EmitterCircle::Update()
{
}

void EmitterCircle::Decode(const Metadata &metadata)
{
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Heading", m_heading);
	metadata.GetChild("Local Transform", m_localTransform);
}

void EmitterCircle::Encode(Metadata &metadata) const
{
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Heading", m_heading);
	metadata.SetChild("Local Transform", m_localTransform);
}

Vector3f EmitterCircle::GeneratePosition() const
{
	auto direction = Vector3f();

	do
	{
		auto randomVector = Emitter::RandomUnitVector();
		direction = randomVector.Cross(m_heading);
	}
	while (direction.Length() == 0.0f);

	direction.Normalize();
	direction *= m_radius;

	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	auto randX = b * std::cos(2.0f * Maths::Pi * (a / b));
	auto randY = b * std::sin(2.0f * Maths::Pi * (a / b));
	auto distance = Vector3f(randX, randY, 0.0f).Length();
	return Vector3f(direction * distance);
}
}
