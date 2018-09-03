#include "Config.hpp"

namespace acid
{
	Config::Config(const std::shared_ptr<IFile> &file) :
		m_file(file)
	{
	}

	Config::~Config()
	{
	}

	void Config::Load()
	{
		m_file->Load();
		Decode(*m_file->GetParent());
	}

	void Config::Save()
	{
		Encode(*m_file->GetParent());
		m_file->Save();
	}
}
