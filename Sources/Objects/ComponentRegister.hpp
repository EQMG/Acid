#pragma once

#include <functional>
#include <memory>
#include <optional>
#include "IComponent.hpp"

namespace acid
{
	struct ComponentCreate
	{
		std::function<std::shared_ptr<IComponent>()> m_create;
		std::function<bool(std::shared_ptr<IComponent>)> m_isSame;
	};

	/// <summary>
	/// A class that holds registerd components.
	/// </summary>
	class ACID_EXPORT ComponentRegister
	{
	private:
		std::map<std::string, ComponentCreate> m_components;
	public:
		/// <summary>
		/// Creates a new component register.
		/// </summary>
		ComponentRegister();

		/// <summary>
		/// Deconstructor for the component register.
		/// </summary>
		~ComponentRegister();

		/// <summary>
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		std::shared_ptr<IComponent> CreateComponent(const std::string &name);

		/// <summary>
		/// Registers a component with the register.
		/// </summary>
		/// <param name="update"> The components update type. </param>
		/// <param name="name"> The components name. </param>
		/// <param name="T"> The components type. </param>
		template<typename T>
		void RegisterComponent(const std::string &name)
		{
			if (m_components.find(name) != m_components.end())
			{
				fprintf(stderr, "Component '%s' is already registered!\n", name.c_str());
				return;
			}

			ComponentCreate componentCreate = {};
			componentCreate.m_create = []() -> std::shared_ptr<IComponent>
			{
				return std::make_shared<T>();
			};
			componentCreate.m_isSame = [](std::shared_ptr<IComponent> component) -> bool
			{
				return std::dynamic_pointer_cast<T>(component) != nullptr;
			};

			m_components.emplace(name, componentCreate);
		}

		/// <summary>
		/// Deregisters a component.
		/// </summary>
		/// <param name="name"> The components name. </param>
		/// <returns> If the component was deregistered. </returns>
		bool DeregisterComponent(const std::string &name);

		/// <summary>
		/// Finds the registered name to a component.
		/// </summary>
		/// <param name="compare"> The components to get the registered name of. </param>
		/// <returns> The name registered to the component. </returns>
		std::optional<std::string> FindComponentName(const std::shared_ptr<IComponent> &compare);
	};
}
