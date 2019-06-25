#pragma once

#include "Helpers/NonCopyable.hpp"

namespace acid
{
/**
 * @brief Class that is used to represent a tree of values, used in file-object serialization.
 */
class ACID_EXPORT Metadata :
	public NonCopyable
{
public:
	enum class Format
	{
		Beautified, Minified
	};

	Metadata() = default;

	explicit Metadata(std::string name, std::string value = "", std::vector<std::unique_ptr<Metadata>> &&children = {});

	virtual void Load(std::istream *inStream);

	virtual void Write(std::ostream *outStream, const Format &format = Format::Beautified) const;

	std::unique_ptr<Metadata> Clone() const;

	const std::string &GetName() const { return m_name; }

	void SetName(const std::string &name) { m_name = name; }

	template<typename T = std::string>
	T GetValue() const;

	template<typename T = std::string>
	void SetValue(const T &value);

	const std::vector<std::unique_ptr<Metadata>> &GetChildren() const { return m_children; }

	void ClearChildren() { m_children.clear(); }

	std::vector<Metadata *> FindChildren(const std::string &name) const;

	Metadata *FindChild(const std::string &name) const;

	Metadata *FindChildWithBackup(const std::string &name, const std::string &backupName) const;

	Metadata *FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value) const;

	Metadata *AddChild(std::unique_ptr<Metadata> &&child);

	void RemoveChild(Metadata *child);

	template<typename T>
	T GetChild(const std::string &name) const;

	template<typename T>
	T GetChildDefault(const std::string &name, const T &value);

	template<typename T>
	void GetChild(const std::string &name, T &dest) const;

	template<typename T>
	void SetChild(const std::string &name, const T &value);

	const std::map<std::string, std::string> &GetAttributes() const { return m_attributes; }

	void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

	void ClearAttributes() { m_attributes.clear(); }

	std::optional<std::string> FindAttribute(const std::string &attribute) const;

	void SetAttribute(const std::string &attribute, const std::string &value);

	void RemoveAttribute(const std::string &attribute);

	Metadata *operator[](const std::string &string) const;

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
