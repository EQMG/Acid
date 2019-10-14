#include "Scenes.hpp"

namespace acid {
Scenes::Scenes() {
}

void Scenes::Update() {
	if (!m_scene) {
		return;
	}

	if (!m_scene->m_started) {
		m_scene->Start();
		m_scene->m_started = true;
	}

	m_scene->Update(Engine::Get()->GetDelta().AsSeconds());

	if (m_scene->GetCamera()) {
		m_scene->GetCamera()->Update();
	}
}
}
