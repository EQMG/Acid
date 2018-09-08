#include "Uis.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	Uis::Uis() :
		m_selector(UiSelector()),
		m_container(std::make_unique<UiObject>(nullptr, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)))),
		m_objects(std::vector<UiObject *>())
	{
	}

	Uis::~Uis()
	{
	}

	void Uis::Update()
	{
		m_objects.clear();
		m_selector.Update(Scenes::Get()->IsPaused(), *Scenes::Get()->GetSelectorJoystick());
		m_container->Update(m_objects);
	}
}
