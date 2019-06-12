#include "EmitterCircle.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterCircle::EmitterCircle(const float &radius, const Vector3f &heading) :
	m_radius{radius},
	m_heading{heading.Normalize()}
{
}

void EmitterCircle::Start()
{
}

void EmitterCircle::Update()
{
}

Vector3f EmitterCircle::GeneratePosition() const
{
	Vector3f direction;

	do
	{
		auto randomVector{RandomUnitVector()};
		direction = randomVector.Cross(m_heading);
	} while (direction.Length() == 0.0f);

	direction.Normalize();
	direction *= m_radius;

	auto a{Maths::Random(0.0f, 1.0f)};
	auto b{Maths::Random(0.0f, 1.0f)};

	if (a > b)
	{
		std::swap(a, b);
	}

	auto randX{b * std::cos(2.0f * Maths::Pi<float> * (a / b))};
	auto randY{b * std::sin(2.0f * Maths::Pi<float> * (a / b))};
	auto distance{Vector3f{randX, randY, 0.0f}.Length()};
	return direction * distance;
}

const Metadata &operator>>(const Metadata &metadata, EmitterCircle &emitter)
{
	metadata.GetChild("radius", emitter.m_radius);
	metadata.GetChild("heading", emitter.m_heading);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EmitterCircle &emitter)
{
	metadata.SetChild("radius", emitter.m_radius);
	metadata.SetChild("heading", emitter.m_heading);
	return metadata;
}
}
