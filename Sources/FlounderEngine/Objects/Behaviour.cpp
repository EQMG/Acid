#include "Behaviour.hpp"

namespace Flounder
{
	Behaviour::Behaviour() :
		Component(),
		m_enabled(false)
	{
	}

	Behaviour::~Behaviour()
	{
	}

	void Behaviour::Update()
	{
	}

	void Behaviour::OnEnable()
	{
	}

	void Behaviour::OnDisable()
	{
	}

	void Behaviour::SetEnabled(const bool &enable)
	{
		if (m_enabled && !enable)
		{
			OnDisable();
		}
		else if (!m_enabled && enable)
		{
			OnEnable();
		}

		m_enabled = enable;
	}
}
