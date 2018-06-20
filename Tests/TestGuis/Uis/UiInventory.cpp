#include "UiInventory.hpp"

namespace test
{
	UiInventory::UiInventory(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)))
	{
		// Inventory.
		/*	UiObject *uiInventory = new UiObject(this, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f)));
	
			Text *username = new Text(uiInventory, Vector3(0.05f, 0.8f, RelativeNone), 1.8f, Vector2(0.0f, 0.0f), "Testing", FontType::Resource("Fonts/ProximaNova", FAMILY_REGULAR), JustifyLeft, 1.0f, 0.003f);
	
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

	UiInventory::~UiInventory()
	{
	}

	void UiInventory::UpdateObject()
	{
	}
}
