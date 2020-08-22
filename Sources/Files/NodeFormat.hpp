#pragma once

#include <codecvt>
#include <sstream>

#include "NodeView.hpp"

namespace acid {
/**
 * @brief Class that represents an abstract node format parser and writer.
 */
class ACID_EXPORT NodeFormat {
public:
	class Token {
	public:
		Token() = default;
		Token(NodeType type, std::string_view view) :
			type(type),
			view(view) {
		}

		/**
		 * Compares if two tokens have the same type and string contents.
		 * @param rhs The other token to compare.
		 * @return If the tokens are equal.
		 */
		bool operator==(const Token &rhs) const {
			return type == rhs.type && view == rhs.view.data();
		}

		bool operator!=(const Token &rhs) const {
			return !operator==(rhs);
		}

		NodeType type;
		std::string_view view;
	};

	/**
	 * @brief Class that is used to print a char, and ignore null char.
	 */
	class NullableChar {
	public:
		constexpr NullableChar(char val) :
			val(val) {
		}

		constexpr operator const char &() const noexcept { return val; }

		friend std::ostream &operator<<(std::ostream &stream, const NullableChar &c) {
			if (c.val != '\0') stream << c.val;
			return stream;
		}

		char val;
	};

	/**
	 * @brief Class that represents a configurable output format.
	 */
	class Format {
	public:
		constexpr Format(int8_t spacesPerIndent, NullableChar newLine, NullableChar space, bool inlineArrays) :
			spacesPerIndent(spacesPerIndent),
			newLine(newLine),
			space(space),
			inlineArrays(inlineArrays) {
		}

		/**
		 * Creates a string for the indentation level.
		 * @param indent The node level to get indentation for.
		 * @return The indentation string.
		 */
		std::string GetIndents(int8_t indent) const {
			return std::string(spacesPerIndent * indent, ' ');
		}

		int8_t spacesPerIndent;
		NullableChar newLine, space;
		bool inlineArrays;
	};

	/// Writes a node with full padding.
	inline static const Format Beautified = Format(2, '\n', ' ', true);
	/// Writes a node with no padding.
	inline static const Format Minified = Format(0, '\0', '\0', false);

	virtual ~NodeFormat() = default;
	
	virtual void ParseString(Node &node, std::string_view string) = 0;
	virtual void WriteStream(const Node &node, std::ostream &stream, Format format = Minified) const = 0;

	// TODO: Duplicate ParseStream/WriteString templates from Node.
	template<typename _Elem = char>
	void ParseStream(Node &node, std::basic_istream<_Elem> &stream) {
		// We must read as UTF8 chars.
		if constexpr (!std::is_same_v<_Elem, char>) {
#ifndef ACID_BUILD_MSVC
			throw std::runtime_error("Cannot dynamicly parse wide streams on GCC or Clang");
#else
			stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<char>));
#endif
		}

		// Reading into a string before iterating is much faster.
		std::string s(std::istreambuf_iterator<_Elem>(stream), {});
		return ParseString(node, s);
	}
	
	template<typename _Elem = char>
	std::basic_string<_Elem> WriteString(const Node &node, Format format = Minified) const {
		std::basic_ostringstream<_Elem> stream;
		WriteStream(node, stream, format);
		return stream.str();
	}
};

template<typename T>
class NodeFormatType : public NodeFormat {
public:
	void ParseString(Node &node, std::string_view string) override {
		T::Load(node, string);
	}
	
	void WriteStream(const Node &node, std::ostream &stream, Format format = Minified) const override {
		T::Write(node, stream, format);
	}
};
}
