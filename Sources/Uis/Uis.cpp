#include "Uis.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	Uis::Uis() :
		m_selector(UiSelector()),
		m_container(std::make_unique<UiObject>(nullptr, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, true, Vector2(1.0f, 1.0f)))),
		m_objects(std::vector<UiObject *>())
	{
	}

	void Uis::Update()
	{
		auto selectorJoystick = Scenes::Get()->GetSelectorJoystick();

		if (selectorJoystick != nullptr)
		{
			m_selector.Update(Scenes::Get()->IsPaused(), *selectorJoystick);
		}

		m_objects.clear();
		m_container->Update(m_objects);
	}
}
