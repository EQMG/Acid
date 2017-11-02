#include "OverlayPaused.hpp"

namespace Demo
{
	OverlayPaused::OverlayPaused(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f)))
	{
		Texture *textureWhite = new Texture("res/guis/white.png");
		Texture *textureInv = new Texture("res/guis/geometry-grain.jpg");

		// Examples.
	//	UiObject *uiExamples = new UiObject(this, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f)));

	//	InputButton *inputTest1 = new InputButton(uiExamples, Vector3(0.6f, 0.5f, RelativeScreen), "Dank Memes: True", JustifyCentre);

	//	Text *textExample = new Text(uiExamples, UiBound(Vector2(0.5f, 0.5f), Vector2(), true, true, Vector2(0.5f, 0.5f)), 1.8f, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 0.8f, 0.0012f, 0.024f);

	//	Gui *guiExample = new Gui(uiExamples, *textExample->GetRectangle(), new Texture("res/guis/geometry-grain.jpg"), 1);
	//	guiExample->SetScaleDriver(new DriverConstant(2.9f));
	//	guiExample->SetColourOffset(Colour("#553982"));
		
	//	uiExamples->SetVisible(false);

		// Nav bar.
		UiObject *uiNavigation = new UiObject(this, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f)));

		Gui *barColour = new Gui(uiNavigation, UiBound(Vector2(0.5f, 1.0f), Vector2(0.5f, 1.0f), true, false, Vector2(1.0f, 1.0f)), textureInv, 1);
		barColour->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		barColour->SetColourOffset(Colour("#2969B0"));
		
		Text *barTitle = new Text(uiNavigation, UiBound(Vector2(0.01f, 0.99f), Vector2(0.0f, 1.0f), false), 4.0f, "Kosmos", Uis::Get()->m_proximanova->GetBold(), JustifyLeft, 1.0f, 0.001f);
		
		Text *barCreated = new Text(uiNavigation, UiBound(Vector2(0.02f, 0.915f), Vector2(0.0f, 1.0f), false), 1.1f, "Created By: Equilibrium Games", Uis::Get()->m_proximanova->GetLight(), JustifyLeft, 1.0f, 0.0013f);
		
		Gui *tabPuck = new Gui(uiNavigation, UiBound(Vector2(0.0f, 0.875f), Vector2(0.0f, 0.0f), false, true, Vector2(0.0f, 0.01f)), textureWhite, 1);
		tabPuck->SetColourOffset(Colour("#386AB5"));

		std::vector<std::string> tabNames = { "Inventory", "Achievements", "News", "Play", "Settings", "Exit" }; // TODO: Maybe each tab can have a seperate UI for the bar (like settings using some grey gears, news some green paper)
		std::vector<std::string> tabColours = { "#553982", "#41A85F", "#2969B0", "#B8312F", "#475577", "#75706B" };
		float tabXOffset = 0.35f;

		for (int i = 0; i < tabNames.size(); i++)
		{
			Text *tabText = new Text(uiNavigation, UiBound(Vector2(tabXOffset, 0.955f), Vector2(0.0f, 1.0f), false), 1.6f, tabNames.at(i), Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 1.0f, 0.003f);
			const float width = tabText->GetRectangle()->m_dimensions->m_x * 1.6f;

			if (i == 4)
			{
				tabPuck->GetRectangle()->m_position->m_x = tabXOffset;
				tabPuck->GetRectangle()->m_dimensions->m_x = width;
				barColour->SetColourOffset(Colour(tabColours.at(i)));
			}

			tabXOffset += 0.03f + width;
		}

	//	uiNavigation->SetVisible(false);

		// Inventory.
	/*	UiObject *uiInventory = new UiObject(this, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f)));

		Text *username = new Text(uiInventory, Vector3(0.05f, 0.8f, RelativeNone), 1.8f, Vector2(0.0f, 0.0f), "Dank Memes", Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 1.0f, 0.003f);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				const float size = 0.09f;
				float x = i * (size + 0.02f);
				float y = j * (size + 0.02f);
				Gui *gui = new Gui(uiInventory, Vector3(x + 0.1f, y + 0.15f, RelativeNone), Vector3(size, size, RelativeScreen), Vector2(0.5f, 0.5f), textureWhite, 1);

				if (j == 0)
				{
					gui->SetColourOffset(Colour("#41A85F"));
				}
				else if (i == 0)
				{
					gui->SetColourOffset(Colour("#553982"));
				}
				else
				{
					gui->SetColourOffset(Colour("#D14841"));
				}
			}
		}

		uiInventory->SetVisible(false);*/
	}

	OverlayPaused::~OverlayPaused()
	{
	}

	void OverlayPaused::UpdateObject()
	{
	}
}
