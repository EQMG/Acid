#include "Scenes.hpp"

namespace acid
{
Scenes::Scenes() : m_scene(nullptr) {}

void Scenes::Update()
{
	if(m_scene == nullptr)
		{
			return;
		}

	if(!m_scene->m_started)
		{
			m_scene->Start();
			m_scene->m_started = true;
		}

	m_scene->Update();
	m_scene->GetPhysics()->Update();

	if(m_scene->GetStructure() != nullptr)
		{
			m_scene->GetStructure()->Update();
		}

	if(m_scene->GetCamera() != nullptr)
		{
			m_scene->GetCamera()->Update();
		}
}
}
