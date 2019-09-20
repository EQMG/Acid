#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Helpers/TypeInfo.hpp"

namespace acid {
/**
 * @brief Represents when a module will call <seealso cref="Module#Update()"/> in the update loop.
 */
enum class ModuleStage {
	Always,
	Pre,
	Normal,
	Post,
	Render
};

template<typename Base>
class ModuleFactory {
public:
	using StageIndex = std::pair<ModuleStage, std::size_t>;
	using RegistryMap = std::multimap<StageIndex, Base *>;

	static RegistryMap &Registry() {
		static RegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar : public Base, public NonCopyable {
	public:
		/**
		 * Gets the engines instance.
		 * @return The current module instance.
		 */
		static T *Get() { return ModuleInstance.get(); }

		/**
		 * Creates a new module singleton instance and registers into the module registry map.
		 * @param moduleStage The stage where <seealso cref="Module#Update()"/> will be called from the engine.
		 * @return A dummy value in static initialization.
		 */
		static bool Register(ModuleStage moduleStage) {
			ModuleInstance = std::make_unique<T>();
			auto &registry = ModuleFactory::Registry();
			registry.insert({StageIndex(moduleStage, registry.size()), Get()});
			return true;
		}

		/**
		 * Deletes this module singleton instance and removed it from the module registry map.
		 * @return A dummy value in static initialization.
		 */
		static bool Deregister() {
			auto &registry = ModuleFactory::Registry();
			for (auto it = registry.begin(); it != registry.end();) {
				if (it->second == ModuleInstance.get()) {
					it = registry.erase(it);
				} else {
					++it;
				}
			}
			ModuleInstance = nullptr;
			return true;
		}
		
	private:
		// Named ModuleInstance instead of Instance to avoid name collisions.
		inline static std::unique_ptr<T> ModuleInstance = nullptr;
	};
};

/**
 * @brief A interface used for defining engine modules.
 */
class ACID_EXPORT Module : public ModuleFactory<Module> {
public:
	virtual ~Module() = default;

	/**
	 * The update function for the module.
	 */
	virtual void Update() = 0;
};
}
