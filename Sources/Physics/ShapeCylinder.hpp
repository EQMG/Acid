#pragma once

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCylinder :
		public IShape
	{
	private:
		btCylinderShape *m_shape;
	public:
		ShapeCylinder(const float &diameter = 1.0f, const float &height = 1.0f);

		~ShapeCylinder();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCylinder"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetDiameter() const;

		void SetDiameter(const float &diameter);

		float GetHeight() const;

		void SetHeight(const float &height);
	};
}
