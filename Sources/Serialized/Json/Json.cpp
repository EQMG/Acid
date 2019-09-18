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
	// Tokenizes the string view into small views that are used to build a Node tree.
	std::vector<Token> tokens;

	std::size_t tokenStart = 0;
	bool inString = false;

	// Iterates over all the characters in the string view.
	for (std::size_t index = 0; index < string.length(); ++index) {
		auto c = string[index];
		// Looks for quotes to start or end a string, if the previous character was a
		// backslash the quote will and not break the string.
		if ((c == '"' || c == '\'') && string[index - 1] != '\\')
			inString ^= 1;

		// When not reading a string tokens can be found.
		// While in a string whitespace and tokens are added to the strings view.
		if (!inString) {
			if (String::IsWhitespace(c)) {
				// On whitespace start save current token.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokenStart = index + 1;
			} else if (std::string_view(":{},[]").find(c) != std::string::npos) {
				// Tokens used to read json nodes.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokens.emplace_back(std::string_view(string.data() + index, 1), Type::Unknown);
				tokenStart = index + 1;
			}
		}
	}

	// Converts the tokens into nodes.
	int32_t k = 0;
	Convert(*this, tokens, 0, k);
}

void Json::WriteStream(std::ostream &stream, Format format) const {
	stream << '{';
	if (format != Format::Minified)
		stream << '\n';
	AppendData(*this, stream, format, 1);
	stream << '}';
}

void Json::AddToken(std::string_view view, std::vector<Token> &tokens) {
	if (view.length() != 0) {
		// Finds the node value type of the string and adds it to the tokens vector.
		if (view == "null") {
			tokens.emplace_back(view, Type::Null);
		} else if (view == "true" || view == "false") {
			tokens.emplace_back(view, Type::Boolean);
		} else if (String::IsNumber(view)) {
			tokens.emplace_back(view, Type::Number);
		} else { // if (view.front() == view.back() == '\"')
			tokens.emplace_back(view.substr(1, view.length() - 2), Type::String);
		}
	}
}

void Json::Convert(Node &current, const std::vector<Token> &tokens, int32_t i, int32_t &r) {
	if (tokens[i].view == "{") {
		auto k = i + 1;

		while (tokens[k].view != "}") {
			std::string key(tokens[k].view);
			k += 2; // k + 1 should be ':'
			Convert(current.AddProperty(key), tokens, k, k);
			if (tokens[k].view == ",")
				k++;
		}

		current.SetType(Type::Object);
		r = k + 1;
	} else if (tokens[i].view == "[") {
		auto k = i + 1;

		while (tokens[k].view != "]") {
			Convert(current.AddProperty(), tokens, k, k);
			if (tokens[k].view == ",")
				k++;
		}

		current.SetType(Type::Array);
		r = k + 1;
	} else {
		std::string str(tokens[i].view);
		if (tokens[i].type == Type::String)
			str = String::UnfixReturnTokens(str);
		current.SetValue(str);
		current.SetType(tokens[i].type);
		r = i + 1;
	}
}

void Json::AppendData(const Node &source, std::ostream &stream, Format format, int32_t indent) {
	// Creates a string for the indentation level.
	std::string indents(2 * indent, ' ');

	// Only output the value if no properties exist.
	if (source.GetProperties().empty()) {
		if (source.GetType() == Type::String)
			stream << '\"' << String::FixReturnTokens(source.GetValue()) << '\"';
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

		AppendData(*it, stream, format, indent + 1);

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
