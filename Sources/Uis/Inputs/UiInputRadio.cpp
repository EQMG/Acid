#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
static const Vector2f SIZE = Vector2f(0.22f, 0.0315f);

UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, const Type &type, const bool &value, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform::Left, Image2d::Create("Guis/Radio.png"), UiInputButton::PrimaryColour),
	m_fill(&m_background, UiTransform::Maximum, nullptr, Colour::White),
	m_text(this, UiTransform::Left, UiInputButton::FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::ValueColour),
	m_value(value),
	m_type(type)
{
	GetTransform().SetScale(SIZE);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	m_background.GetTransform().SetScale(Vector2f(GetTransform().GetScale().m_y));
	m_text.GetTransform().SetPosition(Vector2f(5.4f * GetTransform().GetScale().m_y, 0.5f));
	UpdateValue();

	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(
			new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
	OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			CancelEvent(MouseButton::Left);

			m_value = !m_value;
			UpdateValue();
			m_onValue(m_value);
		}
	});
}

void UiInputRadio::UpdateObject()
{
}

void UiInputRadio::SetValue(const bool &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

void UiInputRadio::SetType(const Type &type)
{
	m_type = type;
	UpdateValue();
}

void UiInputRadio::UpdateValue()
{
	switch (m_type)
	{
	case Type::Filled:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Filled.png"));
		break;
	case Type::X:
		m_fill.SetImage(Image2d::Create("Guis/Radio_X.png"));
		break;
	case Type::Check:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Check.png"));
		break;
	case Type::Dot:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Dot.png"));
		break;
	}

	m_fill.SetAlphaDriver(new DriverSlide<float>(m_fill.GetAlpha(), m_value ? 1.0f : 0.0f, UiInputButton::SlideTime));
}
}
