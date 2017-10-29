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

		Text *textExample = new Text(uiExamples, Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.0f, 0.0f), "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer a sem est. Donec et posuere felis. Sed et sollicitudin odio, ut porta nibh. Suspendisse ac leo purus. Nam mattis ultrices enim vitae cursus. Aenean eu porttitor est. Nulla semper ligula ligula, a tempor metus blandit sed. Cras velit nunc, convallis eu interdum eu, pretium in nulla. Praesent auctor at odio non tincidunt.", Uis::Get()->m_proximanova->GetRegular(), 2.0f, JustifyLeft, 0.666f, 0.001f, 0.02f);
		textExample->SetGlowing(new DriverConstant(0.15f));
		
		Gui *guiExample = new Gui(uiExamples, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("res/guis/geometry-grain.jpg"), 1);
		guiExample->SetColourOffset(Colour("#553982"));
		guiExample->SetDimensions(*textExample->GetDimensions());
		
		Text *text2Example = new Text(uiExamples, Vector3(0.5f, 0.85f, RelativeScreen), Vector2(0.5f, 0.0f), "Small red centred text!", Uis::Get()->m_proximanova->GetRegular(), 1.8f, JustifyCentre);
		text2Example->SetTextColour(Colour("#e74c3c"));
		
		//InputButton *buttonExample = new InputButton(uiExamples, Vector3(0.5f, 0.5f, RelativeScreen), "Print Dank Memes", JustifyCentre);
		//buttonExample->SetActionLeft([&]()
		//{
		//	printf("Input 1 (%s) was pressed!", buttonExample->GetText().c_str());
		//});

#ifdef FLOUNDER_CONFIG_RELEASE
		uiExamples->SetVisible(false);
#endif

		// Nav bar.
		UiObject *uiNavigation = new UiObject(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f));

		Gui *barColour = new Gui(uiNavigation, Vector3(0.5f, 1.0f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeNone), Vector2(0.5f, 0.0f), textureInv, 1);
		barColour->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		barColour->SetColourOffset(Colour("#2969B0"));
		
		Text *barTitle = new Text(uiNavigation, Vector3(0.015f, 0.985f, RelativeNone), Vector2(0.0f, 0.0f), "Kosmos", Uis::Get()->m_proximanova->GetBold(), 5.0f, JustifyLeft, 1.0f, 0.001f);
		
		Text *barCreated = new Text(uiNavigation, Vector3(0.019f, 0.91f, RelativeNone), Vector2(0.0f, 0.0f), "Created By: Equilibrium Games", Uis::Get()->m_proximanova->GetLight(), 1.2f, JustifyLeft, 1.0f, 0.0013f);
		
		Gui *tabPuck = new Gui(uiNavigation, Vector3(0.0f, 0.875f, RelativeNone), Vector3(0.0f, 0.01f, RelativeScreen), Vector2(0.0f, 1.0f), textureWhite, 1);
		tabPuck->SetColourOffset(Colour("#386AB5"));

		std::vector<std::string> tabNames = { "Inventory", "Achievements", "News", "Play", "Settings", "Exit" };
		std::vector<std::string> tabColours = { "#553982", "#41A85F", "#2969B0", "#B8312F", "#475577", "#75706B" };
		float tabXOffset = 0.35f;

		for (int i = 0; i < tabNames.size(); i++)
		{
			Text *tabText = new Text(uiNavigation, Vector3(tabXOffset, 0.947f, RelativeNone), Vector2(0.0f, 0.0f), tabNames.at(i), Uis::Get()->m_proximanova->GetRegular(), 1.8f, JustifyLeft, 1.0f, 0.003f);
			const float width = tabText->GetModel()->GetAabb()->GetWidth();

			if (i == 3)
			{
				tabPuck->GetPosition()->m_x = tabXOffset - 0.015f;
				tabPuck->GetDimensions()->m_x = width;
				barColour->SetColourOffset(Colour(tabColours.at(i)));
			}

			tabXOffset += 0.02f + width;
		}

#ifndef FLOUNDER_CONFIG_RELEASE
		uiNavigation->SetVisible(false);
#endif

		// Inventory.
		UiObject *uiInventory = new UiObject(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f));

		Text *username = new Text(uiInventory, Vector3(0.055f, 0.8f, RelativeNone), Vector2(0.0f, 0.0f), "Dank Memes", Uis::Get()->m_proximanova->GetRegular(), 1.9f, JustifyLeft, 1.0f, 0.003f);

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

#ifndef FLOUNDER_CONFIG_RELEASE
		uiInventory->SetVisible(false);
#endif
	}

	OverlayPaused::~OverlayPaused()
	{
	}

	void OverlayPaused::UpdateObject()
	{
	}
}
