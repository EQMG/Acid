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

	auto tagName = String::ReplaceAll(source.GetName(), " ", "_");

	std::stringstream nameAttributes;
	nameAttributes << tagName;

	for (const auto &property : source.GetProperties()) {
		if (property.GetName().rfind('_', 0) == 0)
			nameAttributes << " " << property.GetName().substr(1) << "=\"" << property.GetValue() << "\"";
	}

	auto nameAndAttribs = String::Trim(nameAttributes.str());

	if (format != Format::Minified)
		stream << indents;

	if (source.GetName()[0] == '?') {
		stream << "<" << nameAndAttribs << "?>";

		if (format != Format::Minified) {
			stream << '\n';
		}

		for (const auto &property : source.GetProperties()) {
			if (property.GetName().rfind('_', 0) != 0)
				AppendData(property, stream, format, indent);
		}

		return;
	}

	if (source.GetProperties().empty() && source.GetValue().empty()) {
		stream << "<" << nameAndAttribs << "/>";

		if (format != Format::Minified) {
			stream << '\n';
		}

		return;
	}

	stream << "<" << nameAndAttribs << ">" << source.GetValue();

	if (!source.GetProperties().empty()) {
		if (format != Format::Minified) {
			stream << '\n';
		}

		for (const auto &property : source.GetProperties()) {
			if (property.GetName().rfind('_', 0) != 0)
				AppendData(property, stream, format, indent + 1);
		}

		stream << indents;
	}

	stream << "</" << tagName << '>';

	if (format != Format::Minified) {
		stream << '\n';
	}
}
}
