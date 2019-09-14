#pragma once

#include "NodeReturn.hpp"

namespace acid
{
/**
 * @brief Class that is used to represent a tree of values, used in serialization.
 */
class ACID_EXPORT Node
{
public:
	enum class Type
	{
		Object, Array, String, Boolean, Number, Null, Unknown
	};

	enum class Format
	{
		Beautified, Minified
	};

	using Property = std::pair<std::string, Node>;

	Node() = default;

	explicit Node(std::string value, const Type &type = Type::String);

	Node(std::string value, std::vector<Property> &&properties);

	virtual ~Node() = default;

	virtual void Load(std::istream &stream);

	virtual void Write(std::ostream &stream, const Format &format = Format::Beautified) const;

	Node *GetParent() const { return m_parent; }

	void Remove();

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
	 * Gets if the node has a value, or has properties that have values.
	 * @return If the node is internally valid.
	 **/
	bool IsValid() const;

	/**
	 * Gets the internally stored value.
	 * @return The value.
	 **/
	std::string GetValue() const { return m_value; }

	/**
	 * Sets the internally stored value.
	 * @param value The new value.
	 **/
	void SetValue(const std::string &value) { m_value = value; }

	const Type &GetType() const { return m_type; }

	void SetType(const Type &type) { m_type = type; }

	std::string GetName() const;

	void SetName(const std::string &name);

	template<typename T>
	Node &Append(T value);

	template<typename ...Args>
	Node &Append(Args ... args);

	bool HasProperty(const std::string &name) const;

	NodeReturn GetProperty(const std::string &name) const;

	NodeReturn GetProperty(uint32_t index) const;

	Node &AddProperty();

	Node &AddProperty(const std::string &name, Node &&node);

	Node &AddProperty(uint32_t index, Node &&node);

	void RemoveProperty(const std::string &name);

	void RemoveProperty(const Node &node);

	const std::vector<Property> &GetProperties() const { return m_properties; };

	void ClearProperties() { m_properties.clear(); }

	template <typename T>
	Node &operator=(const T &rhs);

	NodeReturn operator[](const std::string &key) const;

	NodeReturn operator[](uint32_t index) const;

	bool operator==(const Node &other) const;

	bool operator!=(const Node &other) const;

	bool operator<(const Node &other) const;

protected:
	Node *m_parent = nullptr;

	std::string m_value;
	Type m_type = Type::Object;
	std::vector<Property> m_properties;
};
}

#include "Node.inl"
#include "NodeReturn.inl"
