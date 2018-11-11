#include "NameTag.hpp"

#include <Objects/GameObject.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	const float NameTag::TEXT_SIZE = 8.0f;
	const float NameTag::VIEW_DISTANCE = 16.0f;

	NameTag::NameTag(const float &heightOffset) :
		m_heightOffset(heightOffset),
		m_transform(Transform()),
		m_text(std::make_unique<Text>(Uis::Get()->GetContainer(), UiBound(Vector2(0.5f, 0.5f), "Centre", true),
			TEXT_SIZE, "Undefined", FontType::Resource("Fonts/ProximaNova", "Regular"), TEXT_JUSTIFY_CENTRE))
	{
		m_text->SetTextColour(Colour("#ffffff"));
		m_text->SetBorderColour(Colour("#262626"));
		m_text->SetBorderDriver<DriverConstant>(0.1f);
	}

	void NameTag::Start()
	{
		m_text->SetString(GetGameObject()->GetName());
	}

	void NameTag::Update()
	{
		// Calculates the tag position, this component should be added after a rigidbody body.
		Vector3 worldPosition = GetGameObject()->GetTransform().GetPosition();
		worldPosition.m_y += m_heightOffset;

		m_transform.SetPosition(worldPosition);
		m_transform.SetRotation(Vector3::ZERO);

		// Quick way to change alpha values, only if you know the driver type for sure!
		float toCamera = Scenes::Get()->GetCamera()->GetPosition().Distance(worldPosition);
		((DriverConstant *)m_text->GetAlphaDriver())->SetConstant(std::clamp((VIEW_DISTANCE - toCamera) / VIEW_DISTANCE, 0.0f, 1.0f));

		// Will always face the screen, like a particle.
		m_text->SetLockRotation(true);
		m_text->SetWorldTransform(m_transform);
	}

	void NameTag::Decode(const Metadata &metadata)
	{
		m_heightOffset = metadata.GetChild<float>("Height Offset");
	}

	void NameTag::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Height Offset", m_heightOffset);
	}
}
