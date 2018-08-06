#include "UiStartLogo.hpp"

#include "Events/EventTime.hpp"
#include "Events/Events.hpp"
#include "Maths/Visual/DriverConstant.hpp"
#include "Maths/Visual/DriverSlide.hpp"

namespace acid
{
	UiStartLogo::UiStartLogo(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_guiBackground(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)), Texture::Resource("Guis/Black.png"))),
		m_guiLogo(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(0.4f, 0.4f)), Texture::Resource("Logos/Flask-02.png"))),
		m_textCopyright(new Text(this, UiBound(Vector2(0.5f, 0.2f), "Centre", true), 1.6f, "Copyright (C) 2018, Equilibrium Games - All Rights Reserved. This product uses technology from Khronos Group, Bullet Physics, Nothings, and GLFW.", FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_CENTRE, 0.8f, 0.0012f, 0.024f)),
		m_starting(true)
	{
#ifdef ACID_BUILD_DEBUG
		Events::Get()->AddEvent<EventTime>(1.65f, false, [&]()
#else
		Events::Get()->AddEvent<EventTime>(3.6f, false, [&]()
#endif
		{
			SetAlphaDriver<DriverSlide>(1.0f, 0.0f, 1.4f);
		});
	}

	UiStartLogo::~UiStartLogo()
	{
		delete m_guiBackground;
		delete m_guiLogo;
		delete m_textCopyright;
	}

	void UiStartLogo::UpdateObject()
	{
		m_guiBackground->GetRectangle().m_dimensions.m_x = Display::Get()->GetAspectRatio();
		m_guiBackground->SetScaleDriver<DriverConstant>(1.6f);
		m_guiBackground->SetVisible(true);
		m_guiLogo->SetVisible(true);
	}
}
