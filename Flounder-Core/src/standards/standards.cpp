#include "standards.h"

namespace flounder
{
	standards::standards() :
		imodule()
	{
		m_standards = new std::vector<istandard*>();
	}

	standards::~standards()
	{
		for (std::vector<istandard*>::iterator it = m_standards->begin(); it < m_standards->end(); it++)
		{
			delete *it;
		}

		delete m_standards;
	}

	void standards::addStandard(istandard *standard)
	{
		m_standards->push_back(standard);
	}

	void standards::init()
	{
	}

	void standards::update()
	{
		for (std::vector<istandard*>::iterator it = m_standards->begin(); it < m_standards->end(); it++)
		{
			(*it)->update();
		}
	}
}
