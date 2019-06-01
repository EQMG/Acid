#pragma once

#include "Helpers/String.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Helpers/ConstExpr.hpp"

namespace acid
{
/**
 * @brief class  that is used to represent a tree of values, used in file-object serialization.
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
	T GetChild(const std::string &name) const;

	template<typename T>
	T GetChildDefault(const std::string &name, const T &value);

	template<typename T>
	void GetChild(const std::string &name, T &dest) const;

	template<typename T>
	void SetChild(const std::string &name, const T &value);

	template<typename T>
	std::shared_ptr<T> GetResource(const std::string &name) const;

	template<typename T>
	void GetResource(const std::string &name, std::shared_ptr<T> &dest) const;

	template<typename T>
	void SetResource(const std::string &name, const std::shared_ptr<T> &value);

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

#include "Metadata.inl"
