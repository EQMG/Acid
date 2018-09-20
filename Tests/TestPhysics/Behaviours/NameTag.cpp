#include "NameTag.hpp"

#include <Objects/GameObject.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	const float NameTag::RANGE_INNER = 32.0f;
	const float NameTag::RANGE_OUTER = 40.0f;
	const float NameTag::TEXT_SIZE = 3.0f;

	NameTag::NameTag(const float &heightOffset) :
		m_heightOffset(heightOffset),
		m_text(std::make_unique<Text>(Uis::Get()->GetContainer(), UiBound(Vector2(0.5f, 0.5f), "Centre", true), TEXT_SIZE, "Undefined", FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_CENTRE))
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
		Vector3 worldPosition = GetGameObject()->GetTransform().GetPosition();
		worldPosition.m_y += m_heightOffset;

		auto camera = Scenes::Get()->GetCamera();
		Vector3 screenPosition = Matrix4::WorldToScreenSpace(worldPosition, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		screenPosition.m_x = (screenPosition.m_x + 1.0f) / 2.0f;
		screenPosition.m_y = 1.0f - ((screenPosition.m_y + 1.0f) / 2.0f);

		float scale = (RANGE_OUTER - screenPosition.m_z) / RANGE_OUTER;
		float alpha = (RANGE_INNER - screenPosition.m_z) / (RANGE_OUTER - RANGE_INNER);

		m_text->SetVisible(scale > 0.0f && screenPosition.m_z > 0.0f);
		m_text->GetRectangle().SetPosition(screenPosition);
		static_cast<DriverConstant *>(m_text->GetScaleDriver())->SetConstant(TEXT_SIZE * scale);
		static_cast<DriverConstant *>(m_text->GetAlphaDriver())->SetConstant(alpha);
	}

	void NameTag::Decode(const Metadata &metadata)
	{
	}

	void NameTag::Encode(Metadata &metadata) const
	{
	}
}
