#include "Uis.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	Uis::Uis() :
		m_selector(UiSelector()),
		m_container(new UiObject(nullptr, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)))),
		m_objects(std::vector<UiObject *>())
	{
	}

	Uis::~Uis()
	{
		delete m_container;
	}

	void Uis::Update()
	{
		m_objects.clear();

		if (Scenes::Get()->GetScene() != nullptr)
		{
			m_selector.Update(Scenes::Get()->IsGamePaused(), Scenes::Get()->GetScene()->GetSelectorJoystick());
		}

		m_container->Update(m_objects);
	}
}
