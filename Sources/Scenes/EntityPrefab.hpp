#pragma once

#include "Files/File.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
class Entity;

/**
 * @brief Resource that represents a entity prefab.
 */
class ACID_EXPORT EntityPrefab :
	public Resource
{
public:
	/**
	 * Creates a new entity prefab, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The entity prefab with the requested values.
	 */
	static std::shared_ptr<EntityPrefab> Create(const Metadata &metadata);

	/**
	 * Creates a new entity prefab, or finds one with the same values.
	 * @param filename The entity prefab to load the entity prefab from.
	 * @return The entity prefab with the requested values.
	 */
	static std::shared_ptr<EntityPrefab> Create(const std::string &filename);

	/**
	 * Creates a new entity prefab.
	 * @param filename The file to load the entity prefab from.
	 * @param load If this resource will be loaded immediately, otherwise {@link EntityPrefab#Load} can be called later.
	 */
	explicit EntityPrefab(std::string filename, const bool &load = true);

	void Load() override;

	void Write() const;

	const std::string &GetFilename() const { return m_filename; }

	Metadata *GetParent() const { return m_file->GetMetadata(); }

	friend const EntityPrefab &operator>>(const EntityPrefab &entityPrefab, Entity &entity);

	friend EntityPrefab &operator<<(EntityPrefab &entityPrefab, const Entity &entity);

	friend const Metadata &operator>>(const Metadata &metadata, EntityPrefab &entityPrefab);

	friend Metadata &operator<<(Metadata &metadata, const EntityPrefab &entityPrefab);

private:
	std::string m_filename;
	std::unique_ptr<File> m_file;
};
}
