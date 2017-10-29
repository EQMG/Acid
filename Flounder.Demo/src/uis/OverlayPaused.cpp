#include "OverlayPaused.hpp"

namespace Demo
{
	OverlayPaused::OverlayPaused(UiObject *parent) :
		UiObject(parent, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, true), Vector2(0.5f, 0.5f)),
		m_textExample(new Text(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.0f, 0.0f), "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer a sem est. Donec et posuere felis. Sed et sollicitudin odio, ut porta nibh. Suspendisse ac leo purus. Nam mattis ultrices enim vitae cursus. Aenean eu porttitor est. Nulla semper ligula ligula, a tempor metus blandit sed. Cras velit nunc, convallis eu interdum eu, pretium in nulla. Praesent auctor at odio non tincidunt.", Uis::Get()->m_proximanova->GetRegular(), 2.6f, JustifyLeft, 0.7f, 0.002f, 0.01f)),
		m_guiExample(new Gui(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("res/guis/geometry-grain.jpg"), 1))
	{
		m_textExample->SetGlowing(new DriverConstant(0.15f));
	//	m_textExample->SetVisible(false);

		m_guiExample->SetColourOffset(Colour("#553982"));
		m_guiExample->SetDimensions(*m_textExample->GetDimensions());
	//	m_guiExample->SetVisible(false);

		Texture *textureInv = new Texture("res/guis/geometry-grain.jpg");
		Texture *texturePuck = new Texture("res/guis/white.png");

		Gui *barColour = new Gui(this, Vector3(0.5f, 1.0f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeNone), Vector2(0.5f, 0.0f), new Texture("res/guis/geometry-grain.jpg"), 1);
		barColour->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		barColour->SetColourOffset(Colour("#2969B0"));
		Text *barTitle = new Text(this, Vector3(0.015f, 0.985f, false), Vector2(0.0f, 0.0f), "Kosmos", Uis::Get()->m_proximanova->GetBold(), 5.0f, JustifyLeft);
		Text *barCreated = new Text(this, Vector3(0.019f, 0.91f, false), Vector2(0.0f, 0.0f), "Created By: Equilibrium Games", Uis::Get()->m_proximanova->GetLight(), 1.2f, JustifyLeft, 1.0f, 0.0013f);
		Gui *tabPuck = new Gui(this, Vector3(0.0f, 0.875f, false), Vector3(0.0f, 0.01f, true), Vector2(0.0f, 1.0f), texturePuck, 1);
		tabPuck->SetColourOffset(Colour("#386AB5"));

		std::vector<std::string> tabNames = { "Inventory", "Achievements", "News", "Play", "Settings", "Exit" };
		std::vector<std::string> tabColours = { "#553982", "#41A85F", "#2969B0", "#B8312F", "#475577", "#75706B" };
		float tabXOffset = 0.35f;

		for (int i = 0; i < tabNames.size(); i++)
		{
			Text *tabText = new Text(this, Vector3(tabXOffset, 0.947f, false), Vector2(0.0f, 0.0f), tabNames.at(i), Uis::Get()->m_proximanova->GetRegular(), 1.8f, JustifyLeft, 1.0f, 0.0035f);
			const float width = tabText->GetModel()->GetAabb()->GetWidth();

			if (i == 3)
			{
				tabPuck->GetPosition()->m_x = tabXOffset - 0.015f;
				tabPuck->GetDimensions()->m_x = width;
				barColour->SetColourOffset(Colour(tabColours.at(i)));
			}

			tabXOffset += 0.02f + width;
		}

		Text *notification = new Text(this, Vector3(0.5f, 0.85f, RelativeScreen), Vector2(0.5f, 0.0f), "Small red centred text!", Uis::Get()->m_proximanova->GetRegular(), 1.8f, JustifyCentre);
		notification->SetTextColour(Colour("#e74c3c"));

		/*for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				const float size = 0.1f;
				float x = i * (size + 0.025f);
				float y = j * (size + 0.025f);
				//Gui *gui = new Gui(this, Vector3(0.05f + x, 0.175f + y, false), Vector3(size, size, true), Vector2(0.0f, 0.0f), textureInv, 1);
				Gui *gui = new Gui(this, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, false), Vector2(0.5f, 0.5f), textureInv, 1);
				gui->SetScissor(Vector4(x + 0.05f, y - 0.825f, size, size));
				gui->SetColourOffset(Colour("#D14841"));
			}
		}*/
	}

	OverlayPaused::~OverlayPaused()
	{
		delete m_textExample;
		delete m_guiExample;
	}

	void OverlayPaused::UpdateObject()
	{
	}
}
