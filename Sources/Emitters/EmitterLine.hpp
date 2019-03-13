#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterLine : public Emitter
{
  public:
	explicit EmitterLine(const float& length = 1.0f, const Vector3& axis = Vector3::Right, const Transform& localTransform = Transform::Identity);

	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	Vector3 GeneratePosition() const override;

	const float& GetLength() const
	{
		return m_length;
	}

	void SetLength(const float& length)
	{
		m_length = length;
	}

	const Vector3& GetAxis() const
	{
		return m_axis;
	}

	void SetAxis(const Vector3& axis)
	{
		m_axis = axis;
	}

  private:
	float m_length;
	Vector3 m_axis;
};
}
