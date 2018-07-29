#include "FileXml.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FileSystem.hpp"
#include "XmlNode.hpp"

namespace acid
{
	FileXml::FileXml(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parent(new LoadedValue(nullptr, "?xml", "", {{"version",  "1.0"},
													   {"encoding", "utf-8"}}))
	{
	}

	FileXml::~FileXml()
	{
		delete m_parent;
	}

	void FileXml::Load()
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

		std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::shared_ptr<XmlNode> currentSection = nullptr;
		std::stringstream summation;
		bool end = false;

		for (auto it = fileLoaded.begin(); it != fileLoaded.end(); ++it)
		{
			if (*it == '<')
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = std::make_shared<XmlNode>(nullptr, "", "");
					continue;
				}

				if (*(it + 1) == '/') // End tag.
				{
					currentSection->SetContent(currentSection->GetContent() + summation.str());
					end = true;
				}
				else // Start tag.
				{
					auto section = std::make_shared<XmlNode>(currentSection, "", "");
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
		currentSection->Clear();

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Xml '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
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
		fprintf(stdout, "Xml '%s' saved in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileXml::Clear()
	{
		m_parent->GetChildren().clear();
	}

	std::map<std::string, std::string> FileXml::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();

		for (auto &child : m_parent->GetChild("Configuration", true)->GetChildren())
		{
			if (child->GetValue().empty())
			{
				continue;
			}

			result.emplace(child->GetName(), child->GetValue());
		}

		return result;
	}

	void FileXml::ConfigPushValue(const std::string &key, const std::string &value)
	{
		std::string keyNoSpaces = FormatString::Replace(key, " ", "_");
		auto child = m_parent->GetChild("Configuration", true);
		auto exiting = child->GetChild(keyNoSpaces, false, false);

		if (exiting != nullptr)
		{
			exiting->SetValue(value);
			return;
		}

		auto newChild = new LoadedValue(child, keyNoSpaces, value);
		child->GetChildren().emplace_back(newChild);
	}

	void FileXml::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}