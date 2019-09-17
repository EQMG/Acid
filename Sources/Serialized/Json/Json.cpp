#include "Json.hpp"

#include "Helpers/String.hpp"

namespace acid {
Json::Json(const Node &node) :
	Node(node) {
	SetType(Type::Object);
}

Json::Json(Node &&node) :
	Node(std::move(node)) {
	SetType(Type::Object);
}

void Json::LoadString(std::string_view string) {
	std::vector<Token> tokens;

	auto tokenStart = string.data();
	bool inString = false;

	// Read stream until end of file.
	for (auto index = string.data(); index < string.data() + string.length(); ++index) {
		// On start of string switch in/out of stream space and ignore this char.
		// If the previous char was a backslash this char should already be in a string.
		if ((*index == '"' || *index == '\'') && *(index - 1) != '\\')
			inString ^= 1;

		// When not reading a string tokens can be found.
		if (!inString) {
			// Tokens used to read json nodes.
			if (std::string_view(":{},[]").find(*index) != std::string::npos) {
				AddToken(tokenStart, index, tokens);
				tokens.emplace_back(index, index + 1, Type::Unknown);
				tokenStart = index + 1;
				continue;
			}

			// On whitespace save current as a string.
			if (String::IsWhitespace(*index)) {
				AddToken(tokenStart, index, tokens);
				tokenStart = index + 1;
				continue;
			}
		}
	}

	// Converts the list of tokens into nodes.
	int32_t k = 0;
	Convert(*this, tokens, 0, k);
}

void Json::WriteStream(std::ostream &stream, Format format) const {
	if (format == Format::Minified) {
		stream << '{';
	} else {
		stream << "{\n";
	}

	AppendData(*this, stream, 1, format);
	stream << '}';
}

std::string_view GetString(const char *start, const char *end) {
	return std::string_view(start, end - start);
}

std::string FixReturnTokens(const std::string &str) {
	// TODO: Optimize.
	return String::ReplaceAll(String::ReplaceAll(str, "\n", "\\n"), "\r", "\\r");
}

std::string UnfixReturnTokens(const std::string &str) {
	// TODO: Optimize.
	return String::ReplaceAll(String::ReplaceAll(str, "\\n", "\n"), "\\r", "\r");
}

void Json::AddToken(const char *start, const char *end, std::vector<Token> &tokens) {
	if (start != end) {
		auto view = GetString(start, end);
		// Finds the node value type of the string and adds it to the tokens vector.
		if (view == "null") {
			tokens.emplace_back(start, end, Type::Null);
		} else if (view == "true" || view == "false") {
			tokens.emplace_back(start, end, Type::Boolean);
		} else if (String::IsNumber(view)) {
			tokens.emplace_back(start, end, Type::Number);
		} else { // if (view.front() == view.back() == '\"')
			tokens.emplace_back(start + 1, end - 1, Type::String);
		}
	}
}

void Json::Convert(Node &current, const std::vector<Token> &v, int32_t i, int32_t &r) {
	if (v[i].GetChar() == '{') {
		auto k = i + 1;

		while (v[k].GetChar() != '}') {
			auto key = v[k].GetString();
			k += 2; // k + 1 should be ':'
			Convert(current.AddProperty(key), v, k, k);

			if (v[k].GetChar() == ',') {
				k++;
			}
		}

		current.SetType(Type::Object);
		r = k + 1;
	} else if (v[i].GetChar() == '[') {
		auto k = i + 1;

		while (v[k].GetChar() != ']') {
			Convert(current.AddProperty(), v, k, k);

			if (v[k].GetChar() == ',') {
				k++;
			}
		}

		current.SetType(Type::Array);
		r = k + 1;
	} else {
		current.SetValue(UnfixReturnTokens(v[i].GetString()));
		current.SetType(v[i].type);
		r = i + 1;
	}
}

void Json::AppendData(const Node &source, std::ostream &stream, int32_t indentation, Format format) {
	// Creates a string for the indentation level.
	std::string indents(2 * indentation, ' ');

	// Only output the value if no properties exist.
	if (source.GetProperties().empty()) {
		if (source.GetType() == Type::String)
			stream << '\"' << FixReturnTokens(source.GetValue()) << '\"';
		else
			stream << source.GetValue();
	}

	// Output each property.
	for (auto it = source.GetProperties().begin(); it < source.GetProperties().end(); ++it) {
		if (format != Format::Minified)
			stream << indents;

		// Output name for property if it exists.
		if (!it->GetName().empty()) {
			stream << '\"' << it->GetName() << "\":";
			if (format != Format::Minified)
				stream << ' ';
		}

		bool isArray = false;
		if (!it->GetProperties().empty()) {
			// If all properties have no names, then this must be an array.
			for (const auto &property2 : it->GetProperties()) {
				if (property2.GetName().empty()) {
					isArray = true;
					break;
				}
			}

			stream << (isArray ? '[' : '{');
			if (format != Format::Minified)
				stream << '\n';
		} else if (it->GetType() == Type::Object) {
			stream << '{';
		} else if (it->GetType() == Type::Array) {
			stream << '[';
		}

		AppendData(*it, stream, indentation + 1, format);

		if (!it->GetProperties().empty()) {
			if (format != Format::Minified)
				stream << indents;
			stream << (isArray ? ']' : '}');
		} else if (it->GetType() == Type::Object) {
			stream << '}';
		} else if (it->GetType() == Type::Array) {
			stream << ']';
		}
		
		// Separate properties by comma.
		if (it != source.GetProperties().end() - 1)
			stream << ',';

		if (format != Format::Minified)
			stream << '\n';
	}
}
}
