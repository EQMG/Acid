#include "Joint.hpp"

namespace acid {
Joint::Joint(uint32_t index, std::string name, const Matrix4 &bindLocalTransform) :
	index(index),
	name(std::move(name)),
	localBindTransform(bindLocalTransform) {
}

void Joint::CalculateInverseBindTransform(const Matrix4 &parentBindTransform) {
	auto bindTransform = parentBindTransform * localBindTransform;
	inverseBindTransform = bindTransform.Inverse();

	for (auto &child : children) {
		child.CalculateInverseBindTransform(bindTransform);
	}
}

void Joint::AddChild(const Joint &child) {
	children.emplace_back(child);
}

const Node &operator>>(const Node &node, Joint &joint) {
	node["index"].Get(joint.index);
	node["name"].Get(joint.name);
	node["children"].Get(joint.children);
	node["localBindTransform"].Get(joint.localBindTransform);
	//node["inverseBindTransform"].Get(joint.inverseBindTransform);
	return node;
}

Node &operator<<(Node &node, const Joint &joint) {
	node["index"].Set(joint.index);
	node["name"].Set(joint.name);
	node["children"].Set(joint.children);
	node["localBindTransform"].Set(joint.localBindTransform);
	//node["inverseBindTransform"].Set(joint.inverseBindTransform);
	// TODO: When loading 'CalculateInverseBindTransform' must be called on the head joint. 
	return node;
}
}
