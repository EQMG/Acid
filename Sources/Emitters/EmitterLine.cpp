#include "EmitterLine.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
EmitterLine::EmitterLine(const float &length, const Vector3f &axis, const Transform &localTransform) :
	Emitter(localTransform),
	m_length(length),
	m_axis(axis.Normalize())
{
}

void EmitterLine::Start()
{
}

void EmitterLine::Update()
{
}

Vector3f EmitterLine::GeneratePosition() const
{
	return m_axis * m_length * Maths::Random(-0.5f, 0.5f);
}

const Metadata& operator>>(const Metadata& metadata, EmitterLine& emitter)
{
	metadata.GetChild("Length", emitter.m_length);
	metadata.GetChild("Axis", emitter.m_axis);
	metadata.GetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const EmitterLine& emitter)
{
	metadata.SetChild("Length", emitter.m_length);
	metadata.SetChild("Axis", emitter.m_axis);
	metadata.SetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}
}
