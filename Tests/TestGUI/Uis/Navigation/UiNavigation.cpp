#include "UiNavigation.hpp"

#include <Maths/Visual/DriverSlide.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include "ContentExit.hpp"

namespace test
{
const std::vector<std::pair<std::string, Colour>> TABS = {{ "Play", Colour("#B8312F") }, { "News", Colour("#2969af") }, { "Achievements", Colour("#41A85F") },
	{ "Settings", Colour("#475577") }, { "Exit", Colour("#75706B") }};
const Colour UiNavigation::ColourPanel = Colour("#161618", 0.9f);
const Colour UiNavigation::ColourButton = Colour("#3e4148", 0.92f);

UiNavigation::UiNavigation(UiObject *parent) :
	UiObject(parent, UiBound::Screen),
	m_background(std::make_unique<Gui>(this, UiBound::Screen, Image2d::Create("Guis/White.png"))),
	m_navigation(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, Vector2(0.4f, 1.0f)),
		Image2d::Create("Guis/Gradient_A.png"), ColourPanel)),
	m_title(std::make_unique<Text>(m_navigation.get(), UiBound(Vector2(0.5f, 0.05f), UiReference::TopCentre), 5.5f, "TESTING", FontType::Create("Fonts/ProximaNova", "Bold"),
		Text::Justify::Left, 1.0f, Colour::White, 0.0018f)),
	m_createdBy(std::make_unique<Text>(m_navigation.get(), UiBound(Vector2(0.5f, 0.985f), UiReference::BottomCentre), 1.2f, "Created By: Equilibrium Games",
		FontType::Create("Fonts/ProximaNova", "Light"), Text::Justify::Left, 1.0f, Colour::White, 0.001f)),
	m_tabs(std::vector<std::unique_ptr<UiTab>>()),
	m_driverTarget(nullptr),
	m_currentTab(nullptr),
	m_targetTab(nullptr)
{
	float tabYOffset = 0.25f;

	for (auto &tab : TABS)
	{
		auto tabButton = new UiInputButton(m_navigation.get(), tab.first, UiBound(Vector2(0.5f, tabYOffset), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));
		tabButton->GetRectangle().SetDimensions(Vector2(0.3f, 0.05f));

		auto tabContent = new ContentExit(this);
		auto uiTab = new UiTab(tab.first, tab.second, tabButton, tabContent);
		m_tabs.emplace_back(uiTab);
		tabYOffset += 0.075f;

		/*tabButton->OnClick() += [&](UiObject *object, MouseButton button) -> void
		{
			if (button == MouseButton::Left)
			{
				SwitchTab(uiTab);
				object->CancelEvent(button);
			}
		};*/
		tabContent->SetAlphaDriver(new DriverConstant<float>(0.0f));
	}

	SwitchTab(m_tabs[0].get());
}

void UiNavigation::UpdateObject()
{
	if (m_driverTarget != nullptr && m_targetTab != nullptr)
	{
		float progress = m_driverTarget->Update(Engine::Get()->GetDelta());

		if (m_currentTab != nullptr)
		{
			//m_title->SetTextColour(m_currentTab->GetColour().Interpolate(m_targetTab->GetColour(), progress));
		}
		else
		{
			progress = 1.0f;
			//m_title->SetTextColour(m_targetTab->GetColour());
		}

		if (progress == 1.0f)
		{
			m_currentTab = m_targetTab;
			m_targetTab = nullptr;
		}
	}
}

void UiNavigation::SwitchTab(UiTab *tab)
{
	if (m_targetTab != nullptr)
	{
		return;
	}

	if (m_currentTab != nullptr && m_currentTab == tab)
	{
		return;
	}

	m_driverTarget = std::make_unique<DriverSlide<float>>(0.0f, 1.0f, Time::Seconds(0.4f));
	m_targetTab = tab;

	if (m_currentTab != nullptr)
	{
		m_currentTab->GetContent()->SetAlphaDriver(new DriverSlide<float>(1.0f, 0.0f, Time::Seconds(0.1f)));
	}

	//Events::Get()->AddEvent<EventTime>([&]()
	//{
	m_targetTab->GetContent()->SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, Time::Seconds(0.1f)));
	//}, Time::Seconds(0.32f));
}
}
