#include "ModuleHolder.hpp"

namespace acid {
void ModuleHolder::RemoveModuleStage(const TypeId &id) {
	for (auto it = m_stages.begin(); it != m_stages.end();) {
		if (it->second == id) {
			it = m_stages.erase(it);
		} else {
			++it;
		}
	}
}

void ModuleHolder::UpdateStage(const Module::Stage &stage) {
	for (const auto &[stageIndex, typeId] : m_stages) {
		if (stageIndex.first != stage) {
			continue;
		}

		if (auto &module = m_modules[typeId]) {
			module->Update();
		}
	}
}
}
