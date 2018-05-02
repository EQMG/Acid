#pragma once

#include <map>
#include <string>
#include "IModule.hpp"

namespace fl
{
	/// <summary>
	/// The default updater for the engine.
	/// </summary>
	class FL_EXPORT ModuleRegister
	{
	private:
		typedef std::pair<std::string, IModule *> ModulePair;

		std::multimap<float, ModulePair> *m_modules;
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
		/// Registers a module with the register.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		/// <param name="name"> The modules name. </param>
		/// <param name="T"> The modules type. </param>
		template<typename T>
		void RegisterModule(const ModuleUpdate &update, const std::string &name)
		{
			//	if (m_modules->find(name) != m_modules->end())
			//	{
			//		fprintf(stderr, "Module '%s' is already registered!\n", name.c_str());
			//		return;
			//	}

			T *module = static_cast<T *>(malloc(sizeof(T)));
			float offset = update + (0.01f * static_cast<float>(m_modules->size()));
			m_modules->insert(std::make_pair(offset, std::make_pair(name, module)));
			new(module) T();
		}

		/// <summary>
		/// Deregisters a module.
		/// </summary>
		/// <param name="name"> The modules name. </param>
		void DeregisterModule(const std::string &name);

		/// <summary>
		/// Gets a module from the register.
		/// </summary>
		/// <param name="name"> The module name to get. </param>
		/// <returns> The module object. </returns>
		IModule *GetModule(const std::string &name);

		/// <summary>
		/// Runs updates for all module update types.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		void RunUpdate(const ModuleUpdate &update) const;
	};
}
