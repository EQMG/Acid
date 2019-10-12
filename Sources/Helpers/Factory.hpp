#pragma once

#include "Engine/Log.hpp"

namespace acid {
template<typename Base, class... Args>
class Factory {
public:
	using TCreateReturn = std::unique_ptr<Base>;

	using TCreateMethod = std::function<TCreateReturn(Args...)>;
	using TRegistryMap = std::unordered_map<std::string, TCreateMethod>;

	static TCreateReturn Create(const std::string &name, Args &&... args) {
		auto it = Registry().find(name);
		if (it == Registry().end()) {
			Log::Error("Failed to create ", std::quoted(name), " from factory\n");
			return nullptr;
		}
		return it->second(std::forward<Args>(args)...);
	}

	static TRegistryMap &Registry() {
		static TRegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar : public Base {
	protected:
		static bool Register(const std::string &name) {
			Factory::Registry()[name] = [](Args... args) -> TCreateReturn {
				return std::make_unique<T>(std::forward<Args>(args)...);
			};
			return true;
		}
	};
};
}
