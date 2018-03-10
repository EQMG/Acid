#pragma once

#include <string>
#include <functional>
#include <map>
#include "../Prerequisites.hpp"
#include "../Files/LoadedValue.hpp"
#include "../Helpers/FormatString.hpp"

namespace Flounder
{
	class GameObject;

	class F_EXPORT Component
	{
	private:
		std::string m_name;
		GameObject *m_gameObject;
		bool m_enabled;
	public:
		Component();

		virtual ~Component();

		virtual void Update() = 0;

		virtual void Load(LoadedValue *value) = 0;

		virtual void Write(LoadedValue *value) = 0;

		virtual std::string GetName() const = 0;

		void SetName(const std::string &name) { m_name = name; }

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }
	};
}
