#include "Resources.hpp"

namespace Flounder
{
	Resources::Resources() :
		m_managed(new std::vector<IResource *>())
	{
	}

	Resources::~Resources()
	{
		for (auto managed : *m_managed)
		{
			delete managed;
		}

		delete m_managed;
	}

	void Resources::Update()
	{
	}

	IResource *Resources::Get(const std::string &filename)
	{
		for (auto managed : *m_managed)
		{
			if (managed != nullptr && managed->GetFilename() == filename)
			{
				return managed;
			}
		}

		return nullptr;
	}

	void Resources::Add(IResource *managed)
	{
		m_managed->push_back(managed);
	}

	void Resources::Remove(IResource *managed)
	{
	}

	void Resources::Remove(const std::string &filename)
	{
	}
}
