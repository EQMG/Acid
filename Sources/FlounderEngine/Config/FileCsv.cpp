#include "Config.hpp"

namespace Flounder
{
	FileXml::FileXml(const std::string &filename) :
		m_filename(filename)
	{
		ReadConfig();
	}

	FileXml::~FileXml()
	{
		WriteConfig();
	}

	void FileXml::ReadConfig()
	{
		// TODO: Read json into config buffer.
		Action(ActionRead);
	}

	void FileXml::WriteConfig()
	{
		Action(ActionWrite);
		// TODO: Write config buffer into json.
	}
}
