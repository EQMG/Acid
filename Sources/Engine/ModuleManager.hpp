#pragma once

#include <map>
#include <memory>
#include "Module.hpp"

namespace acid
{
	/// <summary>
	/// A class that contains and manages modules registered to a engine.
	/// </summary>
	class ACID_EXPORT ModuleManager :
		public NonCopyable
	{
	public:
		ModuleManager();

		~ModuleManager();

		/// <summary>
		/// Fills the module register with default modules.
		/// </summary>
		void FillRegister();

		/// <summary>
		/// Gets if a module is contained in this registry.
		/// </summary>
		/// <param name="module"> The module to find. </param>
		/// <returns> If the module is in the registry. </returns>
		bool Contains(Module *module);

		/// <summary>
		/// Gets a module instance by type from the register.
		/// </summary>
		/// <param name="T"> The module type to find. </param>
		/// <returns> The found module. </returns>
		template<typename T>
		T *Get() const
		{
			for (const auto &[key, module] : m_modules)
			{
				auto casted = dynamic_cast<T *>(module.get());

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
		Module *Add(Module *module, const Module::Stage &update);

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		/// <param name="T"> The modules type. </param>
		/// <returns> The registered module. </returns>
		template<typename T>
		T *Add(const Module::Stage &update)
		{
			auto module = static_cast<T *>(malloc(sizeof(T)));
			Add(module, update);
			new(module) T();
			return module;
		}

		/// <summary>
		/// Deregisters a module.
		/// </summary>
		/// <param name="module"> The module to deregister. </param>
		void Remove(Module *module);

		/// <summary>
		/// Removes a module by type from this entity.
		/// </summary>
		/// <param name="T"> The type of module to deregister. </param>
		template<typename T>
		void Remove()
		{
			for (auto it = --m_modules.end(); it != m_modules.begin(); --it)
			{
				auto casted = dynamic_cast<T *>((*it).second.get());

				if (casted != nullptr)
				{
					m_modules.erase(it);
				}
			}
		}

	private:
		friend class ModuleUpdater;

		/// <summary>
		/// Runs updates for all module update types.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		void RunUpdate(const Module::Stage &update);

		std::map<float, std::unique_ptr<Module>> m_modules;
	};
}
