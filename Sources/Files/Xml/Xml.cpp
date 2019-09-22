#include "Xml.hpp"

#include "Helpers/String.hpp"

namespace acid {
Xml::Xml(const std::string &rootName) {
	SetName(rootName);
}

Xml::Xml(const std::string &rootName, const Node &node) :
	Node(node) {
	SetName(rootName);
}

Xml::Xml(const std::string &rootName, Node &&node) :
	Node(std::move(node)) {
	SetName(rootName);
}

void Xml::LoadString(std::string_view string) {
}

void Xml::WriteStream(std::ostream &stream, Format format) const {
	stream << R"(<?xml version="1.0" encoding="utf-8"?>)";
	if (format != Format::Minified)
		stream << '\n';
	AppendData(*this, stream, format, 0);
}

void Xml::AddToken(std::string_view view, std::vector<Token> &tokens) {
}

void Xml::Convert(Node &current, const std::vector<Token> &tokens, int32_t i, int32_t &r) {
}

void Xml::AppendData(const Node &source, std::ostream &stream, Format format, int32_t indent) {
	// Creates a string for the indentation level.
	std::string indents(2 * indent, ' ');

	// Only output the value if no properties exist.
	if (source.GetProperties().empty())
		stream << source.GetValue();

	// Output each property.
	for (auto it = source.GetProperties().begin(); it < source.GetProperties().end(); ++it) {
		if (format != Format::Minified)
			stream << indents;
		// Output name for property.
		stream << '<' << it->GetName() << ">";

		if (!it->GetProperties().empty()) {
			if (format != Format::Minified)
				stream << '\n';
		}

		AppendData(*it, stream, format, indent + 1);

		if (!it->GetProperties().empty()) {
			if (format != Format::Minified)
				stream << indents;
		}

		stream << "</" << it->GetName() << ">";
		if (format != Format::Minified)
			stream << '\n';
	}
}
}
