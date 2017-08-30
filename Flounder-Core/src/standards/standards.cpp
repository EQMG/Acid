#include "standards.hpp"

namespace flounder
{
	standards::standards() :
		imodule(),
		m_standards(new std::vector<istandard*>())
	{
	}

	standards::~standards()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); it++)
		{
			delete *it;
		}

		delete m_standards;
	}

	void standards::addStandard(istandard *standard)
	{
		m_standards->push_back(standard);
	}

	void standards::update()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); it++)
		{
			(*it)->update();
		}
	}
}
