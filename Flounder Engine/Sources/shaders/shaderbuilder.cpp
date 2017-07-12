#include "shaderbuilder.h"

namespace flounder {
	shaderbuilder::shaderbuilder()
	{
		m_types = new std::vector<shadertype*>();
	}

	shaderbuilder::~shaderbuilder()
	{
	}

	shaderbuilder *shaderbuilder::addName(const std::string &name)
	{
		m_name = name;
		return this;
	}

	shaderbuilder *shaderbuilder::addType(shadertype *type)
	{
		m_types->push_back(type);
		return this;
	}
}
