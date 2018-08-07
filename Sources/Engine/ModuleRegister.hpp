#pragma once

#include <map>
#include "IModule.hpp"

namespace acid
{
	/// <summary>
	/// The default updater for the engine.
	/// </summary>
	class ACID_EXPORT ModuleRegister
	{
	private:
		std::map<float, IModule *> m_modules;
	public:
		/// <summary>
		/// Creates a new module register.
		/// </summary>
		ModuleRegister();

		/// <summary>
		/// Deconstructor for the module register.
		/// </summary>
		~ModuleRegister();

		/// <summary>
		/// Fills the module register with default modules.
		/// </summary>
		void FillRegister();

		/// <summary>
		/// Gets if a module is contained in this registry.
		/// </summary>
		/// <param name="module"> The module to find. </param>
		/// <returns> If the module is in the registry. </returns>
		bool ContainsModule(IModule *module) const;

		/// <summary>
		/// Gets a module instance by type from the register.
		/// </summary>
		/// <param name="T"> The module type to find. </param>
		/// <returns> The found module. </returns>
		template<typename T>
		T *GetModule() const
		{
			for (auto &module : m_modules)
			{
				auto casted = dynamic_cast<T *>(module.second);

				if (casted != nullptr)
				{
					return casted;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="module"> The modules object. </param>
		/// <param name="update"> The modules update type. </param>
		/// <returns> The registered module. </returns>
		IModule *RegisterModule(IModule *module, const ModuleUpdate &update);

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		/// <param name="T"> The modules type. </param>
		/// <returns> The registered module. </returns>
		template<typename T>
		T *RegisterModule(const ModuleUpdate &update)
		{
			T *module = static_cast<T *>(malloc(sizeof(T)));
			RegisterModule(module, update);
			return new(module) T();
		}

		/// <summary>
		/// Deregisters a module.
		/// </summary>
		/// <param name="module"> The module to deregister. </param>
		/// <returns> If the module was deregistered. </returns>
		bool DeregisterModule(IModule *module);

		/// <summary>
		/// Removes a module by type from this game object.
		/// </summary>
		/// <param name="T"> The type of module to deregister. </param>
		/// <returns> If the module was deregistered. </returns>
		template<typename T>
		bool DeregisterModule()
		{
			for (auto &module : m_modules)
			{
				auto casted = dynamic_cast<T *>(module.second);

				if (casted != nullptr)
				{
					DeregisterModule(module.second);
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Runs updates for all module update types.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		void RunUpdate(const ModuleUpdate &update) const;

		uint32_t GetModuleCount() const { return m_modules.size(); }
	};
}
