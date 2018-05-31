#include "JointData.hpp"

namespace fl
{
	JointData::JointData(const int &index, const std::string &nameId, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_nameId(nameId),
		m_bindLocalTransform(Matrix4(bindLocalTransform)),
		m_children(std::vector<JointData *>())
	{
	}

	JointData::~JointData()
	{
		for (auto child : m_children)
		{
			delete child;
		}
	}

	void JointData::AddChild(JointData *child)
	{
		m_children.emplace_back(child);
	}
}
