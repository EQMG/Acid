#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"

class btCapsuleShape;

namespace acid
{
	class ACID_EXPORT ColliderCapsule :
		public Collider
	{
	private:
		btCapsuleShape *m_shape;
		float m_radius;
		float m_height;
	public:
		ColliderCapsule(const float &radius = 0.5f, const float &height = 1.0f);

		~ColliderCapsule();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		btCollisionShape *GetCollisionShape() const override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		float GetHeight() const { return m_height; }

		void SetHeight(const float &height) { m_height = height; }
	};
}
