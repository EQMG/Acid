#include "ShimSystem.hpp"

namespace acid {
ShimSystem::ShimSystem() {
}

void ShimSystem::Update(float delta) {
	ForEach([](Entity entity) {
		for (auto &component : entity.GetComponents()) {
			component->m_entity = &entity;
			if (!component->m_started) {
				component->Start();
				component->m_started = true;
			}
			component->Update();
		}
	});
}
}
