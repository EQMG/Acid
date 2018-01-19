#include "UiNavigation.hpp"

namespace Demo
{
	const std::vector<std::pair<std::string, Colour>> TABS = {
		{ "Play", Colour("#B8312F") },
		{ "News", Colour("#2969af") },
		{ "Achievements", Colour("#41A85F") },
		{ "Settings", Colour("#475577") },
		{ "Exit", Colour("#75706B") }
	};

	UiNavigation::UiNavigation(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_uiBar(nullptr),
		m_barBackground(nullptr),
		m_barTitle(nullptr),
		m_barCreatedBy(nullptr),
		m_tabPuck(nullptr),
		m_tabs(std::vector<UiTab*>())
	{
		Texture *textureWhite = new Texture("Resources/Guis/White.png");
		Texture *textureGeometry = new Texture("Resources/Guis/Geometry-Grain.png");

		m_uiBar = new UiObject(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)));

		m_barBackground = new Gui(m_uiBar, UiBound(Vector2(0.5f, 1.0f), "TopCentre", true, false, Vector2(1.0f, 1.0f)), textureGeometry, 1);
		m_barBackground->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		m_barBackground->SetColourOffset(Colour("#2969B0"));

		m_barTitle = new Text(m_uiBar, UiBound(Vector2(0.01f, 0.99f), "TopLeft", false), 4.0f, "Starting", Uis::Get()->m_proximaNova->GetBold(), JustifyLeft, 1.0f, 0.001f);
		m_barCreatedBy = new Text(m_uiBar, UiBound(Vector2(0.02f, 0.915f), "TopLeft", false), 1.1f, "Created By: Equilibrium Games", Uis::Get()->m_proximaNova->GetLight(), JustifyLeft, 1.0f, 0.0013f);

		m_tabPuck = new Gui(m_uiBar, UiBound(Vector2(0.0f, 0.875f), "BottomLeft", false, true, Vector2(0.0f, 0.01f)), textureWhite, 1);
		m_tabPuck->SetColourOffset(Colour("#386AB5"));

		float tabXOffset = 0.35f;
		int i = 0;

		for (auto tabType : TABS)
		{
			UiBound rectangle = UiBound(Vector2(tabXOffset, 0.955f), "TopLeft", false);
			UiTab *uiTab = new UiTab(m_uiBar, rectangle, tabType.first, tabType.second);
			tabXOffset += 0.03f + uiTab->GetWidth();
			m_tabs.push_back(uiTab);

			//uiTab->SetActionLeft([&](){
			//	SwitchTab(uiTab->GetName());
			//});

			if (i == 0)
			{
				SwitchTab(uiTab->GetName());
			}

			i++;
		}
	}

	UiNavigation::~UiNavigation()
	{
		delete m_uiBar;
		delete m_barBackground;
		delete m_barTitle;
		delete m_barCreatedBy;
		delete m_tabPuck;

		for (auto text : m_tabs)
		{
			delete text;
		}
	}

	void UiNavigation::UpdateObject()
	{
	}

	void UiNavigation::SwitchTab(const std::string &tabName)
	{
		for (UiTab *tab : m_tabs)
		{
			if (tab->GetName() == tabName)
			{
				printf("Tab activated: %s\n", tab->GetName().c_str());
				m_tabPuck->GetRectangle()->m_position->m_x = tab->GetRectangle()->m_position->m_x;
				m_tabPuck->GetRectangle()->m_dimensions->m_x = tab->GetWidth();
				m_barBackground->SetColourOffset(tab->GetColour());
			}
		}
	}
}
