#pragma once

#include "../../Maths/Matrix4.hpp"

namespace Flounder
{
	class Joint
	{
	private:
		int m_index;
		std::string m_name;
		std::vector<Joint*> *m_children;

		Matrix4 *m_localBindTransform;
		Matrix4 *m_animatedTransform;
		Matrix4 *m_inverseBindTransform;

	public:
		Joint(const int &index, const std::string &name, const Matrix4 &bindLocalTransform);

		~Joint();

		void CalculateInverseBindTransform(const Matrix4 &parentBindTransform);

		int GetIndex() const { return m_index; }

		void SetIndex(const int &index) { m_index = index; }

		const std::string &GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::vector<Joint *> *GetChildren() const { return m_children; }

		void AddChild(Joint *child);

		void AddSelfAndChildren(std::vector<Joint *> *children);

		Matrix4 *GetLocalBindTransform() const { return m_localBindTransform; }

		void SetLocalBindTransform(const Matrix4 &localBindTransform) { *m_localBindTransform = localBindTransform; }

		Matrix4 *GetAnimatedTransform() const { return m_animatedTransform; }

		void SetAnimatedTransform(const Matrix4 &animatedTransform) { *m_animatedTransform = animatedTransform; }

		Matrix4 *GetInverseBindTransform() const { return m_inverseBindTransform; }

		void SetInverseBindTransform(const Matrix4 &inverseBindTransform) { *m_inverseBindTransform = inverseBindTransform; }
	};
}
