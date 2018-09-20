#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"

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
		explicit ColliderCylinder(const float &radius = 1.0f, const float &height = 1.0f);

		~ColliderCylinder();

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
