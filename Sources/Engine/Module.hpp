#pragma once

#include <map>
#include <memory>

#include "Helpers/NonCopyable.hpp"

namespace acid {
template<typename Base>
class ModuleFactory {
public:
	/**
	 * @brief Represents when a module will have <seealso cref="Module#Update()"/> called in the update loop.
	 */
	enum class Stage : uint8_t { Never, Always, Pre, Normal, Post, Render };
	
	using StageIndex = std::pair<Stage, std::size_t>;
	using TRegistryMap = std::multimap<StageIndex, std::unique_ptr<Base>>;

	virtual ~ModuleFactory() = default;

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	static std::size_t GetNextId() {
		static std::size_t id = 0;
		return ++id;
	}

	template<typename T, Stage S>
	class Registrar : public Base {
	public:
		/**
		 * Gets the engines instance.
		 * @return The current module instance.
		 */
		static T *Get() { return ModuleInstance; }

		/**
		 * Creates a new module singleton instance and registers into the module registry map.
		 * @return A dummy value in static initialization.
		 */
		static bool Register() {
			auto it = Registry().insert({StageIndex(S, GetNextId()), std::make_unique<T>()});
			ModuleInstance = dynamic_cast<T *>(it->second.get());
			return true;
		}

		/**
		 * Deletes this module singleton instance and removed it from the module registry map.
		 * @return A dummy value in static initialization.
		 */
		static bool Deregister() {
			for (auto it = Registry().begin(); it != Registry().end();) {
				if (it->second.get() == ModuleInstance) {
					it = Registry().erase(it);
				} else {
					++it;
				}
			}
			ModuleInstance = nullptr;
			return true;
		}
		
	private:
		// Named ModuleInstance instead of Instance to avoid name collisions.
		inline static T *ModuleInstance = nullptr;
	};
};

/**
 * @brief A interface used for defining engine modules.
 */
class ACID_EXPORT Module : public ModuleFactory<Module>, NonCopyable {
public:
	virtual ~Module() = default;

	/**
	 * The update function for the module.
	 */
	virtual void Update() = 0;
};
}
