#pragma once

#include <ostream>

#include "NodeFormat.hpp"
#include "NodeView.hpp"

namespace acid {
/**
 * @brief Class that is used to represent a tree of UFT-8 values, used in serialization.
 */
class ACID_CORE_EXPORT Node final {
public:
	Node() {} // = default;
	Node(const Node &node) = default;
	Node(Node &&node) noexcept = default;

	template<typename T, typename = std::enable_if_t<std::is_convertible_v<T *, NodeFormat *>>>
	void ParseString(std::string_view string);
	template<typename T, typename = std::enable_if_t<std::is_convertible_v<T *, NodeFormat *>>>
	void WriteStream(std::ostream &stream, NodeFormat::Format format = NodeFormat::Minified) const;

	template<typename T, typename _Elem = char, typename = std::enable_if_t<
		std::is_convertible_v<T *, NodeFormat *>
#ifndef ACID_BUILD_MSVC
		// Cannot dynamicly parse wide streams on GCC or Clang
		&& std::is_same_v<_Elem, char>
#endif
	>>
	void ParseStream(std::basic_istream<_Elem> &stream);
	template<typename T, typename _Elem = char, typename = std::enable_if_t<std::is_convertible_v<T *, NodeFormat *>>>
	std::basic_string<_Elem> WriteString(NodeFormat::Format format = NodeFormat::Minified) const;

	template<typename T>
	T Get() const;
	template<typename T>
	T GetWithFallback(const T &fallback) const;
	template<typename T>
	bool Get(T &dest) const;
	template<typename T, typename K>
	bool GetWithFallback(T &dest, const K &fallback) const;
	template<typename T>
	bool Get(T &&dest) const;
	template<typename T, typename K>
	bool GetWithFallback(T &&dest, const K &fallback) const;
	template<typename T>
	void Set(const T &value);
	template<typename T>
	void Set(T &&value);
	
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
	
	//Node &Merge(Node &&node);

	bool HasProperty(const std::string &name) const;
	bool HasProperty(uint32_t index) const;
	NodeConstView GetProperty(const std::string &name) const;
	NodeConstView GetProperty(uint32_t index) const;
	NodeView GetProperty(const std::string &name);
	NodeView GetProperty(uint32_t index);
	Node &AddProperty(const Node &node);
	Node &AddProperty(Node &&node = {});
	Node &AddProperty(const std::string &name, const Node &node);
	Node &AddProperty(const std::string &name, Node &&node = {});
	Node &AddProperty(uint32_t index, const Node &node);
	Node &AddProperty(uint32_t index, Node &&node = {});
	Node RemoveProperty(const std::string &name);
	Node RemoveProperty(const Node &node);

	NodeConstView GetPropertyWithBackup(const std::string &name, const std::string &backupName) const;
	NodeConstView GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) const;
	NodeView GetPropertyWithBackup(const std::string &name, const std::string &backupName);
	NodeView GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue);

	NodeConstView operator[](const std::string &name) const;
	NodeConstView operator[](uint32_t index) const;
	NodeView operator[](const std::string &name);
	NodeView operator[](uint32_t index);

	Node &operator=(const Node &rhs) = default;
	Node &operator=(Node &&rhs) noexcept = default;
	Node &operator=(const NodeConstView &rhs);
	Node &operator=(NodeConstView &&rhs);
	Node &operator=(NodeView &rhs);
	Node &operator=(NodeView &&rhs);
	template<typename T>
	Node &operator=(const T &rhs);

	bool operator==(const Node &rhs) const;
	bool operator!=(const Node &rhs) const;
	bool operator<(const Node &rhs) const;

	const NodeProperties &GetProperties() const { return properties; }
	NodeProperties &GetProperties() { return properties; }

	const NodeValue &GetValue() const { return value; }
	void SetValue(NodeValue value) { this->value = std::move(value); }

	const NodeType &GetType() const { return type; }
	void SetType(NodeType type) { this->type = type; }

protected:
	NodeProperties properties;
	NodeValue value;
	NodeType type = NodeType::Object;
};
}

#include "Node.inl"
#include "NodeConstView.inl"
#include "NodeView.inl"
