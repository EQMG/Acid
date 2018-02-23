#pragma once

#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT ComponentRegister
	{
	private:
		static std::map<std::string, std::function<Component*()>*> *VALUES;
	public:
#define REGISTER_CREATE(t) new std::function<Component*()>([&]() -> Component* { return static_cast<Component*>(new t()); })

		static void Register(const std::string &name, std::function<Component*()>* create);

		static Component *Create(const std::string &name);
	};
}