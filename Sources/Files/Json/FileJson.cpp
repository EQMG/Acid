#include "FileJson.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FileSystem.hpp"

namespace fl
{
	FileJson::FileJson(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parent(new LoadedValue(nullptr, "", ""))
	{
	}

	FileJson::~FileJson()
	{
		delete m_parent;
	}

	void FileJson::Load()
	{
#if FL_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(m_filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		for (auto &child : *m_parent->GetChildren())
		{
			delete child;
		}

		m_parent->GetChildren()->clear();

		std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		JsonSection *currentSection = nullptr;
		std::string summation;

		for (auto &c : fileLoaded)
		{
			if (c == '{' || c == '[')
			{
				if (currentSection == nullptr)
				{
					currentSection = new JsonSection(nullptr, "", "");
					continue;
				}

				std::string name;

				if (!summation.empty())
				{
					auto contentSplit = FormatString::Split(summation, "\"");

					if ((int) contentSplit.size() - 2 >= 0)
					{
						name = contentSplit.at(contentSplit.size() - 2);
					}
				}

				currentSection->SetContent(currentSection->GetContent() + summation);
				summation.clear();

				auto section = new JsonSection(currentSection, name, "");
				currentSection->AddChild(section);
				currentSection = section;
			}
			else if (c == '}' || c == ']')
			{
				currentSection->SetContent(currentSection->GetContent() + summation);
				summation.clear();

				if (currentSection->GetParent() != nullptr)
				{
					currentSection = currentSection->GetParent();
				}
			}
			else if (c == '\n')
			{
			}
			else
			{
				summation += c;
			}
		}

		JsonSection::Convert(currentSection, m_parent, true);
		delete currentSection;

#if FL_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		printf("Json '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileJson::Save()
	{
		std::string data;
		JsonSection::AppendData(m_parent, &data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
	}

	void FileJson::Clear()
	{
		for (auto &child : *m_parent->GetChildren())
		{
			delete child;
		}

		m_parent->GetChildren()->clear();
	}

	std::map<std::string, std::string> FileJson::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();

		for (auto &child : *m_parent->GetChildren())
		{
			if (child->GetValue().empty())
			{
				continue;
			}

			result.emplace(child->GetName(), child->GetValue());
		}

		return result;
	}

	void FileJson::ConfigPushValue(const std::string &key, const std::string &value)
	{
		LoadedValue *exiting = m_parent->GetChild(key);

		if (exiting != nullptr)
		{
			exiting->SetValue(value);
			return;
		}

		LoadedValue *newChild = new LoadedValue(m_parent, key, value);
		m_parent->GetChildren()->emplace_back(newChild);
	}

	void FileJson::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}