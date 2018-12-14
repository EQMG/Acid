#pragma once

#include <Scenes/Component.hpp>

using namespace acid;

namespace test
{
	class HeightDespawn :
		public Component
	{
	private:
		float m_removeHeight;
	public:
		explicit HeightDespawn(const float &removeHeight = -100.0f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
