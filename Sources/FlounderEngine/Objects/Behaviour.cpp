#include "Behaviour.hpp"

namespace Flounder
{
	Behaviour::Behaviour(const bool &enabled) :
		Component(),
		m_enabled(false)
	{
		SetEnabled(enabled);
	}

	Behaviour::~Behaviour()
	{
	}

	void Behaviour::Update()
	{
	}

	void Behaviour::Load(LoadedValue *value)
	{
	}

	void Behaviour::Write(LoadedValue *value)
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
