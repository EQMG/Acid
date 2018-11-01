#pragma once

#include <vector>
#include "Maths/Time.hpp"
#include "Animations/Joint/JointTransformData.hpp"

namespace acid
{
	class ACID_EXPORT KeyframeData
	{
	private:
		Time m_time;
		std::vector<JointTransformData> m_jointTransforms;
	public:
		explicit KeyframeData(const Time &time);

		Time GetTime() const { return m_time; }

		std::vector<JointTransformData> GetJointTransforms() const { return m_jointTransforms; }

		void AddJointTransform(const JointTransformData &transform);
	};
}
