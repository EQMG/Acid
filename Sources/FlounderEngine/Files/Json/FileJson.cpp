#include "FileJson.hpp"

#include "../../Helpers/HelperString.hpp"
#include "../../Helpers/HelperFile.hpp"

namespace Flounder
{
	FileJson::FileJson(const std::string &filename) :
		IFile(),
		m_filename(filename)
	{
	}

	FileJson::~FileJson()
	{
	}

	void FileJson::Load()
	{
		Verify();
		std::string fileLoaded = HelperFile::ReadTextFile(m_filename);
		std::vector<std::string> lines = HelperString::Split(fileLoaded, "\n", true);

	}

	void FileJson::Save()
	{
		std::string data = "";

		Verify();
		HelperFile::ClearFile(m_filename);
		HelperFile::WriteTextFile(m_filename, data);
	}

	void FileJson::Clear()
	{

	}

	std::map<std::string, std::string> FileJson::ConfigReadValues() 
	{
		auto result = std::map<std::string, std::string>();
		return result;
	}
	
	void FileJson::ConfigPushValue(const std::string &key, const std::string &value) 
	{
	}
	
	void FileJson::Verify()
	{
		if (!HelperFile::FileExists(m_filename))
		{
			HelperFile::CreateFile(m_filename);
		}
	}
}