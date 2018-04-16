#include "Joint.hpp"

namespace Flounder
{
	Joint::Joint(const int &index, const std::string &name, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_name(name),
		m_children(new std::vector<Joint *>()),
		m_localBindTransform(new Matrix4(bindLocalTransform)),
		m_animatedTransform(new Matrix4()),
		m_inverseBindTransform(new Matrix4())
	{
	}

	Joint::~Joint()
	{
		for (auto child : *m_children)
		{
			delete child;
		}

		delete m_children;

		delete m_localBindTransform;
		delete m_animatedTransform;
		delete m_inverseBindTransform;
	}

	void Joint::CalculateInverseBindTransform(const Matrix4 &parentBindTransform)
	{
		Matrix4 *bindTransform = Matrix4::Multiply(parentBindTransform, *m_localBindTransform, nullptr);
		Matrix4::Invert(*bindTransform, m_inverseBindTransform);

		for (auto child : *m_children)
		{
			child->CalculateInverseBindTransform(*bindTransform);
		}

		delete bindTransform;
	}

	void Joint::AddChild(Joint *child)
	{
		m_children->push_back(child);
	}

	void Joint::AddSelfAndChildren(std::vector<Joint *> *children)
	{
		children->push_back(this);

		for (auto child : *children)
		{
			child->AddSelfAndChildren(children);
		}
	}
}
