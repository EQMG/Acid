#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Engine/Exports.hpp"
#include "Helpers/FormatString.hpp"

namespace acid
{
	class ACID_EXPORT Serialized
	{
	protected:
		std::string m_name;
		std::string m_value;
		std::vector<std::shared_ptr<Serialized>> m_children;
		std::map<std::string, std::string> m_attributes;
	public:
		Serialized(const std::string &name = "", const std::string &value = "", const std::map<std::string, std::string> &attributes = {});

		Serialized(const Serialized &source);

		~Serialized();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value) { m_value = value; }

		std::string GetString() const;

		void SetString(const std::string &data);

		std::vector<std::shared_ptr<Serialized>> &GetChildren() { return m_children; }

		std::shared_ptr<Serialized> AddChild(const std::shared_ptr<Serialized> &value);

		bool RemoveChild(const std::shared_ptr<Serialized> &value);

		std::vector<std::shared_ptr<Serialized>> FindChildren(const std::string &name) const;

		std::shared_ptr<Serialized> FindChild(const std::string &name) const;

		std::shared_ptr<Serialized> FindChildSafe(const std::string &name);

		std::shared_ptr<Serialized> FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError = true) const;

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
				child = AddChild(std::make_shared<Serialized>(name));
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
				child = std::make_shared<Serialized>(name, "");
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
			else if constexpr(std::is_enum_v<T>)
			{
				std::string data = GetValue();
				return static_cast<T>(FormatString::ConvertTo<int32_t>(data));
			}
			else if constexpr(std::is_class_v<T>)
			{
				T result = T();
				result.Decode(*this); // Unsafe, unchecked.
				return result;
			}
			else
			{
				std::string data = GetValue();
				return FormatString::ConvertTo<T>(data);
			}
		}

		template<typename T>
		void Set(const T &value)
		{
			if constexpr(std::is_same_v<std::string, T>)
			{
				SetString(value);
			}
			else if constexpr(std::is_enum_v<T>) // TODO: Move into ConvertTo and to_string.
			{
				SetValue(std::to_string(static_cast<int32_t>(value)));
			}
			else if constexpr(std::is_class_v<T>)
			{
				value.Encode(*this);
			}
			else
			{
				SetValue(std::to_string(value));
			}
		}

		std::map<std::string, std::string> GetAttributes() const { return m_attributes; }

		void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

		void AddAttribute(const std::string &attribute, const std::string &value);

		bool RemoveAttribute(const std::string &attribute);

		std::string FindAttribute(const std::string &attribute) const;
	};
}
