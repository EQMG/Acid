#pragma once

#include "Module.hpp"

namespace acid
{
/**
 * Class that contains and manages modules registered to a engine.
 */
class ACID_EXPORT ModuleManager :
	public NonCopyable
{
public:
	ModuleManager();

	~ModuleManager();

	/**
	 * Fills the module register with default modules.
	 */
	void FillRegister();

	/**
	 * Gets if a module is contained in this registry.
	 * @param module The module to find.
	 * @return If the module is in the registry.
	 */
	bool Contains(Module *module);

	/**
	 * Gets a module instance by type from the register.
	 * @tparam T The module type to find.
	 * @return The found module.
	 */
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

	/**
	 * Registers a module with the register.
	 * @param module The modules object.
	 * @param update The modules update type.
	 * @return The registered module.
	 */
	Module *Add(Module *module, const Module::Stage &update);

	/**
	 * Registers a module with the register.
	 * @tparam T The modules type.
	 * @param update The modules update type.
	 * @return The registered module.
	 */
	template<typename T>
	T *Add(const Module::Stage &update)
	{
		auto module = static_cast<T *>(malloc(sizeof(T)));
		Add(module, update);
		new(module) T();
		return module;
	}

	/**
	 * Deregisters a module.
	 * @param module The module to deregister.
	 */
	void Remove(Module *module);

	/**
	 * Removes a module by type from this entity.
	 * @tparam T The type of module to deregister.
	 */
	template<typename T>
	void Remove()
	{
		for (auto it = m_modules.begin(); it != m_modules.end();)
		{
			if (dynamic_cast<T *>((*it).second.get()) != nullptr)
			{
				it = m_modules.erase(it);
				continue;
			}

			++it;
		}
	}

private:
	friend class ModuleUpdater;

	/**
	 * Runs updates for all module update types.
	 * @param update The modules update type.
	 */
	void RunUpdate(const Module::Stage &update);

	std::map<float, std::unique_ptr<Module>> m_modules;
};
}
