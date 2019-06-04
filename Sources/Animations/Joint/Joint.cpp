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
		child->CalculateInverseBindTransform(bindTransform);
	}
}

void Joint::AddChild(Joint *child)
{
	m_children.emplace_back(child);
}

void Joint::AddSelfAndChildren(std::vector<Joint *> &children)
{
	children.emplace_back(this);

	for (const auto &child : children)
	{
		child->AddSelfAndChildren(children);
	}
}
}
