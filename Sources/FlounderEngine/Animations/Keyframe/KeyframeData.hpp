#pragma once

#include <vector>
#include "Animations/Joint/JointTransformData.hpp"

namespace fl
{
	class FL_EXPORT KeyframeData
	{
	private:
		float m_time;
		std::vector<JointTransformData *> m_jointTransforms;
	public:
		KeyframeData(const float &time);

		~KeyframeData();

		float GetTime() const { return m_time; }

		std::vector<JointTransformData *> GetJointTransforms() const { return m_jointTransforms; }

		void AddJointTransform(JointTransformData *transform);
	};
}