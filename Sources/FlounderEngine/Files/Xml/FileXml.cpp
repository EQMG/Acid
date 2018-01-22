#include "FileXml.hpp"

#include "Helpers/FormatString.hpp"
#include "Helpers/FileSystem.hpp"

namespace Flounder
{
	FileXml::FileXml(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parentNode(nullptr)
	{
	}

	FileXml::~FileXml()
	{
	}

	void FileXml::Load()
	{
		Verify();
		std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n", true);

		for (std::string line : lines)
		{
			/*line = FormatString::Trim(line);

			if (FormatString::StartsWith(line, "</"))
			{
				m_parentNode = nullptr;
			}

			std::vector<std::string> startTagParts = FormatString::Split(GetStartTag(line), " ");
			XmlNode *node = new XmlNode(FormatString::Replace(startTagParts.at(0), "/", ""));
			AddAttributes(startTagParts, node);
			AddData(line, node);

			if (CLOSED.matcher(line).find())
			{
				m_parentNode = node;
			}

			XmlNode child;

			while ((child = LoadNode(reader)) != null) {
				node->AddChild(child);
			}

			m_parentNode = node;*/
		}
	}

	void FileXml::Save()
	{
		std::string data = "";

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
	}

	void FileXml::Clear()
	{

	}

	std::map<std::string, std::string> FileXml::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();
		return result;
	}

	void FileXml::ConfigPushValue(const std::string &key, const std::string &value)
	{
	}

	void FileXml::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}

	std::string FileXml::GetStartTag(const std::string &line)
	{
		//	Matcher match = START_TAG.matcher(line);
		//	match.find();
		//	return match.group(1);
		return "";
	}
}