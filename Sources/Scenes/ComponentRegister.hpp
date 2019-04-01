#pragma once

#include "Engine/Log.hpp"
#include "Component.hpp"

namespace acid
{
/**
 * @brief Class that holds registered component types.
 */
class ACID_EXPORT ComponentRegister
{
public:
	ComponentRegister();

	/**
	 * Adds a component to this register.
	 * @tparam T The components type.
	 * @param name The components name.
	 */
	template<typename T>
	void Add(const std::string &name)
	{
		if (m_components.find(name) != m_components.end())
		{
			Log::Error("Component '%s' is already registered!\n", name.c_str());
			return;
		}

		ComponentCreate componentCreate;
		componentCreate.m_create = []()
		{
			return new T();
		};
		componentCreate.m_isSame = [](Component *component)
		{
			return dynamic_cast<T *>(component) != nullptr; // TODO: Ignore type inheritance
		};

		m_components.emplace(name, componentCreate);
	}

	/**
	 * Removes a component type from the register.
	 * @param name The components name.
	 */
	void Remove(const std::string &name);

	/**
	 * Creates a new component from the register.
	 * @param name The component name to create.
	 * @return The new component.
	 */
	Component *Create(const std::string &name) const;

	/**
	 * Finds the registered name to a component.
	 * @param compare The components to get the registered name of.
	 * @return The name registered to the component.
	 */
	std::optional<std::string> FindName(Component *compare) const;

private:
	struct ComponentCreate
	{
		std::function<Component *()> m_create;
		std::function<bool(Component *)> m_isSame;
	};

	std::map<std::string, ComponentCreate> m_components;
};
}
