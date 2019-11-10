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

void Json::ParseString(std::string_view string) {
	// Tokenizes the string view into small views that are used to build a Node tree.
	Tokens tokens;

	std::size_t tokenStart = 0;
	enum class QuoteState : uint8_t {
		None = '\0', Single = '\'', Double = '"'
	} quoteState = QuoteState::None;

	// Iterates over all the characters in the string view.
	for (std::size_t index = 0; index < string.length(); ++index) {
		auto c = string[index];
		// If the previous character was a backslash the quote will not break the string.
		if (c == '\'' && quoteState != QuoteState::Double && string[index - 1] != '\\')
			quoteState = quoteState == QuoteState::None ? QuoteState::Single : QuoteState::None;
		else if (c == '"' && quoteState != QuoteState::Single && string[index - 1] != '\\')
			quoteState = quoteState == QuoteState::None ? QuoteState::Double : QuoteState::None;

		// When not reading a string tokens can be found.
		// While in a string whitespace and tokens are added to the strings view.
		if (quoteState == QuoteState::None) {
			if (String::IsWhitespace(c)) {
				// On whitespace start save current token.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokenStart = index + 1;
			} else if (std::string_view(":{},[]").find(c) != std::string::npos) {
				// Tokens used to read json nodes.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokens.emplace_back(Type::Token, std::string_view(string.data() + index, 1));
				tokenStart = index + 1;
			}
		}
	}

	if (tokens.empty())
		throw std::runtime_error("No tokens found in document");

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

void Json::AddToken(std::string_view view, Tokens &tokens) {
	if (view.length() != 0) {
		// Finds the node value type of the string and adds it to the tokens vector.
		if (view == "null") {
			tokens.emplace_back(Type::Null, std::string_view());
		} else if (view == "true" || view == "false") {
			tokens.emplace_back(Type::Boolean, view);
		} else if (String::IsNumber(view)) {
			// This is a quick hack to get if the number is a decimal.
			if (view.find('.') != std::string::npos) {
				if (view.size() >= std::numeric_limits<long double>::digits)
					throw std::runtime_error("Decimal number is too long");
				tokens.emplace_back(Type::Decimal, view);
			} else {
				if (view.size() >= std::numeric_limits<uint64_t>::digits)
					throw std::runtime_error("Integer number is too long");
				tokens.emplace_back(Type::Integer, view);
			}
		} else { // if (view.front() == view.back() == '\"')
			tokens.emplace_back(Type::String, view.substr(1, view.length() - 2));
		}
	}
}

void Json::Convert(Node &current, const Tokens &tokens, int32_t i, int32_t &r) {
	if (tokens[i] == Token(Type::Token, "{")) {
		auto k = i + 1;

		while (tokens[k] != Token(Type::Token, "}")) {
			auto key = tokens[k].view;
			if (k + 2 >= tokens.size())
				throw std::runtime_error("Missing end of {} array");
			if (tokens[k + 1].view != ":")
				throw std::runtime_error("Missing object colon");
			k += 2;
			Convert(current.AddProperty(key), tokens, k, k);
			if (tokens[k].view == ",")
				k++;
		}

		current.SetType(Type::Object);
		r = k + 1;
	} else if (tokens[i] == Token(Type::Token, "[")) {
		auto k = i + 1;

		while (tokens[k] != Token(Type::Token, "]")) {
			if (k >= tokens.size())
				throw std::runtime_error("Missing end of [] object");
			Convert(current.AddProperty(), tokens, k, k);
			if (tokens[k].view == ",")
				k++;
		}

		current.SetType(Type::Array);
		r = k + 1;
	} else {
		std::string str(tokens[i].view);
		if (tokens[i].type == Type::String)
			str = String::UnfixEscapedChars(str);
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
			stream << '\"' << String::FixEscapedChars(source.GetValue()) << '\"';
		else if (source.GetType() == Type::Null)
			stream << "null";
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

		// Shorten primitive array output length.
		if (isArray && format != Format::Minified && !it->GetProperties().empty() && it[0].GetType() != Type::Object) {
			stream << indents << "  ";
			AppendData(*it, stream, format, 0);
			stream << '\n';
		} else {
			AppendData(*it, stream, format, indent + 1);
		}

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
		// No new line if the indent level is zero (if primitive array type).
		if (format != Format::Minified && indent != 0)
			stream << (indent != 0 ? '\n' : ' ');
	}
}
}
