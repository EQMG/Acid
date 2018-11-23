#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"

class btConeShape;

namespace acid
{
	class ACID_EXPORT ColliderCone :
		public Collider
	{
	private:
		std::unique_ptr<btConeShape> m_shape;
		float m_radius;
		float m_height;
	public:
		explicit ColliderCone(const float &radius = 1.0f, const float &height = 1.0f, const Transform &localTransform = Transform::ZERO);

		~ColliderCone();

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
