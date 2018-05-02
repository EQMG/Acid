#pragma once

#include <string>
#include <vector>
#include "Maths/Matrix4.hpp"

namespace fl
{
	class FL_EXPORT JointData
	{
	private:
		int m_index;
		std::string m_nameId;
		Matrix4 m_bindLocalTransform;

		std::vector<JointData *> m_children;
	public:
		JointData(const int &index, const std::string &nameId, const Matrix4 &bindLocalTransform);

		~JointData();

		int GetIndex() const { return m_index; }

		std::string GetNameId() const { return m_nameId; }

		Matrix4 GetBindLocalTransform() const { return m_bindLocalTransform; }

		std::vector<JointData *> GetChildren() const { return m_children; }

		void AddChild(JointData *child);
	};
}
