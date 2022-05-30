#include "NameTag.hpp"

#include <Scenes/Entity.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>

namespace test {
static const float TEXT_SIZE = 8.0f;
static const float VIEW_DISTANCE = 16.0f;

NameTag::NameTag(const std::string &name, float heightOffset) :
	name(name),
	heightOffset(heightOffset)
//text(&Uis::Get()->GetCanvas(), {{0.5f, 0.5f}, UiAnchor::BottomCentre), TEXT_SIZE, "Undefined", FontType::Create("Fonts/ProximaNova-Regular.ttf"),
//	Text::Justify::Left}
{
	//text.SetTextColour(0xFFFFFF);
	//text.SetBorderColour(0x262626);
	//text.SetBorderDriver<ConstantDriver>(0.1f);
}

void NameTag::Start() {
	//text.SetString(name);
}

void NameTag::Update() {
	// Calculates the tag position, this component should be added after a rigidbody body.
	auto transform = GetEntity()->GetComponent<Transform>();
	if (!transform)
		return;

	auto worldPosition = transform->GetPosition();
	worldPosition.y += heightOffset;

	this->transform.SetLocalPosition(worldPosition);
	this->transform.SetLocalRotation(Vector3f());

	// Quick way to change alpha values, only if you know the driver type for sure!
	auto toCamera = Scenes::Get()->GetScene()->GetCamera()->GetPosition().Distance(worldPosition);
	//dynamic_cast<ConstantDriver<float> *>(text.GetAlphaDriver())->SetConstant(std::clamp((VIEW_DISTANCE - toCamera) / VIEW_DISTANCE, 0.0f, 1.0f));

	// Will always face the screen, like a particle.
	//text.SetLockRotation(true);
	//text.SetWorldTransform(transform);
}

const Node &operator>>(const Node &node, NameTag &nameTag) {
	node["heightOffset"].Get(nameTag.heightOffset);
	return node;
}

Node &operator<<(Node &node, const NameTag &nameTag) {
	node["heightOffset"].Set(nameTag.heightOffset);
	return node;
}
}
