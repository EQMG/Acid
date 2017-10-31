#include "OverlayPaused.hpp"

namespace Demo
{
	OverlayPaused::OverlayPaused(UiObject *parent) :
		UiObject(parent, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f))
	{
		Texture *textureWhite = new Texture("res/guis/white.png");
		Texture *textureInv = new Texture("res/guis/geometry-grain.jpg");

		// Examples.
		UiObject *uiExamples = new UiObject(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f));

		Text *textExample = new Text(uiExamples, Vector3(0.6f, 0.5f, RelativeScreen), 1.8f, Vector2(0.0f, 0.5f), "This is a UI testing screen! Controls: \n - WASD, Shift, Space (Movement) \n - N (Noclip toggle) \n - M (Camera lock) \n - Escape (Pause screen)", Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 0.8f, 0.0012f, 0.024f);

	//	Gui *guiExample = new Gui(uiExamples, *textExample->GetPosition(), *textExample->GetDimensions(), *textExample->GetPivot(), new Texture("res/guis/geometry-grain.jpg"), 1);
	//	guiExample->SetScaleDriver(new DriverConstant(2.9f));
	//	guiExample->SetColourOffset(Colour("#553982"));
		
	//	uiExamples->SetVisible(false);

		// Nav bar.
		UiObject *uiNavigation = new UiObject(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f));

		Gui *barColour = new Gui(uiNavigation, Vector3(0.5f, 1.0f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeNone), Vector2(0.5f, 0.0f), textureInv, 1);
		barColour->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		barColour->SetColourOffset(Colour("#2969B0"));
		
		Text *barTitle = new Text(uiNavigation, Vector3(0.01f, 0.99f, RelativeNone), 4.0f, Vector2(0.0f, 0.0f), "Kosmos", Uis::Get()->m_proximanova->GetBold(), JustifyLeft, 1.0f, 0.001f);
		
		Text *barCreated = new Text(uiNavigation, Vector3(0.02f, 0.915f, RelativeNone), 1.1f, Vector2(0.0f, 0.0f), "Created By: Equilibrium Games", Uis::Get()->m_proximanova->GetLight(), JustifyLeft, 1.0f, 0.0013f);
		
		Gui *tabPuck = new Gui(uiNavigation, Vector3(0.0f, 0.875f, RelativeNone), Vector3(0.0f, 0.01f, RelativeScreen), Vector2(0.0f, 1.0f), textureWhite, 1);
		tabPuck->SetColourOffset(Colour("#386AB5"));

		std::vector<std::string> tabNames = { "Inventory", "Achievements", "News", "Play", "Settings", "Exit" }; // TODO: Maybe each tab can have a seperate UI for the bar (like settings using some grey gears, news some green paper)
		std::vector<std::string> tabColours = { "#553982", "#41A85F", "#2969B0", "#B8312F", "#475577", "#75706B" };
		float tabXOffset = 0.35f;

		for (int i = 0; i < tabNames.size(); i++)
		{
			Text *tabText = new Text(uiNavigation, Vector3(tabXOffset, 0.955f, RelativeNone), 1.6f, Vector2(0.0f, 0.0f), tabNames.at(i), Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 1.0f, 0.003f);
			const float width = tabText->GetDimensions()->m_x * 1.6f;

			if (i == 4)
			{
				tabPuck->GetPosition()->m_x = tabXOffset;
				tabPuck->GetDimensions()->m_x = width;
				barColour->SetColourOffset(Colour(tabColours.at(i)));
			}

			tabXOffset += 0.03f + width;
		}

	//	uiNavigation->SetVisible(false);

		// Inventory.
		UiObject *uiInventory = new UiObject(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f));

		Text *username = new Text(uiInventory, Vector3(0.05f, 0.8f, RelativeNone), 1.6f, Vector2(0.0f, 0.0f), "Dank Memes", Uis::Get()->m_proximanova->GetRegular(), JustifyLeft, 1.0f, 0.003f);

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

	//	uiInventory->SetVisible(false);
	}

	OverlayPaused::~OverlayPaused()
	{
	}

	void OverlayPaused::UpdateObject()
	{
	}
}
