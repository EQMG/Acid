#pragma once

#include <unordered_map>
#include <functional>
#include <string>

namespace acid
{
template<typename Base, class... Args>
class Factory
{
public:
	using FuncType = std::function<std::unique_ptr<Base>(Args...)>;
	using RegistryMap = std::unordered_map<std::string, FuncType>;

	static std::unique_ptr<Base> Create(const std::string &name, Args &&... args)
	{
		auto it = Registry().find(name);
		return it == Registry().end() ? nullptr : it->second(std::forward<Args>(args)...);
	}

	static RegistryMap &Registry()
	{
		static RegistryMap impl;
		return impl;
	}

	template<typename T>
	struct Registrar : Base
	{
		static void Register(const std::string &name)
		{
			Factory::Registry()[name] = [](Args... args) -> std::unique_ptr<Base>
			{
				return std::make_unique<T>(std::forward<Args>(args)...);
			};
			registered = name;
		}

		static void Deregister()
		{
			Factory::Registry().erase(registered);
			registered.clear();
		}

		static std::string registered;
	};
};
}
