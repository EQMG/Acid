#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
class Entity;

/**
 * Class that represents a functional component attached to entity.
 */
class ACID_EXPORT Component
{
public:
	Component() :
		m_started(false),
		m_enabled(true),
		m_removed(false),
		m_parent(nullptr)
	{
	}

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

	/**
	 * Used to decode this component from a loaded data format.
	 * @param metadata The metadata to decode from.
	 */
	virtual void Decode(const Metadata &metadata)
	{
	}

	/**
	 * Used to encode this component into a data format.
	 * @param metadata The metadata to encode into.
	 */
	virtual void Encode(Metadata &metadata) const
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
	Entity *GetParent() const { return m_parent; }

	/**
	 * Sets the parent entity that this component is attached to.
	 * @param parent The new parent this is attached to.
	 */
	void SetParent(Entity *parent) { m_parent = parent; }

private:
	friend class Entity;
	bool m_started;
	bool m_enabled;
	bool m_removed;
	Entity *m_parent;
};
}
