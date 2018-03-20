#pragma once

#include <vector>
#include "../Keyframe/Keyframe.hpp"

namespace Flounder
{
	class F_EXPORT Animation
	{
	private:
		float m_length;

		std::vector<Keyframe *> *m_keyframes;
	public:
		Animation(const float &length, const std::vector<Keyframe *> &keyframes);

		Animation(const float &length, const std::vector<KeyframeData *> &keyframeData);

		~Animation();

		float GetLength() const { return m_length; }

		std::vector<Keyframe *> *GetKeyframes() const { return m_keyframes; }
	};
}
