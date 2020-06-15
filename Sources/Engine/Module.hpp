#pragma once

#include <map>
#include <memory>
#include <functional>

#include "Utils/NonCopyable.hpp"
#include "Utils/TypeInfo.hpp"

namespace acid {
template<typename Base>
class ModuleFactory {
public:
	class TCreateValue {
	public:
		std::function<std::unique_ptr<Base>()> create;
		typename Base::Stage stage;
		std::vector<TypeId> requires;
	};
	using TRegistryMap = std::unordered_map<TypeId, TCreateValue>;

	virtual ~ModuleFactory() = default;

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	template<typename ... Args>
	class Requires {
	public:
		std::vector<TypeId> Get() const {
			std::vector<TypeId> requires;
			(requires.emplace_back(TypeInfo<Base>::template GetTypeId<Args>()), ...);
			return requires;
		}
	};

	template<typename T>
	class Registrar : public Base {
	public:
		/**
		 * Gets the engines instance.
		 * @return The current module instance.
		 */
		static T *Get() { return moduleInstance; }

	protected:
		/**
		 * Creates a new module singleton instance and registers into the module registry map.
		 * @tparam Args Modules that will be initialized before this module.
		 * @return A dummy value in static initialization.
		 */
		template<typename ... Args>
		static bool Register(typename Base::Stage stage, Requires<Args...> &&requires = {}) {
			ModuleFactory::Registry()[TypeInfo<Base>::template GetTypeId<T>()] = {[]() {
				moduleInstance = new T();
				return std::unique_ptr<Base>(moduleInstance);
			}, stage, requires.Get()};
			return true;
		}
		
		inline static T *moduleInstance = nullptr;
	};
};

/**
 * @brief A interface used for defining engine modules.
 */
class ACID_EXPORT Module : public ModuleFactory<Module>, NonCopyable {
public:
	/**
	 * @brief Represents when a module will have <seealso cref="Module#Update()"/> called in the update loop.
	 */
	enum class Stage : uint8_t {
		Never, Always, Pre, Normal, Post, Render
	};

	using StageIndex = std::pair<Stage, std::size_t>;

	virtual ~Module() = default;

	/**
	 * The update function for the module.
	 */
	virtual void Update() = 0;
};

template class ACID_EXPORT TypeInfo<Module>;

class ACID_EXPORT ModuleFilter {
public:
	ModuleFilter() {
		// Include all modules by default.
		include.set();
	}
	
	template<typename T>
	bool Check() const noexcept {
		return include.test(TypeInfo<Module>::GetTypeId<T>());
	}

	bool Check(TypeId typeId) const noexcept {
		return include.test(typeId);
	}

	template<typename T>
	ModuleFilter &Exclude() noexcept {
		include.reset(TypeInfo<Module>::GetTypeId<T>());
		return *this;
	}

	template<typename T>
	ModuleFilter &Include() noexcept {
		include.set(TypeInfo<Module>::GetTypeId<T>());
		return *this;
	}

	ModuleFilter &ExcludeAll() noexcept {
		include.reset();
		return *this;
	}

	ModuleFilter &IncludeAll() noexcept {
		include.set();
		return *this;
	}

private:
	std::bitset<64> include;
};
}
