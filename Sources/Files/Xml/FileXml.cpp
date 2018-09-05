#include "FileXml.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FileSystem.hpp"
#include "XmlNode.hpp"

namespace acid
{
	FileXml::FileXml(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parent(std::make_shared<Metadata>("?xml", "", std::map<std::string, std::string>{{"version",  "1.0"}, {"encoding", "utf-8"}}))
	{
	}

	FileXml::~FileXml()
	{
	}

	void FileXml::Load()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(m_filename))
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

		XmlNode *currentSection = nullptr;
		std::stringstream summation;
		bool end = false;

		for (auto it = fileLoaded->begin(); it != fileLoaded->end(); ++it)
		{
			if (*it == '<')
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = new XmlNode(nullptr, "", "");
					continue;
				}

				if (*(it + 1) == '/') // End tag.
				{
					currentSection->SetContent(currentSection->GetContent() + summation.str());
					end = true;
				}
				else // Start tag.
				{
					auto section = new XmlNode(currentSection, "", "");
					currentSection->AddChild(section);
					currentSection = section;
				}

				summation.str(std::string());
			}
			else if (*it == '>')
			{
				if (!end)
				{
					currentSection->SetAttributes(currentSection->GetAttributes() + summation.str());
				}

				summation.str(std::string());

				if (end || *(it - 1) == '/') // End tag.
				{
					end = false;

					if (currentSection->GetParent() != nullptr)
					{
						currentSection = currentSection->GetParent();
					}
				}
			}
			else if (*it == '\n')
			{
			}
			else
			{
				summation << *it;
			}
		}

		XmlNode::Convert(*currentSection, m_parent, true);
		delete currentSection;

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Xml '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileXml::Save()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		std::stringstream data;
		XmlNode::AppendData(m_parent, data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data.str());

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Xml '%s' saved in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileXml::Clear()
	{
		m_parent->ClearChildren();
	}

	void FileXml::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}
