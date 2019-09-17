#pragma once

#include "NodeReturn.hpp"

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
		Number,
		Null,
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

	virtual void LoadString(std::string_view string);
	virtual void WriteStream(std::ostream &stream, Format format = Format::Minified) const;

	template<typename _Elem = char>
	void LoadStream(std::basic_istream<_Elem> &stream) {
		// We must read as UTF8 chars.
		stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<char>));

		// Reading into a string before iterating is much faster.
		std::string s(std::istreambuf_iterator<_Elem>(stream), {});
		LoadString( s);
	}

	template<typename _Elem = char>
	std::basic_string<_Elem> WriteString(Format format = Format::Minified) const {
		std::basic_stringstream<_Elem> stream;
		WriteStream(stream, format);
		return stream.str();
	}
	
	template<typename T>
	T Get() const;
	template<typename T>
	T Get(const T &fallback) const;
	template<typename T>
	void Get(T &dest) const;
	template<typename T, typename K>
	void Get(T &dest, const K &fallback) const;
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

	bool HasProperty(const std::string &name) const;
	NodeReturn GetProperty(const std::string &name) const;
	NodeReturn GetProperty(uint32_t index) const;
	Node &AddProperty();
	Node &AddProperty(const std::string &name, Node &&node = {});
	Node &AddProperty(uint32_t index, Node &&node = {});
	void RemoveProperty(const std::string &name);
	void RemoveProperty(const Node &node);

	NodeReturn operator[](const std::string &key) const;
	NodeReturn operator[](uint32_t index) const;

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
	void SetName(const std::string &name) { m_name = name; }

	const std::string &GetValue() const { return m_value; }
	void SetValue(const std::string &value) { m_value = value; }

	const Type &GetType() const { return m_type; }
	void SetType(Type type) { m_type = type; }

protected:
	std::vector<Node> m_properties;
	std::string m_name;
	std::string m_value;
	Type m_type = Type::Object;
};
}

#include "Node.inl"
#include "NodeReturn.inl"
