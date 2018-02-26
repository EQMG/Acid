#include "FileJson.hpp"

#include "../../Helpers/FileSystem.hpp"

namespace Flounder
{
	FileJson::FileJson(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parent(nullptr)
	{
	}

	FileJson::~FileJson()
	{
		delete m_parent;
	}

	void FileJson::Load()
	{
		Verify();
		std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n", true);

		int bracketLevel = -1;
		std::string content;
		LoadedSection *loadedParent = new LoadedSection(nullptr, "", "");
		LoadedSection *currentSection = loadedParent;

		for (auto &line : lines)
		{
			for (auto &c : line)
			{
				if (c == '{')
				{
					bracketLevel++;

					if (bracketLevel == 0)
					{
						continue;
					}

					std::string name;

					if (!content.empty())
					{
						auto contentSplit = FormatString::Split(content, "\"");

						if (contentSplit.size() - 2 >= 0)
						{
							name = contentSplit.at(contentSplit.size() - 2);
						}
					}

					content.clear();
					currentSection->m_content = FormatString::Replace(currentSection->m_content, "\"" + name + "\":", "");

					auto section = new LoadedSection(currentSection, name, "");
					currentSection->m_children.push_back(section);
					currentSection = section;
				}
				else if (c == '}')
				{
					currentSection = currentSection->m_parent;
					bracketLevel--;
					content.clear();
				}
				else
				{
					content += c;
					currentSection->m_content += c;
				}
			}
		}

		m_parent = new ValueJson(nullptr, "", "");
		loadedParent->Covert(m_parent);

		printf("Roughness: %s\n", m_parent->Get("Roughness").c_str());
	}

	void FileJson::Save()
	{
		std::string data = "";



		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
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
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}