#include "Uis.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	Uis::Uis() :
		m_selector(UiSelector()),
		m_container(std::make_unique<UiObject>(nullptr, UiBound::Screen)),
		m_objects(std::vector<UiObject *>())
	{
	}

	void Uis::Update()
	{
		auto virtualJoystick = Scenes::Get()->GetVirtualJoystick();
		m_objects.clear();
		m_selector.Update(virtualJoystick);
		m_container->Update(m_objects);
	}
}
