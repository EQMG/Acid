#pragma once

#include <string>
#include <functional>
#include <map>
#include "../Prerequisites.hpp"
#include "../Helpers/FormatString.hpp"
#include "../Objects/Prefabs/PrefabComponent.hpp"

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
#define LINK_GET(f) [&]() -> std::string { return std::to_string(f); }
#define LINK_GET_STR(f) [&]() -> std::string { return f; }
#define LINK_GET_RES(f) [&]() -> std::string { return (f == nullptr) ? "nullptr" : f->GetFilename(); }
#define LINK_SET(t, f) [&](const t &v) -> void { f; }

		Component();

		virtual ~Component();

		virtual void Update();

		virtual std::string GetName() const = 0;

		void SetName(const std::string &name) { m_name = name; }

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		void Load(PrefabComponent *componentPrefab);

		void Write(PrefabComponent *componentPrefab);

	protected:
		void LinkString(const unsigned int &index, const std::function<std::string()> &getter, const std::function<void(std::string)> &setter = [](const std::string &v) -> void { })
		{
			ComponentGetSet componentGetSet{};
			componentGetSet.dataType = "string";

			if (getter != nullptr)
			{
				componentGetSet.getter = [&]() -> std::string { return getter(); };
			}

			if (setter != nullptr)
			{
				componentGetSet.setter = [&](std::string v) -> void {  }; // setter(v);
			}

			m_values->insert(std::make_pair(index, componentGetSet));
		}

		template<typename T>
		void Link(const unsigned int &index, const std::function<std::string()> &getter, const std::function<void(T)> &setter = [](const T &v) -> void { })
		{
			ComponentGetSet componentGetSet{};
			componentGetSet.dataType = std::string(typeid(T).name());

			if (getter != nullptr)
			{
				componentGetSet.getter = [&]() -> std::string { return getter(); };
			}

			if (setter != nullptr)
			{
				componentGetSet.setter = [&](std::string v) -> void { setter(FormatString::ConvertTo<T>(v)); };
			}

			m_values->insert(std::make_pair(index, componentGetSet));
		}
	};
}
