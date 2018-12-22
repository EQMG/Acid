#include "UiNavigation.hpp"

#include <Maths/Visual/DriverSlide.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include "ContentExit.hpp"

namespace test
{
	const std::vector<std::pair<std::string, Colour>> TABS =
	{
		{"Play", Colour("#B8312F")},
		{"News", Colour("#2969af")},
		{"Achievements", Colour("#41A85F")},
		{"Settings", Colour("#475577")},
		{"Exit", Colour("#75706B")}
	};
	const Colour UiNavigation::PANEL_COLOUR = Colour("#000000", 0.9f);
	const Colour UiNavigation::BUTTON_COLOUR = Colour("#d32f2f", 0.92f);
	const Vector2 UiNavigation::BUTTON_SIZE = Vector2(0.3f, 0.05f);

	UiNavigation::UiNavigation(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, false, Vector2(1.0f, 1.0f))),
		m_background(nullptr),
		m_barBackground(nullptr),
		m_title(nullptr),
		m_createdBy(nullptr),
		m_tabs(std::vector<std::unique_ptr<UiTab>>()),
		m_driverTarget(nullptr),
		m_currentTab(nullptr),
		m_targetTab(nullptr)
	{
	//	auto guiA = new Gui(this, UiBound(Vector2(0.0f, 0.2f), UiBound::BOTTOM_LEFT, true, true, Vector2(0.5f, 0.6f)), Texture::Resource("Guis/White.png"));
	//	guiA->SetDepth(1.0f);
	//	guiA->SetColourOffset(Colour::TEAL);

	//	auto guiB = new Gui(guiA, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, true, Vector2(0.35f, 0.35f)), Texture::Resource("Guis/White.png"));
	//	guiB->SetDepth(2.0f);
	//	guiB->SetColourOffset(Colour::RED);

		m_background = std::make_unique<Gui>(this, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, false, Vector2(1.0f, 1.0f)), Texture::Resource("Guis/White.png"));

		m_barBackground = std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 1.0f), UiBound::TOP_LEFT, true, true, Vector2(0.4f, 1.0f)), Texture::Resource("Guis/Gradient_A.png"));
		m_barBackground->SetColourOffset(UiNavigation::PANEL_COLOUR); // TODO: Blur underneath.

		m_title = std::make_unique<Text>(this, UiBound(Vector2(0.2f, 0.95f), UiBound::TOP_CENTRE, false), 5.5f, "TESTING",
			FontType::Resource("Fonts/ProximaNova", "Bold"), TEXT_JUSTIFY_LEFT, 1.0f, BUTTON_COLOUR, 0.0018f);
		m_createdBy = std::make_unique<Text>(this, UiBound(Vector2(0.2f, 0.015f), UiBound::BOTTOM_CENTRE, false), 1.2f, "Created By: Equilibrium Games",
			FontType::Resource("Fonts/ProximaNova", "Light"), TEXT_JUSTIFY_LEFT, 1.0f, Colour::WHITE, 0.001f);

		float tabYOffset = 0.7f;

		for (auto &tab : TABS)
		{
			auto tabButton = new UiInputButton(this, tab.first, UiBound(Vector2(0.2f, tabYOffset), UiBound::CENTRE, false, true, Vector2(0.27f, 0.05f)), BUTTON_COLOUR);
			auto tabContent = new ContentExit(this);
			auto uiTab = new UiTab(tab.first, tab.second, tabButton, tabContent);
			m_tabs.emplace_back(uiTab);
			tabYOffset -= 0.075f;

			/*tabButton->GetOnClick() += [&](UiObject *object, MouseButton button) -> void
			{
			    if (button == MOUSE_BUTTON_LEFT)
			    {
			        SwitchTab(uiTab);
				    object->CancelEvent(button);
			    }
			};*/
			tabContent->SetAlphaDriver<DriverConstant>(0.0f);
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
			//	m_title->SetTextColour(m_currentTab->GetColour().Interpolate(m_targetTab->GetColour(), progress));
			}
			else
			{
				progress = 1.0f;
			//	m_title->SetTextColour(m_targetTab->GetColour());
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

		m_driverTarget = std::make_unique<DriverSlide>(0.0f, 1.0f, Time::Seconds(0.4f));
		m_targetTab = tab;

		if (m_currentTab != nullptr)
		{
			m_currentTab->GetContent()->SetAlphaDriver<DriverSlide>(1.0f, 0.0f, Time::Seconds(0.1f));
		}

		Events::Get()->AddEvent<EventTime>(Time::Seconds(0.32f), [&]()
		{
			m_targetTab->GetContent()->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, Time::Seconds(0.1f));
		}, false);
	}
}
