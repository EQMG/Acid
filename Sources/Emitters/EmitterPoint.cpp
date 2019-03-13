#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterPoint::EmitterPoint(const Transform& localTransform) : Emitter(localTransform) {}

void EmitterPoint::Start() {}

void EmitterPoint::Update() {}

void EmitterPoint::Decode(const Metadata& metadata)
{
	metadata.GetChild("Point", m_point);
	metadata.GetChild("Local Transform", m_localTransform);
}

void EmitterPoint::Encode(Metadata& metadata) const
{
	metadata.SetChild("Point", m_point);
	metadata.SetChild("Local Transform", m_localTransform);
}

Vector3 EmitterPoint::GeneratePosition() const
{
	return m_point;
}
}
