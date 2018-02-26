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
		JsonSection *loadedParent = new JsonSection(nullptr, "", "");
		JsonSection *currentSection = loadedParent;

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

						if ((int)contentSplit.size() - 2 >= 0)
						{
							name = contentSplit.at(contentSplit.size() - 2);
						}
					}

					content.clear();

					auto section = new JsonSection(currentSection, name, "");
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

		m_parent = loadedParent->Convert(nullptr);
		delete loadedParent;
	}

	void FileJson::Save()
	{
		std::string data;
		m_parent->AppendData(&data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
	}

	void FileJson::Clear()
	{
		for (auto child : m_parent->m_children)
		{
			delete child;
		}

		m_parent->m_children.clear();
	}

	std::map<std::string, std::string> FileJson::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();

		for (auto child : m_parent->m_children)
		{
			if (child->m_value.empty())
			{
				continue;
			}

			result.insert(std::make_pair(child->m_name, child->m_value));
		}

		return result;
	}

	void FileJson::ConfigPushValue(const std::string &key, const std::string &value)
	{
		ValueJson *exiting = m_parent->GetChild(key);

		if (exiting != nullptr)
		{
			exiting->m_value = value;
			return;
		}

		m_parent->m_children.push_back(new ValueJson(m_parent, key, value));
	}

	void FileJson::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}