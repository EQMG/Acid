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
		std::unique_ptr<btCapsuleShape> m_shape;
		float m_radius;
		float m_height;
	public:
		explicit ColliderCapsule(const float &radius = 0.5f, const float &height = 1.0f, const Transform &localTransform = Transform::ZERO);

		~ColliderCapsule();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		float GetHeight() const { return m_height; }

		void SetHeight(const float &height) { m_height = height; }
	};
}
