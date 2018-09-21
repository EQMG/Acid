#pragma once

#include <Objects/IComponent.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test
{
	class NameTag :
		public IComponent
	{
	private:
		float m_heightOffset;

		std::unique_ptr<Text> m_text;
	public:
		static const float RANGE_INNER;
		static const float RANGE_OUTER;
		static const float TEXT_SIZE;

		explicit NameTag(const float &heightOffset = 0.0f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
