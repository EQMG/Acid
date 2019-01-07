#include "Joint.hpp"

namespace acid
{
	Joint::Joint(const uint32_t &index, const std::string &name, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_name(name),
		m_children(std::vector<std::unique_ptr<Joint>>()),
		m_localBindTransform(bindLocalTransform),
		m_animatedTransform(Matrix4()),
		m_inverseBindTransform(Matrix4())
	{
	}

	void Joint::CalculateInverseBindTransform(const Matrix4 &parentBindTransform)
	{
		Matrix4 bindTransform = parentBindTransform * m_localBindTransform;
		m_inverseBindTransform = bindTransform.Invert();

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
