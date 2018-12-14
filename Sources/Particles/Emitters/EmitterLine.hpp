#pragma once

#include "ParticleEmitter.hpp"

namespace acid
{
	class ACID_EXPORT EmitterLine :
		public ParticleEmitter
	{
	private:
		float m_length;
		Vector3 m_axis;
	public:
		explicit EmitterLine(const float &length = 1.0f, const Vector3 &axis = Vector3::RIGHT, const Transform &localTransform = Transform::IDENTITY);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GeneratePosition() const override;

		float GetLength() const { return m_length; }

		void SetLength(const float &length) { m_length = length; }

		Vector3 GetAxis() const { return m_axis; }

		void SetAxis(const Vector3 &axis) { m_axis = axis; }
	};
}
