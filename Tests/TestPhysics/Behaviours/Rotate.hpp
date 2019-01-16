#pragma once

#include <Scenes/Component.hpp>
#include <Maths/Vector3.hpp>

using namespace acid;

namespace test
{
	class Rotate :
		public Component
	{
	private:
		Vector3 m_direction;
		int m_test;
		Vector3 m_rotation;
	public:
		explicit Rotate(const Vector3 &direction = Vector3::Zero, const int &test = 0);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
