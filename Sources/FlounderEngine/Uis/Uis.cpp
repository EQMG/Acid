#include "Uis.hpp"

#include "../Scenes/Scenes.hpp"

namespace Flounder
{
	Uis::Uis() :
		IModule(),
		m_selector(new UiSelector()),
		m_container(new UiObject(nullptr, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)))),
		m_objects(new std::vector<UiObject *>()),
		m_proximaNova(FontFamily::Resource("Resources/Fonts/ProximaNova"))
	{
	}

	Uis::~Uis()
	{
		delete m_selector;
		delete m_container;
		delete m_objects;

		delete m_proximaNova;
	}

	void Uis::Update()
	{
		m_objects->clear();

		m_selector->Update(Scenes::Get()->IsGamePaused());
		m_container->Update();
		m_container->GetAll(m_objects);
	}
}
