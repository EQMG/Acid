#pragma once

#include "Maths/Vector3.hpp"
#include "Collider.hpp"

class btConeShape;

namespace acid
{
	class ACID_EXPORT ColliderCone :
		public Collider
	{
	public:
		explicit ColliderCone(const float &radius = 1.0f, const float &height = 1.0f, const Transform &localTransform = Transform::Identity);

		~ColliderCone();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		const float &GetRadius() const { return m_radius; }

		void SetRadius(const float &radius);

		const float &GetHeight() const { return m_height; }

		void SetHeight(const float &height);
	private:
		std::unique_ptr<btConeShape> m_shape;
		float m_radius;
		float m_height;
	};
}
