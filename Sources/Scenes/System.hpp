#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Helpers/TypeInfo.hpp"
#include "Holders/ComponentFilter.hpp"
#include "Entity.hpp"

namespace acid {
class ACID_EXPORT System : public NonCopyable {
	friend class Scene;
	friend class SystemHolder;
public:
	System() = default;

	virtual ~System() = default;

	/**
	 * Iterates through all enabled Entities.
	 * @tparam Func The function type.
	 * @param func The function.
	 */
	template<typename Func>
	void ForEach(Func &&func);

	template<typename... Ts, typename Func>
	void ForJoinedEach(Func &&func);

	/**
	 * Detaches all entities.
	 */
	void DetachAll();

	/**
	 * Gets Entities attached to this System.
	 * @return The Entities.
	 */
	const std::vector<Entity> &GetEntities() const { return m_enabledEntities; }

	/**
	 * Gets the Scene that the System belongs to.
	 * @return The Scene.
	 */
	const Scene *GetScene() const { return m_scene; }

protected:
	/**
	 * Gets the component filter.
	 * @return The component filter.
	 */
	ComponentFilter &GetFilter() { return m_filter; }

	virtual void OnStart();
	virtual void OnShutdown();
	virtual void OnEntityAttach(Entity entity);
	virtual void OnEntityDetach(Entity entity);
	virtual void OnEntityEnable(Entity entity);
	virtual void OnEntityDisable(Entity entity);
	virtual void Update(float delta);

private:
	enum class EntityStatus {
		NotAttached, Enabled, Disabled
	};

	/**
	 * Attach an Entity to the System.
	 * @param entity
	 */
	void AttachEntity(const Entity &entity);

	/**
	 * Detach an Entity from the System.
	 * @param entity The Entity.
	 */
	void DetachEntity(const Entity &entity);

	/**
	 * Enable Entity.
	 * @param entity The Entity.
	 */
	void EnableEntity(const Entity &entity);

	/**
	 * Disable Entity.
	 * @param entity The Entity.
	 */
	void DisableEntity(const Entity &entity);

	/**
	 * Get Entity status.
	 * @param id The Entity ID.
	 * @return The status.
	 */
	EntityStatus GetEntityStatus(Entity::Id id) const;

	/**
	 * Set Entity status.
	 * @param id The Entity ID.
	 * @param status The status.
	 */
	void SetEntityStatus(Entity::Id id, const EntityStatus &status);

	// Enabled Entities attached to this System.
	std::vector<Entity> m_enabledEntities;

	// Disabled Entities attached to this System.
	std::vector<Entity> m_disabledEntities;

	// Entities attach and enable status.
	std::unordered_map<Entity::Id, EntityStatus> m_status;

	// The Scene that this System belongs to.
	Scene *m_scene = nullptr;

	// The mask that the Entities must matched to be attached to this System.
	ComponentFilter m_filter;
};

// Get the Type ID for the System T
template<typename T>
TypeId GetSystemTypeId() noexcept;
}

#include "System.inl"
