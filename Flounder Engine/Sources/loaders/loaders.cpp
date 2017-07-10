#include "loaders.h"

namespace flounder {
	loaders::loaders()
		: imodule()
	{
		m_loaded = new std::map<GLuint*, std::vector<int>*>();
	}

	loaders::~loaders()
	{
	}

	void loaders::init()
	{
	}

	void loaders::update()
	{
	}
}