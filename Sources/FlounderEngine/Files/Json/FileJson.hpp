#pragma once

#include <string>
#include <utility>
#include <vector>
#include "../../Helpers/FormatString.hpp"
#include "../IFile.hpp"

namespace Flounder
{
	class F_EXPORT ValueJson
	{
	public:
		ValueJson *m_parent;
		std::vector<ValueJson*> m_children;

		std::string m_name;
		std::string m_value;

		ValueJson(ValueJson *parent, const std::string &name, const std::string &value) :
			m_parent(parent),
			m_name(FormatString::RemoveAll(name, '\"')),
			m_value(value)
		{
		}

		ValueJson *GetChild(const std::string &name)
		{
			for (auto child : m_children)
			{
				if (child->m_name == name)
				{
					return child;
				}
			}

			return nullptr;
		}

		std::string GetRaw() const { return m_value; }

		void SetRaw(const std::string &data) { m_value = data; }

		template<typename T>
		T Get()
		{
			std::string data = GetRaw();
			return FormatString::ConvertTo<T>(data);
		}

		template<typename T>
		void Set(const T &data)
		{
			SetRaw(std::to_string(data));
		}

		void AppendData(std::string *data, const int &indentation, const bool &end = false)
		{
			std::string indent;

			for (int i = 0; i < indentation; i++)
			{
				indent += "  ";
			}

			*data += indent;

			if (m_name.empty())
			{
				*data += "{\n";
			}
			else if (m_value.empty())
			{
				*data += "\"" + m_name + "\": {\n";
			}
			else
			{
				*data += "\"" + m_name + "\": " + m_value;

				if (!end)
				{
					*data += ", ";
				}

				*data += "\n";
			}

			for (auto child : m_children)
			{
				child->AppendData(data, indentation + 1, child == m_children.back());
			}

			if (m_name.empty())
			{
				*data += indent;
				*data += "}\n";
			}
			else if (m_value.empty())
			{
				*data += indent;

				if (end)
				{
					*data += "}\n";
				}
				else
				{
					*data += "},\n";
				}
			}
		}
	};

	class F_EXPORT JsonSection
	{
	public:
		JsonSection *m_parent;
		std::vector<JsonSection*> m_children;

		std::string m_name;
		std::string m_content;

		JsonSection(JsonSection *parent, const std::string &name, const std::string &content) :
			m_parent(parent),
			m_name(name),
			m_content(content)
		{
		}

		~JsonSection()
		{
			for (auto child : m_children)
			{
				delete child;
			}
		}

		ValueJson *Convert(ValueJson *destination)
		{
			ValueJson *thisValue;

			if (destination != nullptr)
			{
				thisValue = new ValueJson(destination, m_name, "");
				destination->m_children.push_back(thisValue);

				auto contentSplit = FormatString::Split(m_content, ",");

				for (auto data : contentSplit)
				{
					auto dataSplit = FormatString::Split(data, ":", true);

					if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
					{
						continue;
					}

					ValueJson *newChild = new ValueJson(thisValue, dataSplit.at(0), dataSplit.at(1));
					thisValue->m_children.push_back(newChild);
				}
			}
			else
			{
				destination = new ValueJson(nullptr, "", "");
				thisValue = destination;
			}

			for (auto child : m_children)
			{
				child->Convert(thisValue);
			}

			return thisValue;
		}
	};

	class F_EXPORT FileJson :
		public IFile
	{
	public://private:
		std::string m_filename;
		ValueJson *m_parent;
	public:
		FileJson(const std::string &filename);

		~FileJson();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::map<std::string, std::string> ConfigReadValues() override;

		void ConfigPushValue(const std::string &key, const std::string &value) override;

		ValueJson *GetChild(const std::string &name) const { return m_parent->GetChild(name); }
	private:
		void Verify();
	};
}