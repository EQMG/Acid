#include "Uis.hpp"

namespace Flounder
{
	Uis::Uis() :
		IModule(),
		m_managerUis(nullptr),
		m_selector(new UiSelector()),
		m_container(new UiObject(nullptr, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f))),
		m_objects(new std::vector<UiObject*>()),
		m_arial(new FontFamily("arial")),
		m_berlinSans(new FontFamily("berlinSans")),
		m_cafeFrancoise(new FontFamily("cafefrancoise")),
		m_proximanova(new FontFamily("proximanova")),
		m_candara(new FontFamily("candara")),
		m_segoe(new FontFamily("segoe"))
	{
	}

	Uis::~Uis()
	{
		delete m_managerUis;
		delete m_selector;
		delete m_container;
		delete m_objects;

		delete m_arial;
		delete m_berlinSans;
		delete m_cafeFrancoise;
		delete m_proximanova;
		delete m_candara;
		delete m_segoe;
	}

	void Uis::Update()
	{
		if (m_managerUis != nullptr)
		{
			m_selector->Update(m_managerUis->IsGamePaused());
			m_managerUis->Update();
		}

		m_objects->clear();
		m_container->Update();
		m_container->GetAll(m_objects);
	}
}
