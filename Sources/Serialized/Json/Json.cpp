#include "Json.hpp"

#include "Helpers/String.hpp"

// TODO: Fix loading and writing chars like /n, /", ... \"http://google.com\"

namespace acid
{
Json::Json(const Node &node) :
	Node{node}
{
	SetType(Type::Object);
}

void Json::Load(std::istream &stream)
{
	std::vector<std::pair<Type, std::string>> tokens;

	std::string current;
	bool inString{};

	// Reading into a string before iterating is much faster.
	std::string s{std::istreambuf_iterator<char>{stream}, {}};

	// Read stream until end of file.
	for (const auto &c : s)
	{
		// On start of string switch in/out of stream space and ignore this char.
		if (c == '"' || c == '\'')
		{			 
			inString ^= 1;
			//continue;
		}

		// When not reading a string tokens can be found.
		if (!inString)
		{
			// Tokens used to read json nodes.
			if (std::string{":{},[]"}.find(c) != std::string::npos)
			{
				AddToken(tokens, current);
				tokens.emplace_back(Type::Unknown, std::string{c});
				continue;
			}

			// On whitespace save current as a string.
			if (String::IsWhitespace(c))
			{
				AddToken(tokens, current);
				continue;
			}
		}

		// Add this char to the current builder stream.
		current += c;
	}

	// Converts the list of tokens into nodes.
	int32_t k{};
	Convert(*this, tokens, 0, k);
}

void Json::Write(std::ostream &stream, const Format &format) const
{
	// Json files are wrapped with an extra set of braces.
	AppendData({"", {{"", *this}}}, stream, 0, format);
}

void Json::Load(const std::string &string)
{
	std::stringstream stream{string};
	Load(stream);
}

std::string Json::Write(const Format &format) const
{
	std::stringstream stream;
	Write(stream, format);
	return stream.str();
}

void Json::AddToken(std::vector<std::pair<Type, std::string>> &tokens, std::string &current)
{
	if (!current.empty())
	{
		// Finds the node value type of the string and adds it to the tokens vector.
		if (String::IsNumber(current))
		{
			tokens.emplace_back(Type::Number, current);
		}
		else if (current == "null")
		{
			tokens.emplace_back(Type::Null, current);
		}
		else if (current == "true" || current == "false")
		{
			tokens.emplace_back(Type::Boolean, current);
		}
		else // if (current.front() == current.back() == '\"')
		{
			tokens.emplace_back(Type::String, current.substr(1, current.size() - 2));
		}
	}

	// Clears the current summation stream.
	current.clear();
}

void Json::Convert(Node &current, const std::vector<std::pair<Type, std::string>> &v, const int32_t &i, int32_t &r)
{
	if (v[i].second == "{")
	{
		auto k{i + 1};

		while (v[k].second != "}")
		{
			auto key{v[k].second};
			k += 2; // k + 1 should be ':'
			Convert(current.AddProperty(key, {}), v, k, k);

			if (v[k].second == ",")
			{
				k++;
			}
		}

		current.SetType(Type::Object);
		r = k + 1;
	}
	else if (v[i].second == "[")
	{
		auto k{i + 1};

		while (v[k].second != "]")
		{
			Convert(current.AddProperty(), v, k, k);

			if (v[k].second == ",")
			{
				k++;
			}
		}

		current.SetType(Type::Array);
		r = k + 1;
	}
	else
	{
		current.SetValue(String::UnfixReturnTokens(v[i].second));
		current.SetType(v[i].first);
		r = i + 1;
	}
}

void Json::AppendData(const Node &source, std::ostream &stream, const int32_t &indentation, const Format &format)
{
	// Creates a string for the indentation level.
	std::string indents(2 * indentation, ' ');

	// Only output the value if no properties exist.
	if (source.GetProperties().empty())
	{
		auto value{String::FixReturnTokens(source.GetValue())};

		if (source.GetType() == Type::String)
		{
			stream << '\"' << value << '\"';
		}
		else
		{
			stream << value;
		}
	}

	// Output each property.
	for (auto it{source.GetProperties().begin()}; it < source.GetProperties().end(); ++it)
	{
		std::string openString;
		std::string closeString;

		// Gets array or object type braces.
		if (!it->second.GetProperties().empty())
		{
			openString = "{";
			closeString = "}";

			// Sets the braces to an array style if all properties have no names.
			for (const auto &[propertyName2, property2] : it->second.GetProperties())
			{
				if (propertyName2.empty())
				{
					openString = "[";
					closeString = "]";
					break;
				}
			}

			if (format != Format::Minified)
			{
				openString += '\n';
				closeString.insert(0, indents);
			}
		}
		else if (it->second.GetType() == Type::Object)
		{
			openString = "{";
			closeString = "}";
		}
		else if (it->second.GetType() == Type::Array)
		{
			openString = "[";
			closeString = "]";
		}

		// Separate properties by comma.
		if (it != source.GetProperties().end() - 1)
		{
			closeString += ',';
		}

		if (format != Format::Minified)
		{
			stream << indents;
		}

		// Output name for property if it exists.
		if (!it->first.empty())
		{
			stream << "\"" << it->first << "\":";

			if (format != Format::Minified)
			{
				stream << ' ';
			}
		}

		// Appends the current stream with the property data.
		stream << openString;
		AppendData(it->second, stream, indentation + 1, format);
		stream << closeString;

		if (format != Format::Minified)
		{
			stream << '\n';
		}
	}
}
}
