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

	class F_EXPORT ComponentGetSet
	{
	public:
		std::string m_name;
		std::string m_type;
		std::function<std::string()> *m_getter;
		std::function<void(PrefabComponent *, unsigned int)> *m_setter;

		ComponentGetSet()
		{
		}

		~ComponentGetSet()
		{
			delete m_getter;
			delete m_setter;
		}
	};

	class F_EXPORT Component
	{
	private:
		std::map<unsigned int, ComponentGetSet*> *m_values;
		std::string m_name;
		GameObject *m_gameObject;
	public:
#define LINK_GET(f) (new std::function<std::string()>([&]() -> std::string { return std::to_string(f); }))
#define LINK_GET_STR(f) (new std::function<std::string()>([&]() -> std::string { return f; }))
#define LINK_GET_RES(f) (new std::function<std::string()>([&]() -> std::string { return (f == nullptr) ? "nullptr" : f->GetFilename(); }))
#define LINK_SET(t, f) (new std::function<void(PrefabComponent *, unsigned int)>([&](PrefabComponent *componentPrefab, unsigned int i) -> void { t v = componentPrefab->Get<t>(i); f; }))

		Component();

		virtual ~Component();

		virtual void Update();

		virtual std::string GetName() const = 0;

		void Load(PrefabComponent *componentPrefab);

		void Write(PrefabComponent *componentPrefab);

		std::map<unsigned int, ComponentGetSet*> *GetValues() const { return m_values; }

		void SetName(const std::string &name) { m_name = name; }

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

	protected:
		template<typename T>
		void Link(const unsigned int &index, const std::string &name, std::function<std::string()> *getter, std::function<void(PrefabComponent *, unsigned int)> *setter = nullptr)
		{
			ComponentGetSet *componentGetSet = new ComponentGetSet();
			componentGetSet->m_name = name;
			componentGetSet->m_type = typeid(T).name();
			componentGetSet->m_getter = getter;
			componentGetSet->m_setter = setter;

			if (FormatString::Contains(componentGetSet->m_type, "basic_string"))
			{
				componentGetSet->m_type = "string";
			}

			m_values->insert(std::make_pair(index, componentGetSet));
		}
	};
}
