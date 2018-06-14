#include "Files.hpp"

namespace fl
{
	Files::Files() :
		IModule(),
		m_searchPaths(std::vector<std::string>())
	{
	}

	Files::~Files()
	{
	}

	void Files::Update()
	{
	}

	void Files::AddSearchPath(const std::string &path)
	{
		m_searchPaths.emplace_back(path);
	}
}
