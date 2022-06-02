#pragma once

#include <Scenes/Component.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test {
class NameTag : public Component::Registrar<NameTag> {
	inline static const bool Registered = Register("heightDespawn");
public:
	explicit NameTag(const std::string &name = "", float heightOffset = 0.0f);

	void Start() override;
	void Update() override;

	friend const Node &operator>>(const Node &node, NameTag &nameTag);
	friend Node &operator<<(Node &node, const NameTag &nameTag);

private:
	std::string name;
	float heightOffset;

	Transform transform;
	//Text text;
};
}
