#pragma once

#include "Helpers/String.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Helpers/ConstExpr.hpp"

namespace acid
{
/**
 * @brief Class that is used to represent a tree of values, used in file-object serialization.
 */
class ACID_EXPORT Metadata :
	public NonCopyable
{
public:
	explicit Metadata(const std::string &name = "", const std::string &value = "", std::map<std::string, std::string> attributes = {});

	const std::string &GetName() const { return m_name; }

	void SetName(const std::string &name) { m_name = name; }

	const std::string &GetValue() const { return m_value; }

	void SetValue(const std::string &value) { m_value = value; }

	std::string GetString() const;

	void SetString(const std::string &data);

	const std::vector<std::unique_ptr<Metadata>> &GetChildren() const { return m_children; }

	uint32_t GetChildCount() const { return static_cast<uint32_t>(m_children.size()); }

	void ClearChildren() { m_children.clear(); }

	Metadata *AddChild(Metadata *child);

	void RemoveChild(Metadata *child);

	std::vector<Metadata *> FindChildren(const std::string &name) const;

	Metadata *FindChild(const std::string &name, const bool &reportError = true) const;

	Metadata *FindChildWithBackup(const std::string &name, const std::string &backupName, const bool &reportError = true) const;

	Metadata *FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError = true) const;

	template<typename T>
	T GetChild(const std::string &name) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return T();
		}

		T result;
		*child >> result;
		return std::move(result);
	}

	template<typename T>
	T GetChildDefault(const std::string &name, const T &value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = AddChild(new Metadata(name));
			*child << value;
		}

		T result;
		*child >> result;
		return std::move(result);
	}

	template<typename T>
	void GetChild(const std::string &name, T &dest) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return;
		}

		*child >> dest;
	}

	template<typename T>
	void SetChild(const std::string &name, const T &value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = AddChild(new Metadata(name));
		}

		*child << value;
	}

	template<typename T>
	std::shared_ptr<T> GetResource(const std::string& name) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return nullptr;
		}

		return T::Create(*child);
	}

	template<typename T>
	void GetResource(const std::string& name, std::shared_ptr<T>& dest) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			dest = nullptr;
			return;
		}

		dest = T::Create(*child);
	}

	template<typename T>
	void SetResource(const std::string& name, const std::shared_ptr<T>& value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = AddChild(new Metadata(name));
		}

		if (value == nullptr)
		{
			child->SetValue("null");
			return;
		}

		*child << *value;
	}

	const std::map<std::string, std::string> &GetAttributes() const { return m_attributes; }

	uint32_t GetAttributeCount() const { return static_cast<uint32_t>(m_attributes.size()); }

	void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

	void ClearAttributes() { m_attributes.clear(); }

	void AddAttribute(const std::string &attribute, const std::string &value);

	void RemoveAttribute(const std::string &attribute);

	std::string FindAttribute(const std::string &attribute) const;

	virtual void Load(std::istream *inStream);

	virtual void Write(std::ostream *outStream) const;

	Metadata *Clone() const;

	bool operator==(const Metadata &other) const;

	bool operator!=(const Metadata &other) const;

	bool operator<(const Metadata &other) const;

protected:
	std::string m_name;
	std::string m_value;
	std::vector<std::unique_ptr<Metadata>> m_children;
	std::map<std::string, std::string> m_attributes;
};

template<typename T>
const Metadata& operator>>(const Metadata& metadata, T& object)
{
	object = String::From<T>(metadata.GetValue());
	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const T& object)
{
	metadata.SetValue(String::To(object));
	return metadata;
}

template<typename T>
const Metadata& operator>>(const Metadata& metadata, std::unique_ptr<T>& object)
{
	T x;
	metadata >> x;
	object = std::make_unique<T>(std::move(x));
	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const std::unique_ptr<T>& object)
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
const Metadata& operator>>(const Metadata& metadata, std::shared_ptr<T>& object)
{
	T x;
	metadata >> x;
	object = std::make_shared<T>(std::move(x));
	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const std::shared_ptr<T>& object)
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
const Metadata& operator>>(const Metadata& metadata, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& string)
{
	string = metadata.GetString();
	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& string)
{
	metadata.SetString(string);
	return metadata;
}

template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, const Metadata&> operator>>(const Metadata& metadata, T& object)
{
	auto result = T();
	metadata >> ConstExpr::AsRef(result);
	return metadata;
}

template<typename T>
std::enable_if_t<std::is_class_v<T> || std::is_pointer_v<T>, Metadata&> operator<<(Metadata& metadata, const T& object)
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
const Metadata& operator>>(const Metadata& metadata, std::pair<T, K>& pair)
{
	pair.first = String::From<T>(metadata.GetName());
	metadata >> pair.second;
	return metadata;
}

template<typename T, typename K>
Metadata& operator<<(Metadata& metadata, const std::pair<T, K>& pair)
{
	metadata.SetName(String::To(pair.first));
	metadata << pair.second;
	return metadata;
}

template<typename T>
const Metadata& operator>>(const Metadata& metadata, std::optional<T>& optional)
{
	if (metadata.GetValue() != "nullopt")
	{
		T x;
		metadata >> x;
		optional = std::move(x);
	}
	else
	{
		optional = {};
	}

	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const std::optional<T>& optional)
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
const Metadata& operator>>(const Metadata& metadata, std::vector<T>& vector)
{
	vector = std::vector<T>();
	vector.reserve(metadata.GetChildren().size());

	for (const auto& child : metadata.GetChildren())
	{
		T x;
		*child >> x;
		vector.emplace_back(std::move(x));
	}

	return metadata;
}

template<typename T>
Metadata& operator<<(Metadata& metadata, const std::vector<T>& vector)
{
	for (const auto &x : vector)
	{
		auto child = metadata.AddChild(new Metadata());
		*child << x;
	}

	return metadata;
}

template<typename T, typename K>
const Metadata& operator>>(const Metadata& metadata, std::map<T, K>& map)
{
	map = std::map<T, K>();

	for (const auto& child : metadata.GetChildren())
	{
		std::pair<T, K> pair;
		*child >> pair;
		map.emplace(std::move(pair));
	}

	return metadata;
}

template<typename T, typename K>
Metadata& operator<<(Metadata& metadata, const std::map<T, K>& map)
{
	for (const auto &x : map)
	{
		auto child = metadata.AddChild(new Metadata());
		*child << x;
	}

	return metadata;
}
}
