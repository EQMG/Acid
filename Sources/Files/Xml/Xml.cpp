#include "Xml.hpp"

#include <sstream>

#include "Utils/Enumerate.hpp"
#include "Utils/String.hpp"

namespace acid {
void Xml::ParseString(Node &node, std::string_view string) {
	// Tokenizes the string view into small views that are used to build a Node tree.
	std::vector<Token> tokens;

	std::size_t tokenStart = 0;
	enum class QuoteState : char {
		None = '\0', Single = '\'', Double = '"'
	} quoteState = QuoteState::None;
	enum class TagState : char {
		None = '\0', Open = '<', Close = '>'
	} tagState = TagState::None;

	// Iterates over all the characters in the string view.
	for (auto &&[index, c] : Enumerate(string)) {
		// If the previous character was a backslash the quote will not break the string.
		if (c == '\'' && quoteState != QuoteState::Double && string[index - 1] != '\\')
			quoteState = quoteState == QuoteState::None ? QuoteState::Single : QuoteState::None;
		else if (c == '"' && quoteState != QuoteState::Single && string[index - 1] != '\\')
			quoteState = quoteState == QuoteState::None ? QuoteState::Double : QuoteState::None;

		// When not reading a string tokens can be found.
		// While in a string whitespace and tokens are added to the strings view.
		if (quoteState == QuoteState::None) {
			if (tagState == TagState::Open && String::IsWhitespace(c)) {
				// On whitespace start save current token.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokenStart = index + 1;
			} else if (c == '<' || c == '>' || (tagState == TagState::Open && (c == '?' || c == '!' || c == '=' || c == '/'))) {
				// Tokens used to read XML nodes.
				AddToken(std::string_view(string.data() + tokenStart, index - tokenStart), tokens);
				tokens.emplace_back(Node::Type::Token, std::string_view(string.data() + index, 1));
				if (c == '<')
					tagState = TagState::Open;
				else if (c == '>')
					tagState = TagState::Close;
				tokenStart = index + 1;
			}
		}
	}

	// Converts the tokens into nodes.
	int32_t k = 0;
	Convert(node, tokens, k);
}

void Xml::WriteStream(const Node &node, std::ostream &stream, Format format) const {
	stream << R"(<?xml version="1.0" encoding="utf-8"?>)" << format.newLine;
	// TODO: Taken from body of AppendData properties loop to write parent node tags.
	stream << '<' << node.GetName();
	for (const auto &property : node.GetProperties()) {
		if (property.GetName().rfind('-', 0) != 0) continue;
		stream << " " << property.GetName().substr(1) << "=\"" << property.GetValue() << "\"";
	}
	stream << '>' << format.newLine;
	AppendData(node, stream, format, 0);
	stream << "</" << node.GetName() << ">";
}

void Xml::AddToken(std::string_view view, std::vector<Token> &tokens) {
	if (view.length() != 0 && !std::all_of(view.cbegin(), view.cend(), String::IsWhitespace))
		tokens.emplace_back(Node::Type::String, view);
}

void Xml::Convert(Node &current, const std::vector<Token> &tokens, int32_t &k) {
	// Only start to parse if we are at the start of a tag.
	if (tokens[k] != Token(Node::Type::Token, "<"))
		return;
	k++;

	// Ignore prolog and DOCTYPE.
	if (tokens[k] == Token(Node::Type::Token, "?") || tokens[k] == Token(Node::Type::Token, "!")) {
		k += 2;
		while (tokens[k] != Token(Node::Type::Token, ">"))
			k++;
		k++;
		Convert(current, tokens, k);
		return;
	}

	// The next token after the open tag is the name.
	auto name = tokens[k].view;
	k++;
	// Create the property that will contain the attributes and children found in the tag.
	auto &property = CreateProperty(current, std::string(name));

	while (tokens[k] != Token(Node::Type::Token, ">")) {
		// Attributes are added as properties.
		if (tokens[k] == Token(Node::Type::Token, "=")) {
			property.AddProperty("-" + std::string(tokens[k - 1].view)) = tokens[k + 1].view.substr(1, tokens[k + 1].view.size() - 2);
			k++;
		}
		k++;
	}
	k++;

	// Inline tag has no children.
	if (tokens[k - 2] == Token(Node::Type::Token, "/"))
		return;
	// Continue through all children until the end tag is found.
	while (!(tokens[k] == Token(Node::Type::Token, "<") && tokens[k + 1] == Token(Node::Type::Token, "/") && tokens[k + 2].view == name)) {
		if (tokens[k].type == Node::Type::String) {
			property = tokens[k].view;
			k++;
		} else {
			// TODO: If the token at k is not a '<' this will cause a infinite loop, or if k + 2 > tokens.size() vector access will be violated.
			Convert(property, tokens, k);
		}
	}
	k += 4;
}

Node &Xml::CreateProperty(Node &current, const std::string &name) {
	// Combine duplicate tags.
	if (auto duplicate = current[name]) {
		// If the node is already an array add the new property to it.
		if (duplicate->GetType() == Node::Type::Array)
			return duplicate->AddProperty();

		// Copy the duplicate node so we can add it to the new array.
		Node original("", duplicate);
		current.RemoveProperty(duplicate);
		auto &array = current.AddProperty(name);
		array.SetType(Node::Type::Array);
		array.AddProperty(std::move(original));
		return array.AddProperty();
	}

	return current.AddProperty(name);
}

void Xml::AppendData(const Node &node, std::ostream &stream, Format format, int32_t indent) {
	stream << node.GetValue();

	auto indents = format.GetIndents(indent + 1);
	// Output each property.
	for (auto it = node.GetProperties().begin(); it < node.GetProperties().end(); ++it) {
		if (it->GetName().rfind('-', 0) == 0) continue;

		// Skip property tag for arrays and move onto appending its properties.
		if (it->GetType() == Node::Type::Array) {
			AppendData(*it, stream, format, indent);
			continue;
		}

		// If the node is an array, then all properties will inherit the array name.
		const auto &name = node.GetType() == Node::Type::Array ? node.GetName() : it->GetName();
		stream << indents << '<' << name;

		// Add attributes to opening tag.
		int attributeCount = 0;
		for (const auto &property : it->GetProperties()) {
			if (property.GetName().rfind('-', 0) != 0) continue;
			stream << " " << property.GetName().substr(1) << "=\"" << property.GetValue() << "\"";
			attributeCount++;
		}

		// When the property has a value or children recursively append them, otherwise shorten tag ending.
		if (it->GetProperties().size() - attributeCount != 0 || !it->GetValue().empty()) {
			stream << '>';

			if (it->GetProperties().size() - attributeCount != 0) {
				stream << format.newLine;
				AppendData(*it, stream, format, indent + 1);
				stream << indents;
			} else {
				AppendData(*it, stream, format, indent + 1);
			}
			stream << "</" << name << '>' << format.newLine;
		} else {
			stream << "/>" << format.newLine;
		}
	}
}
}
