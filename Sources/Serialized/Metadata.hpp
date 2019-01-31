#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Helpers/String.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
	/// <summary>
	/// A class that is used to represent a tree of values, used in file-object serialization.
	/// </summary>
	class ACID_EXPORT Metadata :
		public NonCopyable
	{
	public:
		explicit Metadata(const std::string &name = "", const std::string &value = "", const std::map<std::string, std::string> &attributes = {});

		const std::string &GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		const std::string &GetValue() const { return m_value; }

		void SetValue(const std::string &value) { m_value = value; }

		std::string GetString() const;

		void SetString(const std::string &data);

		const std::vector<std::unique_ptr<Metadata>> &GetChildren() const { return m_children; }

		uint32_t GetChildCount() const { return static_cast<uint32_t>(m_children.size()); }

		void ClearChildren() { m_children.clear(); }

		Metadata *AddChild(Metadata *value);

		void RemoveChild(Metadata *child);

		std::vector<Metadata *> FindChildren(const std::string &name) const;

		Metadata *FindChild(const std::string &name, const bool &reportError = true) const;

		Metadata *FindChildWithBackup(const std::string &name, const std::string &backupName, const bool &reportError = true) const;

		Metadata *FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError = true) const;

		template<typename T> struct is_vector : public std::false_type {};

		template<typename T, typename A>
		struct is_vector<std::vector<T, A>> : public std::true_type {};

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
		void GetChild(const std::string &name, T &dest) const
		{
			auto child = FindChild(name);

			if (child == nullptr)
			{
				return;
			}

			if constexpr (is_vector<T>::value)
			{
				dest = T();

				for (const auto &child2 : child->GetChildren())
				{
					typedef typename T::value_type base_type;

					if constexpr (std::is_same_v<std::pair<std::string, std::string>, base_type>)
					{
						dest.emplace_back(child2->GetName(), child2->Get<std::string>());
					}
					else
					{
						dest.emplace_back(child2->Get<base_type>());
					}
				}
			}
			else
			{
				dest = child->Get<T>();
			}
		}

		template<typename T>
		T GetChildDefault(const std::string &name, const T &value)
		{
			auto child = FindChild(name, false);

			if (child == nullptr)
			{
				child = AddChild(new Metadata(name));
				child->Set(value);
			}

			return child->Get<T>();
		}

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string &name) const
		{
			auto child = FindChild(name);

			if (child == nullptr)
			{
				return nullptr;
			}

			return T::Create(*child);
		}

		template<typename T>
		void GetResource(const std::string &name, std::shared_ptr<T> &dest) const
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
		void SetChild(const std::string &name, const T &value)
		{
			auto child = FindChild(name, false);

			if (child == nullptr)
			{
				child = new Metadata(name);
				m_children.emplace_back(child);
			}

			if constexpr (is_vector<T>::value)
			{
				for (const auto &x : value)
				{
					typedef typename T::value_type base_type;

					if constexpr (std::is_same_v<std::pair<std::string, std::string>, base_type>)
					{
						child->AddChild(new Metadata(x.first, x.second));
					}
					else
					{
						child->AddChild(new Metadata("", x));
					}
				}
			}
			else
			{
				child->Set<T>(value);
			}
		}

		template<typename T>
		void SetResource(const std::string &name, const std::shared_ptr<T> &value)
		{
			auto child = FindChild(name, false);

			if (child == nullptr)
			{
				child = new Metadata(name);
				m_children.emplace_back(child);
			}

			child->Set<std::shared_ptr<T>>(value);
		}

		template<typename T>
		T Get() const
		{
			if constexpr (std::is_same_v<std::string, T>)
			{
				return GetString();
			}
			else if constexpr (std::is_class_v<T>)
			{
				T result = T();
				result.Decode(*this);
				return result;
			}
			else
			{
				return String::From<T>(m_value);
			}
		}

		template<typename T>
		T *Ptr(T &obj) { return &obj; }

		template<typename T>
		T *Ptr(T *obj) { return obj; }

		template<typename T>
		T *Ptr(std::shared_ptr<T> obj) { return obj.get(); }

		template<typename T>
		T *Ptr(std::unique_ptr<T> obj) { return obj.get(); }

		template<typename T>
		void Set(const T &value)
		{
			if constexpr (std::is_same_v<std::string, T>)
			{
				SetString(value);
			}
			else if constexpr (std::is_class_v<T> || std::is_pointer_v<T>)
			{
				if (Ptr(value) == nullptr)
				{
					SetValue("null");
					return;
				}

				Ptr(value)->Encode(*this);
			}
			else
			{
				SetValue(String::To<T>(value));
			}
		}

		const std::map<std::string, std::string> &GetAttributes() const { return m_attributes; }

		uint32_t GetAttributeCount() const { return static_cast<uint32_t>(m_attributes.size()); }

		void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

		void ClearAttributes() { m_attributes.clear(); }

		void AddAttribute(const std::string &attribute, const std::string &value);

		void RemoveAttribute(const std::string &attribute);

		std::string FindAttribute(const std::string &attribute) const;

		virtual void Load(const std::string &data);

		virtual std::string Write() const;

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
