#pragma once

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCone :
		public IShape
	{
	private:
		btConeShape *m_shape;
	public:
		ShapeCone(const float &diameter = 1.0f, const float &height = 1.0f);

		~ShapeCone();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCone"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetDiameter() const;

		void SetDiameter(const float &diameter);

		float GetHeight() const;

		void SetHeight(const float &height);
	};
}
