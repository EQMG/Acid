#include "version.h"

version::version(std::string version)
{
	this->m_version = version;
}

version::~version()
{
	delete &m_version;
}
