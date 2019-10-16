#include "ShimSystem.hpp"

namespace acid {
ShimSystem::ShimSystem() {
}

void ShimSystem::Update(const Time &delta) {
	ForEach([](Entity entity) {
		for (auto &component : entity.GetComponents()) {
			// I know this will memory leak, this is a shim so I don't need to be 100% correct.
			if (!component->m_entity)
				component->m_entity = new Entity(entity);
			if (!component->m_started) {
				component->Start();
				component->m_started = true;
			}
			component->Update();
		}
	});
}
}
