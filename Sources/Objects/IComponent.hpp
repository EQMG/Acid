#pragma once

#include <functional>
#include <map>
#include <string>
#include "Engine/Exports.hpp"
#include "Files/LoadedValue.hpp"

namespace acid
{
	class GameObject;

	class ACID_EXPORT IComponent
	{
	private:
		GameObject *m_gameObject;
		bool m_started;
		bool m_enabled;
	public:
		IComponent() :
			m_gameObject(nullptr),
			m_started(false),
			m_enabled(true)
		{
		}

		virtual ~IComponent()
		{
		}

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void Load(LoadedValue *value) = 0;

		virtual void Write(LoadedValue *destination) = 0;

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		ACID_HIDDEN bool IsStarted() const { return m_started; };

		ACID_HIDDEN void SetStarted(const bool &started) { m_started = started; }

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }
	};
}
