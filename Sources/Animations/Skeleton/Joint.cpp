#include "Joint.hpp"

namespace acid {
Joint::Joint(uint32_t index, std::string name, const Matrix4 &bindLocalTransform) :
	m_index(index),
	m_name(std::move(name)),
	m_localBindTransform(bindLocalTransform) {
}

void Joint::CalculateInverseBindTransform(const Matrix4 &parentBindTransform) {
	auto bindTransform = parentBindTransform * m_localBindTransform;
	m_inverseBindTransform = bindTransform.Inverse();

	for (auto &child : m_children) {
		child.CalculateInverseBindTransform(bindTransform);
	}
}

void Joint::AddChild(const Joint &child) {
	m_children.emplace_back(child);
}

const Node &operator>>(const Node &node, Joint &joint) {
	node["index"].Get(joint.m_index);
	node["name"].Get(joint.m_name);
	node["children"].Get(joint.m_children);
	node["localBindTransform"].Get(joint.m_localBindTransform);
	//node["inverseBindTransform"].Get(joint.m_inverseBindTransform);
	return node;
}

Node &operator<<(Node &node, const Joint &joint) {
	node["index"].Set(joint.m_index);
	node["name"].Set(joint.m_name);
	node["children"].Set(joint.m_children);
	node["localBindTransform"].Set(joint.m_localBindTransform);
	//node["inverseBindTransform"].Set(joint.m_inverseBindTransform);
	// TODO: When loading 'CalculateInverseBindTransform' must be called on the head joint. 
	return node;
}
}
