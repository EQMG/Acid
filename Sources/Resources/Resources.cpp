#include "Resources.hpp"

namespace acid {
const bool Resources::Registered = Register(Stage::Post, Requires<Files>());

Resources::Resources() :
	elapsedPurge(5s) {
}

void Resources::Update() {
	if (elapsedPurge.GetElapsed() != 0) {
		for (auto it = resources.begin(); it != resources.end();) {
			for (auto it1 = it->second.begin(); it1 != it->second.end();) {
				if ((*it1).second.use_count() <= 1) {
					it1 = it->second.erase(it1);
					continue;
				}

				++it1;
			}

			if (it->second.empty()) {
				it = resources.erase(it);
				continue;
			}
			
			++it;
		}
	}
}

std::shared_ptr<Resource> Resources::Find(const std::type_index &typeIndex, const Node &node) const {
	if (resources.find(typeIndex) == resources.end())
		return nullptr;

	for (const auto &[key, resource] : resources.at(typeIndex)) {
		if (key == node)
			return resource;
	}

	return nullptr;
}

void Resources::Add(const Node &node, const std::shared_ptr<Resource> &resource) {
	if (Find(resource->GetTypeIndex(), node))
		return;

	resources[resource->GetTypeIndex()].emplace(node, resource);
}

void Resources::Remove(const std::shared_ptr<Resource> &resource) {
	auto &resources = this->resources[resource->GetTypeIndex()];
	for (auto it = resources.begin(); it != resources.end(); ++it) { // TODO: Clean remove.
		if ((*it).second == resource)
			resources.erase(it);
	}
	if (resources.empty())
		this->resources.erase(resource->GetTypeIndex());
}
}
