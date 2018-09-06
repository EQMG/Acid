#pragma once

#include <string>
#include <vector>
#include "Maths/Matrix4.hpp"

namespace acid
{
	class ACID_EXPORT JointData
	{
	private:
		uint32_t m_index;
		std::string m_nameId;
		Matrix4 m_bindLocalTransform;

		std::vector<std::shared_ptr<JointData>> m_children;
	public:
		JointData(const uint32_t &index, const std::string &nameId, const Matrix4 &bindLocalTransform);

		uint32_t GetIndex() const { return m_index; }

		std::string GetNameId() const { return m_nameId; }

		Matrix4 GetBindLocalTransform() const { return m_bindLocalTransform; }

		std::vector<std::shared_ptr<JointData>> GetChildren() const { return m_children; }

		void AddChild(const std::shared_ptr<JointData> &child);
	};
}
