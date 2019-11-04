#pragma once

#include "Node.hpp"

#include <codecvt>

#include "Helpers/ConstExpr.hpp"
#include "Helpers/String.hpp"
#include "Resources/Resource.hpp"

namespace acid {
template<typename _Elem>
void Node::ParseStream(std::basic_istream<_Elem> & stream) {
	// We must read as UTF8 chars.
	if constexpr (!std::is_same_v<_Elem, char>) {
		stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<char>));
	}

	// Reading into a string before iterating is much faster.
	std::string s(std::istreambuf_iterator<_Elem>(stream), {});
	ParseString(s);
}

template<typename _Elem>
std::basic_string<_Elem> Node::WriteString(Node::Format format) const {
	std::basic_stringstream<_Elem> stream;
	WriteStream(stream, format);
	return stream.str();
}

template<typename T>
T Node::Get() const {
	T value;
	*this >> value;
	return value;
}

template<typename T>
T Node::Get(const T &fallback) const {
	if (!IsValid())
		return fallback;
	
	return Get<T>();
}

template<typename T>
bool Node::Get(T &dest) const {
	if (!IsValid())
		return false;
	
	*this >> dest;
	return true;
}

template<typename T, typename K>
bool Node::Get(T &dest, const K &fallback) const {
	if (!IsValid()) {
		dest = fallback;
		return false;
	}

	*this >> dest;
	return true;
}

template<typename T>
void Node::Set(const T &value) {
	*this << value;
}

template<typename T>
Node &Node::Append(const T &value) {
	AddProperty() << value;
	return *this;
}

template<typename ...Args>
Node &Node::Append(const Args &...args) {
	(Append(args), ...);
	return *this;
}

template<typename T>
Node &Node::operator=(const T &rhs) {
	Set(rhs);
	return *this;
}

/*const Node &operator>>(const Node &node, std::nullptr_t &object) {
	object = nullptr;
	return node;
}*/

inline Node &operator<<(Node &node, const std::nullptr_t &object) {
	node.SetValue("");
	node.SetType(Node::Type::Null);
	return node;
}

inline const Node &operator>>(const Node &node, bool &object) {
	object = String::From<bool>(node.GetValue());
	return node;
}

inline Node &operator<<(Node &node, bool object) {
	node.SetValue(String::To(object));
	node.SetType(Node::Type::Boolean);
	return node;
}

template<typename T>
std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, const Node &> operator>>(const Node &node, T &object) {
	object = String::From<T>(node.GetValue());
	return node;
}

template<typename T>
std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, Node &> operator<<(Node &node, T object) {
	node.SetValue(String::To(object));
	node.SetType(std::is_floating_point_v<T> ? Node::Type::Decimal : Node::Type::Integer);
	return node;
}

/*template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, const Node &> operator>>(const Node &node, T &object) {
	node >> ConstExpr::AsRef(object);
	return node;
}*/

template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, Node &> operator<<(Node &node, const T &object) {
	if (ConstExpr::AsPtr(object) == nullptr) {
		return node << nullptr;
	}

	node << ConstExpr::AsRef(object);
	return node;
}

template<typename T>
const Node &operator>>(const Node &node, std::unique_ptr<T> &object) {
	object = std::make_unique<T>();
	node >> *object;
	return node;
}

template<typename T>
Node &operator<<(Node &node, const std::unique_ptr<T> &object) {
	if (object == nullptr) {
		return node << nullptr;
	}

	node << *object;
	return node;
}

template<typename T>
const Node &operator>>(const Node &node, std::shared_ptr<T> &object) {
	// TODO: Abstract Resource streams out from shared_ptr.
	if constexpr (std::is_base_of_v<Resource, T>) {
		object = T::Create(node);
		return node;
	} else {
		object = std::make_shared<T>();
		node >> *object;
		return node;
	}
}

template<typename T>
Node &operator<<(Node &node, const std::shared_ptr<T> &object) {
	if (object == nullptr) {
		return node << nullptr;
	}

	node << *object;
	return node;
}

inline const Node &operator>>(const Node &node, char *&string) {
	std::strcpy(string, node.GetValue().c_str());
	return node;
}

inline Node &operator<<(Node &node, const char *string) {
	node.SetValue(string);
	node.SetType(Node::Type::String);
	return node;
}

inline const Node &operator>>(const Node &node, std::string &string) {
	string = node.GetValue();
	return node;
}

inline Node &operator<<(Node &node, const std::string &string) {
	node.SetValue(string);
	node.SetType(Node::Type::String);
	return node;
}

inline const Node &operator>>(const Node &node, std::filesystem::path &object) {
	object = node.GetValue();
	return node;
}

inline Node &operator<<(Node &node, const std::filesystem::path &object) {
	auto str = object.string();
	std::replace(str.begin(), str.end(), '\\', '/');
	node.SetValue(str);
	node.SetType(Node::Type::String);
	return node;
}

template<typename T, typename K>
const Node &operator>>(const Node &node, std::pair<T, K> &pair) {
	pair.first = String::From<T>(node.GetName());
	node >> pair.second;
	return node;
}

template<typename T, typename K>
Node &operator<<(Node &node, const std::pair<T, K> &pair) {
	node.SetName(String::To(pair.first));
	node << pair.second;
	return node;
}

template<typename T>
const Node &operator>>(const Node &node, std::optional<T> &optional) {
	if (node.GetValue() != "null") {
		T x;
		node >> x;
		optional = std::move(x);
	} else {
		optional = std::nullopt;
	}

	return node;
}

template<typename T>
Node &operator<<(Node &node, const std::optional<T> &optional) {
	if (optional) {
		return node << *optional;
	}

	return node << nullptr;
}

template<typename T>
const Node &operator>>(const Node &node, std::vector<T> &vector) {
	vector.clear();
	vector.reserve(node.GetProperties().size());

	for (const auto &property : node.GetProperties()) {
		T x;
		property >> x;
		vector.emplace_back(std::move(x));
	}

	return node;
}

template<typename T>
Node &operator<<(Node &node, const std::vector<T> &vector) {
	for (const auto &x : vector) {
		node.AddProperty() << x;
	}

	node.SetType(Node::Type::Array);
	return node;
}

template<typename T, typename K>
const Node &operator>>(const Node &node, std::map<T, K> &map) {
	map.clear();

	for (const auto &property : node.GetProperties()) {
		std::pair<T, K> pair;
		property >> pair;
		map.emplace(std::move(pair));
	}

	return node;
}

template<typename T, typename K>
Node &operator<<(Node &node, const std::map<T, K> &map) {
	for (const auto &x : map) {
		node.AddProperty() << x;
	}

	node.SetType(Node::Type::Array);
	return node;
}
}
