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
		/// <param name="module"> The modules object. </param>
		/// <param name="update"> The modules update type. </param>
		/// <param name="name"> The modules name. </param>
		IModule *RegisterModule(IModule *module, const ModuleUpdate &update, const std::string &name);

		/// <summary>
		/// Registers a module with the register.
		/// </summary>
		/// <param name="update"> The modules update type. </param>
		/// <param name="name"> The modules name. </param>
		/// <param name="T"> The modules type. </param>
		template<typename T>
		T *RegisterModule(const ModuleUpdate &update, const std::string &name)
		{
			T *module = static_cast<T *>(malloc(sizeof(T)));
			RegisterModule(module, update, name);
			return new(module) T();
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
