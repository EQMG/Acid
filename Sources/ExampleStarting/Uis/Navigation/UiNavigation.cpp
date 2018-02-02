#include "UiNavigation.hpp"

#include <Visual/DriverSlide.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include "ContentExit.hpp"

namespace Demo
{
	const std::vector<std::pair<std::string, Colour>> TABS = {
		{"Play",         Colour("#B8312F")},
		{"News",         Colour("#2969af")},
		{"Achievements", Colour("#41A85F")},
		{"Settings",     Colour("#475577")},
		{"Exit",         Colour("#75706B")}
	};

	UiNavigation::UiNavigation(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_barBackground(nullptr),
		m_barTitle(nullptr),
		m_barCreatedBy(nullptr),
		m_tabPuck(nullptr),
		m_tabs(std::vector<UiTab *>()),
		m_driverTarget(nullptr),
		m_currentTab(nullptr),
		m_targetTab(nullptr)
	{
		Texture *textureWhite = new Texture("Resources/Guis/White.png");
		Texture *textureGeometry = new Texture("Resources/Guis/Geometry-Grain.png");

		m_barBackground = new Gui(this, UiBound(Vector2(0.5f, 1.0f), "TopCentre", true, false, Vector2(1.0f, 1.0f)), textureGeometry, 1);
		m_barBackground->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		m_barBackground->SetColourOffset(Colour("#2969B0"));

		m_barTitle = new Text(this, UiBound(Vector2(0.01f, 0.99f), "TopLeft", false), 4.0f, "Starting", Uis::Get()->m_proximaNova->GetBold(), JustifyLeft, 1.0f, 0.001f);
		m_barCreatedBy = new Text(this, UiBound(Vector2(0.02f, 0.915f), "TopLeft", false), 1.1f, "Created By: Equilibrium Games", Uis::Get()->m_proximaNova->GetLight(), JustifyLeft, 1.0f, 0.0013f);

		m_tabPuck = new Gui(this, UiBound(Vector2(0.0f, 0.875f), "BottomLeft", false, true, Vector2(0.0f, 0.01f)), textureWhite, 1);
		m_tabPuck->SetColourOffset(Colour("#386AB5"));

		float tabXOffset = 0.35f;
		unsigned int i = 0;

		for (auto tabType : TABS)
		{
			UiBound rectangle = UiBound(Vector2(tabXOffset, 0.955f), "TopLeft", false);
			UiTab *uiTab = new UiTab(this, new ContentExit(this), rectangle, tabType.first, tabType.second);
			tabXOffset += 0.03f + uiTab->GetWidth();
			m_tabs.push_back(uiTab);

			if (i == 0)
			{
				SwitchTab(uiTab->GetName());
			}

			i++;
		}
	}

	UiNavigation::~UiNavigation()
	{
		delete m_barBackground;
		delete m_barTitle;
		delete m_barCreatedBy;
		delete m_tabPuck;

		for (auto text : m_tabs)
		{
			delete text;
		}

		delete m_driverTarget;
	}

	void UiNavigation::UpdateObject()
	{
		if (m_driverTarget != nullptr && m_targetTab != nullptr)
		{
			float progress = m_driverTarget->Update(Engine::Get()->GetDelta());

			if (m_currentTab != nullptr)
			{
				Colour::Interpolate(m_currentTab->GetColour(), m_targetTab->GetColour(), progress, m_barBackground->GetColourOffset());
				m_tabPuck->GetRectangle()->m_position->m_x = Maths::Interpolate(m_currentTab->GetRectangle()->m_position->m_x, m_targetTab->GetRectangle()->m_position->m_x, progress);
				m_tabPuck->GetRectangle()->m_dimensions->m_x = Maths::Interpolate(m_currentTab->GetWidth(), m_targetTab->GetWidth(), progress);
			}
			else
			{
				progress = 1.0f;
				m_barBackground->GetColourOffset()->Set(m_targetTab->GetColour());
				m_tabPuck->GetRectangle()->m_position->m_x = m_targetTab->GetRectangle()->m_position->m_x;
				m_tabPuck->GetRectangle()->m_dimensions->m_x = m_targetTab->GetWidth();
			}

			if (progress == 1.0f)
			{
				m_currentTab = m_targetTab;
				m_targetTab = nullptr;
				//	delete m_driverTarget;
			}
		}
	}

	void UiNavigation::SwitchTab(const std::string &tabName)
	{
		if (m_targetTab != nullptr)
		{
			return;
		}

		for (UiTab *tab : m_tabs)
		{
			if (tab->GetName() == tabName)
			{
				if (m_currentTab != nullptr && m_currentTab->GetName() == tabName)
				{
					continue;
				}

				delete m_driverTarget;
				m_driverTarget = new DriverSlide(0.0f, 1.0f, 0.7f);
				m_targetTab = tab;

				if (m_currentTab != nullptr)
				{
					m_currentTab->GetContent()->SetAlphaDriver(new DriverSlide(1.0f, 0.0f, 0.3f));
				}

				Events::Get()->AddEvent(new EventTime(0.4f, false, [&](){
					m_targetTab->GetContent()->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, 0.3f));
				}));
			}
		}
	}
}
