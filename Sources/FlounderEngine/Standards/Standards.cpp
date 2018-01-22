#include "Standards.hpp"

namespace Flounder
{
	Standards::Standards() :
		IModule(),
		m_standards(new std::vector<IStandard *>())
	{
	}

	Standards::~Standards()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); ++it)
		{
			delete *it;
		}

		delete m_standards;
	}

	void Standards::AddStandard(IStandard *standard) const
	{
		m_standards->push_back(standard);
	}

	void Standards::Update()
	{
		for (auto it = m_standards->begin(); it < m_standards->end(); ++it)
		{
			(*it)->Update();
		}
	}
}
