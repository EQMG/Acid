#pragma once

#include "Helpers/Delegate.hpp"

namespace acid
{
class Entity;

/**
 * @brief Class that represents a functional component attached to entity.
 */
class ACID_EXPORT Component :
	public Observer
{
public:
	virtual ~Component() = default;

	/**
	 * Run when starting the component if {@link Component#m_started} is false.
	 */
	virtual void Start()
	{
	}

	/**
	 * Run when updating the entity this is attached to.
	 */
	virtual void Update()
	{
	}

	const bool &IsEnabled() const { return m_enabled; };

	void SetEnabled(const bool &enable) { m_enabled = enable; }

	const bool &IsRemoved() const { return m_removed; }

	void SetRemoved(const bool &removed) { m_removed = removed; }

	/**
	 * Gets the entity this component is attached to.
	 * @return The entity this component is attached to.
	 */
	Entity *GetEntity() const { return m_entity; }

	/**
	 * Sets the entity that this component is attached to.
	 * @param entity The new entity this is attached to.
	 */
	void SetEntity(Entity *entity) { m_entity = entity; }

private:
	friend class Entity;

	bool m_started{};
	bool m_enabled{true};
	bool m_removed{};
	Entity *m_entity{};
};

}
