#pragma once

#include "NodeView.hpp"

namespace acid {
/**
 * @brief Class that is used to represent a tree of UFT-8 values, used in serialization.
 */
class ACID_EXPORT Node {
public:
	enum class Type : uint8_t {
		Object,
		Array,
		String,
		Boolean,
		Integer,
		Decimal,
		Null,
		Token,
		Unknown
	};

	enum class Format : uint8_t {
		Beautified,
		Minified
	};

	Node() = default;
	Node(const Node &node) = default;
	Node(Node &&node) = default;
	explicit Node(std::string value, Type type = Type::String);
	Node(std::string value, std::vector<Node> &&properties);

	virtual ~Node() = default;

	virtual void ParseString(std::string_view string);
	virtual void WriteStream(std::ostream &stream, Format format = Format::Minified) const;

	template<typename _Elem = char>
	void ParseStream(std::basic_istream<_Elem> & stream);
	template<typename _Elem = char>
	std::basic_string<_Elem> WriteString(Format format = Format::Minified) const;
	
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
	 **/
	void Clear();

	/**
	 * Gets if the node has a value, or has properties that have values.
	 * @return If the node is internally valid.
	 **/
	bool IsValid() const;

	template<typename T>
	Node &Append(const T &value);
	template<typename ...Args>
	Node &Append(const Args &...args);

	bool HasProperty(std::string_view name) const;
	NodeView GetProperty(std::string_view name) const;
	NodeView GetProperty(uint32_t index) const;
	Node &AddProperty();
	Node &AddProperty(std::string_view name, Node &&node);
	Node &AddProperty(uint32_t index, Node &&node);
	Node &AddProperty(std::string_view name);
	Node &AddProperty(uint32_t index);
	void RemoveProperty(std::string_view name);
	void RemoveProperty(const Node &node);

	std::vector<NodeView> GetProperties(std::string_view name) const;
	NodeView GetPropertyWithBackup(std::string_view name, std::string_view backupName) const;
	NodeView GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const;

	NodeView operator[](std::string_view key) const;
	NodeView operator[](uint32_t index) const;

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

	std::vector<Node> m_properties;
	std::string m_name; // key
	std::string m_value;
	Type m_type = Type::Object;
};
}

#include "Node.inl"
#include "NodeView.inl"
