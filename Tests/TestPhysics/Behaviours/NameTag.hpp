#pragma once

#include <Scenes/Component.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test {
class NameTag : public Component::Registrar<NameTag> {
public:
	explicit NameTag(const std::string &name = "", float heightOffset = 0.0f);

	void Update() override;

	friend const Node &operator>>(const Node &node, NameTag &nameTag);
	friend Node &operator<<(Node &node, const NameTag &nameTag);

private:
	static bool registered;

	std::string m_name;
	float m_heightOffset;

	Transform m_transform;
	//Text m_text;
};
}
