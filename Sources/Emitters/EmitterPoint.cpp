#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterPoint::EmitterPoint(const Transform &localTransform) :
	Emitter(localTransform)
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
	metadata.GetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EmitterPoint &emitter)
{
	metadata.SetChild("Point", emitter.m_point);
	metadata.SetChild("Local Transform", emitter.m_localTransform);
	return metadata;
}
}
