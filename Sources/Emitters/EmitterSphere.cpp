#include "EmitterSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
EmitterSphere::EmitterSphere(const float &radius, const Transform &localTransform) :
	Emitter(localTransform),
	m_radius(radius)
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
	float a = Maths::Random(0.0f, 1.0f);
	float b = Maths::Random(0.0f, 1.0f);

	if (a > b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	float randX = b * std::cos(2.0f * Maths::Pi * (a / b));
	float randY = b * std::sin(2.0f * Maths::Pi * (a / b));
	float distance = Vector2f(randX, randY).Length();
	return m_radius * distance * Emitter::RandomUnitVector();
}

const Metadata& operator>>(const Metadata& metadata, EmitterSphere& emitter)
{
	metadata.GetChild("Radius", emitter.m_radius);
	metadata.GetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const EmitterSphere& emitter)
{
	metadata.SetChild("Radius", emitter.m_radius);
	metadata.SetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}
}
