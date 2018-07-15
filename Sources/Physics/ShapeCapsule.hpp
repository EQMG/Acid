#pragma once

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCapsule :
		public IShape
	{
	private:
		btCapsuleShape *m_shape;
	public:
		ShapeCapsule(const float &diameter = 1.0f, const float &height = 1.0f);

		~ShapeCapsule();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCapsule"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetDiameter() const;

		void SetDiameter(const float &diameter);

		float GetHeight() const;

		void SetHeight(const float &height);
	};
}
