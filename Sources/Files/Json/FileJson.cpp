#include "FileJson.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
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
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(m_filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		m_parent->GetChildren().clear();

		auto fileLoaded = FileSystem::ReadTextFile(m_filename);

		if (!fileLoaded.has_value())
		{
			return;
		}

		std::shared_ptr<JsonSection> currentSection = nullptr;
		std::stringstream summation;

		for (char &c : fileLoaded.value())
		{
			if (c == '{' || c == '[')
			{
				if (currentSection == nullptr)
				{
					currentSection = std::make_shared<JsonSection>(nullptr, "", "");
					continue;
				}

				std::string name;

				if (!summation.str().empty())
				{
					auto contentSplit = FormatString::Split(summation.str(), "\"");

					if (static_cast<int>(contentSplit.size()) - 2 >= 0)
					{
						name = contentSplit.at(contentSplit.size() - 2);
					}
				}

				currentSection->SetContent(currentSection->GetContent() + summation.str());
				summation.str(std::string());

				auto section = std::make_shared<JsonSection>(currentSection, name, "");
				currentSection->AddChild(section);
				currentSection = section;
			}
			else if (c == '}' || c == ']')
			{
				currentSection->SetContent(currentSection->GetContent() + summation.str());
				summation.str(std::string());

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
				summation << c;
			}
		}

		JsonSection::Convert(*currentSection, m_parent, true);
		currentSection->Clear();

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Json '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileJson::Save()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		std::stringstream data;
		JsonSection::AppendData(m_parent, data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data.str());

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Json '%s' saved in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileJson::Clear()
	{
		m_parent->GetChildren().clear();
	}

	std::map<std::string, std::string> FileJson::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();

		for (auto &child : m_parent->GetChildren())
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
		auto exiting = m_parent->GetChild(key, false, false);

		if (exiting != nullptr)
		{
			exiting->SetValue(value);
			return;
		}

		auto newChild = new LoadedValue(m_parent, key, value);
		m_parent->GetChildren().emplace_back(newChild);
	}

	void FileJson::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}