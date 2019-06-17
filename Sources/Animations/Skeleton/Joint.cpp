#include "Joint.hpp"

namespace acid
{
Joint::Joint(const uint32_t &index, std::string name, const Matrix4 &bindLocalTransform) :
	m_index{index},
	m_name{std::move(name)},
	m_localBindTransform{bindLocalTransform}
{
}

void Joint::CalculateInverseBindTransform(const Matrix4 &parentBindTransform)
{
	auto bindTransform{parentBindTransform * m_localBindTransform};
	m_inverseBindTransform = bindTransform.Inverse();

	for (auto &child : m_children)
	{
		child.CalculateInverseBindTransform(bindTransform);
	}
}

void Joint::AddChild(const Joint &child)
{
	m_children.emplace_back(child);
}

const Metadata &operator>>(const Metadata &metadata, Joint &joint)
{
	metadata.GetChild("index", joint.m_index);
	metadata.GetChild("name", joint.m_name);
	metadata.GetChild("children", joint.m_children);
	metadata.GetChild("localBindTransform", joint.m_localBindTransform);
	//metadata.GetChild("inverseBindTransform", joint.m_inverseBindTransform);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Joint &joint)
{
	metadata.SetChild("index", joint.m_index);
	metadata.SetChild("name", joint.m_name);
	metadata.SetChild("children", joint.m_children);
	metadata.SetChild("localBindTransform", joint.m_localBindTransform);
	//metadata.SetChild("inverseBindTransform", joint.m_inverseBindTransform);
	// TODO: When loading 'CalculateInverseBindTransform' must be called on the head joint. 
	return metadata;
}
}
