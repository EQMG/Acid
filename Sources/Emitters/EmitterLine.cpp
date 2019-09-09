#include "EmitterLine.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
EmitterLine::EmitterLine(float length, const Vector3f &axis) :
	m_length{length},
	m_axis{axis.Normalize()}
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

const Node &operator>>(const Node &node, EmitterLine &emitter)
{
	node["length"].Get(emitter.m_length);
	node["axis"].Get(emitter.m_axis);
	return node;
}

Node &operator<<(Node &node, const EmitterLine &emitter)
{
	node["length"].Set(emitter.m_length);
	node["axis"].Set(emitter.m_axis);
	return node;
}
}
