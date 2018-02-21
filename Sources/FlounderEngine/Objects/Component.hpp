#pragma once

#include <string>
#include <functional>
#include <map>
#include "../Prerequisites.hpp"
#include "Prefabs/ComponentPrefab.hpp"

namespace Flounder
{
	class GameObject;

	struct ComponentGetSet
	{
		std::string name;
		std::string dataType;
		std::function<std::string()> getter;
		std::function<void(std::string)> setter;
	};

	class F_EXPORT Component
	{
	protected:
		std::string m_name;
		GameObject *m_gameObject;
	private:
		std::map<unsigned int, ComponentGetSet> *m_values;
	public:
#define LINK_GET(t, f) [&]() -> t { return f; }
#define LINK_SET(t, f) [&](const t &v) -> void { f; }

		Component();

		virtual ~Component();

		virtual void Update();

		virtual std::string GetName() const = 0;

		void SetName(const std::string &name) { m_name = name; }

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		void Load(ComponentPrefab *componentPrefab);

		void Write(ComponentPrefab *componentPrefab);
	protected:
		template<typename T>
		void Link(const unsigned int &index, const std::function<T()> &getter, const std::function<void(T)> &setter = [](const T &v) -> void { })
		{
			ComponentGetSet componentGetSet{};
			componentGetSet.dataType = "Float_Slider";

			if (getter != nullptr)
			{
				componentGetSet.getter = [&]() -> std::string { return std::to_string(getter()); };
			}

			if (setter != nullptr)
			{
				componentGetSet.setter = [&](std::string value) -> void { setter(value); };
			}

			m_values->insert(std::make_pair(index, componentGetSet));
		}
	};
}
