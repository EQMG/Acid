#include "FileXml.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FormatString.hpp"
#include "Helpers/FileSystem.hpp"
#include "XmlNode.hpp"

namespace fl
{
	FileXml::FileXml(const std::string &filename) :
		IFile(),
		m_filename(filename),
		m_parent(new LoadedValue(nullptr, "", ""))
	{
	}

	FileXml::~FileXml()
	{
		delete m_parent;
	}

	void FileXml::Load()
	{
#if FL_VERBOSE
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
		std::string summation = "";
		bool end = false;

		for (auto it = fileLoaded.begin(); it != fileLoaded.end(); ++it)
		{
			switch (*it)
			{
			case '<':
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = std::make_shared<XmlNode>(nullptr, "", "");
					continue;
				}

				if (*(it + 1) == '/') // End tag.
				{
					summation = FormatString::Trim(summation);
					currentSection->SetContent(currentSection->GetContent() + summation);
					end = true;
				}
				else // Start tag.
				{
					auto section = std::make_shared<XmlNode>(currentSection, "", "");
					currentSection->AddChild(section);
					currentSection = section;
				}

				summation.clear();

				break;
			}
			case '>':
			{
			//	if (*(it - 1) == '?') // Prolog.
			//	{
			//		summation.clear();
			//		continue;
			//	}

				if (!end)
				{
					summation = FormatString::Trim(summation);
					currentSection->SetAttributes(currentSection->GetAttributes() + summation);
				}

				summation.clear();

				if (end || *(it - 1) == '/') // End tag.
				{
					end = false;

					if (currentSection->GetParent() != nullptr)
					{
						currentSection = currentSection->GetParent();
					}
				}

				break;
			}
			case '\n':
				break;
			default:
				summation += *it;
				break;
			}
		}

		XmlNode::Convert(*currentSection, m_parent, true);
		currentSection->Clear();

#if FL_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Xml '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileXml::Save()
	{
		std::string data = "";
		XmlNode::AppendData(m_parent, data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
	}

	void FileXml::Clear()
	{
		m_parent->GetChildren().clear();
	}

	std::map<std::string, std::string> FileXml::ConfigReadValues()
	{
		// TODO: Xml config.
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

	void FileXml::ConfigPushValue(const std::string &key, const std::string &value)
	{
		// TODO: Xml config.
		auto exiting = m_parent->GetChild(key, false, false);

		if (exiting != nullptr)
		{
			exiting->SetValue(value);
			return;
		}

		auto newChild = new LoadedValue(m_parent, key, value);
		m_parent->GetChildren().emplace_back(newChild);
	}

	void FileXml::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}