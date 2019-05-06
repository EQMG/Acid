#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterPoint::EmitterPoint()
{
}

void EmitterPoint::Start()
{
}

void EmitterPoint::Update()
{
}

Vector3f EmitterPoint::GeneratePosition() const
{
	return m_point;
}

const Metadata &operator>>(const Metadata &metadata, EmitterPoint &emitter)
{
	metadata.GetChild("Point", emitter.m_point);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EmitterPoint &emitter)
{
	metadata.SetChild("Point", emitter.m_point);
	return metadata;
}
}
