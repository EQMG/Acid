#pragma once

#include <string>
#include <utility>
#include <vector>
#include "../../Helpers/FormatString.hpp"
#include "../IFile.hpp"

namespace Flounder
{
	enum TypeJson
	{
		JsonNone = 0,
		JsonObject = 1,
		JsonArray = 2,
		JsonInteger = 3,
		JsonDouble = 4,
		JsonString = 5,
		JsonBool = 6,
		JsonNull = 7
	};

	class F_EXPORT ValueJson
	{
	public:
		ValueJson *m_parent;
		std::vector<ValueJson*> m_children;

		std::string m_name;
		std::string m_value;
		TypeJson m_type;

		ValueJson(ValueJson *parent, const std::string &name, const std::string &value, const TypeJson &type = JsonNull) :
			m_parent(parent),
			m_name(FormatString::RemoveAll(name, '\"')),
			m_value(value),
			m_type(type)
		{
		}

		std::string Get(const std::string &name)
		{
			if (m_name == name)
			{
				return m_value;
			}

			for (auto child : m_children)
			{
				std::string value = child->Get(name);

				if (!value.empty())
				{
					return value;
				}
			}

			return "";
		}
	};

	class F_EXPORT LoadedSection
	{
	public:
		LoadedSection *m_parent;
		std::vector<LoadedSection*> m_children;

		std::string m_name;
		std::string m_content;

		LoadedSection(LoadedSection *parent, const std::string &name, const std::string &content) :
			m_parent(parent),
			m_name(name),
			m_content(content)
		{
		}

		void Covert(ValueJson *destination)
		{
			ValueJson *value = new ValueJson(destination, m_name, "");
			destination->m_children.push_back(value);

			auto contentSplit = FormatString::Split(m_content, ",");

			for (auto data : contentSplit)
			{
				auto dataSplit = FormatString::Split(data, ":", true);

				if (dataSplit.size() != 2)
				{
					continue;
				}

				ValueJson *newChild = new ValueJson(value, dataSplit.at(0), dataSplit.at(1));
				value->m_children.push_back(newChild);
			}

			for (auto child : m_children)
			{
				child->Covert(value);
			}
		}
	};

	class F_EXPORT FileJson :
		public IFile
	{
	private:
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

	private:
		void Verify();
	};
}