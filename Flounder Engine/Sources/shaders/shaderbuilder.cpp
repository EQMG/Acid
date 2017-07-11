#include "shaderbuilder.h"

namespace flounder {
	shaderbuilder::shaderbuilder(const std::string &name)
	{
		m_name = name;
		m_types = new std::vector<shadertype*>();
	}

	shaderbuilder::~shaderbuilder()
	{
	}

	shaderbuilder *shaderbuilder::addType(shadertype *type)
	{
		m_types->push_back(type);
		return this;
	}
}
