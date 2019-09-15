#include "NameTag.hpp"

#include <Scenes/Entity.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test {
static const float TEXT_SIZE = 8.0f;
static const float VIEW_DISTANCE = 16.0f;

NameTag::NameTag(const std::string &name, float heightOffset) :
	m_name(name),
	m_heightOffset(heightOffset)
//m_text(&Uis::Get()->GetCanvas(), {{0.5f, 0.5f}, UiAnchor::BottomCentre), TEXT_SIZE, "Undefined", FontType::Create("Fonts/ProximaNova"),
//	Text::Justify::Left}
{
	//m_text.SetTextColour("#ffffff");
	//m_text.SetBorderColour("#262626");
	//m_text.SetBorderDriver(std::make_unique<DriverConstant<float>>(0.1f));
}

void NameTag::Start() {
	//m_text.SetString(m_name);
}

void NameTag::Update() {
	// Calculates the tag position, this component should be added after a rigidbody body.
	auto transform = GetEntity()->GetComponent<Transform>();

	if (!transform) {
		return;
	}

	auto worldPosition = transform->GetPosition();
	worldPosition.m_y += m_heightOffset;

	m_transform.SetLocalPosition(worldPosition);
	m_transform.SetLocalRotation(Vector3f());

	// Quick way to change alpha values, only if you know the driver type for sure!
	auto toCamera = Scenes::Get()->GetCamera()->GetPosition().Distance(worldPosition);
	//dynamic_cast<DriverConstant<float> *>(m_text.GetAlphaDriver())->SetConstant(std::clamp((VIEW_DISTANCE - toCamera) / VIEW_DISTANCE, 0.0f, 1.0f));

	// Will always face the screen, like a particle.
	//m_text.SetLockRotation(true);
	//m_text.SetWorldTransform(m_transform);
}

const Node &operator>>(const Node &node, NameTag &nameTag) {
	node["heightOffset"].Get(nameTag.m_heightOffset);
	return node;
}

Node &operator<<(Node &node, const NameTag &nameTag) {
	node["heightOffset"].Set(nameTag.m_heightOffset);
	return node;
}
}
