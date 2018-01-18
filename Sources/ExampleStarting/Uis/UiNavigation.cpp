#include "UiNavigation.hpp"

namespace Demo
{
	// TODO: Maybe each tab can have a separate UI for the bar (like settings using some grey gears, news some green paper)
	static const std::vector<std::string> TAB_NAMES = { "Inventory", "Achievements", "News", "Play", "Settings", "Exit" };
	static const std::vector<std::string> TAB_COLOURS = { "#553982", "#41A85F", "#2969af", "#B8312F", "#475577", "#75706B" };

	UiNavigation::UiNavigation(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_uiBar(nullptr),
		m_barBackground(nullptr),
		m_barTitle(nullptr),
		m_barCreatedBy(nullptr),
		m_tabPuck(nullptr),
		tabTexts(std::vector<Text*>())
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

		for (unsigned int i = 0; i < TAB_NAMES.size(); i++)
		{
			Text *tabText = new Text(m_uiBar, UiBound(Vector2(tabXOffset, 0.955f), "TopLeft", false), 1.6f, TAB_NAMES.at(i), Uis::Get()->m_proximaNova->GetRegular(), JustifyLeft, 1.0f, 0.003f);
			float width = tabText->GetRectangle()->m_dimensions->m_x * 1.6f;
			auto tabAction = [&](){
				printf("Tab activated: %s\n", TAB_NAMES.at(i).c_str());
				m_tabPuck->GetRectangle()->m_position->m_x = tabXOffset;
				m_tabPuck->GetRectangle()->m_dimensions->m_x = width;
				m_barBackground->SetColourOffset(Colour(TAB_COLOURS.at(i)));
			};
			tabText->SetActionLeft(tabAction);

			if (i == 3)
			{
				tabAction();
			}

			tabXOffset += 0.03f + width;
			tabTexts.push_back(tabText);
		}
	}

	UiNavigation::~UiNavigation()
	{
		delete m_uiBar;
		delete m_barBackground;
		delete m_barTitle;
		delete m_barCreatedBy;
		delete m_tabPuck;

		for (auto text : tabTexts)
		{
			delete text;
		}
	}

	void UiNavigation::UpdateObject()
	{
	}
}
