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
	auto bindTransform = parentBindTransform * m_localBindTransform;
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

const Node &operator>>(const Node &node, Joint &joint)
{
	node.GetChild("index", joint.m_index);
	node.GetChild("name", joint.m_name);
	node.GetChild("children", joint.m_children);
	node.GetChild("localBindTransform", joint.m_localBindTransform);
	//node.GetChild("inverseBindTransform", joint.m_inverseBindTransform);
	return node;
}

Node &operator<<(Node &node, const Joint &joint)
{
	node.SetChild("index", joint.m_index);
	node.SetChild("name", joint.m_name);
	node.SetChild("children", joint.m_children);
	node.SetChild("localBindTransform", joint.m_localBindTransform);
	//node.SetChild("inverseBindTransform", joint.m_inverseBindTransform);
	// TODO: When loading 'CalculateInverseBindTransform' must be called on the head joint. 
	return node;
}
}
