#pragma once

#include <Scenes/Component.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test
{
	class NameTag :
		public Component
	{
	private:
		std::string m_name;
		float m_heightOffset;

		Transform m_transform;
		std::unique_ptr<Text> m_text;
	public:
		static const float TEXT_SIZE;
		static const float VIEW_DISTANCE;

		explicit NameTag(const std::string &name = "", const float &heightOffset = 0.0f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}
