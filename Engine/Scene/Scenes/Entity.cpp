#include "Entity.hpp"

namespace acid {
void Entity::Update() {
	for (auto it = components.begin(); it != components.end();) {
		if ((*it)->IsRemoved()) {
			it = components.erase(it);
			continue;
		}

		if ((*it)->GetEntity() != this)
			(*it)->SetEntity(this);

		if ((*it)->IsEnabled()) {
			if (!(*it)->started) {
				(*it)->Start();
				(*it)->started = true;
			}

			(*it)->Update();
		}

		++it;
	}
}

Component *Entity::AddComponent(std::unique_ptr<Component> &&component) {
	if (!component) return nullptr;

	component->SetEntity(this);
	return components.emplace_back(std::move(component)).get();
}

void Entity::RemoveComponent(Component *component) {
	components.erase(std::remove_if(components.begin(), components.end(), [component](const auto &c) {
		return c.get() == component;
	}), components.end());
}

void Entity::RemoveComponent(const std::string &name) {
	components.erase(std::remove_if(components.begin(), components.end(), [name](const auto &c) {
		return name == c->GetTypeName();
	}), components.end());
}
}
