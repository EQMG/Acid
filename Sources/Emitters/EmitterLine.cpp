#include "EmitterLine.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
EmitterLine::EmitterLine(const float& length, const Vector3& axis, const Transform& localTransform) : Emitter(localTransform), m_length(length), m_axis(axis.Normalize()) {}

void EmitterLine::Start() {}

void EmitterLine::Update() {}

void EmitterLine::Decode(const Metadata& metadata)
{
	metadata.GetChild("Length", m_length);
	metadata.GetChild("Axis", m_axis);
	metadata.GetChild("Local Transform", m_localTransform);
}

void EmitterLine::Encode(Metadata& metadata) const
{
	metadata.SetChild("Length", m_length);
	metadata.SetChild("Axis", m_axis);
	metadata.SetChild("Local Transform", m_localTransform);
}

Vector3 EmitterLine::GeneratePosition() const
{
	return m_axis * m_length * Maths::Random(-0.5f, 0.5f);
}
}
