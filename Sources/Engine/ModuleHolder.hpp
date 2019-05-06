#pragma once

#include "Module.hpp"
#include "Log.hpp"

namespace acid
{
/**
 * @brief Class that contains and manages modules registered to a engine.
 */
class ACID_EXPORT ModuleHolder :
	public NonCopyable
{
public:
	/**
	 * Checks whether a Module exists or not.
	 * @tparam T The Module type.
	 * @return If the Module has the System.
	 */
	template<typename T>
	bool HasModule() const
	{
		const auto it = m_modules.find(GetModuleTypeId<T>());

		return it != m_modules.end() && it->second != nullptr;
	}

	/**
	 * Gets a module instance by type from the register.
	 * @tparam T The Module type.
	 * @return The Module.
	 */
	template<typename T>
	T *GetModule() const
	{
		const auto typeId = GetModuleTypeId<T>();

		auto it = m_modules.find(typeId);

		if (it == m_modules.end() || it->second == nullptr)
		{
			throw std::runtime_error("Module Holder does not have requested Module");
			return nullptr;
		}

		return static_cast<T *>(it->second.get());
	}

	/**
	 * Adds a Module.
	 * @tparam T The Module type.
	 * @param stage The Module stage.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor arguments.
	 */
	template<typename T, typename... Args>
	void AddModule(const Module::Stage &stage, Args &&...args)
	{
		// Remove previous Module, if it exists.
		//Remove<T>();

		const auto typeId = GetModuleTypeId<T>();

		// Insert the stage value
		auto stageKey = static_cast<float>(stage) + (0.01f * static_cast<float>(m_modules.size()));
		m_stages.insert({ stageKey, typeId });

		// Then, add the Module
		m_modules[typeId] = std::make_unique<T>(std::forward<Args>(args)...);
	}

	/**
	 * Removes a Module.
	 * @tparam T The Module type.
	 */
	template<typename T>
	void RemoveModule()
	{
		const auto typeId = GetModuleTypeId<T>();

		// Remove the stage value for this Module.
		RemoveModuleStage(typeId);

		// Then, remove the Module.
		m_modules.erase(typeId);
	}

private:
	friend class Engine;

	void RemoveModuleStage(const TypeId &id);

	/**
	 * Iterates through all Modules.
	 * @param stage The Module stage.
	 */
	void UpdateStage(const Module::Stage &stage)
	{
		for (const auto &typeId : m_stages)
		{
			if (static_cast<uint32_t>(std::floor(typeId.first)) != static_cast<uint32_t>(stage))
			{
				continue;
			}

			auto &module = m_modules[typeId.second];

			if (module != nullptr)
			{
				module->Update();
			}
		}
	}

	// List of all Modules.
	std::unordered_map<TypeId, std::unique_ptr<Module>> m_modules;

	// List of module stages.
	std::multimap<float, TypeId> m_stages;
};
}
