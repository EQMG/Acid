#pragma once

#include "Engine/Exports.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class Entity;

	/// <summary>
	/// A class that represents a functional component attached to entity.
	/// </summary>
	class ACID_EXPORT Component
	{
	private:
		friend class Entity;

		bool m_started;
		bool m_enabled;
		bool m_removed;
		Entity *m_parent;
	public:
		explicit Component() :
			m_started(false),
			m_enabled(true),
			m_removed(false),
			m_parent(nullptr)
		{
		}

		/// <summary>
		/// Run when starting the component if <seealso cref="#m_started"/> is false.
		/// </summary>
		virtual void Start()
		{
		}

		/// <summary>
		/// Run when updating the entity this is attached to.
		/// </summary>
		virtual void Update()
		{
		}

		/// <summary>
		/// Used to decode this component from a loaded data format.
		/// </summary>
		/// <param name="metadata"> The metadata to decode from. </param>
		virtual void Decode(const Metadata &metadata)
		{
		}

		/// <summary>
		/// Used to encode this component into a data format.
		/// </summary>
		/// <param name="metadata"> The metadata to encode into. </param>
		virtual void Encode(Metadata &metadata) const
		{
		}

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }

		bool IsRemoved() const { return m_removed; }

		void SetRemoved(const bool &removed) { m_removed = removed; }

		/// <summary>
		/// Gets the entity this component is attached to.
		/// </summary>
		/// <returns> The entity this component is attached to. </returns>
		Entity *GetParent() const { return m_parent; }

		/// <summary>
		/// Sets the parent entity that this component is attached to.
		/// </summary>
		/// <param name="parent"> The new parent this is attached to. </param>
		void SetParent(Entity *parent) { m_parent = parent; }
	};
}
