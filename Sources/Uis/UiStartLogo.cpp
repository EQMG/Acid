#include "UiStartLogo.hpp"

#include "Events/Events.hpp"
#include "Events/EventTime.hpp"
#include "Maths/Visual/DriverConstant.hpp"
#include "Maths/Visual/DriverSlide.hpp"

namespace fl
{
	UiStartLogo::UiStartLogo(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_guiBackground(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)), Texture::Resource("Resources/Guis/Black.png"), 1)),
		m_guiLogo(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(0.4f, 0.4f)), Texture::Resource("Resources/Logos/Equilibrium-Games-02.png"), 1)),
		m_textCopyright(new Text(this, UiBound(Vector2(0.5f, 0.2f), "Centre", true), 1.4f, "Copyright (C) 2018, Equilibrium Games - All Rights Reserved. This product uses GLFW, Vulkan, MoltenVK, OpenAL, and STB Image", Uis::Get()->m_proximaNova->GetRegular(), JustifyCentre, 0.8f, 0.0012f, 0.024f)),
		m_starting(true)
	{
#ifdef FL_BUILD_DEBUG
		Events::Get()->AddEvent(new EventTime(1.65f, false, [&]()
#else
			Events::Get()->AddEvent(new EventTime(3.6f, false, [&]()
#endif
		{
			SetAlphaDriver(new DriverSlide(1.0f, 0.0f, 1.0f));
		}));
	}

	UiStartLogo::~UiStartLogo()
	{
		delete m_guiBackground;
		delete m_guiLogo;
		delete m_textCopyright;
	}

	void UiStartLogo::UpdateObject()
	{
		m_guiBackground->GetRectangle()->m_dimensions->m_x = Display::Get()->GetAspectRatio();
		m_guiBackground->SetScaleDriver(new DriverConstant(1.6f));
		m_guiBackground->SetVisible(true);
		m_guiLogo->SetVisible(true);
	}
}
