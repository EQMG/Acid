#include "Config.hpp"

namespace Flounder
{
	Config::Config(const std::string &filename) :
		m_filename(filename)
	{
		ReadConfig();
	}

	Config::~Config()
	{
		WriteConfig();
	}

	void Config::ReadConfig()
	{
		// TODO: Read json into config buffer.
		Action(ActionRead);
	}

	void Config::WriteConfig()
	{
		Action(ActionWrite);
		// TODO: Write config buffer into json.
	}
}
