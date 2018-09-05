#pragma once

#include "ISpawnParticle.hpp"

namespace acid
{
	class ACID_EXPORT SpawnLine :
		public ISpawnParticle
	{
	private:
		float m_length;
		Vector3 m_axis;
		Vector3 m_spawnPosition;
	public:
		SpawnLine(const float &length = 1.0f, const Vector3 &axis = Vector3::RIGHT);

		~SpawnLine();

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GetBaseSpawnPosition() override;

		float GetLength() const { return m_length; }

		void SetLength(const float &length) { m_length = length; }

		Vector3 GetAxis() const { return m_axis; }

		void SetAxis(const Vector3 &axis) { m_axis = axis; }
	};
}
