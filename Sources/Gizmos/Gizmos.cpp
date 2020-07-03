#include "Gizmos.hpp"

namespace acid {
Gizmos::Gizmos() {
}

void Gizmos::Update() {
	/*for (auto it = gizmos.begin(); it != gizmos.end();) {
		if (it->second.empty()) {
			it = gizmos.erase(it);
			continue;
		}

		(*it).first->Update((*it).second);
		++it;
	}*/
}

Gizmo *Gizmos::AddGizmo(std::unique_ptr<Gizmo> &&gizmo) {
	/*auto it = gizmos.find(gizmo->GetGizmoType());

	if (it == gizmos.end()) {
		gizmos.emplace(gizmo->GetGizmoType(), std::vector<std::unique_ptr<Gizmo>>());
		it = gizmos.find(gizmo->GetGizmoType());
	}

	return (*it).second.emplace_back(std::move(gizmo)).get();*/
	return nullptr;
}

void Gizmos::RemoveGizmo(Gizmo *gizmo) {
	/*auto it = gizmos.find(gizmo->GetGizmoType());

	if (it != gizmos.end()) {
		it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [gizmo](std::unique_ptr<Gizmo> &g) {
			return g.get() == gizmo;
		}), it->second.end());
	}*/
}

void Gizmos::Clear() {
	//gizmos.clear();
}
}
