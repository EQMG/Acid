#include "EntityPrefab.hpp"

#include "Files/File.hpp"
#include "Resources/Resources.hpp"
#include "Entity.hpp"
#include "Scenes.hpp"

namespace acid {
std::shared_ptr<EntityPrefab> EntityPrefab::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<EntityPrefab>(node))
		return resource;

	auto result = std::make_shared<EntityPrefab>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<EntityPrefab> EntityPrefab::Create(const std::filesystem::path &filename) {
	EntityPrefab temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

EntityPrefab::EntityPrefab(std::filesystem::path filename, bool load) :
	filename(std::move(filename)) {
	if (load) {
		Load();
	}
}

void EntityPrefab::Load() {
	if (filename.empty()) {
		return;
	}

	file = std::make_unique<File>(filename);
	file->Load();
}

void EntityPrefab::Write(Node::Format format) const {
	file->Write(filename, format);
}

const EntityPrefab &operator>>(const EntityPrefab &entityPrefab, Entity &entity) {
	for (const auto &property : entityPrefab.GetParent()->GetProperties()) {
		if (property.GetName().empty()) {
			continue;
		}

		if (auto component = Component::Create(property.GetName())) {
			property >> *component;
			entity.AddComponent(std::move(component));
		}
	}

	return entityPrefab;
}

EntityPrefab &operator<<(EntityPrefab &entityPrefab, const Entity &entity) {
	entityPrefab.file->Clear();

	for (const auto &component : entity.GetComponents()) {
		auto componentName = component->GetTypeName();

		if (componentName.empty()) {
			continue;
		}

		auto property = (*entityPrefab.file->GetNode())[componentName];
		property << *component;
	}

	return entityPrefab;
}

const Node &operator>>(const Node &node, EntityPrefab &entityPrefab) {
	node["filename"].Get(entityPrefab.filename);
	return node;
}

Node &operator<<(Node &node, const EntityPrefab &entityPrefab) {
	node["filename"].Set(entityPrefab.filename);
	return node;
}
}
