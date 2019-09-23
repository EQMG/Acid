#pragma once

#include "Files/File.hpp"
#include "Files/Node.hpp"
#include "Resources/Resource.hpp"

namespace acid {
class Entity;

/**
 * @brief Resource that represents a entity prefab.
 */
class ACID_EXPORT EntityPrefab : public Resource {
public:
	/**
	 * Creates a new entity prefab, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The entity prefab with the requested values.
	 */
	static std::shared_ptr<EntityPrefab> Create(const Node &node);

	/**
	 * Creates a new entity prefab, or finds one with the same values.
	 * @param filename The entity prefab to load the entity prefab from.
	 * @return The entity prefab with the requested values.
	 */
	static std::shared_ptr<EntityPrefab> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new entity prefab.
	 * @param filename The file to load the entity prefab from.
	 * @param load If this resource will be loaded immediately, otherwise {@link EntityPrefab#Load} can be called later.
	 */
	explicit EntityPrefab(std::filesystem::path filename, bool load = true);

	void Load();
	void Write() const;

	const std::filesystem::path &GetFilename() const { return m_filename; }
	Node *GetParent() const { return m_file->GetNode(); }

	friend const EntityPrefab &operator>>(const EntityPrefab &entityPrefab, Entity &entity);
	friend EntityPrefab &operator<<(EntityPrefab &entityPrefab, const Entity &entity);
	friend const Node &operator>>(const Node &node, EntityPrefab &entityPrefab);
	friend Node &operator<<(Node &node, const EntityPrefab &entityPrefab);

private:
	std::filesystem::path m_filename;
	std::unique_ptr<File> m_file;
};
}
