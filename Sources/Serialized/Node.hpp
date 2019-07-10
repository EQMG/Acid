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
		String, Object, Array, Boolean, Number, Null, Unknown
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

	template<typename T>
	T GetValue() const;

	template<typename T>
	void SetValue(const T &value);

	const Type &GetType() const { return m_type; }

	void SetType(const Type &type) { m_type = type; }

	Node *GetParent() const { return m_parent; }

	std::string GetName() const;

	void SetName(const std::string &name);

	template<typename T>
	Node &Append(T value);

	template<typename ...Args>
	Node &Append(Args ... args);

	bool HasProperty(const std::string &name) const;

	NodeReturn GetProperty(const std::string &name) const;

	NodeReturn GetProperty(const uint32_t &index) const;

	Node &AddProperty(const std::string &name = "", Node &&node = {});

	Node &AddProperty(const uint32_t &index, Node &&node = {});

	void RemoveProperty(const std::string &name);

	const std::vector<Property> &GetProperties() const { return m_properties; };

	void ClearProperties();

	template <typename T>
	Node &operator=(const T &rhs);

	NodeReturn operator[](const std::string &key) const;

	NodeReturn operator[](const uint32_t &index) const;

	bool operator==(const Node &other) const;

	bool operator!=(const Node &other) const;

	bool operator<(const Node &other) const;

protected:
	std::string m_value;
	Type m_type{Type::String};
	Node *m_parent{};
	std::vector<Property> m_properties;
};
}

#include "Node.inl"
