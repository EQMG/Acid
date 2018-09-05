#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Engine/Exports.hpp"
#include "Helpers/String.hpp"

namespace acid
{
	/// <summary>
	/// A class that is used to represent a tree of values, used in file-object serialization.
	/// </summary>
	class ACID_EXPORT Metadata
	{
	protected:
		std::string m_name;
		std::string m_value;
		std::vector<std::shared_ptr<Metadata>> m_children;
		std::map<std::string, std::string> m_attributes;
	public:
		Metadata(const std::string &name = "", const std::string &value = "", const std::map<std::string, std::string> &attributes = {});

		Metadata(const Metadata &source);

		~Metadata();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value) { m_value = value; }

		std::string GetString() const;

		void SetString(const std::string &data);

		std::vector<std::shared_ptr<Metadata>> GetChildren() const { return m_children; }

		uint32_t GetChildCount() const { return static_cast<uint32_t>(m_children.size()); }

		void ClearChildren() { m_children.clear(); }

		std::shared_ptr<Metadata> AddChild(const std::shared_ptr<Metadata> &value);

		bool RemoveChild(const std::shared_ptr<Metadata> &value);

		std::vector<std::shared_ptr<Metadata>> FindChildren(const std::string &name) const;

		std::shared_ptr<Metadata> FindChild(const std::string &name, const bool &reportError = true) const;

		std::shared_ptr<Metadata> FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError = true) const;

		template<typename T>
		T GetChild(const std::string &name) const
		{
			auto child = FindChild(name);

			if (child == nullptr)
			{
				return T();
			}

			return child->Get<T>();
		}

		template<typename T>
		T GetChild(const std::string &name, const T &value)
		{
			auto child = FindChild(name);

			if (child == nullptr)
			{
				child = AddChild(std::make_shared<Metadata>(name));
				child->Set(value);
			}

			return child->Get<T>();
		}

		template<typename T>
		void SetChild(const std::string &name, const T &value)
		{
			auto child = FindChild(name);

			if (child == nullptr)
			{
				child = std::make_shared<Metadata>(name, "");
				m_children.emplace_back(child);
			}

			child->Set<T>(value);
		}

		template<typename T>
		T Get() const
		{
			if constexpr(std::is_same_v<std::string, T>)
			{
				return GetString();
			}
			else if constexpr(std::is_class_v<T>)
			{
				T result = T();
				result.Decode(*this); // FIXME: Unsafe, unchecked.
				return result;
			}
			else
			{
				return String::From<T>(m_value);
			}
		}

		template<typename T>
		void Set(const T &value)
		{
			if constexpr(std::is_same_v<std::string, T>)
			{
				SetString(value);
			}
			else if constexpr(std::is_class_v<T>)
			{
				value.Encode(*this); // FIXME: Unsafe, unchecked.
			}
			else
			{
				SetValue(String::To<T>(value));
			}
		}

		std::map<std::string, std::string> GetAttributes() const { return m_attributes; }

		uint32_t GetAttributeCount() const { return static_cast<uint32_t>(m_attributes.size()); }

		void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

		void AddAttribute(const std::string &attribute, const std::string &value);

		bool RemoveAttribute(const std::string &attribute);

		std::string FindAttribute(const std::string &attribute) const;
	};
}
