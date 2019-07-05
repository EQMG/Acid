#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterLine :
	public Emitter
{
public:
	explicit EmitterLine(const float &length = 1.0f, const Vector3f &axis = Vector3f::Right);

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const float &GetLength() const { return m_length; }

	void SetLength(const float &length) { m_length = length; }

	const Vector3f &GetAxis() const { return m_axis; }

	void SetAxis(const Vector3f &axis) { m_axis = axis; }

	friend const Node &operator>>(const Node &node, EmitterLine &emitter);

	friend Node &operator<<(Node &node, const EmitterLine &emitter);

private:
	float m_length;
	Vector3f m_axis;
};
}
