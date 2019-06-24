#include "Json.hpp"

#include "Files/Files.hpp"
#include "Helpers/String.hpp"

namespace acid
{
Json::Json(Metadata *metadata)
{
	AddChildren(metadata, this);
}

bool IsWhitespace(const char c)
{
	return std::string{" \n\r  "}.find(c) != std::string::npos;
}

int32_t NextWhitespace(const std::string &source, int32_t i)
{
	while (i < static_cast<int32_t>(source.length()))
	{
		if (source[i] == '"')
		{
			i++;
			
			while (i < static_cast<int32_t>(source.length()) && (source[i] != '"' || source[i - 1] == '\\'))
			{
				i++;
			}
		}

		if (source[i] == '\'')
		{
			i++;
			
			while (i < static_cast<int32_t>(source.length()) && (source[i] != '\'' || source[i - 1] == '\\'))
			{
				i++;
			}
		}

		if (IsWhitespace(source[i]))
		{
			return i;
		}

		i++;
	}

	return source.length();
}

int32_t SkipWhitespaces(const std::string &source, int32_t i)
{
	while (i < static_cast<int32_t>(source.length()))
	{
		if (!IsWhitespace(source[i]))
		{
			return i;
		}

		i++;
	}

	return -1;
}

void Json::Load(std::istream *inStream)
{
	ClearChildren();
	ClearAttributes();

	std::string tmp;
	std::string source;

	while (std::getline(*inStream, tmp))
	{
		source += tmp;
	}

	std::vector<std::pair<Token, std::string>> tokens;

	source += " ";

	auto index{SkipWhitespaces(source, 0)};
	
	while (index >= 0)
	{
		auto next{NextWhitespace(source, index)};
		auto str{source.substr(index, next - index)};

		size_t k{};

		while (k < str.length())
		{
			if (str[k] == '"')
			{
				auto tmpK{k + 1};

				while (tmpK < str.length() && (str[tmpK] != '"' || str[tmpK - 1] == '\\'))
				{
					tmpK++;
				}

				tokens.emplace_back(Token::String, str.substr(k + 1, tmpK - k - 1));
				k = tmpK + 1;
				continue;
			}
			if (str[k] == '\'')
			{
				auto tmpK{k + 1};

				while (tmpK < str.length() && (str[tmpK] != '\'' || str[tmpK - 1] == '\\'))
				{
					tmpK++;
				}

				tokens.emplace_back(Token::String, str.substr(k + 1, tmpK - k - 1));
				k = tmpK + 1;
				continue;
			}
			if (str[k] == '-' || (str[k] <= '9' && str[k] >= '0'))
			{
				auto tmpK{k};

				if (str[tmpK] == '-')
				{
					tmpK++;
				}

				while (tmpK < str.size() && ((str[tmpK] <= '9' && str[tmpK] >= '0') || str[tmpK] == '.'))
				{
					tmpK++;
				}

				tokens.emplace_back(Token::Number, str.substr(k, tmpK - k));
				k = tmpK;
				continue;
			}
			/*static const std::vector<std::pair<std::string, Token>> MATCHES{std::pair("null", Token::Null),
				std::pair("true", Token::Boolean), std::pair("false", Token::Boolean),
				std::pair(",", Token::Comma), std::pair(":", Token::Colon),
				std::pair("}", Token::CroushClose), std::pair("{", Token::CroushOpen),
				std::pair("]", Token::BracketClose), std::pair("[", Token::BracketOpen)
			};
			bool matched{};
			for (const auto &match : MATCHES)
			{
				if (str[k] == match.first[0] && k + match.first.size() - 1 < str.length() && str.substr(k, match.first.size()) == match.first)
				{
					tokens.emplace_back(match.second, match.first);
					k += match.first.size();
					matched = true;
					break;
				}
			}
			if (matched)
			{
				continue;
			}*/
			if (str[k] == 't' && k + 3 < str.length() && str.substr(k, 4) == "true")
			{
				tokens.emplace_back(Token::Boolean, "true");
				k += 4;
				continue;
			}
			if (str[k] == 'f' && k + 4 < str.length() && str.substr(k, 5) == "false")
			{
				tokens.emplace_back(Token::Boolean, "false");
				k += 5;
				continue;
			}
			if (str[k] == 'n' && k + 3 < str.length() && str.substr(k, 4) == "null")
			{
				tokens.emplace_back(Token::Null, "null");
				k += 4;
				continue;
			}
			if (str[k] == ',')
			{
				tokens.emplace_back(Token::Comma, ",");
				k++;
				continue;
			}
			if (str[k] == '}')
			{
				tokens.emplace_back(Token::CroushClose, "}");
				k++;
				continue;
			}
			if (str[k] == '{')
			{
				tokens.emplace_back(Token::CroushOpen, "{");
				k++;
				continue;
			}
			if (str[k] == ']')
			{
				tokens.emplace_back(Token::BracketClose, "]");
				k++;
				continue;
			}
			if (str[k] == '[')
			{
				tokens.emplace_back(Token::BracketOpen, "[");
				k++;
				continue;
			}
			if (str[k] == ':')
			{
				tokens.emplace_back(Token::Colon, ":");
				k++;
				continue;
			}

			tokens.emplace_back(Token::Unknown, str.substr(k));
			k = str.length();
		}

		index = SkipWhitespaces(source, next);
	}

	int32_t k{};
	Convert(this, tokens, 0, k);
}

void Json::Write(std::ostream *outStream, const Format &format) const
{
	AppendData(this, outStream, 0, format);
}

void Json::Load(const std::string &string)
{
	std::stringstream stream{string};
	Load(&stream);
}

std::string Json::Write(const Format &format) const
{
	std::stringstream stream;
	Write(&stream, format);
	return stream.str();
}

void Json::AddChildren(const Metadata *source, Metadata *destination)
{
	for (const auto &child : source->GetChildren())
	{
		auto created{destination->AddChild(std::make_unique<Metadata>(child->GetName(), child->GetValue()))};
		AddChildren(child.get(), created);
	}

	for (const auto &attribute : source->GetAttributes())
	{
		destination->SetAttribute(attribute.first, attribute.second);
	}
}

Metadata *Json::Convert(Metadata *current, std::vector<std::pair<Token, std::string>> v, const int32_t &i, int32_t &r)
{
	if (v[i].first == Token::CroushOpen)
	{
		auto k{i + 1};

		while (v[k].first != Token::CroushClose)
		{
			auto key = v[k].second;
			k += 2; // k+1 should be ':'
			auto j{k};
			auto vv{current->AddChild(std::make_unique<Metadata>(key))};
			Convert(vv, v, k, j);
			k = j;

			if (v[k].first == Token::Comma)
			{
				k++;
			}
		}

		current->SetType(Type::Object);
		r = k + 1;
		return current;
	}
	if (v[i].first == Token::BracketOpen)
	{
		auto k{i + 1};

		while (v[k].first != Token::BracketClose)
		{
			auto j{k};
			auto vv{current->AddChild(std::make_unique<Metadata>())};
			Convert(vv, v, k, j);
			k = j;
			
			if (v[k].first == Token::Comma)
			{
				k++;
			}
		}

		current->SetType(Type::Array);
		r = k + 1;
		return current;
	}
	if (v[i].first == Token::Number)
	{
		current->SetValue(v[i].second);
		current->SetType(Type::Number);
		r = i + 1;
		return current;
	}
	if (v[i].first == Token::String)
	{
		current->SetValue(v[i].second);
		current->SetType(Type::String);
		r = i + 1;
		return current;
	}
	if (v[i].first == Token::Boolean)
	{
		current->SetValue(v[i].second);
		current->SetType(Type::Boolean);
		r = i + 1;
		return current;
	}
	if (v[i].first == Token::Null)
	{
		current->SetValue(v[i].second);
		current->SetType(Type::Null);
		r = i + 1;
		return current;
	}

	return nullptr;
}

void Json::AppendData(const Metadata *source, std::ostream *outStream, const int32_t &indentation, const Format &format, const bool &end)
{
	std::stringstream indents;

	if (format != Format::Minified)
	{
		for (int32_t i{}; i < indentation; i++)
		{
			indents << "  ";
		}
	}

	auto openBrace{'{'};
	auto closeBrace{'}'};

	for (const auto &child : source->GetChildren())
	{
		if (child->GetName().empty())
		{
			openBrace = '[';
			closeBrace = ']';
			break;
		}
	}

	*outStream << indents.str();

	if (source->GetName().empty() && source->GetValue().empty())
	{
		*outStream << openBrace;
	}
	else if (source->GetValue().empty())
	{
		*outStream << "\"" << source->GetName();
		*outStream << (format != Format::Minified ? "\": " : "\":");
		*outStream << openBrace;
	}
	else
	{
		if (source->GetName().empty())
		{
			*outStream << String::FixReturnTokens(source->GetValue());
		}
		else
		{
			*outStream << "\"" << source->GetName();
			*outStream << (format != Format::Minified ? "\": " : "\":");
			*outStream << String::FixReturnTokens(source->GetValue());
		}

		if (!(end && source->GetAttributes().empty()))
		{
			*outStream << (format != Format::Minified ? ", " : ",");
		}
	}

	if (format != Format::Minified)
	{
		*outStream << '\n';
	}

	for (const auto &attribute : source->GetAttributes())
	{
		*outStream << indents.str() << "  \"_" << attribute.first << "\": \"" << attribute.second << "\"";

		if (!(end && source->GetChildren().empty()))
		{
			*outStream << (format != Format::Minified ? ", " : ",");
		}

		if (format != Format::Minified)
		{
			*outStream << '\n';
		}
	}

	for (const auto &child : source->GetChildren())
	{
		AppendData(child.get(), outStream, indentation + 1, format, child == source->GetChildren().back());
	}

	if (source->GetValue().empty())
	{
		*outStream << indents.str() << closeBrace;

		if (!(end || indentation == 0))
		{
			*outStream << ',';
		}

		if (format != Format::Minified)
		{
			*outStream << '\n';
		}
	}
}
}
