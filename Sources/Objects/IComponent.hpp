#pragma once

#include "Engine/Exports.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class GameObject;

	/// <summary>
	/// A class that represents a functional component attached to game object.
	/// </summary>
	class ACID_EXPORT IComponent
	{
	private:
		GameObject *m_gameObject;
		bool m_started;
		bool m_enabled;
		bool m_removed;
	public:
		explicit IComponent() :
			m_gameObject(nullptr),
			m_started(false),
			m_enabled(true),
			m_removed(false)
		{
		}

		/// <summary>
		/// Run when starting the component if <seealso cref="#m_started"/> is false.
		/// </summary>
		virtual void Start()
		{
		}

		/// <summary>
		/// Run when updating the game object this is attached to.
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

		/// <summary>
		/// Gets the game object this components is attached to.
		/// </summary>
		/// <returns> The registered game object. </returns>
		GameObject *GetGameObject() const { return m_gameObject; }

		/// <summary>
		/// Sets the game object this components is attached to.
		/// </summary>
		/// <param name="gameObject"> The new game object this is attached to. </param>
		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		ACID_HIDDEN bool IsStarted() const { return m_started; };

		ACID_HIDDEN void SetStarted(const bool &started) { m_started = started; }

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }

		bool IsRemoved() const { return m_removed; }

		void SetRemoved(const bool &removed) { m_removed = removed; }
	};
}
