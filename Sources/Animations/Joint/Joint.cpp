#include "Joint.hpp"

namespace acid
{
	Joint::Joint(const unsigned int &index, const std::string &name, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_name(name),
		m_localBindTransform(bindLocalTransform)
	{
	}

	Joint::~Joint()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
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

	void Joint::AddSelfAndChildren(std::vector<Joint *> *children)
	{
		children->emplace_back(this);

		for (auto &child : *children)
		{
			child->AddSelfAndChildren(children);
		}
	}
}
