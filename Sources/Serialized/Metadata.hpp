#pragma once

#include "Helpers/String.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Helpers/TypeTraits.hpp"

namespace acid
{
/// <summary>
/// A class that is used to represent a tree of values, used in file-object serialization.
/// </summary>
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

	template<typename T> T GetChild(const std::string &name) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return T();
		}

		return child->Get<T>();
	}

	template<typename T> T GetChildDefault(const std::string &name, const T &value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = AddChild(new Metadata(name));
			child->Set(value);
		}

		return child->Get<T>();
	}

	template<typename T> void GetChild(const std::string &name, T &dest) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return;
		}

		dest = child->Get<T>();
	}

	template<typename T> void SetChild(const std::string &name, const T &value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = new Metadata(name);
			m_children.emplace_back(child);
		}

		child->Set(value);
	}

	template<typename T> T Get() const
	{
		if constexpr (std::is_same_v<std::string, T>)
		{
			return GetString();
		}
		else if constexpr (TypeTraits::is_pair<T>::value)
		{
			typedef typename T::first_type first_type;
			typedef typename T::second_type second_type;

			return T(String::From<first_type>(m_name), Get<second_type>());
		}
		else if constexpr (TypeTraits::is_vector<T>::value)
		{
			auto result = T();

			for (const auto &child : GetChildren())
			{
				typedef typename T::value_type base_type;

				result.emplace_back(child->Get<base_type>());
			}

			return result;
		}
		else if constexpr (TypeTraits::is_map<T>::value)
		{
			auto result = T();

			for (const auto &child : GetChildren())
			{
				typedef typename T::key_type key_type;
				typedef typename T::mapped_type mapped_type;

				result.emplace(child->Get<std::pair<key_type, mapped_type>>());
			}

			return result;
		}
		else if constexpr (std::is_class_v<T> || std::is_pointer_v<T>)
		{
			auto result = T();
			TypeTraits::AsPtr(result)->Decode(*this);
			return result;
		}
		else
		{
			return String::From<T>(m_value);
		}
	}

	template<typename T> void Set(const T &value)
	{
		if constexpr (std::is_same_v<std::string, T>)
		{
			SetString(value);
		}
		else if constexpr (TypeTraits::is_pair<T>::value)
		{
			// Pairs and maps must have keys that can be converted to a string using String::To.
			SetName(String::To(value.first));
			Set(value.second);
		}
		else if constexpr (TypeTraits::is_vector<T>::value || TypeTraits::is_map<T>::value)
		{
			for (const auto &x : value)
			{
				auto child = AddChild(new Metadata());
				child->Set(x);
			}
		}
		else if constexpr (std::is_class_v<T> || std::is_pointer_v<T>)
		{
			if (TypeTraits::AsPtr(value) == nullptr)
			{
				SetValue("null");
				return;
			}

			TypeTraits::AsPtr(value)->Encode(*this);
		}
		else
		{
			SetValue(String::To(value));
		}
	}

	template<typename T> std::shared_ptr<T> GetResource(const std::string &name) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			return nullptr;
		}

		return T::Create(*child);
	}

	template<typename T> void GetResource(const std::string &name, std::shared_ptr<T> &dest) const
	{
		auto child = FindChild(name);

		if (child == nullptr)
		{
			dest = nullptr;
			return;
		}

		dest = T::Create(*child);
	}

	template<typename T> void SetResource(const std::string &name, const std::shared_ptr<T> &value)
	{
		auto child = FindChild(name, false);

		if (child == nullptr)
		{
			child = new Metadata(name);
			m_children.emplace_back(child);
		}

		child->Set<std::shared_ptr<T>>(value);
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
}
