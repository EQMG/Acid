﻿#pragma once

#include "ParticleEmitter.hpp"

namespace acid
{
	class ACID_EXPORT EmitterCircle :
		public ParticleEmitter
	{
	private:
		float m_radius;
		Vector3 m_heading;
	public:
		explicit EmitterCircle(const float &radius = 1.0f, const Vector3 &heading = Vector3::UP, const Transform &localTransform = Transform::IDENTITY);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GeneratePosition() const override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 GetHeading() const { return m_heading; }

		void SetHeading(const Vector3 &heading) { m_heading = heading; }
	};
}
