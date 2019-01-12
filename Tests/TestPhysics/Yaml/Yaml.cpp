#include "Yaml.hpp"

#include "Helpers/String.hpp"

namespace acid
{
	Yaml::Yaml() :
		Metadata("", "")
	{
	}

	Yaml::Yaml(Metadata *metadata) :
		Metadata("", "")
	{
		AddChildren(metadata, this);
	}

	void Yaml::Load(const std::string &data)
	{
		ClearChildren();
		ClearAttributes();

		std::stringstream summation;

		for (auto it = data.begin(); it != data.end(); ++it)
		{
		}
	}

	std::string Yaml::Write() const
	{
		std::stringstream data;
		data << "---\n";
		AppendData(this, nullptr, data, -1);
		return data.str();
	}

	void Yaml::AddChildren(const Metadata *source, Metadata *destination)
	{
		for (const auto &child : source->GetChildren())
		{
			auto created = destination->AddChild(new Metadata(child->GetName(), child->GetValue()));
			AddChildren(child.get(), created);
		}

		for (const auto &attribute : source->GetAttributes())
		{
			destination->AddAttribute(attribute.first, attribute.second);
		}
	}

	void Yaml::AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		if (!source->GetName().empty())
		{
			builder << indents.str();

			if (parent != nullptr && parent->GetName().empty() && indentation > 0 &&
				parent->GetChildren()[0].get() == source)
			{
				builder.seekp(-2, std::stringstream::cur);
				builder << "- ";
			}

			if (source->GetValue().empty())
			{
				builder << source->GetName() << ": \n";
			}
			else
			{
				builder << source->GetName() << ": " << source->GetValue() << "\n";
			}
		}

		for (const auto &attribute : source->GetAttributes())
		{
			builder << indents.str() << "  _" << attribute.first + ": " << attribute.second << "\n";
		}

		for (const auto &child : source->GetChildren())
		{
			AppendData(child.get(), source, builder, indentation + 1);
		}
	}
}
