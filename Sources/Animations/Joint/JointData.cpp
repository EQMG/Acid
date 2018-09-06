#include "JointData.hpp"

namespace acid
{
	JointData::JointData(const uint32_t &index, const std::string &nameId, const Matrix4 &bindLocalTransform) :
		m_index(index),
		m_nameId(nameId),
		m_bindLocalTransform(bindLocalTransform),
		m_children(std::vector<std::shared_ptr<JointData>>())
	{
	}

	void JointData::AddChild(const std::shared_ptr<JointData> &child)
	{
		m_children.emplace_back(child);
	}
}
