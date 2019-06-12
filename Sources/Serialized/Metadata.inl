#pragma once

#include "Helpers/ConstExpr.hpp"
#include "Metadata.hpp"

namespace acid
{
template<typename T>
T Metadata::GetChild(const std::string &name) const
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		return T{};
	}

	T result;
	*child >> result;
	return std::move(result);
}

template<typename T>
T Metadata::GetChildDefault(const std::string &name, const T &value)
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		child = AddChild(std::make_unique<Metadata>(name));
		*child << value;
	}

	T result;
	*child >> result;
	return std::move(result);
}

template<typename T>
void Metadata::GetChild(const std::string &name, T &dest) const
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		return;
	}

	*child >> dest;
}

template<typename T>
void Metadata::SetChild(const std::string &name, const T &value)
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		child = AddChild(std::make_unique<Metadata>(name));
	}

	*child << value;
}

template<typename T>
std::shared_ptr<T> Metadata::GetResource(const std::string &name) const
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		return nullptr;
	}

	return T::Create(*child);
}

template<typename T>
void Metadata::GetResource(const std::string &name, std::shared_ptr<T> &dest) const
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		dest = nullptr;
		return;
	}

	dest = T::Create(*child);
}

template<typename T>
void Metadata::SetResource(const std::string &name, const std::shared_ptr<T> &value)
{
	auto child{FindChild(name)};

	if (child == nullptr)
	{
		child = AddChild(std::make_unique<Metadata>(name));
	}

	if (value == nullptr)
	{
		child->SetValue("null");
		return;
	}

	*child << *value;
}
template<typename T>
const Metadata &operator>>(const Metadata &metadata, T &object)
{
	object = String::From<T>(metadata.GetValue());
	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const T &object)
{
	metadata.SetValue(String::To(object));
	return metadata;
}

template<typename T>
const Metadata &operator>>(const Metadata &metadata, std::unique_ptr<T> &object)
{
	T x;
	metadata >> x;
	object = std::make_unique<T>(std::move(x));
	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const std::unique_ptr<T> &object)
{
	if (object == nullptr)
	{
		metadata.SetValue("null");
		return metadata;
	}

	metadata << *object;
	return metadata;
}

template<typename T>
const Metadata &operator>>(const Metadata &metadata, std::shared_ptr<T> &object)
{
	T x;
	metadata >> x;
	object = std::make_shared<T>(std::move(x));
	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const std::shared_ptr<T> &object)
{
	if (object == nullptr)
	{
		metadata.SetValue("null");
		return metadata;
	}

	metadata << *object;
	return metadata;
}

template<typename T>
const Metadata &operator>>(const Metadata &metadata, std::basic_string<T, std::char_traits<T>, std::allocator<T>> &string)
{
	string = metadata.GetString();
	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const std::basic_string<T, std::char_traits<T>, std::allocator<T>> &string)
{
	metadata.SetString(string);
	return metadata;
}

template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, const Metadata &> operator>>(const Metadata &metadata, T &object)
{
	metadata >> ConstExpr::AsRef(object);
	return metadata;
}

template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, Metadata &> operator<<(Metadata &metadata, const T &object)
{
	if (ConstExpr::AsPtr(object) == nullptr)
	{
		metadata.SetValue("null");
		return metadata;
	}

	metadata << ConstExpr::AsRef(object);
	return metadata;
}

template<typename T, typename K>
const Metadata &operator>>(const Metadata &metadata, std::pair<T, K> &pair)
{
	pair.first = String::From<T>(metadata.GetName());
	metadata >> pair.second;
	return metadata;
}

template<typename T, typename K>
Metadata &operator<<(Metadata &metadata, const std::pair<T, K> &pair)
{
	metadata.SetName(String::To(pair.first));
	metadata << pair.second;
	return metadata;
}

template<typename T>
const Metadata &operator>>(const Metadata &metadata, std::optional<T> &optional)
{
	if (metadata.GetValue() != "nullopt")
	{
		T x;
		metadata >> x;
		optional = std::move(x);
	}
	else
	{
		optional = std::nullopt;
	}

	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const std::optional<T> &optional)
{
	if (optional)
	{
		metadata << *optional;
	}
	else
	{
		metadata.SetValue("nullopt");
	}

	return metadata;
}

template<typename T>
const Metadata &operator>>(const Metadata &metadata, std::vector<T> &vector)
{
	vector = {};
	vector.reserve(metadata.GetChildren().size());

	for (const auto &child : metadata.GetChildren())
	{
		T x;
		*child >> x;
		vector.emplace_back(std::move(x));
	}

	return metadata;
}

template<typename T>
Metadata &operator<<(Metadata &metadata, const std::vector<T> &vector)
{
	for (const auto &x : vector)
	{
		auto child{metadata.AddChild(std::make_unique<Metadata>())};
		*child << x;
	}

	return metadata;
}

template<typename T, typename K>
const Metadata &operator>>(const Metadata &metadata, std::map<T, K> &map)
{
	map = {};

	for (const auto &child : metadata.GetChildren())
	{
		std::pair<T, K> pair;
		*child >> pair;
		map.emplace(std::move(pair));
	}

	return metadata;
}

template<typename T, typename K>
Metadata &operator<<(Metadata &metadata, const std::map<T, K> &map)
{
	for (const auto &x : map)
	{
		auto child{metadata.AddChild(std::make_unique<Metadata>())};
		*child << x;
	}

	return metadata;
}
}
