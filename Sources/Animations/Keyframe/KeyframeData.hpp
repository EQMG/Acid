#pragma once

#include <vector>
#include "Animations/Joint/JointTransformData.hpp"

namespace acid
{
	class ACID_EXPORT KeyframeData
	{
	private:
		float m_time;
		std::vector<JointTransformData> m_jointTransforms;
	public:
		KeyframeData(const float &time);

		float GetTime() const { return m_time; }

		std::vector<JointTransformData> GetJointTransforms() const { return m_jointTransforms; }

		void AddJointTransform(const JointTransformData &transform);
	};
}
