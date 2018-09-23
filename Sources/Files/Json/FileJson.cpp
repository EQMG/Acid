#include "FileJson.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/String.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
	FileJson::FileJson(const std::string &filename) :
		m_filename(filename),
		m_parent(std::make_unique<Metadata>("", ""))
	{
	}

	void FileJson::Load()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::Exists(m_filename) || !FileSystem::IsFile(m_filename))
		{
			Log::Error("File does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		m_parent->ClearChildren();

		auto fileLoaded = FileSystem::ReadTextFile(m_filename);

		if (!fileLoaded)
		{
			return;
		}

		JsonSection *currentSection = nullptr;
		std::stringstream summation;

		for (char &c : *fileLoaded)
		{
			if (c == '{' || c == '[')
			{
				if (currentSection == nullptr)
				{
					currentSection = new JsonSection(nullptr, "", "");
					continue;
				}

				std::string name;

				if (!summation.str().empty())
				{
					auto contentSplit = String::Split(summation.str(), "\"");

					if (static_cast<int32_t>(contentSplit.size()) - 2 >= 0)
					{
						name = contentSplit.at(contentSplit.size() - 2);
					}
				}

				currentSection->SetContent(currentSection->GetContent() + summation.str());
				summation.str(std::string());

				auto section = new JsonSection(currentSection, name, "");
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

		if (currentSection != nullptr)
		{
			JsonSection::Convert(*currentSection, m_parent.get(), true);
		}

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Json '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileJson::Save()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		std::stringstream data;
		JsonSection::AppendData(*m_parent, data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data.str());

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Json '%s' saved in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileJson::Clear()
	{
		m_parent->ClearChildren();
	}

	void FileJson::Verify()
	{
		if (!FileSystem::Exists(m_filename))
		{
			FileSystem::Create(m_filename);
		}
	}
}
