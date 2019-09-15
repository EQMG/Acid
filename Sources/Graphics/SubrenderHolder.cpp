#include "SubrenderHolder.hpp"

namespace acid {
void SubrenderHolder::Clear() {
	m_stages.clear();
}

void SubrenderHolder::RemoveSubrenderStage(const TypeId &id) {
	for (auto it = m_stages.begin(); it != m_stages.end();) {
		if (it->second == id) {
			it = m_stages.erase(it);
		} else {
			++it;
		}
	}
}

void SubrenderHolder::RenderStage(const Pipeline::Stage &stage, const CommandBuffer &commandBuffer) {
	for (const auto &[stageIndex, typeId] : m_stages) {
		if (stageIndex.first != stage) {
			continue;
		}

		if (auto &subrender = m_subrenders[typeId]) {
			if (subrender->IsEnabled()) {
				subrender->Render(commandBuffer);
			}
		}
	}
}
}
