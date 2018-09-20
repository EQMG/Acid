#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"

class btBoxShape;

namespace acid
{
	class ACID_EXPORT ColliderBox :
		public Collider
	{
	private:
		std::unique_ptr<btBoxShape> m_shape;
		Vector3 m_extents;
	public:
		explicit ColliderBox(const Vector3 &extents = Vector3::ONE);

		~ColliderBox();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		Vector3 GetExtents() const { return m_extents; }

		void SetExtents(const Vector3 &extents) { m_extents = extents; }
	};
}
