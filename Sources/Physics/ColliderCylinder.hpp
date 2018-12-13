#pragma once

#include "Maths/Vector3.hpp"
#include "Collider.hpp"

class btCylinderShape;

namespace acid
{
	class ACID_EXPORT ColliderCylinder :
		public Collider
	{
	private:
		std::unique_ptr<btCylinderShape> m_shape;
		float m_radius;
		float m_height;
	public:
		explicit ColliderCylinder(const float &radius = 1.0f, const float &height = 1.0f, const Transform &localTransform = Transform::ZERO);

		~ColliderCylinder();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius);

		float GetHeight() const { return m_height; }

		void SetHeight(const float &height);
	};
}
