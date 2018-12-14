#pragma once

#include "ParticleEmitter.hpp"

namespace acid
{
	class ACID_EXPORT EmitterPoint :
		public ParticleEmitter
	{
	private:
		Vector3 m_point;
	public:
		EmitterPoint(const Transform &localTransform = Transform::IDENTITY);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GeneratePosition() const override;

		Vector3 GetPoint() const { return m_point; }

		void SetPoint(const Vector3 &point) { m_point = point; }
	};
}
