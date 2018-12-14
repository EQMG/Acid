#pragma once

#include <functional>
#include <memory>
#include <optional>
#include "Engine/Log.hpp"
#include "Component.hpp"

namespace acid
{
	/// <summary>
	/// A class that holds registerd components.
	/// </summary>
	class ACID_EXPORT ComponentRegister
	{
	private:
		struct ComponentCreate
		{
			std::function<Component *()> m_create;
			std::function<bool(Component *)> m_isSame;
		};

		std::map<std::string, ComponentCreate> m_components;
	public:
		/// <summary>
		/// Creates a new component register.
		/// </summary>
		ComponentRegister();

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
				Log::Error("Component '%s' is already registered!\n", name.c_str());
				return;
			}

			ComponentCreate componentCreate = {};
			componentCreate.m_create = []() -> Component *
			{
				return new T();
			};
			componentCreate.m_isSame = [](Component *component) -> bool
			{
				return dynamic_cast<T *>(component) != nullptr;
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
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		Component *CreateComponent(const std::string &name);

		/// <summary>
		/// Finds the registered name to a component.
		/// </summary>
		/// <param name="compare"> The components to get the registered name of. </param>
		/// <returns> The name registered to the component. </returns>
		std::optional<std::string> FindComponentName(Component *compare);
	};
}
