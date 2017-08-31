#include "standards.hpp"

namespace Flounder
{
	standards::standards() :
		IModule(),
		m_standards(new std::vector<IStandard*>())
	{
	}

	standards::~standards()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); ++it)
		{
			delete *it;
		}

		delete m_standards;
	}

	void standards::addStandard(IStandard *standard)
	{
		m_standards->push_back(standard);
	}

	void standards::Update()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); ++it)
		{
			(*it)->Update();
		}
	}
}
