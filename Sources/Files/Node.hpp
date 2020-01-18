#pragma once

#include <ostream>

#include "NodeView.hpp"

namespace acid {
/**
 * @brief Class that is used to represent a tree of UFT-8 values, used in serialization.
 */
class ACID_EXPORT Node {
public:
	/**
	 * @brief Class that is used to print a char, and ignore null char.
	 */
	class NullableChar {
	public:
		constexpr NullableChar(char val) : val(val) {}

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
		constexpr Format(int8_t spacesPerIndent, char newLine, char space, bool inlineArrays) :
			m_spacesPerIndent(spacesPerIndent),
			m_newLine(newLine),
			m_space(space),
			m_inlineArrays(inlineArrays) {
		}

		/**
		 * Creates a string for the indentation level.
		 * @param indent The node level to get indentation for.
		 * @return The indentation string.
		 */
		std::string GetIndents(int8_t indent) const {
			return std::string(m_spacesPerIndent * indent, ' ');
		}

		/// Writes a node with full padding.
		ACID_EXPORT static const Format Beautified;
		/// Writes a node with no padding.
		ACID_EXPORT static const Format Minified;

		int8_t m_spacesPerIndent;
		NullableChar m_newLine, m_space;
		bool m_inlineArrays;
	};

	using Type = NodeConstView::Type;

	Node() = default;
	Node(const Node &node) = default;
	Node(Node &&node) = default;
	explicit Node(std::string value, Type type = Type::String);
	Node(std::string value, std::vector<Node> &&properties);

	virtual ~Node() = default;

	virtual void ParseString(std::string_view string);
	virtual void WriteStream(std::ostream &stream, const Format &format = Format::Minified) const;

	template<typename _Elem = char>
	void ParseStream(std::basic_istream<_Elem> &stream);
	template<typename _Elem = char>
	std::basic_string<_Elem> WriteString(const Format &format = Format::Minified) const;

	template<typename T>
	T GetName() const;
	template<typename T>
	void SetName(const T &value);

	template<typename T>
	T Get() const;
	template<typename T>
	T Get(const T &fallback) const;
	template<typename T>
	bool Get(T &dest) const;
	template<typename T, typename K>
	bool Get(T &dest, const K &fallback) const;
	template<typename T>
	void Set(const T &value);

	/**
	 * Clears all properties from this node.
	 */
	void Clear();

	/**
	 * Gets if the node has a value, or has properties that have values.
	 * @return If the node is internally valid.
	 */
	bool IsValid() const;

	template<typename T>
	Node &Append(const T &value);
	template<typename ...Args>
	Node &Append(const Args &...args);

	bool HasProperty(std::string_view name) const;
	NodeConstView GetProperty(std::string_view name) const;
	NodeConstView GetProperty(uint32_t index) const;
	NodeView GetProperty(std::string_view name);
	NodeView GetProperty(uint32_t index);
	Node &AddProperty();
	Node &AddProperty(std::string_view name, Node &&node);
	Node &AddProperty(std::string_view name);
	Node &AddProperty(uint32_t index, Node &&node);
	Node &AddProperty(uint32_t index);
	void RemoveProperty(std::string_view name);
	void RemoveProperty(const Node &node);

	std::vector<NodeConstView> GetProperties(std::string_view name) const;
	NodeConstView GetPropertyWithBackup(std::string_view name, std::string_view backupName) const;
	NodeConstView GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const;
	std::vector<NodeView> GetProperties(std::string_view name);
	NodeView GetPropertyWithBackup(std::string_view name, std::string_view backupName);
	NodeView GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue);

	NodeConstView operator[](std::string_view key) const;
	NodeConstView operator[](uint32_t index) const;
	NodeView operator[](std::string_view key);
	NodeView operator[](uint32_t index);
	
	Node &operator=(const Node &node) = default;
	Node &operator=(Node &&node) = default;
	template<typename T>
	Node &operator=(const T &rhs);

	bool operator==(const Node &other) const;
	bool operator!=(const Node &other) const;
	bool operator<(const Node &other) const;

	const std::vector<Node> &GetProperties() const { return m_properties; }
	std::vector<Node> &GetProperties() { return m_properties; }

	const std::string &GetName() const { return m_name; }
	void SetName(std::string name) { m_name = std::move(name); }

	const std::string &GetValue() const { return m_value; }
	void SetValue(std::string value) { m_value = std::move(value); }

	const Type &GetType() const { return m_type; }
	void SetType(Type type) { m_type = type; }

protected:
	class Token {
	public:
		Token() = default;
		Token(Type type, std::string_view view) :
			type(type),
			view(view) {
		}

		/**
		 * Compares if two tokens have the same type and string contents.
		 * @param other The other token to compare.
		 * @return If the tokens are equal.
		 */
		bool operator==(const Token &other) const {
			return type == other.type && view == other.view.data();
		}

		bool operator!=(const Token &other) const {
			return !operator==(other);
		}

		Type type;
		std::string_view view;
	};
	using Tokens = std::vector<Token>;

	std::vector<Node> m_properties; // members
	std::string m_name; // key
	std::string m_value;
	Type m_type = Type::Object;
};
}

#include "Node.inl"
#include "NodeConstView.inl"
#include "NodeView.inl"
