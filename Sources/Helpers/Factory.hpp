#pragma once

#include "StdAfx.hpp"

namespace acid {
template<typename Base, class... Args>
class Factory {
public:
	using FuncType = std::function<std::unique_ptr<Base>(Args...)>;
	using RegistryMap = std::unordered_map<std::string, FuncType>;

	static std::unique_ptr<Base> Create(const std::string &name, Args &&... args) {
		auto it = Registry().find(name);
		return it == Registry().end() ? nullptr : it->second(std::forward<Args>(args)...);
	}

	static RegistryMap &Registry() {
		static RegistryMap impl;
		return impl;
	}

	template<typename T>
	class Registrar : public Base {
	protected:
		static bool Register(const std::string &name, std::function<std::unique_ptr<T>(Args...)> f = &std::make_unique<T>) {
			Factory::Registry()[name] = [f](Args... args) -> std::unique_ptr<Base> {
				return f(std::forward<Args>(args)...);
			};
			return true;
		}
	};
};
}
