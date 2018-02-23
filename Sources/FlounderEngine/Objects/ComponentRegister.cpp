#include "ComponentRegister.hpp"

namespace Flounder
{
	std::map<std::string, std::function<Component*()>*> *ComponentRegister::VALUES = new std::map<std::string, std::function<Component*()>*>();

	void ComponentRegister::Register(const std::string &name, std::function<Component *()> *create)
	{
		if (VALUES->find(name) != VALUES->end())
		{
			fprintf(stderr, "Component '%s' is already registered!\n", name.c_str());
			return;
		}

		VALUES->insert(std::make_pair(name, create));
	}

	Component *ComponentRegister::Create(const std::string &name)
	{
		auto found = VALUES->find(name);

		if (found == VALUES->end())
		{
			fprintf(stderr, "Could not find registered componentL '%s'\n", name.c_str());
			return nullptr;
		}

		return (*(*found).second)();
	}
}
