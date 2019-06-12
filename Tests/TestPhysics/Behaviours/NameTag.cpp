#include "NameTag.hpp"

#include <Scenes/Entity.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
static const float TEXT_SIZE = 8.0f;
static const float VIEW_DISTANCE = 16.0f;

NameTag::NameTag(const std::string &name, const float &heightOffset) :
	m_name{name},
	m_heightOffset{heightOffset}
	//m_text{&Uis::Get()->GetCanvas(), {{0.5f, 0.5f}, UiAnchor::BottomCentre}, TEXT_SIZE, "Undefined", FontType::Create("Fonts/ProximaNova"),
	//	Text::Justify::Left}
{
	//m_text.SetTextColour("#ffffff");
	//m_text.SetBorderColour("#262626");
	//m_text.SetBorderDriver(std::make_unique<DriverConstant<float>>(0.1f));
}

void NameTag::Start()
{
	//m_text.SetString(m_name);
}

void NameTag::Update()
{
	// Calculates the tag position, this component should be added after a rigidbody body.
	auto worldPosition{GetParent()->GetWorldTransform().GetPosition()};
	worldPosition.m_y += m_heightOffset;

	m_transform.SetPosition(worldPosition);
	m_transform.SetRotation(Vector3f());

	// Quick way to change alpha values, only if you know the driver type for sure!
	float toCamera{Scenes::Get()->GetCamera()->GetPosition().Distance(worldPosition)};
	//dynamic_cast<DriverConstant<float> *>(m_text.GetAlphaDriver())->SetConstant(std::clamp((VIEW_DISTANCE - toCamera) / VIEW_DISTANCE, 0.0f, 1.0f));

	// Will always face the screen, like a particle.
	//m_text.SetLockRotation(true);
	//m_text.SetWorldTransform(m_transform);
}

const Metadata &operator>>(const Metadata &metadata, NameTag &nameTag)
{
	metadata.GetChild("Height Offset", nameTag.m_heightOffset);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const NameTag &nameTag)
{
	metadata.SetChild("Height Offset", nameTag.m_heightOffset);
	return metadata;
}
}
